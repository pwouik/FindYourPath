#ifndef INIT_H
#define INIT_H

#include "header.h"

Case** init_map(LevelSettings* level);
Player init_player(LevelSettings* level);
History* init_history(Player player, Case** map);
void set_type(Case** map, int x, int y, uint8_t type);
void set_type_boucle(Case** map, int i, int j, History* history, int history_size);

#endif