/**
 * ENCE260 BattleShips Game
 * 
 * @authors
 *  Joseph Hendry (jhe135)
 *  Jack Malcolm (jma476)
*/

#include "system.h"
#include "pacer.h"
#include "button.h"
#include "navswitch.h"
#include "display_ships.h"
#include "data_communication.h"
#include "pio.h"
#include "led.h"

#define SCREEN_LENGTH 7
#define SCREEN_WIDTH 5
#define DISPLAY_TASK_RATE 100
#define DISPLAY_MESSAGE_RATE 10
#define PACER_RATE 500

int user_hit_count = 0;
int opponent_hit_count = 0;
int turn; // 1 = user, 0 = opponent

void game_init(void)
{
    system_init ();
    pacer_init (PACER_RATE);
    navswitch_init ();
    button_init ();
    display_ships_init();
    data_communication_init();
    led_init();
    led_set(LED1, 0);
    pio_output_high(LEDMAT_ROW1_PIO);
}

/**
 * Sets the LED to hight to indicate that user is waiting for opponents response.
*/
void set_led_high(void)
{
    led_set(LED1, 1);
}

/**
 * Sets the LED to low.
*/
void set_led_low(void)
{
    led_set(LED1, 0);
}

/**
 * The main funciton for adding a ship to the display.
*/
void place_ships (void)
{
    // Loop through each boat length
    for (int i = 3; i >= 1; i--) {

        // Set the ship length
        add_ship(i);

        // Create counter to flash the ship
        uint8_t counter = 0;

        while (1) {
            // Wait and update display, navswitch and button
            pacer_wait ();
            navswitch_update ();
            button_update ();
            display_ships_update ();

            // Flash the ship
            if (counter == 200) {
                counter = 0;
                display_selected();
            } else if (counter == 100) {
                display_ships();
            }

            // Check which button was pushed
            if (button_push_event_p(0)) {
                rotate_ship();
            } else if (navswitch_push_event_p(NAVSWITCH_PUSH)  && place_ship()) {
                break;
            } else if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
                move_up();
            } else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
                move_down();
            } else if (navswitch_push_event_p(NAVSWITCH_EAST)) {
                move_right();
            } else if (navswitch_push_event_p(NAVSWITCH_WEST)) {
                move_left();
            }

            // Increment the counter
            counter++;
        }
    }

    // Wait for the opponent to finish placing their ships
    wait_for_opponent();
}

/**
 * The main function for attacking.
*/
void attack (void)
{
    // Add the shot to the screen
    add_shot();

    // Create counter to flash the shot
    uint8_t counter = 0;
    while (1) {

        // Wait and update display, navswitch and button
        pacer_wait ();
        navswitch_update ();
        display_ships_update ();

        // Flash the shot
        if (counter == 200) {
            counter = 0;
            display_selected();
            display_user_hits();
        } else if (counter == 100) {
            display_user_misses();
        }

        // Check which button was pushed
        if (navswitch_push_event_p(NAVSWITCH_PUSH) && place_shot()) {
            break;
        } else if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
            move_up();
        } else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
            move_down();
        } else if (navswitch_push_event_p(NAVSWITCH_EAST)) {
            move_right();
        } else if (navswitch_push_event_p(NAVSWITCH_WEST)) {
            move_left();
        }
        // Increment the counter
        counter++;
    }

    // Send message to opponent
    set_led_high();
    char message = get_shot_location();
    
    // Send the shot
    if (send_shot(message)) {
        shot_hit(message);
        user_hit_count++;
    }
    set_led_low();

    
}

/**
 * The main function for defending.
*/
void defend (void)
{
    // Create counter to flash the shot
    uint8_t counter = 0;
    while (1) {

        // Wait and update display, navswitch and button
        pacer_wait ();
        navswitch_update ();
        display_ships_update ();

        // Flash the shot
        if (counter == 200) {
            counter = 0;
            display_opponent_shots();
        } else if (counter == 100) {
            display_ships();
        }

        // Check for a received shot
        if (check_for_shot()) {
            break;
        }

        // Increment the counter
        counter++;
    }

    // Get the shot
    char shot = receive_shot();
    
    // Check if the shot was a hit
    if (check_shot(shot)) {
        // Send the hit message
        send_shot_result(1);
        opponent_hit_count++;
    } else {
        // Send the miss message
        send_shot_result(0);
    }
}

/**
 * Checks if the game has ended.
*/
int check_end_game(void)
{
    if (user_hit_count == 6 || opponent_hit_count == 6) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * The main function for the game.
*/
void game_turn(void)
{
    while (1) {
        if (turn == 1) {
            attack();
            turn = 0; // Switch to the other player's turn
        } else {
            defend();
            turn = 1; // Switch to the other player's turn
        }
        if (check_end_game()) {
            break;
        }
    }
}

/**
 * Displays a message to the user if they win or lose.
*/
void endGame(void)
{
    // Set the message.
    if (user_hit_count == 6) {
        display_win();
    } else {
        display_loss();
    }

    // Display the message
    while (1)
    {
        pacer_wait ();
        display_ships_update ();
    }
}


/**
 * @brief Main function
 * 
 * This function controls the flow of the program.
 */
int main (void)
{
    // Initialize the game
    game_init();

    // Await connection
    turn = connect_to_opponent();

    // Place ships
    place_ships();


    // Start the game
    game_turn();


    // End the game
    endGame(); 

    return 0;
}