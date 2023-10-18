/**
 * Data communication.
 * 
 * @authors
 *  Joseph Hendry (jhe135)
 *  Jack Malcolm (jma476)
*/

#include "system.h"
#include "data_communication.h"
#include "pacer.h"
#include "ir_uart.h"

#define PACER_RATE 500
#define CONNECTING_KEY_HASH 67
#define HANDSHAKE_KEY_HASH 68
#define READY_KEY_HASH 69
#define SEND_SHOT_KEY_HASH 70

/**
 * Initialises the data communication.
 */
void data_communication_init(void)
{
    system_init();
    pacer_init(PACER_RATE);
    ir_uart_init();
}

/**
 * Connects to the opponent.
 * 
 * @return 1 if the connection was successful, 0 otherwise.
 */
uint8_t connect_to_opponent(void)
{
    // Send the connection key hash to the opponent
    ir_uart_putc(CONNECTING_KEY_HASH);

    // Await a message from the opponent
    while (1)
    {
        pacer_wait();

        if (ir_uart_read_ready_p())
        {
            // Read the message from the opponent
            char message = ir_uart_getc();

            // Check if the message is the connection key hash
            if (message == CONNECTING_KEY_HASH)
            {
                // Send the handshake key hash to the opponent
                ir_uart_putc(HANDSHAKE_KEY_HASH);

                // Set this user to go first
                return 1;

            } else if (message == HANDSHAKE_KEY_HASH) {

                // Set the opponent to go first
                return 0;
            }
        }
    }
}

void send_ready(void)
{
    // Send the ready key hash to the opponent
    ir_uart_putc(READY_KEY_HASH);
}

/**
 * Sends a message to the opponent saying ready and waits for one back.
 */
void wait_for_opponent(void)
{
    // Send the ready key hash to the opponent
    ir_uart_putc(READY_KEY_HASH);

    // Await a message from the opponent
    while (1)
    {
        pacer_wait();

        if (ir_uart_read_ready_p())
        {
            // Read the message from the opponent
            char message = ir_uart_getc();

            // Check if the message is the ready key hash
            if (message == READY_KEY_HASH)
            {
                // Break out of the loop
                break;
            }
        }
    }
}


/**
 * Sends a shot to the opponent.
 * 
 * @param message The message to send.
 * 
 * @return 1 if hit, 0 if miss.
*/
uint8_t send_shot(char message)
{
    // Create the varble to send
    //char message = (location->x & 0x0F) << 4 | (location->y & 0x0F);
    
    // Send the location of the shot to the opponent
    ir_uart_putc(message);

    // Await a message from the opponent
    while (1)
    {
        pacer_wait();

        if (ir_uart_read_ready_p())
        {
            // Read the message from the opponent
            char message = ir_uart_getc();

            // Check if the message is the hit key hash
            if (message == 'h')
            {
                // Break out of the loop
                return 1;

            } else if (message == 'm') {

                // Break out of the loop
                return 0;
            }
        }
    }
}

/**
 * Checks if the opponent has sent a shot.
*/
uint8_t check_for_shot(void)
{
    return ir_uart_read_ready_p();
}

/**
 * Receives a shot from the opponent.
 * 
 * @return The location of the shot.
*/
char receive_shot(void)
{
    // Read the message from the opponent
    char message = ir_uart_getc();

    //return location;
    return message;
}

/**
 * Sends the result of the shot to the opponent.
 * 
 * @param hit 1 if hit, 0 if miss.
*/
void send_shot_result(uint8_t hit)
{
    // Send the result of the shot to the opponent
    if (hit)
    {
        ir_uart_putc('h');
    } else {
        ir_uart_putc('m');
    }
}