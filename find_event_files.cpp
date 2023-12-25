#include <iostream>
#include <string>
#include <vector>
//this module's header
#include "find_event_files.hpp"
//input path
#define INPUT_DIR "/dev/input/"

//checks if is a char device
static int is_char_device(const struct dirent *file){
    struct stat filestat;
    char filename[512];
    int err;
    snprintf(filename, sizeof(filename), "%s%s", INPUT_DIR, file->d_name);
    err = stat(filename, &filestat);
    if(err){
        return 0;
    }
    return S_ISCHR(filestat.st_mode);
}

//note from orignal author of this part of code (https://github.com/SCOTPAUL/keylog/tree/master)
// Some of this implementation is based on lessons learned from the following:
// http://unix.stackexchange.com/questions/94322/is-it-possible-for-a-daemon-i-e-background-process-to-look-for-key-presses-fr/94329#94329

std::vector<std::string> get_keyboard_event_files(){
    std::vector<std::string> keyboards_files;
    int number_of_found_input_files, i;
    struct dirent **event_files;
    std::string currently_testing_filename;
    int opened_file_descriptor;
    int32_t kbd_bitmap = KEY_A | KEY_B | KEY_C | KEY_Z;
    int32_t event_bitmap;

    //get number of files in input directory
    number_of_found_input_files = scandir(INPUT_DIR, &event_files, &is_char_device, &alphasort);

    //if directory not empty check whether files are keyboards
    if(number_of_found_input_files > 0) {
        for(i = 0; i < number_of_found_input_files; ++i){
            event_bitmap = 0;
            currently_testing_filename = INPUT_DIR + std::string(event_files[i]->d_name);
            opened_file_descriptor = open(currently_testing_filename.c_str(), O_RDONLY);
            if(opened_file_descriptor == -1){
                perror("open");
                continue;
            }
            ioctl(opened_file_descriptor, EVIOCGBIT(0, sizeof(event_bitmap)), &event_bitmap);
            if((EV_KEY & event_bitmap) == EV_KEY){
                // The device acts like a keyboard
                ioctl(opened_file_descriptor, EVIOCGBIT(EV_KEY, sizeof(event_bitmap)), &event_bitmap);
                if((kbd_bitmap & event_bitmap) == kbd_bitmap){
                    // The device supports A, B, C, Z keys, so it probably is a keyboard, so add it to return vector
                    keyboards_files.push_back(currently_testing_filename);
                }
            }
            close(opened_file_descriptor);
        }
    }
    // Cleanup scandir
    for(i = 0; i < number_of_found_input_files; ++i){
        free(event_files[i]);
    }
    free(event_files);

    return keyboards_files;
}
