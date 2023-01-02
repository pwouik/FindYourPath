#ifndef GET_FUNCTIONS_H
#define GET_FUNCTIONS_H


#include "header.h"

uint8_t get_type(Case** map, int x, int y);
int get_dist(Case** map,LevelSettings* level,int i,int j,int direction);
int getDirLength (char* path);
int get_mapSize(char* path);
Case** Get_map (int map_size, char* path);
History* get_Histo (char* path);
int getHistorySize(char* path);
int getCancel(char* path);
int get_Vitesse();

#endif