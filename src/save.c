#include "header.h"

//-----------------------------[ Saving ]-----------------------------//

void save(Case** map, History* history, uint8_t finished, LevelSettings* level, int history_size, int cancel){
    // Fonction permettant la savegarde des parties
    char deb_path[50];
    char* path = (char*)malloc(150);
    char* format = (char*)malloc(500);
    time_t temps;

    time(&temps);
    strftime(format, 128, "%m%j%H%M%S", localtime(&temps)); // On récupère le mois, le jour, l'heure, les minutes et les secondes de la date actuelle

    if (finished == TRUE)strcpy (deb_path, "./save_finished/game_"); // Si c'est une sauvegarde de fin de partie alors on entre dans ./save_finished
    else strcpy (deb_path, "./save_load/game_"); // Sinon on entre dans ./save_load

    switch (level->map_size){ // Selon la taille de la carte
        case 5: // Cas difficulté facile
            path = strcat(deb_path, strcat(format, "_facile.txt")); // On sauvegarde un fichier sous la forme game_date_facile.txt
            break;
        case 8:
            path =  strcat(deb_path, strcat(format, "_moyen.txt")); // On sauvegarde un fichier sous la forme game_date_moyen.txt
            break;
        case 12:
            path =  strcat(deb_path, strcat(format, "_difficile.txt")); // On sauvegarde un fichier sous la forme game_date_difficile.txt
            break;
        default:
            break;
    }
    FILE* fp = fopen(path, "w"); // On ouvre le fichier
    if (fp == NULL){ // Si le fichier ne s'est pas ouvert on sort
        perror("\n ERREUR : Chargement du fichier impossible");
        exit(0);
    }else{ // Sinon
        fprintf (fp, "%d \n", level->map_size); // On écrit la taille de la carte
        for (int i = 0; i<level->map_size; i++){
            for (int j = 0; j<level->map_size; j++){
                fprintf (fp, "[%d,%d,%d,%d,%d]", map[i][j].type, map[i][j].E, map[i][j].S, map[i][j].SE, map[i][j].SO); // On écrit la carte
            }
        }
        fprintf(fp, "\n");
        for (int i = 0; i<history_size; i++){// On écrit l'historique
            fprintf (fp, "[%d,%d,%d,%d,%d,%d]", history[i].player_state.dist, history[i].player_state.stamina, history[i].player_state.stamina_added, history[i].player_state.stamina_removed, history[i].player_state.x, history[i].player_state.y);
        }
        fprintf(fp, "\n%d", cancel); // On écrit le nombre de retours en arrière disponibles
        fclose(fp); // On ferme le fichier
    }
}

//----------------------------------

void recup_dir (char** Tab, int pos, int is_finished){
    // Fonction de récupération en fichier 
    LevelSettings level;
    History* history;
    int history_size;
    int cancel;
    char* path = (char*)malloc(350);

    if (is_finished == 1){ //Si on récupère un fichier d'une partie finie
        strcpy(path, "./save_finished/");
    }else{ // Sinon
        strcpy(path, "./save_load/");
    }
    // On créée la chaine caractère contenant le chemin vers le fichier entré par l'utilisateur
    const char* end_path = strcat (Tab[pos], "");
    strcat (path, end_path);
    puts ("Test 0");
    // On appelle toutes les fonctions nous donnant les informations désirées
    level.map_size = get_mapSize(path);
    puts("Test 1");
    Case** map = Get_map(level.map_size, path);
    puts("Test 2");
    history = get_Histo(path);
    puts("Test 3");
    history_size = getHistorySize (path);
    puts("Test 4");
    cancel = getCancel(path);
    puts("Test 6");
    // On libère l'espace mémoire de path
    free(path); 
    if (is_finished == 0){ // Si on utilise une partie non finie, on appelle la fonction de déplacement
        move (map, &level, history, history[history_size].player_state, history_size, cancel); 
    }else{ // Sinon, on appelle la fonction lançant l'animation
        choose_velocity(map, &level, history, history_size);
    }
}  

//----------------------------------

void menu_save (int get_finished){
    // Fonction permettant d'afficher et de choisir un fichier et de récupérer une partie sauvegarder dans un fichier
    char** Tab = print_dir(get_finished); // On récupère les éléments des répertoires
    int i;
    // On demande à l'utilisateur d'entrer la position dans le tableau du fichier souhaité
    puts("Entrer le numéro du fichier que vous voulez");
    scanf("%d", &i);
    if (i > 1){ // Si i est différent de "." et de ".."
        recup_dir(Tab, i, get_finished); //On récupère les informations en fichier
    }else{ //Sinon, on sort
        puts("ERREUR: '.' ou '..' n'est pas un fichier valide.");
        exit(0);
    }
}

//----------------------------------

void choose_velocity (Case** map, LevelSettings* level, History* history, int history_size){
    // Fonction permettant de choisir la vitesse de l'animation
    int choix;
    choix = get_Vitesse();
    switch (choix){
    case 1: // Cas 1: On appelle la fonction d'animation avec 3 secondes d'attente
        print_anim(map, level, history, history_size, 3);
        break;
    case 2: // Cas 2: On appelle la fonction d'animation avec 2 secondes d'attente
        print_anim(map, level, history, history_size, 2);
        break;
    case 3: // Cas 3: On appelle la fonction d'animation avec 1 secondes d'attente
        print_anim(map, level, history, history_size, 1);
        break;
    default:
        break;
    }
}