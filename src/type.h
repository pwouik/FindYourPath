#ifndef TYPE_H
#define TYPE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <time.h>

typedef struct {
    int map_size;
    int dist_max;
    int stamina;
    int tree;
    int heal;
    int ground;
} LevelSettings;

typedef struct {
    uint8_t type;
    uint8_t E;
    uint8_t SE;
    uint8_t S;
    uint8_t SO;
} Case;

typedef struct {
    int x;
    int y;
    int stamina;
    int stamina_added;
    int stamina_removed;
    int dist;
} Player;

typedef struct{
    uint8_t type;
    Player player_state;
} History;

typedef struct {
    int* path;
    int size;
} PathfindResult;

#endif