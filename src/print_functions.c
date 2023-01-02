#include "header.h"

//-----------------------------[ Function Print ]-----------------------------//

void print_map(Player* player, Case** map, LevelSettings* level){
    // Fonction permettant d'afficher la carte
    puts("\e[1;1H\e[2J"); // On clear le terminal
    for(int i=0;i<level->map_size;i++){
        for(int j=0;j<level->map_size;j++){

            switch(get_type(map, j, i)){ // Suivant le type de la case
                case 5: // Cas 5 (le joueur repasse sur une case)
                    printf("%s~ ",KYEL); // On affiche ~ en jaune
                    break;
                case 4: // Cas 4 (l'arrivée)
                    printf("%sH ",KYEL); // On affiche H en jaune
                    break;
                case 3: // Cas 3 (les obstacles)
                    printf("%sX ",KMAG); // on affiche X en magenta
                    break;
                case 2: // Cas 2 (les bonus)
                    printf("%s+ ",KGRN); // On affiche + en vert
                    break;
                default: // Cas par défaut
                    printf("%s- ",KWHT); // On affiche - en blanc
                    break;
            }
        }
        printf("\n");
    }
    // On affiche * en bleu (représentation du joueur avec les bonnes coordonnées)
    printf(KBLU);
    printf("\e[%d;%dH\e[1m*\e[0m", player->y+2, player->x*2+1);
    printf("\e[%dH", level->map_size);
}

//----------------------------------

void print_dist(Player* player, Case** map, LevelSettings* level) {
    //Fonction affichant les distances autour du joueur
    int x = player->x;
    int y = player->y;
    // On affiche en blanc les touches pour jouer et les distances autour du joueur
    printf(KWHT);
    printf("%d %d %d     a z e \n%d %s* ", get_dist(map,level,x,y,7), get_dist(map,level,x,y,0), get_dist(map,level,x,y,1), get_dist(map,level,x,y,6), KBLU);
    
    printf(KWHT);
    printf("%d     q - d \n%d %d %d     w x c \n", get_dist(map,level,x,y,2), get_dist(map,level,x,y,5), get_dist(map,level,x,y,4), get_dist(map,level,x,y,3));
}

//----------------------------------

void print_history(Case** map, History* history, int history_size) {
    //Fonction permettant d'afficher les coordonnées du joueur contenu dans l'historique
    printf("\e[4mHistorique des mouvements :\e[0m [0,0]");
    for(int i=1;i<history_size;i++) {
        // Si le joueur est déjà passé sur la case, on affiche ses coordonnées en jaune
        if(get_type(map, history[i].player_state.x, history[i].player_state.y) == BOUCLE) printf(" → %s[%d,%d]%s", KYEL, history[i].player_state.x, history[i].player_state.y, KWHT);
        // Sinon on affiche en blanc les coordonnées
        else printf(" → [%d,%d]", history[i].player_state.x, history[i].player_state.y);
    }
}

//----------------------------------

void refresh(Player* player, Case** map, LevelSettings* level, History* history, int history_size, int cancel_remaining, int boucle) {
    // Fonction permettant de tout afficher
    print_map(player, map,level); // On affiche la carte
    printf("\n\n\n");

    if(boucle) printf("%sVous venez d'effectuer une boucle vers la case [%d,%d] !%s\n\n", KYEL, player->x, player->y, KWHT); //Alerte en cas de boucle

    print_dist(player, map,level); // On affiche les distance autour du joueur
    printf("\n");

    print_history(map, history, history_size); // On affiche les coordonnées contenue dans l'historique
    printf("\n\n");

    //Changement de la couleur de l'énergie restante en fonction de son nombre
    printf("\e[4mEnergie restante :\e[0m");
    if(player->stamina<=3) printf(KRED); 
    else if (player->stamina<=6) printf(KYEL);
    else printf(KGRN);
    printf(" %d%s \n", player->stamina, KWHT);

    //On affiche la distance parcourue
    printf("\e[4mDistance parcourue :\e[0m %s%d%s \n", KBLU, player->dist, KWHT);

    //On affiche le nombre de retours en arrière disponibles
    printf("\n\e[2mTapez \e[1mt\e[0m\e[2m pour sauvegarder la partie et quitter.\nTapez \e[1mr\e[0m\e[2m pour annuler votre précédent coup. Annulation possible restantes : %d\e[0m", cancel_remaining);
}

//-----------------------------[ Function End-Game ]-----------------------------//

void print_win(Player* player, Case** map, LevelSettings* level, History* history, int history_size, int cancel){
    //Fonction affichant l'écran de victoire
    printf ("\e[1m Victoire ! Vous avez reussi ! \e[0m\n\n");

    //Changement de la couleur de l'énergie restante en fonction de son nombre
    printf("   %sEnergie restante : ", KWHT);
    if(player->stamina<=3) printf(KRED);
    else if (player->stamina<=6) printf(KYEL);
    else printf(KGRN);
    printf("%d%s \n", player->stamina, KWHT);

    // Affichage de la distance parcourue, de la santé ajoutée et supprimée
    printf("   Distance parcourue : %s%d%s \n   Energie gagnée : +%d \n   Energie perdue: -%d \n\n\n", KBLU, player->dist, KWHT, player->stamina_added, player->stamina_removed);

    // Appel de la savegarde
    save(map, history, TRUE, level, history_size, cancel);
}

//----------------------------------

void print_lose(Player* player, Case** map, LevelSettings* level, History* history, int history_size, int cancel){
    // Fonction affichant l'écran de défaite
    printf ("\e[1m Défaite, vous n'avez plus assez d'energie... \e[0m\n\n");

    // Affichage de l'énergie restante (0), de la distance parcourue, de l'énergie ajoutée et supprimée
    printf("   %sEnergie restante : %s0%s \n   Distance parcourue : %s%d%s \n   Energie gagnée : +%d \n   Energie perdue: -%d \n\n\n", KWHT, KRED, KWHT, KBLU, player->dist, KWHT, player->stamina_added, player->stamina_removed);

    // Appel de la sauvegarde
    save(map, history, TRUE, level, history_size, cancel);
}

//-----------------------------[ Print End-Map ]-----------------------------//

int is_it_in(int j, int i, History* history, int history_size) {
    // Fonction permettant de savoir si le joueur est passé par la case [i,j]
    for(int n = 0; n < history_size; n++){
        if(history[n].player_state.x == i && history[n].player_state.y == j) return 1;
    }
    return 0;
}

int is_it_shortest(int j, int i, PathfindResult path) {

    for(int n=0;n<path.size;n++){
        if( path.path[n*2]==i && path.path[n*2+1]==j ) return 1;
    }
    return 0;
}
//----------------------------------

void print_end_map(Player* player, Case** map, LevelSettings* level, History* history, int history_size) {
    // Fonction permettant d'afficher le chemin parcouru par le joueur à la fin de la partie
    printf("\e[1;1H\e[2J");
    PathfindResult path = pathfind(map,level);
    for(int i=0;i<level->map_size;i++){
        for(int j=0;j<level->map_size;j++){
            if(is_it_shortest(i,j,path))
            {
                printf("%s",KBLU);
            }
            switch(get_type(map, j, i)){

                case 4:
                    if( is_it_in(i,j,history,history_size) ) printf("%s\e[1mH\e[0m",KBLU); //Le joueur est-il allé jusqu'à la fin ?
                    else printf("%sH ",KYEL);
                    break;
                case 3:
                    printf("%sX",KMAG);
                    break;
                case 2:
                    printf("%s+",KGRN);
                    break;
                default:
                    if( is_it_in(i,j,history,history_size) ) {
                        printf("%s\e[1m*",KRED); 
                    }
                    else printf("%s-",KWHT);
                    break;
            }
            printf("\e[0m ");
        }
        printf("\n%s", KWHT);
    }
}

//----------------------------------

void print_save (){
    // Fonction affichant la sauvegarde
    printf ("\e[1m Vous avez sauvegardé la partie... \e[0m\n\n");
}

//----------------------------------

char** print_dir (int get_finished){
    // Fonction affichant les éléments d'un répertoire
    struct dirent *dir;
    char* path = (char*)malloc(sizeof(char)*300);
    // Si on récupère une partie terminée, alors le répertoire est ./save_finished, sinon c'est ./save_load
    if (get_finished == 1) path = "./save_finished/";
    else path = "./save_load/";

    DIR *d = opendir(path); // On ouvre le répertoire
    int i = 0;
    int n = getDirLength(path); // On récupère la taille du repertoire
    char** Tab = (char**)malloc(sizeof(char*)*n); // On alloue de la mémoire pour un tableau de chaine de caratères
    if (d){ // Si d est ouvert
        while ((dir = readdir(d)) != NULL){ // Tant qu'il y a des éléments dans le répertoire
            Tab[i] = dir->d_name; // On met le nom de l'élément dans le tableau
            printf("%d: %s \n", i, Tab[i]); // On affiche l'élémént
            i++;
        }
        closedir(d); // On ferme le répertoire
    }else{ // Sinon on sort
        perror("ERREUR dans l'ouverture du répertoire");
        exit(0);
    }  

    // On retourne le tableau
    return Tab; 
}

//----------------------------------

void print_anim (Case** map, LevelSettings* level, History* history, int history_size, int velocity){
    // Fonction permettant d'afficher l'animation de revisionnage de parties terminées 
    for (int i = 0; i<history_size; i++){
        refresh(&history[i+1].player_state, map, level, history, history_size, 0, 0); // On affiche la carte avec toutes les informations
        fflush(stdout); // On vide le buffer de sortie
        sleep(velocity); // On attend, d'une durée de l'entier contenu dans velocity
    }
    // On clear le terminal et on sort
    puts("\e[1;1H\e[2J");
    exit(0);
}
