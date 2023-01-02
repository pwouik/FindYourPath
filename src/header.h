#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include "type.h"
#include "init.h"
#include "get_functions.h"
#include "print_functions.h"
#include "save.h"
#include "level.h"

//True&False
#define TRUE 0
#define FALSE 1

//Color
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KWHT  "\x1B[37m"

//Case types
#define BOUCLE 5
#define VICTORY 4
#define TREE 3
#define HEAL 2
#define GROUND 1

#endif