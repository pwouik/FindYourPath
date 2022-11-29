//---------------------[ Include ]---------------------//

#include <stdio.h>
#include <stdlib.h>
#include "level.h"

//---------------------[ FindYourPath ]---------------------//

int main() {

    int choice = 0;
    int will = 1;

    while(will) {

        printf("\e[1;1H\e[2J");
        printf("\n+ + + + + + + + + + + + + + + + + + + + + + +\n+                                           +\n+          FIND     YOUR     PATH           +\n+                                           +\n+ + + + + + + + + + + + + + + + + + + + + + +\n\n");

        switch(choice) {

            case(1) :
                printf("        \033[4mChoix de la difficulté :\033[0m\n\n            11 - Facile\n            12 - Moyen\n            13 - Difficile\n\n        0 - Retour au menu principal\n\n");
                break;
            case(11) :
                level((LevelSettings){5,4,15,1,2,7});
                break;
            case(12) :
                level((LevelSettings){8,6,11,2,1,7});
                break;
            case(13) :
                level((LevelSettings){15,9,10,7,1,8});
                break;

            //----------

            case(2) :
                printf("        21 - Historique des scores\n        22 - Revoir une partie\n\n        0 - Retour au menu principal\n\n");
                break;
            /*case(21) :
                
                break;
            case(22) :
                
                break;*/

            //----------

            case(3) :
                printf("                \033[4mComment jouer\033[0m\n\n\n     Le principe du jeu est très simple : il s'agit de déplacer le personnage sur la carte.\n     Initialement, le personnage se trouve en haut à gauche de la carte et doit atteindre sa maison en bas à droite.\n\n     Une distance est associée à chaque couple de cases adjacentes x et y.\n     Elle représente la distance à parcourir pour atteindre la case y en partant de la case x.\n     Le personnage dispose d'une énergie initiale fixée à l'avance dépendant de la difficulté choisie.\n\n     A chaque déplacement, il perd une unité de son énergie indépendamment de la distance parcourue.\n     L'énergie peut être récupérée après consommation de soin présent sur la carte, ou être diminuée lorsque le joueur rencontre un obstacle.\n\n\n            0 - Retour au menu principal\n\n");
                break;

            //----------

            case(9) :
                will=0;
                break;

            default :
                printf("        1 - Jouer\n        2 - Historique des parties\n        3 - Aide\n\n        9 - Quitter\n\n");
                break;
        }

        if(will) {
            printf("\nQue voulez-vous faire ? ");
            scanf("%d", &choice);
        }
    }

    printf("\e[1;1H\e[2J");
    printf("\n+ + + + + + + + + + + + + + + + + + + + + + +\n+                                           +\n+          FIND     YOUR     PATH           +\n+                                           +\n+ + + + + + + + + + + + + + + + + + + + + + +\n\n");
    printf("\n              Fin du programme.\n\n\n\n");

    return 0;
}