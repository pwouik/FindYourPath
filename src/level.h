//---------------------[ Include ]---------------------//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <time.h>

//---------------------[ Define ]---------------------//

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
#define VICTORY 4
#define TREE 3
#define HEAL 2
#define GROUND 1

//---------------------[ Struct ]---------------------//

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

//---------------------[ Init / Set ]---------------------//

Case** init_map(LevelSettings* level){

    srand( time( NULL ) );
    Case** map = (Case**)malloc(sizeof(Case*)*level->map_size);

    for(int i=0;i<level->map_size;i++){
        map[i]= (Case*)malloc(sizeof(Case)*level->map_size);

        for(int j=0;j<level->map_size;j++){

            int n=rand()%(level->tree+level->heal+level->ground);
            uint8_t type=0;

            if(n<level->tree){
                type=TREE;
            }
            else if(n<level->tree + level->heal){
                type=HEAL;
            }
            else {
                type=GROUND;
            }
            map[i][j]=(Case){type, rand()%level->dist_max+1, rand()%level->dist_max+1, rand()%level->dist_max+1, rand()%level->dist_max+1};
        }
    }
    map[0][0].type=1;
    map[level->map_size-1][level->map_size-1].type=4;
    
    return map;
}

//--------------------------

Player init_player(LevelSettings* level){

    Player player;

    player.x=0;
    player.y=0;
    player.stamina=level->stamina;
    player.stamina_added=0;
    player.stamina_removed=0;
    player.dist=0;

    return player;
}

//--------------------------

void set_type(Case** map, int x, int y, uint8_t type){
    map[y][x].type=type;
}

uint8_t get_type(Case** map, int x, int y){
    return map[y][x].type;
}
//---------------------[ Function Get ]---------------------//

int get_dist(Case** map,LevelSettings* level,int i,int j,int direction){
    switch(direction){
        case 0:
            if(j>0){
                return map[i][j-1].S;
            } else {
                return 0;
            }
            break;
        case 1:
            if(j>0 && i<level->map_size-1){
                return map[i+1][j-1].SO;
            } else {
                return 0;
            }
            break;
        case 2:
            if(i<level->map_size-1){
                return map[i][j].E;
            } else {
                return 0;
            }
            break;
        case 3:
            if(j<level->map_size-1 && i<level->map_size-1){
                return map[i][j].SE;
            } else {
                return 0;
            }
            break;
        case 4:
            if(j<level->map_size-1){
                return map[i][j].S;
            } else {
                return 0;
            }
            break;
        case 5:
            if(j<level->map_size-1 && i>0){
                return map[i][j].SO;
            } else {
                return 0;
            }
            break;
        case 6:
            if(i>0){
                return map[i-1][j].E;
            } else {
                return 0;
            }
            break;
        case 7:
            if(i>0 && j>0){
                return map[i-1][j-1].SE;
            } else {
                return 0;
            }
            break;
    }
}

//--------------------------


//---------------------[ Function Print ]---------------------//

void print_map(Player* player, Case** map,LevelSettings* level){
    printf("\e[1;1H\e[2J");

    for(int i=0;i<level->map_size;i++){
        for(int j=0;j<level->map_size;j++){

            switch(get_type(map, j, i)){

                case 4:
                    printf("%sH ",KYEL);
                    break;
                case 3:
                    printf("%sX ",KMAG);
                    break;
                case 2:
                    printf("%s+ ",KGRN);
                    break;
                default:
                    printf("%s- ",KWHT);
                    break;
            }
        }
        printf("\n");
    }
    printf(KBLU);
    printf("\e[%d;%dH\e[1m*\e[0m", player->y+1, player->x*2+1);
    printf("\e[%dH", level->map_size);
}

//--------------------------

void print_dist(Player* player, Case** map, LevelSettings* level) {

    int x = player->x;
    int y = player->y;

    printf(KWHT);
    printf("%d %d %d     a z e \n%d %s* ", get_dist(map,level,x,y,7), get_dist(map,level,x,y,0), get_dist(map,level,x,y,1), get_dist(map,level,x,y,6), KBLU);
    
    printf(KWHT);
    printf("%d     q - d \n%d %d %d     w x c \n", get_dist(map,level,x,y,2), get_dist(map,level,x,y,5), get_dist(map,level,x,y,4), get_dist(map,level,x,y,3));
}

//--------------------------

void refresh(Player* player, Case** map, LevelSettings* level) {

    print_map(player, map,level);
    printf("\n\n");
    print_dist(player, map,level);
    printf("\n");

    printf("\e[4mEnergie restante :\e[0m");

    if(player->stamina<=3) printf(KRED); //Changement de la couleur de l'énergie restante en fonction de son nombre
    else if (player->stamina<=6) printf(KYEL);
    else printf(KGRN);
    printf(" %d%s \n", player->stamina, KWHT);

    printf("\e[4mDistance parcourue :\e[0m %s%d%s \n", KBLU, player->dist, KWHT);
}

//---------------------[ Function End-Game ]---------------------//

void print_win(Player* player){

    printf ("\e[1m Victoire ! Vous avez reussi ! \e[0m\n\n");

    printf("   %sEnergie restante : ", KWHT);
    if(player->stamina<=3) printf(KRED); //Changement de la couleur de l'énergie restante en fonction de son nombre
    else if (player->stamina<=6) printf(KYEL);
    else printf(KGRN);
    printf("%d%s \n", player->stamina, KWHT);

    printf("   Distance parcourue : %s%d%s \n   Energie gagnée : +%d \n   Energie perdue: -%d \n\n\n", KBLU, player->dist, KWHT, player->stamina_added, player->stamina_removed);
}

//--------------------------

void print_lose(Player* player){

    printf ("\e[1m Défaite, vous n'avez plus assez d'energie... \e[0m\n\n");
    printf("   %sEnergie restante : %s0%s \n   Distance parcourue : %s%d%s \n   Energie gagnée : +%d \n   Energie perdue: -%d \n\n\n", KWHT, KRED, KWHT, KBLU, player->dist, KWHT, player->stamina_added, player->stamina_removed);
}

//---------------------[ Other ]---------------------//

//---------------------[ Level1 ]---------------------//

void level(LevelSettings level) {
    Case** map=init_map(&level);
    Player player=init_player(&level);
    History* history = NULL;
    int history_size = 0;
    refresh(&player, map,&level);
    //Init variable
    char ch;
    int x, y;
    int launch = TRUE;
    int victory = FALSE;
    int dist;

    while (launch == 0){
        puts("\n\nProchain déplacement :");
        scanf(" %c", &ch);
        switch (ch){
        case 'z':
            x = player.x;
            y = player.y - 1;
            dist = get_dist(map,&level,player.x,player.y,0);
            break;
        case 'x':
            x = player.x;
            y = player.y + 1;
            dist = get_dist(map,&level,player.x,player.y,4);
            break;
        case 'd':
            x = player.x + 1;
            y = player.y;
            dist = get_dist(map,&level,player.x,player.y,2);
            break;
        case 'q':
            x = player.x - 1;
            y = player.y;
            dist = get_dist(map,&level,player.x,player.y,6);
            break;
        case 'a':
            y = player.y - 1;
            x = player.x - 1;
            dist = get_dist(map,&level,player.x,player.y,7);
            break;
        case 'e':
            y = player.y - 1;
            x = player.x + 1;
            dist = get_dist(map,&level,player.x,player.y,1);
            break;
        case 'w':
            y = player.y + 1;
            x = player.x - 1;
            dist = get_dist(map,&level,player.x,player.y,5);
            break;
        case 'c':
            y = player.y + 1;
            x = player.x + 1;
            dist = get_dist(map,&level,player.x,player.y,3);
            break;
        case '\n':
            continue;
        default:
            printf("'%c' n'est pas une entrée valide.",ch);
            continue;
        }
        if (y >= 0 && x >= 0 && x < level.map_size && y < level.map_size){
            history_size++;
            history = (History*)realloc(history,sizeof(History)*history_size);
            history[history_size-1].player_state = player;
            history[history_size-1].type = get_type(map, player.x, player.y);
            if (get_type(map, x, y) == TREE){
                player.stamina -= 10;
                player.stamina_removed += 10;
            }else{
                player.x = x;
                player.y = y;
                if (get_type(map, x, y) == HEAL){
                    set_type(map,x,y,GROUND);
                    player.stamina += 9;
                    player.stamina_added+=9;
                    player.dist += dist;
                }else if (get_type(map, x, y) == VICTORY){
                    player.stamina--;
                    launch = FALSE;
                    victory = TRUE;
                    player.dist += dist;
                }else{
                    player.stamina--;
                    player.stamina_removed ++;
                    player.dist += dist;
                }
            }
        }
        if (player.stamina <= 0) launch = FALSE;

        refresh(&player, map,&level);

        if (launch == FALSE){
            print_map(&player, map,&level);

            printf ("\n\n%s\e[1m - - - - La partie est finie - - - -  \e[0m\n \n %s", KRED, KWHT);

            if (victory == TRUE) print_win(&player);
            else print_lose(&player);

            exit(0);
        }
    }
}