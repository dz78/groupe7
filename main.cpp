#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#define CIRCLE_INVALID -1
#define CIRCLE_EMPTY 0
#define CIRCLE_YELLOW 1
#define CIRCLE_RED 2
#define CIRCLE_DRAW 3


typedef struct board {
    char holes[6*7];
} board;

int lireUneCellule(board *b, int col, int level) {
    if (col < 0 || col > 6 || level < 0 || level > 5) return CIRCLE_INVALID; // si la colonne ou le niveau n'est pas valide retourne -1
    return b->holes[(5*7)+col-(level*7)]; //sinon retourne la case
}


void metsLavaleurDansLaCellule(board *b, int col, int level, int value) {
    if (col < 0 || col > 6 || level < 0 || level > 5) return;
    /***
        Mettre 1 ou 2 dans la cellule
            1 => Joeur 1 qui a  joué
            2=> Joeur 2 qui a joué
    **/

    b->holes[(5*7)+col-(level*7)] = value;
}

void initialiser_board(board *b) {
    /* reserver ue place dans la mémoire*/
    memset(b->holes,CIRCLE_EMPTY,sizeof(b->holes));// sizeof permet de mesurer la taille d'une variable
}

/* Copy the source board to the destination board. */
//void c4Copy(board *dst, board *src) {
//    memcpy(dst->holes,src->holes,sizeof(dst->holes)); // copier l'espace mémoire de l'ancienne variable           pas compris, demander a Eihad !!!
//}

/* Return true if the given colum is all full with circles. */
int columnRemplit(board *b, int col) {
    /***
        b : board
        col : numéro de colonne
        Role => pour chaque colonne vérifier la cellule du niveau 5 de la colonne col est vide
    */
    return lireUneCellule(b,col,5) != CIRCLE_EMPTY;
}

int metsLaValeurDansLeTableau(board *b, int col, int value) {
    if (columnRemplit(b,col)) return 0; // si la colonne est remplit retorune 0

    /***
        Cette boucle cherche la premiere position vide dans la colonne
    */
    for (int level = 0; level < 6; level++) {
        if (lireUneCellule(b,col,level) == CIRCLE_EMPTY) { // si la case elle est vide
            metsLavaleurDansLaCellule(b,col,level,value); // mettre la valeur dans le tableau
            break;
        }
    }
    return 1;
}

void afficherBoard(board *b) {
    for (int level = 5; level >= 0; level--) {
        for (int col = 0; col < 7; col++) {

            int color = lireUneCellule(b,col,level);

            if(color==CIRCLE_EMPTY)
                printf("[ ]");
            else if(color==CIRCLE_YELLOW)
                printf("[O]");
            else if(color==CIRCLE_RED)
                printf("[X]");
        }
        printf("\n");
    }
    for (int col = 0; col < 7; col++)
        printf(" %d ",col);
    printf("\n");
}

int c4GetWinner(board *b) {
    int empty = 0;
    for (int level = 5; level >= 0; level--) {
            /***
            Pour cherche les cellules adjacents de choix de l'utilisateur*/
        for (int col = 0; col < 7; col++) {
            int color = lireUneCellule(b,col,level);
            if (color == CIRCLE_EMPTY) {
                empty++;
                continue;
            }
            struct {
                int col_incr;
                int level_incr;
            } dir[4] = {
                {1,0},
                {0,1},
                {1,1},
                {-1,1}
            };

            for (int d = 0; d < 4; d++) {
                int start_col = col;
                int start_level = level;
                while(lireUneCellule(b,start_col-dir[d].col_incr,
                              start_level-dir[d].level_incr) == color)
                {
                    start_col -= dir[d].col_incr;
                    start_level -= dir[d].level_incr;
                }

                int count = 0; /* Count consecutive circles. */
                while(lireUneCellule(b,start_col,start_level) == color) {
                    count++;
                    start_col += dir[d].col_incr;
                    start_level += dir[d].level_incr;
                }
                if (count >= 4) return color;
            }
        }
    }
    return empty ? CIRCLE_EMPTY : CIRCLE_DRAW; // demander ce que ca fait ?

}

int lireUnChoixUtilisateur(board *b, int playerID)  {
    int choix_utilisateur = -1; //-1 pour indiquer que le choix de l'utilisateur n'est pas valide
    while(choix_utilisateur == -1) { //tant que l'utilisatue n'a pas saisi un choix valide

        printf("Player %d move =  ",playerID); // je demande d'enter le numéro de colonne
        scanf("%d",&choix_utilisateur);
        /* Check the validity. */
        if (choix_utilisateur > 6) choix_utilisateur = -1;
        if (choix_utilisateur < 0) choix_utilisateur = -1;
        /***
            si le choix de l'utilisateur est valide mais la colonne n'est pas remplit alors il devrait donner un autre choix
        */
        if (choix_utilisateur != -1 && columnRemplit(b,choix_utilisateur)) choix_utilisateur = -1;
    }
    return choix_utilisateur;
}
void play(void) {
    board b; // instancier un nouveau objet du type Board
    int index; // pour dire Joueur 1 ou Joeur 2
    initialiser_board(&b); // vider l'objet Board et remplir par des 0 ( initialiser une board )
    afficherBoard(&b); // je l'affiche sur l'écan
    int exit=0; // un flag qui permet de déterminer qui a gagné
    while(1 && !exit) { // on veut que le jeu continuer sans fin !
        index=1; // je commence par le joueur 1
        while(index<=2 && !exit) // je fais une boucle pour prendre le choix de deux joueurs (index = 1 et index = 2)
        {
            int player1Choice = lireUnChoixUtilisateur(&b,index); // lire le choix d'un joueur numéro index
            /* J'ai passé le board pour vérifier s'il y'en a une place libre dans la colonne*/

            metsLaValeurDansLeTableau(&b,player1Choice,index); //  mettre le jeton de l'utlisateur index dans le board
            afficherBoard(&b); //afficher le board
            if (c4GetWinner(&b) != 0) exit=1; // tester si le joueur index a gagné sinon passer au joueur suivant
            else index++; // sinon avancer
        }
    }
    if(index==1) printf("Player 1 win");
    else printf("Player 2 win");
}

int main(void) {
    srand(time(NULL)); // générer des valeurs aléatoires
    play(); // Commencer le jeu
}
