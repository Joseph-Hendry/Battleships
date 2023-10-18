/**
 * Display ships module.
 * 
 * @authors
 *  Joseph Hendry (jhe135)
 *  Jack Malcolm (jma476)
*/

#include "display_ships.h"
#include "system.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"

#define SCREEN_LENGTH 7
#define SCREEN_WIDTH 5
#define DISPLAY_TASK_RATE 500
#define DISPLAY_MESSAGE_RATE 10
#define MESSAGE_RATE 10

// Define a global matrix to represent the display, shots and opponent shots
int gameBoard[SCREEN_WIDTH][SCREEN_LENGTH];
int userShots[SCREEN_WIDTH][SCREEN_LENGTH];
int opponentShots[SCREEN_WIDTH][SCREEN_LENGTH];

uint8_t mode = 0; // 0 = place ships, 1 = place shots, 2 = defend
uint8_t length;
uint8_t location[2] = {2, 2};
uint8_t ship_rotated = 1;

/**
 * Initialises the display.
*/
void display_ships_init(void)
{
    tinygl_init (500);

    // Init locations
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        for (int j = 0; j < SCREEN_LENGTH; j++) {
            gameBoard[i][j] = 0;
            userShots[i][j] = 0;
            opponentShots[i][j] = 0;
        }
    }
}

/**
 * Updates the display.
*/
void display_ships_update(void)
{
    // Update the display
    tinygl_update();
}

/**
 * Adds a ship of a given length to the display.
 * 
 * @param _length The length of the ship.
*/
void add_ship(uint8_t _length)
{    
    length = _length;
    mode = 0;
    display_ships();
}

/**
 * Adds a shot to the display.
*/
void add_shot(void)
{
    // Reset the length and mode
    length = 1;
    mode = 1;

    // Reset the location
    location[0] = 2;
    location[1] = 3;

    display_user_misses();
}

/**
 * Checks if the ship can be placed and places it.
 * 
 * @return 1 if the ship was placed, 0 otherwise.
*/
uint8_t place_ship(void)
{
    // Check that the ship is not overlapping with another ship
    if (ship_rotated) {
        for (uint8_t i = 0; i < length; i++) {
            if (gameBoard[location[0]][location[1] + i] != 0) {
                return 0;
            }
        }

        // Add the ship to the board
        for (uint8_t i = 0; i < length; i++) {
            gameBoard[location[0]][location[1] + i] = 1;
        }

        // Reset the ship location
        location[0] = 2;
        location[1] = 2;

        display_ships();
        return 1;

    } else {
        for (uint8_t i = 0; i < length; i++) {
            if (gameBoard[location[0] + i][location[1]] != 0) {
                return 0;
            }
        }

        // Add the ship to the board
        for (uint8_t i = 0; i < length; i++) {
            gameBoard[location[0] + i][location[1]] = 1;
        }

        // Reset the ship location
        location[0] = 1;
        location[1] = 3;

        display_ships();
        return 1;
    }
}

/**
 * Checks if the shot can be placed and places it.
 * 
 * @return 1 if the shot was placed, 0 otherwise.
*/
uint8_t place_shot(void)
{
    // Check that the shot is not overlapping with another shot
    if (userShots[location[0]][location[1]] == 0) {
        // Add the shot to the board
        userShots[location[0]][location[1]] = 1;

        return 1;
    } else {
        return 0;
    }
}

/**
 * Get location of the shot.
*/
char get_shot_location(void)
{
    char message = (location[0] & 0x0F) << 4 | (location[1] & 0x0F);
    return message;
}

/**
 * Adds the shot to the display and adds it to the list of shots.
*/
uint8_t check_shot(char message)
{
    // Adds the shot to the list of shots
    uint8_t x = (message >> 4) & 0x0F;
    uint8_t y = message & 0x0F;

    // Check if the shot is a hit
    if (gameBoard[x][y] == 1) {
        // Add the shot to the board
        opponentShots[x][y] = 2;

        return 1;
    } else {
        // Add the shot to the board
        opponentShots[x][y] = 1;


        return 0;
    }
}

/**
 * Adds the shot to the display matrix.
 * 
 * @param message The location of the shot.
*/
void shot_hit(char message)
{
    uint8_t x = (message >> 4) & 0x0F;
    uint8_t y = message & 0x0F;
    // Add the shot to the board
    userShots[x][y] = 2;
}

/**
 * Displays the users ships and turns the other pixels off.
*/
void display_ships(void)
{
    // Display the ships on the board that have not been shot
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        for (int j = 0; j < SCREEN_LENGTH; j++) {
            if (gameBoard[i][j] == 1 && opponentShots[i][j] == 0) {
                tinygl_pixel_set(tinygl_point(i, j), 1);
            }  else {
                tinygl_pixel_set(tinygl_point(i, j), 0);
            }
        }
    }
}

/**
 * Displays the selected ship or shot.
*/
void display_selected (void)
{
    // Display the selected ship
    if (ship_rotated) {
        for (int i = 0; i < length; i++) {
            tinygl_pixel_set(tinygl_point(location[0], location[1] + i), 1);
        }
    } else {
        for (int i = 0; i < length; i++) {
            tinygl_pixel_set(tinygl_point(location[0] + i, location[1]), 1);
        }
    }
}

/**
 * Displays the user's misses and turns the other pixels off/
*/
void display_user_misses(void)
{
    // Display the board as a solid dot
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        for (int j = 0; j < SCREEN_LENGTH; j++) {
            if (userShots[i][j] == 1) {
                tinygl_pixel_set(tinygl_point(i, j), 1);
            } else {
                tinygl_pixel_set(tinygl_point(i, j), 0);
            }
        }
    }
}

/**
 * Displays the user's hits.
*/
void display_user_hits(void)
{
    // Display the board as a solid dot
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        for (int j = 0; j < SCREEN_LENGTH; j++) {
            if (userShots[i][j] == 2) {
                tinygl_pixel_set(tinygl_point(i, j), 1);
            }
        }
    }
}

/**
 * Displays the opponent's shots.
*/
void display_opponent_shots(void)
{
    // Display the board as a solid dot
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        for (int j = 0; j < SCREEN_LENGTH; j++) {
            if (opponentShots[i][j] != 0) {
                tinygl_pixel_set(tinygl_point(i, j), 1);
            }
        }
    }
}

/**
 * Clears the screen.
*/
void clear_screen(void)
{
    tinygl_clear();
}

/**
 * Moves the current object up.
*/
void move_up(void)
{
    // Move the ship up
    if (location[1] > 0) {
        location[1]--;
    }
}

/**
 * Moves the current object down.
*/
void move_down(void)
{

    // Move the ship down
    if (!ship_rotated && location[1] < (SCREEN_LENGTH - 1)) {
        location[1]++;
    } else if (ship_rotated && location[1] < (SCREEN_LENGTH - length)) {
        location[1]++;
    }
}

/**
 * Moves the current object left.
*/
void move_left(void)
{
    // Move the ship left
    if (location[0] > 0) {
        location[0]--;
    }
}

/**
 * Moves the current object right.
*/
void move_right(void)
{
    // Move the ship right
    if (!ship_rotated && location[0] < (SCREEN_WIDTH - length)) {
        location[0]++;
        
    } else if (ship_rotated && location[0] < (SCREEN_WIDTH - 1)) {
        location[0]++;
    }
}

/**
 * Rotates the ship.
*/
void rotate_ship(void)
{
    // Rotate the ship
    if (ship_rotated) {
        ship_rotated = 0;

        // Reset the ship location
        location[0] = 1;
        location[1] = 3;
    } else {
        ship_rotated = 1;

        // Reset the ship location
        location[0] = 2;
        location[1] = 2;
    }
}

/**
 * Displays the message "You Win!".
*/
void display_win (void)
{
    tinygl_clear();
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(DISPLAY_MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text("You Win!");
}

/**
 * Displays the message "You Lose!".
*/
void display_loss (void)
{
    tinygl_clear();
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(DISPLAY_MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text("You Lose!");
}