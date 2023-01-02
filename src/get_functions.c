#include "header.h"

uint8_t get_type(Case** map, int x, int y){
    // On retourne le type de la case
    return map[y][x].type;
}

//----------------------------------

int get_dist(Case** map,LevelSettings* level,int i,int j,int direction){
    // Fonction permettant de connaître les distances
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

//----------------------------------

int getDirLength (char* path){
    // Fonction renvoyant le nombre d'éléments dans le répertoire
    struct dirent *dir;
    int i = 0;
    DIR *d = opendir(path); // On ouvre le répertoire
    if (d) { // Si d s'est bien ouvert
        while ((dir = readdir(d)) != NULL){ // Tant qu'il y a des éléments on incrémente i
            i++;
        }
        closedir(d); // On ferme le répertoire
    }else{ // Sinon, on sort
        perror("ERREUR dans l'ouverture du répertoire");
        exit(0);
    }
    return i;
}

//----------------------------------

int get_mapSize(char* path){
    // Fonction permettant de récupérer la taille de la carte en fichier
    FILE* fo = fopen (path, "r"); // On ouvre le fichier 
    char c;
    int map_size;
    int compt = 0;
    char str[500] = "";
    if (fo == NULL){ // Si le fichier ne s'est pas ouvert, on sort
        perror("ERREUR: Impossible de charger le fichier");
        exit(0);
    }else{ // Sinon
        while (fread(&c, 1, 1, fo)){ // Tant que l'on lit dans le fichier
            if (c != '\n'){ // Si on ne saute pas de lignes
                if (compt == 0){ // Si on est à la première ligne
                    strncat(str, &c, 1); // On ajoute le caractère c à la chaine str
                }
            }else{ // Sinon
                sscanf(str, "%d", &map_size); // On lit la taille de la carte dans la chaine str
                compt++; // On incrémente le compteur
            }
        }
    }

    // On retourne la taille de la carte
    return map_size;
}

//----------------------------------

Case** Get_map (int map_size, char* path){
    // Fonction permettant de récupérer la carte en fichier
    FILE* fo = fopen (path, "r"); // On ouvre le fichier
    char c;
    int i = 0, j = 0, inside = 0;
    char str[500];
    // On fait les allocations mémoires de la carte
    Case** map = (Case**)malloc(sizeof(Case*)*map_size);
    for (int k = 0; k<map_size; k++){
        map[k] = (Case*)malloc(sizeof(Case)*map_size); 
    }
    int compt = 0;
    if (fo == NULL){ // Si le fichier ne s'est pas ouvert, alors on sort
        perror("ERREUR: Impossible de charger le fichier");
        exit(0);
    }else{ // Sinon
        while (fread(&c, 1, 1, fo)){ // Tant qu'on lit dans le fichier
            if (c != '\n'){ // Si on ne saute pas de ligne 
                if (compt == 1){ // Si on est dans la ligne contenant les informations de la carte
                    // Si on est dans un des éléments de la carte
                    if (c != '['){
                        if (c != ']'){ // Si on ne change pas de case
                            if (c != ','){
                                if (i == map_size && j == map_size){ // Si on est arrivé à la fin
                                    compt++;
                                }else{ // Sinon
                                    strncat(str, &c, 1); // On ajoute le caractère c dans str
                                }
                            }else{ 
                                switch (inside){
                                case 0: // Cas 0, on scan le type de la carte dans la chaine str
                                    sscanf(str, "%hhd", &map[i][j].type);
                                    break;
                                case 1: // Cas 1, on scan la distance dans la direction EST dans la chaine str
                                    sscanf(str, "%hhd", &map[i][j].E);
                                    break;
                                case 2: // Cas 2, on scan la distance dans la direction SUD dans la chaine str
                                    sscanf(str, "%hhd", &map[i][j].S);
                                    break;
                                case 3: // Cas 3, on scan la distance dans la direction SUD-EST dans la chaine str
                                    sscanf (str, "%hhd", &map[i][j].SE);
                                    break;
                                case 4: // Cas 4, on scan la distance dans la direction SUD-OUEST dans la chaine str
                                    sscanf(str, "%hhd", &map[i][j].SO);
                                    break;
                                default:
                                    break;
                                }
                                // On incrémente inside et on vide str
                                inside++;
                                strcpy(str, "");
                            }
                        }else{// Sinon on change de case
                            j++; // On incrémente j
                            if (j == map_size){ // Si la ligne est remplie on incremente la colonne
                                j = 0;
                                i++;
                            }
                            // On vide inside et str
                            inside = 0; 
                            strcpy(str, "");
                        }
                    }else{// Sinon on vide str
                        strcpy(str, "");
                    }
                }
            }else{ // Sinon on incrémente le compteur
                compt++;
            }
        }
    }

    // On retourne la carte
    return map;
}

//----------------------------------

History* get_Histo (char* path){
    // Fonction permettant de récupérer l'historique en fichier
    FILE* fo = fopen (path, "r"); // On ouvre le fichier
    char c;
    Player player;
    int compt = 0;
    int inside = 0;
    int history_size = 1;
    History* history = (History*)malloc(sizeof(History)); // On alloue la mémoire pour l'historique
    char str[500];
    if (fo == NULL){ // Si le fichier ne s'est pas ouvert, alors on sort
        perror("ERREUR: Impossible de charger le fichier");
        exit(0);
    }else{ // Sinon
        while (fread(&c, 1, 1, fo)){ // Tant que l'on peut lire des caractères
            if (c!='\n'){ // Si on ne saute pas de ligne
                if (compt == 2){ // Si on est dans la ligne contenant les informations de l'historique 
                    if (c == '['){ // Si on ouvre un nouvel élément, on vide str
                        strcpy(str, "");
                    }else{ // Sinon
                        if (c != ']'){ // Si on ne ferme pas un élément
                            if (c != ','){ // Si on ne croise pas le délimiteur
                                strncat(str, &c, 1); // On ajoute c à str
                            }else{ // Sinon
                                switch (inside){ // Selon où on est dans l'élément
                                case 0: // Cas 0: On lit la distance parcourue par le joueur
                                    sscanf(str, "%d", &player.dist);
                                    break;
                                case 1: // Cas 1: On lit l'énergie du joueur
                                    sscanf(str, "%d", &player.stamina);
                                    break;
                                case 2: // Cas 2: On lit l'énergie ajoutée au joueur
                                    sscanf (str, "%d", &player.stamina_added);
                                    break;
                                case 3: // Cas 3: On lit l'énergie supprimée au joueur
                                    sscanf(str, "%d", &player.stamina_removed);
                                    break;
                                case 4: // Cas 4: On lit la position x du joueur
                                    sscanf(str, "%d", &player.x);
                                    break;
                                default:
                                    break;
                                }
                                // On incrémente inside et on vide str pour un nouvel élément (après délémiteur ',')
                                inside++;
                                strcpy(str, "");
                            }
                        }else{ // Sinon
                            // On lit la position y du joueur
                            sscanf(str, "%d", &player.y);

                            // On ajoute le joueur à l'historique
                            history_size++;
                            history = (History*)realloc(history, sizeof(History)*history_size);
                            history[history_size-1].player_state = player;
                            inside = 0;
                        }
                    }
                }
            }else{ // Sinon on incrémente le compteur
                compt++;
            }
        }
    }  

    // On retourne l'historique
    return history;
}

//----------------------------------

int getHistorySize(char* path){
    //Fonction permettant de donner la taille de l'historique 
    FILE* fo = fopen (path, "r"); // On ouvre le fichier
    char c;
    int size = 0;
    int compt = 0;
    if (fo == NULL){ // Si le fichier ne s'est pas ouvert, alors on sort
        perror("ERREUR: Impossible de charger le fichier");
        exit(0);
    }else{ // Sinon
        while (fread(&c, 1, 1, fo)){ // Tant que l'on peut lire des caractères
            if (c != '\n'){ // Si on ne saute pas de lignes
                if (compt == 2){ // Si on est dans la ligne contenant les informations de l'historique 
                    if (c == '['){ // Si on a un nouvel élément, on incrémente la taille de l'historique
                        size++; 
                    }
                }
            }else{ // Sinon, on incrémente la ligne lue
                compt++;
            }
        }
    }
    // On renvoie la taille
    return size;
}

//----------------------------------

int getCancel(char* path){
    //Fonction permettant de récupérer, en fichier, le nombre de retours en arrière disponibles
    FILE* fo = fopen (path, "r"); // On ouvre le fichier
    char c;
    int cancel;
    int compt = 0;
    if (fo == NULL){ // Si le fichier ne s'est pas ouvert, alors on sort
        perror("ERREUR: Impossible de charger le fichier");
        exit(0);
    }else{ // SiSnon
        while (fread(&c, 1, 1, fo)){ // Tant que l'on peut lire des caractères
            if (c != '\n'){ // Si on ne saute pas de lignes
                if (compt == 3){ // Si on est dans la ligne contenant le nombre de retours en arrière disponibles
                    sscanf(&c, "%d", &cancel); // On lit dans c le nombre de retours en arrière disponibles
                }
            }else{ // On change de ligne
                compt++;
            }
        }
    }
    // On retourne cancel
    return cancel;
}

//----------------------------------

int get_Vitesse(){
    // Fonction permettant de savoir à quelle vitesse d'affichage l'animation de visionnage va se dérouler 
    int i;
    puts("Entrer la vitesse que vous voulez (de 1 à 3, avec 1 le plus lent et 3 le plus rapide)");
    scanf("%d", &i);
    if (i>1 && i<4){
        return i;
    }
}