#ifndef SAVE_H
#define SAVE_H

#include "header.h"

void save(Case** map, History* history, uint8_t finished, LevelSettings* level, int history_size, int cancel);
void menu_save (int get_finished);
void choose_velocity (Case** map, LevelSettings* level, History* history, int history_size);
void recup_dir (char** Tab, int pos, int is_finished);

#endif