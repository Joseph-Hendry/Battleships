/**
 * Display ships module.
 * 
 * @authors
 *  Joseph Hendry (jhe135)
 *  Jack Malcolm (jma476)
*/
#ifndef DISPLAY_SHIPS_H
#define DISPLAY_SHIPS_H

#include "system.h"

/**
 * Initialises the display.
*/
void display_ships_init(void);

/**
 * Updates the display.
*/
void display_ships_update(void);

/**
 * Adds a ship of a given length to the display.
 * 
 * @param _length The length of the ship.
*/
void add_ship(uint8_t _length);

/**
 * Adds a shot to the display.
*/
void add_shot(void);

/**
 * Checks if the ship can be placed and places it.
 * 
 * @return 1 if the ship was placed, 0 otherwise.
*/
uint8_t place_ship(void);

/**
 * Checks if the shot can be placed and places it.
 * 
 * @return a pointer to the location if the shot was placed, NULL otherwise.
*/
uint8_t place_shot(void);

/**
 * Get location of the shot.
*/
char get_shot_location(void);

/**
 * Adds the shot to the display and adds it to the list of shots.
*/
uint8_t check_shot(char message);

/**
 * Adds the shot to the display matrix.
 * 
 * @param message The location of the shot.
*/
void shot_hit(char message);

/**
 * Displays the users ships and turns the other pixels off.
*/
void display_ships(void);

/**
 * Displays the selected ship or shot.
*/
void display_selected(void);

/**
 * Displays the user's misses and turns the other pixels off/
*/
void display_user_misses(void);

/**
 * Displays the user's hits.
*/
void display_user_hits(void);

/**
 * Displays the opponent's shots.
*/
void display_opponent_shots(void);

/**
 * Clears the screen.
*/
void clear_screen(void);

/**
 * Moves the selected ship or shot up.
*/
void move_up(void);

/**
 * Moves the selected ship or shot down.
*/
void move_down(void);

/**
 * Moves the selected ship or shot left.
*/
void move_left(void);

/**
 * Moves the selected ship or shot right.
*/
void move_right(void);

/**
 * Rotates the selected ship.
*/
void rotate_ship(void);

/**
 * Displays the message "You Win!".
*/
void display_loss (void);

/**
 * Displays the message "You Lose!".
*/
void display_win (void);

#endif