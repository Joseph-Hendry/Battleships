/**
 * Data communication.
 * 
 * @authors
 *  Joseph Hendry (jhe135)
 *  Jack Malcolm (jma476)
*/

#ifndef DATA_COMMUNICATION_H
#define DATA_COMMUNICATION_H

#include "system.h"

/**
 * Initializes the data communication.
 */
void data_communication_init(void);

/**
 * Connects to the opponent.
 * 
 * @return 1 if user goes first, 0 if opponent goes first.
*/
uint8_t connect_to_opponent(void);

/**
 * Sends a message to the opponent saying ready and waits for one back.
*/
void wait_for_opponent(void);

/**
 * Sends a shot to the opponent.
 * 
 * @param message The message to send.
 * 
 * @return 1 if hit, 0 if miss.
*/
uint8_t send_shot(char message);

/**
 * Checks if the opponent has sent a shot.
*/
uint8_t check_for_shot(void);

/**
 * Receives a shot from the opponent.
 * 
 * @return The location of the shot.
*/
char receive_shot(void);

/**
 * Sends the result of the shot to the opponent.
 * 
 * @param hit 1 if hit, 0 if miss.
*/
void send_shot_result(uint8_t hit);

#endif // DATA_COMMUNICATION_H