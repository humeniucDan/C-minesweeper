#ifndef MINESWEEPER_H_INCLUDED
#define MINESWEEPER_H_INCLUDED

typedef struct c{
    char tipCmd, col;
    int row;
}comanda;

int nc(char a); //1

void zm(int h, int w, char[][w]); //2

void colN(int w); //3

void print(int h, int w, char t[][w]); //4

void create(int nr_bombs, char cols[], int rows[], int h, int w, char t[][w]); //5

char** init_state(int h, int w);

int discover(int i, int j, int h, int w, char t[h][w], char** s);

void prins(int h, int w, char **s);

comanda* getCmd(char *s, int *nrC);

void mark(int i, int j, int h, int w, char** s);

int player_view(int h, int w, char t[h][w], char** s);

void printc(char c);

void title();

#endif // MINESWEEPER_H_INCLUDED
