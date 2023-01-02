#ifndef LEVEL_H
#define LEVEL_H

#include "header.h"

PathfindResult pathfind(Case** map,LevelSettings* level);
void move (Case** map, LevelSettings* level, History* history, Player player, int history_size, int cancel_remaining);
void level(LevelSettings level);


#endif