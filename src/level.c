#include "header.h"


const int DIRECTIONS[][2] = {{ 0,-1},
                            { 1,-1},
                            { 1, 0}, 
                            { 1, 1},
                            { 0, 1},
                            {-1, 1},
                            {-1, 0},
                            {-1,-1}};

//----------------------------------

PathfindResult pathfind(Case** map,LevelSettings* level){
    unsigned int** dist_map = (unsigned int**)malloc(level->map_size*sizeof(unsigned int*));
    for(int i=0;i<level->map_size;i++){
        dist_map[i]=(unsigned int*)malloc(level->map_size*sizeof(unsigned int));
        for(int j=0;j<level->map_size;j++){
            dist_map[i][j]=999;
        }
        //memset(dist_map[i],40000000u,level->map_size);
    }
    dist_map[0][0]=0;
    int set_size = 1;
    int* set =  (int*)malloc(2*set_size*sizeof(int));
    set[0] = 0;
    set[1] = 0;
    int x,y;
    do{
        int min_i=0;
        for(int i=2;i<2*set_size;i+=2){
            if(dist_map[set[i+1]][set[i]]<dist_map[set[min_i+1]][set[min_i]]){
                min_i=i;
            }
        }
        x = set[min_i];
        y = set[min_i+1];
        set[min_i]=set[2*set_size-2];
        set[min_i+1]=set[2*set_size-1];
        set_size--;
        set =  realloc(set,2*set_size*sizeof(int));
        for(int i=0;i<8;i++){
            int nx = x+DIRECTIONS[i][1];
            int ny = y+DIRECTIONS[i][0];
            if(nx>=0 && nx<level->map_size && ny>=0 && ny<level->map_size && get_type(map,nx,ny) != TREE)
            {
                int d = get_dist(map,level,y,x,i);
                if(d!=0 && dist_map[y][x]+d<dist_map[ny][nx]){
                    int j=0;
                    while(j<set_size*2 && !(nx==set[j] && ny==set[j+1])){
                        j+=2;
                    }
                    dist_map[ny][nx]=dist_map[y][x]+d;
                    if(j==set_size*2){
                        set_size++;
                        set =  realloc(set,2*set_size*sizeof(int));
                        set[2*set_size-2]=nx;
                        set[2*set_size-1]=ny;
                    }
                }
            }
        }
    }while(set_size != 0 && !(x==level->map_size-1 && y==level->map_size-1));
    PathfindResult path=(PathfindResult){NULL,0};
    if(x==level->map_size-1 && y==level->map_size-1)
    {
        while (x!=0 || y!=0)
        {
            int nx,ny;
            int i;
            for(i=0;i<8;i++){
                nx = x+DIRECTIONS[i][1];
                ny = y+DIRECTIONS[i][0];
                if(nx>=0 && nx<level->map_size && ny>=0 && ny<level->map_size && dist_map[ny][nx]+get_dist(map,level,ny,nx,(i+4)%8)==dist_map[y][x])
                {
                    break;
                }
            }
            path.size++;
            path.path =  realloc(path.path,2*path.size*sizeof(int));
            path.path[2*path.size-2]=nx;
            path.path[2*path.size-1]=ny;
            x=nx;
            y=ny;
        }
    }
    return path;
}

//----------------------------------

void move (Case** map, LevelSettings* level, History* history, Player player, int history_size, int cancel_remaining){
    char ch;
    int x, y;
    int launch = TRUE;
    int victory = FALSE;
    int saved = FALSE;
    int dist;
    int is_cancel = 0;
    int boucle = 0;

    // On appelle l'affichage des différents éléménts une première fois
    refresh(&player, map, level, history, history_size, cancel_remaining, boucle); 

    while (launch == 0){ // Tant que le jeu est cours de partie

        // On demande à l'utilisateur d'entrer un déplacement
        puts("\n\nProchain déplacement :");
        scanf(" %c", &ch);

        switch (ch){ // Selon le déplacement
        case 'z': // Cas 'z' (ligne droite verticale): on ne bouge pas le x et on perd une postion sur le y. On appelle la récupération de la distance
            x = player.x;
            y = player.y - 1;
            dist = get_dist(map,level,player.x,player.y,0);
            break;
        case 'x': // Cas 'x' (ligne droite verticale): on ne bouge pas le x et on gagne une postion sur le y. On appelle la récupération de la distance
            x = player.x;
            y = player.y + 1;
            dist = get_dist(map,level,player.x,player.y,4);
            break;
        case 'd': // Cas 'd' (ligne droite horizontale): on ne bouge pas le y et on gagne une postion sur le x. On appelle la récupération de la distance
            x = player.x + 1;
            y = player.y;
            dist = get_dist(map,level,player.x,player.y,2);
            break;
        case 'q': // Cas 'q' (ligne droite horizontale): on ne bouge pas le y et on perd une postion sur le x. On appelle la récupération de la distance
            x = player.x - 1;
            y = player.y;
            dist = get_dist(map,level,player.x,player.y,6);
            break;
        case 'a': // Cas 'a' (diagonale supérieure gauche): On perd une position sur le y et sur le x. On appelle la récupération de la distance
            y = player.y - 1;
            x = player.x - 1;
            dist = get_dist(map,level,player.x,player.y,7);
            break;
        case 'e': // Cas 'e' (diagonale supérieure droite): On perd une position sur le y et on en gagne une sur le x. On appelle la récupération de la distance
            y = player.y - 1;
            x = player.x + 1;
            dist = get_dist(map,level,player.x,player.y,1);
            break;
        case 'w':  // Cas 'e' (diagonale inférieure gauche): On gagne une position sur le y et on en perd une sur le x. On appelle la récupération de la distance
            y = player.y + 1;
            x = player.x - 1;
            dist = get_dist(map,level,player.x,player.y,5);
            break;
        case 'c': // Cas 'c' (diagonale inférieure droite): On gagne une position sur le y et sur le x. On appelle la récupération de la distance
            y = player.y + 1;
            x = player.x + 1;
            dist = get_dist(map,level,player.x,player.y,3);
            break;
        case 'r': // Cas 'r' (retour en arrière)
            if(cancel_remaining>0 && history_size > 1) { // Si il nous reste des retours en arrière disponiles et que la taille de l'historique plus grande que 1

                if( (player.stamina_added - history[history_size-1].player_state.stamina_added) == 10 ) map[player.y][player.x].type=HEAL; //Le jouer revient en arrière juste après avoir consommé un bonus
                else if( (history[history_size].player_state.stamina_added - history[history_size-1].player_state.stamina_added) == 10) map[player.y][player.x].type=HEAL; //Le jouer revient en arrière après avoir consommé un bonus il y a plus d'un déplacement

                player = history[history_size-2].player_state;
                cancel_remaining--;
                player.stamina--;
                
                if(player.x==0 && player.y==0) player.stamina++;  //Si retour au spawn

                history_size--;
            }
            is_cancel = 1;
            break;
        case 't': //Cas 't' (Sauvegarde)
            save(map, history, FALSE, level, history_size, cancel_remaining); // On lance la sauvegarde
            // On arrête le jeux après la sauvegarde
            launch = FALSE; 
            saved = TRUE;
            break;
        case '\n':
            continue;
        default:
            printf("'%c' n'est pas une entrée valide.",ch);
            continue;
        }
        // Actualisation des valeurs x et y
        if (y >= 0 && x >= 0 && x < level->map_size && y < level->map_size && !is_cancel){ // Si tout est bien défini
            
            if (get_type(map, x, y) == TREE){ // Si le type de la case en [x,y] est un obstacle, on retire de l'énergie
                player.stamina -= 10;
                player.stamina_removed += 10;
            }else{// Sinon 
                
                //On actualise les valeurs de x et y dans player
                player.x = x;
                player.y = y;

                if(is_it_in(y,x,history,history_size-1)) {
                    set_type_boucle(map,x,y,history,history_size-1);
                    boucle=1;
                }

                if (get_type(map, x, y) == HEAL){ // Si le type de la case est un bonus, on ajoute de l'énergie au joueur et on actualise la distance parcourue
                    set_type(map,x,y,GROUND);
                    player.stamina += 9;
                    player.stamina_added+=10;
                    player.dist += dist;
                }else if (get_type(map, x, y) == VICTORY){ // Sinon si le type de la case est le type de fin, on arrête la partie après avoir actualiser la distance parcourue
                    player.stamina--;
                    launch = FALSE;
                    victory = TRUE;
                    player.dist += dist;
                }else{ // Sinon on retire de l'énergie et on actualise la distance parcourue
                    player.stamina--;
                    player.stamina_removed ++;
                    player.dist += dist;
                }

                // On met ces nouvelles valeurs dans l'historique
                history_size++;
                history = (History*)realloc(history,sizeof(History)*history_size);
                history[history_size-1].player_state = player;
                history[history_size-1].type = get_type(map, player.x, player.y);
            }
        }
        // Si le joueur n'a plus d'énergie on arrête le jeu
        if (player.stamina <= 0) launch = FALSE; 

        // On affiche les nouvelles informations
        is_cancel = 0;
        refresh(&player, map, level, history, history_size, cancel_remaining, boucle);
        boucle = 0;

        if (launch == FALSE){ // Si on le jeu doit s'arrêter
            //On affiche le chemin parcouru
            print_end_map(&player, map, level, history, history_size);

            //On affiche l'historique
            printf("\n\n");
            print_history(map, history, history_size);
            printf("\n");

            printf ("\n\n%s\e[1m - - - - La partie est finie - - - -  \e[0m\n \n %s", KRED, KWHT);

            if ((victory == TRUE) && (saved == FALSE)){ // Si la personne a gagné et n'a pas sauvegardé, on affiche la victoire et le meilleur chemin
                print_win(&player, map, level, history, history_size, cancel_remaining);
                pathfind(map,level);
            }else if ((victory == FALSE) && (saved == FALSE)) {// Si la personne a perdu et n'a pas sauvegardé, on affiche la défaite et le meilleur chemin
                print_lose(&player, map, level, history, history_size, cancel_remaining);
                pathfind(map,level);
            }else{ // Sinon, on sauvegarde
                print_save ();
            }  
            //On libère l'espace mémoire de map et history et on sort          
            free (map);
            puts("Espace mémoire de la carte libéré");
            free(history);
            puts ("Espace mémoire de l'historique libéré");
            exit(0);
        }
    }
}

//----------------------------------

void level(LevelSettings level) {

    //Inits
    Case** map = init_map(&level); // Initialisation de la carte
    Player player = init_player(&level); // Initialisation du joueur
    History* history = init_history(player, map); // Initialisation de l'historique
    int history_size = 1;
    int cancel_remaining = 6;
    // Appel de la fonction de déplacement et début du jeu
    move(map, &level, history, player, history_size, cancel_remaining);
}
