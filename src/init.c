#include "header.h"

//-----------------------------[ Init / Set ]-----------------------------//

Case** init_map(LevelSettings* level){
    // Fonction permettant d'initialiser la carte
    srand( time( NULL ) );
    Case** map = (Case**)malloc(sizeof(Case*)*level->map_size); // Allocation mémoire de la map

    for(int i=0;i<level->map_size;i++){
        map[i]= (Case*)malloc(sizeof(Case)*level->map_size); // Allocation mémoire de chaque case de la map

        for(int j=0;j<level->map_size;j++){

            int n=rand()%(level->tree+level->heal+level->ground); // On détermine le type du sol aléatoirement
            uint8_t type=0;

            if(n<level->tree){ // Si n nous donne un obstacle alors le type de la case est un obstacle
                type=TREE;
            }
            else if(n<level->tree + level->heal){ // Si n nous donne un bonus alors le type de la case est un bonus
                type=HEAL;
            }
            else { // Sinon le type de la case est du sol
                type=GROUND;
            }
            // On donne à la case le type, la distance de la position EST, SUD-EST, SUD et SUD-OUEST
            map[i][j]=(Case){type, rand()%level->dist_max+1, rand()%level->dist_max+1, rand()%level->dist_max+1, rand()%level->dist_max+1};
        }
    }
    map[0][0].type=1; // On initialise le type de la case de départ à du sol vide
    map[level->map_size-1][level->map_size-1].type=4; // On initialise le dernier élémént de la carte à la maison
    // On renvoie la carte
    return map;
}

//----------------------------------

Player init_player(LevelSettings* level){
    //Fonction permettant d'initialiser le joueur
    Player player;

    // On initialise les position à 0
    player.x=0;
    player.y=0;
    // On lui met l'endurance envoyée en paramètres, qui est l'endurance max
    player.stamina=level->stamina;
    // On initalise la distance parcourue, l'endurance ajoutée et supprimée à 0
    player.stamina_added=0;
    player.stamina_removed=0;
    player.dist=0;
    // On retourne le joueur initialisé
    return player;
}

//----------------------------------

History* init_history(Player player, Case** map){
    // FOnction pour initaliser l'historique
    History* history = (History*)malloc(sizeof(History)); // On alloue la mémoire
    history[0].player_state = player; // On met les informations du joueur dans le tableau
    history[0].type = 1; // On met le type de la case sur le quel le joueur est dans le tableau
    // On retourne l'historique initialisé
    return history;
}

//--------------------------

void set_type(Case** map, int x, int y, uint8_t type){
    // Fonction permettant de mettre le type passé en paramètre dans le type de la map
    map[y][x].type=type;
}

//--------------------------

void set_type_boucle(Case** map, int i, int j, History* history, int history_size){
    // Fonction permettant de savoir si le joueur passe par le même endroit
    int start = 0;

    for(int n=0;n<history_size;n++){

        if( history[n].player_state.x==i && history[n].player_state.y==j ) start = 1; //Case où le joueur est actuellement

        if(start) set_type(map, history[n].player_state.x, history[n].player_state.y, BOUCLE); //Si on est passé par la case de laquelle faire la boucle on change les types des cases
        else if(get_type(map,history[n].player_state.x,history[n].player_state.y)==BOUCLE) set_type(map,history[n].player_state.x,history[n].player_state.y,GROUND); //Supprimer l'ancienne boucle, si elle existe, si on a pas encore commencé à faire la nouvelle
    }
}
