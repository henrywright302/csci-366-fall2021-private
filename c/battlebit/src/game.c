//
// Created by carson on 5/20/20.
//

#include <stdlib.h>
#include <stdio.h>
#include "game.h"

// STEP 9 - Synchronization: the GAME structure will be accessed by both players interacting
// asynchronously with the server.  Therefore the data must be protected to avoid race conditions.
// Add the appropriate synchronization needed to ensure a clean battle.

static game * GAME = NULL;

void game_init() {
    if (GAME) {
        free(GAME);
    }
    GAME = malloc(sizeof(game));
    GAME->status = CREATED;
    game_init_player_info(&GAME->players[0]);
    game_init_player_info(&GAME->players[1]);
}

void game_init_player_info(player_info *player_info) {
    player_info->ships = 0;
    player_info->hits = 0;
    player_info->shots = 0;
}

int game_fire(game *game, int player, int x, int y) {


    player_info *playerFiring = &game->players[player];
    unsigned long long int mask = xy_to_bitval(x,y);
    playerFiring->shots;
    if (mask & playerFiring->shots){

    }
    else {
        playerFiring->shots = playerFiring->shots | mask;
    }

    player_info *playerBeingFiredAt;
    if (player == 1){
        playerBeingFiredAt = &game->players[0];
    }
    else {
        playerBeingFiredAt = &game->players[1];
    }

    // Test ship value using mask to see if opponent has ship
    if (playerBeingFiredAt->ships & playerFiring->shots){
        // update playerFiring using OR
        playerFiring->hits = playerFiring->hits | mask;
        // " " using XOR
        playerBeingFiredAt->ships = playerBeingFiredAt->ships ^ mask;
        if (player == 1){
            game->status = PLAYER_0_TURN;
            if (playerBeingFiredAt->ships == 0){
                    game->status = PLAYER_1_WINS;

            }

        }
        else {
            game->status = PLAYER_1_TURN;
            if (playerBeingFiredAt->ships == 0){
                game->status = PLAYER_0_WINS;

            }

    }

        return 1;

    }



   return 0;



    //ayerFiring->hits;
    // Step 5 - This is the crux of the game.  You are going to take a shot from the given player and
    // update all the bit values that store our game state.
    //
    //  - You will need up update the players 'shots' value
    //  - You will need to see if the shot hits a ship in the opponents ships value.  If so, record a hit in the
    //    current players hits field
    //  - If the shot was a hit, you need to flip the ships value to 0 at that position for the opponents ships field
    //
    //  If the opponents ships value is 0, they have no remaining ships, and you should set the game state to
    //  PLAYER_1_WINS or PLAYER_2_WINS depending on who won.
}

unsigned long long int xy_to_bitval(int x, int y) {
    if (x < 0){
        //printf("Invalid - x < 0\n");
        return 0;
    }
    else if (x > 7){
        //printf("Invalid - x > 7\n");
        return 0;
    }
    else if (y < 0){
        //printf("Invalid - y < 0\n");
        return 0;
    }
    else if (y > 7){
        //printf("Invalid - y > 7\n");
        return 0;
    }

    unsigned long long int mask = 1ull;
    /** Step 1 - DONE */

    mask = mask << (y * 8);
    mask = mask << x;




    // Step 1 - implement this function.  We are taking an x, y position
    // and using bitwise operators, converting that to an unsigned long long

    // with a 1 in the position corresponding to that x, y
    //
    // x:0, y:0 == 0b00000...0001 (the one is in the first position)
    // x:1, y: 0 == 0b00000...10 (the one is in the second position)
    // ....
    // x:0, y: 1 == 0b100000000 (the one is in the eighth position)
    //
    // you will need to use bitwise operators and some math to produce the right
    // value.
    return mask;
}

struct game * game_get_current() {
    return GAME;
}

int game_load_board(struct game *game, int player, char * spec) {
    /** Step 2 - Done */
    player_info *playerGoing = &game->players[player];
    if (player == 1){
        game->status = PLAYER_0_TURN;
    }

    // look at spec. If it is valid...
    if (spec == NULL){
        //printf("Spec is null\n");
        return -1;
    }
    else if (strlen(spec) == 15){

        int seenCarrier = 0;
        int seenBattleship = 0;
        int seenDestroyer = 0;
        int seenSubmarine = 0;
        int seenPatrol = 0;

        int length, x, y;

        for (int i = 0; i < 15; i = i + 3) {
            char *current = spec + i;
            //printf("Current: %c\n", current);
            char ship = *current;
            //printf("Ship: %c\n", ship);
            char col = *(current + 1);
            //printf("Column: %c\n", col);
            char row = *(current + 2);
            //printf("Row: %c\n", row);

            player_info *playerInfo = &game->players[player];

            if (ship == 'C'){
                //printf("Found me a Carrier\n");
                if (seenCarrier == 0){
                    seenCarrier++;
                    length = 5;
                    y = row - '0';
                    x = col - '0';

                    if (add_ship_horizontal(playerInfo, x, y, length) == -1){
                        return -1;
                    }
                }
                else {
                    //printf("already seen this\n");
                    return -1;
                }
            }
            else if (ship == 'c'){
                //printf("Found me a lil Carrier\n");
                if (seenCarrier == 0){
                    seenCarrier++;
                    length = 5;
                    y = row - '0';
                    x = col - '0';

                    if (add_ship_vertical(playerInfo, x, y, length) == -1){
                        return -1;
                    }
                }
                else {
                    //printf("already seen this\n");
                    return -1;
                }
            }
            else if (ship == 'B'){
                //printf("Found me a big battleship\n");
                if (seenBattleship == 0){
                    seenBattleship++;
                    length = 4;
                    y = row - '0';
                    x = col - '0';

                    if (add_ship_horizontal(playerInfo, x, y, length) == -1){
                        return -1;
                    }
                }
                else {
                    //printf("already seen this\n");
                    return -1;
                }
            }
            else if (ship == 'b'){
                //printf("Here's the little battleship\n");
                if (seenBattleship == 0){
                    seenBattleship++;
                    length = 4;
                    y = row - '0';
                    x = col - '0';

                    if (add_ship_vertical(playerInfo, x, y, length) == -1){
                        return -1;
                    }
                }
                else {
                    //printf("already seen this\n");
                    return -1;
                }
            }
            else if (ship == 'D'){
                //printf("Found me a big Destroyer\n");
                if (seenDestroyer == 0){
                    seenDestroyer++;
                    length = 3;
                    y = row - '0';
                    x = col - '0';

                    if (add_ship_horizontal(playerInfo, x, y, length) == -1){
                        return -1;
                    }
                }
                else {
                    //printf("already seen this\n");
                    return -1;
                }
            }
            else if (ship == 'd'){
                //printf("Found me a lil destroyer\n");
                if (seenDestroyer == 0){
                    seenDestroyer++;
                    length = 3;
                    y = row - '0';
                    x = col - '0';

                    if (add_ship_vertical(playerInfo, x, y, length) == -1){
                        return -1;
                    }
                }
                else {
                    //printf("already seen this\n");
                    return -1;
                }
            }
            else if (ship == 'S'){
                //printf("Found me a big Submarine\n");
                if (seenSubmarine == 0){
                    seenSubmarine++;
                    length = 3;
                    y = row - '0';
                    x = col - '0';
                    //printf("Submarine x: %d\n", x);
                    //printf("Submarine y: %d\n", y);

                    if (add_ship_horizontal(playerInfo, x, y, length) == -1){
                        return -1;
                    }
                }
                else {
                    //printf("already seen this\n");
                    return -1;
                }
            }
            else if (ship == 's'){
                //printf("Found me a lil submarine\n");
                if (seenSubmarine == 0){
                    seenSubmarine++;
                    length = 3;
                    y = row - '0';
                    x = col - '0';
                    //printf("Submarine x: %d\n", x);
                    //printf("Submarine y: %d\n", y);

                    if (add_ship_vertical(playerInfo, x, y, length) == -1){
                        return -1;
                    }
                }
                else {
                    //printf("already seen this\n");
                    return -1;
                }
            }
            else if (ship == 'P'){
                //printf("Here's the big PatrolBoat\n");
                if (seenPatrol == 0){
                    seenPatrol++;
                    length = 2;
                    y = row - '0';
                    x = col - '0';

                    if (add_ship_horizontal(playerInfo, x, y, length) == -1){
                        return -1;
                    }
                }
                else {
                    //printf("already seen this\n");
                    return -1;
                }
            }
            else if (ship == 'p'){
                //printf("Here's the little PatrolBoat\n");
                if (seenPatrol == 0){
                    seenPatrol++;
                    length = 2;
                    y = row - '0';
                    x = col - '0';

                    if (add_ship_vertical(playerInfo, x, y, length) == -1){
                        return -1;
                    }
                }
                else {
                    //printf("already seen this\n");
                    return -1;
                }
            }
            else {
                //printf("Invalid Spec (Unrecognizable Ship type)\n");
                return -1;
            }
        }

        return 1;

    }
    else {
        return -1;
    }

    // Step 2 - implement this function.  Here you are taking a C
    // string that represents a layout of ships, then testing
    // to see if it is a valid layout (no off-the-board positions
    // and no overlapping ships)
    //

    // if it is valid, you should write the corresponding unsigned
    // long long value into the Game->players[player].ships data
    // slot and return 1
    //
    // if it is invalid, you should return -1
}

int add_ship_horizontal(player_info *player, int x, int y, int length) {
    // implement this as part of Step 2
    // returns 1 if the ship can be added, -1 if not -> off the board? overlap with other ship?
    // hint: this can be defined recursively
if (length != 0){
    // make sure x and y are on the board (between 0 and 7):
    if (x < 0){
        //printf("Invalid coordinates\n");
        return -1;
    }
    else if (x > 7){
        //printf("Invalid x > 7 coordinates\n");
        return -1;
    }
    else if (y < 0){
        //printf("Invalid y < 0 coordinates\n");
        return -1;
    }
    else if (y > 7){
        //printf("Invalid y > 7 coordinates\n");
        return -1;
    }

    // make sure that there is not already a ship there
    unsigned long long int mask = xy_to_bitval(x,y); // gives us a bitmask
    //printf("Mask : %llu\n", mask);
    player-> ships;


    // 64 bit int, 8x8 grid. DOes this variable have this ^ var set to 1? If so, there is a ship
    // if you can place a ship there, flip bit to 1 (??)
    if (mask & player->ships){
        //printf("There is already a ship there!\n");
        return -1;
    }
    else {
        player->ships = player->ships | mask;
        // this will add a ship? idk

        add_ship_horizontal(player, x + 1, y, length-1);

        // else { add ship (pi->ships = pi->ships .... mask), recursively call add ship}
        //recursively call add_ship_horizontal (decrease length by 1, do whatever to x and y too)

    }
}

    else {
        //printf("Successfully added ship\n");
        return 1;
    }


}

int add_ship_vertical(player_info *player, int x, int y, int length) {
    if (length > 0) {

        if (x < 0) {
            //printf("Invalid coordinates\n");
            return -1;
        } else if (x > 7) {
            //printf("Invalid x > 7 coordinates\n");
            return -1;
        } else if (y < 0) {
            //printf("Invalid y < 0 coordinates\n");
            return -1;
        } else if (y > 7) {
            //printf("Invalid y > 7 coordinates\n");
            return -1;
        }

        unsigned long long int mask = xy_to_bitval(x, y); // gives us a bitmask
        player->ships;

        if (mask & player->ships) {
            //printf("There is already a ship there!\n");
            return -1;
        } else {
            player->ships = player->ships | mask;

            add_ship_vertical(player, x, y + 1, length - 1);

        }
    }
    else {
        //printf("Successfully added ship\n");
        return 1;
    }


    // implement this as part of Step 2
    // returns 1 if the ship can be added, -1 if not
    // hint: this can be defined recursively
}