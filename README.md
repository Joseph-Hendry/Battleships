Embeded Systems Game - Battle Ships

Objective:
The objective of the game is for each player to guess the location of their opponent's ships and shoot at them. The first player to destroy all of the enemy ships wins the game.

Required Equipment:

    Two UCFK4 devices
    Type B Mini USB cables
    BATTLESHIP program
    Screen divider (for separating the players' views)

How to Play:

Set Up Phase:

    Run the BATTLESHIP program located in the "/src/game" directory on the UCFK devices.
    Align the Infrared (IR) emitter and receiver of the two UCFK devices for communication.

Ship Placement:

    In this phase, players will place all 3 of their ships.
    A ship should appear flashing in the middle of the LED matrix.
    Use the NavSwitch (NSEW directions) to move the ship, and the white button on the left to rotate the ship.
    Press the NavSwitch push-down button to confirm the ship's position.
    Repeat these steps until four ships have been placed, still LED's show previously placed ships.

Battle Phase:

    Once both players have placed their ships, the game enters the battle phase.
    The player who's designated as the starting attacker can take the first shot at the enemy ships.
    If a shot hits an enemy ship each users will be able to see it flashing on their corresponding screens.
    If a shot misses, the defending player will see the shot as as flashing dot, while the attacker will see their miss as a solid dot on the screen.
    After each shot, the attacking and defending roles switch between players.
    The game continues until one player destroys all of the enemy ships by hitting each individual part (pixel) of all the ships, a message will then be displayed to the user telling them if they have won or lost.

![image](https://github.com/Joseph-Hendry/BattleShips/assets/107831702/6d0dbbb7-9749-4a5e-89b1-d55a0d92dae1)

Contributors:
Joseph Hendry (jhe135)
Jack Malcolm (jma476).
