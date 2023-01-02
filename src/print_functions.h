#ifndef PRINT_FUNCTIONS_H
#define PRINT_FUNCTIONS_H


#include "header.h"

void print_map(Player* player, Case** map, LevelSettings* level);
void print_dist(Player* player, Case** map, LevelSettings* level);
void print_history(Case** map, History* history, int history_size);
void refresh(Player* player, Case** map, LevelSettings* level, History* history, int history_size, int cancel_remaining, int boucle);
void print_win(Player* player, Case** map, LevelSettings* level, History* history, int history_size, int cancel);
void print_lose(Player* player, Case** map, LevelSettings* level, History* history, int history_size, int cancel);
int is_it_in(int j, int i, History* history, int history_size);
int is_it_shortest(int j, int i, PathfindResult path);
void print_end_map(Player* player, Case** map, LevelSettings* level, History* history, int history_size);
void print_save ();
char** print_dir (int get_finished);
void print_anim (Case** map, LevelSettings* level, History* history, int history_size, int velocity);

#endif