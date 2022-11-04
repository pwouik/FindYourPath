//---------------------[ Include ]---------------------//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <time.h>

//---------------------[ Define ]---------------------//

//Values
#define MAP_SIZE 8
#define DIST_MAX 8
#define STAMINA 12
#define TRUE 0
#define FALSE 1

//Color
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KWHT  "\x1B[37m"

//---------------------[ Case types ]---------------------//
#define VICTORY 4
#define TREE 3
#define HEAL 2
#define GROUND 1

//---------------------[ Struct ]---------------------//


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

//---------------------[ Init ]---------------------//

Case** init_map(){

    srand( time( NULL ) );
    Case** map = (Case**)malloc(sizeof(Case*)*MAP_SIZE);

    for(int i=0;i<MAP_SIZE;i++){
        map[i]= (Case*)malloc(sizeof(Case)*MAP_SIZE);

        for(int j=0;j<MAP_SIZE;j++){

            int n=rand()%20;
            uint8_t type=0;

            if(n<4){
                type=TREE;
            }
            else if(n<6){
                type=HEAL;
            }
            else {
                type=GROUND;
            }
            map[i][j]=(Case){type, rand()%DIST_MAX+1, rand()%DIST_MAX+1, rand()%DIST_MAX+1, rand()%DIST_MAX+1};
        }
    }
    map[0][0].type=1;
    map[MAP_SIZE-1][MAP_SIZE-1].type=4;
    
    return map;
}

//--------------------------

Player init_player(){

    Player player;

    player.x=0;
    player.y=0;
    player.stamina=STAMINA;
    player.stamina_added=0;
    player.stamina_removed=0;
    player.dist=0;

    return player;
}

//---------------------[ Function ]---------------------//

int get_dist(Case** map,int i,int j,int direction){
    switch(direction){
        case 0:
            if(j>0){
                return map[i][j-1].S;
            } else {
                return 0;
            }
            break;
        case 1:
            if(j>0 && i<MAP_SIZE-1){
                return map[i+1][j-1].SO;
            } else {
                return 0;
            }
            break;
        case 2:
            if(i<MAP_SIZE-1){
                return map[i][j].E;
            } else {
                return 0;
            }
            break;
        case 3:
            if(j<MAP_SIZE-1 && i<MAP_SIZE-1){
                return map[i][j].SE;
            } else {
                return 0;
            }
            break;
        case 4:
            if(j<MAP_SIZE-1){
                return map[i][j].S;
            } else {
                return 0;
            }
            break;
        case 5:
            if(j<MAP_SIZE-1 && i>0){
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

int get_type(Case** map, int x, int y){
    return map[y][x].type;
}

//--------------------------

void set_type(Case** map, int x, int y, uint8_t type){
    map[y][x].type=type;
}

//--------------------------

void print_map(Player player, Case** map){
    printf("\e[1;1H\e[2J");

    for(int i=0;i<MAP_SIZE;i++){
        for(int j=0;j<MAP_SIZE;j++){

            switch(get_type(map, j, i)){

                case 4:
                    printf("%sH ",KYEL);
                    break;
                case 3:
                    printf("%sX ",KRED);
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
    printf("\033[%d;%dH*", player.y+1, player.x*2+1);
    printf("\033[%dH", MAP_SIZE);
}

//--------------------------

void print_dist(Player player, Case** map) {

    int x = player.x;
    int y = player.y;

    printf(KWHT);
    printf("%d %d %d\n%d %s* ", get_dist(map,x,y,7), get_dist(map,x,y,0), get_dist(map,x,y,1), get_dist(map,x,y,6), KBLU);
    
    printf(KWHT);
    printf("%d \n%d %d %d\n", get_dist(map,x,y,2), get_dist(map,x,y,5), get_dist(map,x,y,4), get_dist(map,x,y,3));
}

//--------------------------

void refresh(Player player, Case** map) {

    print_map(player, map);
    printf("\n\n");
    print_dist(player, map);
    printf("\n\n");
    printf("Energie restante : %d \nDistance parcourue : %d", player.stamina, player.dist);
    printf("\n\n");
}

//--------------------------

void aff_victoire(Player player){
    printf ("Bravo vous avez reussi ! \n Il vous reste %s%d%s energie(s) et vous avez parcouru %s%d%sm \n On vous a ajoute %s%d%s energie(s) et on vous en a retire %s%d%s \n\n", KGRN, player.stamina, KWHT, KBLU, player.dist, KWHT, KGRN, player.stamina_added, KWHT, KRED, player.stamina_removed, KWHT);
}

//--------------------------

void move(Player player, Case** map){
    // declare variable
    char ch;
    int x, y;
    int launch = TRUE;
    int victory = FALSE;
    while (launch == 0){
        puts("Entrer ce que vous voulez faire");
        scanf(" %c", &ch);
        switch (ch){
        case 'z':
            x = player.x;
            y = player.y - 1;
            break;
        case 'x':
            x = player.x;
            y = player.y + 1;
            break;
        case 'd':
            x = player.x + 1;
            y = player.y;
            break;
        case 'q':
            x = player.x - 1;
            y = player.y;
            break;
        case 'a':
            y = player.y - 1;
            x = player.x - 1;
            break;
        case 'e':
            y = player.y - 1;
            x = player.x + 1;
            break;
        case 'w':
            y = player.y + 1;
            x = player.x - 1;
            break;
        case 'c':
            y = player.y + 1;
            x = player.x + 1;
            break;
        case '\n':
            continue;
        default:
            printf("Le '%c' n'est pas reconnu. seuls : \n 'a' 'z' 'e' \n 'q'     'd' \n 'w' 'x' 'c' \n",ch);
            continue;
        }
        if (y >= 0 && x >= 0 && x < MAP_SIZE && y < MAP_SIZE){
            player.x = x;
            player.y = y;
            if (get_type(map, x, y) == TREE){
                player.stamina -= 10;
                player.y++;
            }else if (get_type(map, x, y) == HEAL){
                set_type(map,x,y,GROUND);
                player.stamina += 10;
                player.stamina_added+=10;
            }else if (get_type(map, x, y) == VICTORY){
                launch = FALSE;
                victory = TRUE;
            }else{
                player.stamina--;
                player.stamina_removed ++;
            }
        }
        if (player.stamina <= 0){
            launch = FALSE;
        }
        refresh(player, map);
        if (launch == FALSE){
            printf ("%sLa partie est finie \n \n %s", KRED, KWHT);
            if (victory == TRUE){
                aff_victoire(player);
            }
            exit(0);
        }
    }
}
//---------------------[ Level ]---------------------//

int main() {

    Case** map=init_map();
    Player player=init_player();

    refresh(player, map);

    move(player, map);
}