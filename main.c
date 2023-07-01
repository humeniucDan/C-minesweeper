#include <stdio.h>
#include <stdlib.h>
#include "minesweeper.h"

int main()
{
    //scanf("")
    int h = 3, w = 6;
    char t[h][w];
    int nb = 3, rw[] = {2, 3, 2}, n, ndCel = h * w;
    char  cl[] = {'B', 'C', 'D'}, **s = init_state(h, w), *sc = malloc(sizeof(char) * 100), brnr;
    create(nb, cl, rw, h, w, t);
    int state = 1;
    title();
    scanf("%c", &brnr);
    printf("\e[1;1H\e[2J");
    player_view(h, w, t, s);
    while(state != -1 && ndCel > nb)
    {
        scanf("%[^\n]", sc);
        fflush(stdin);
        comanda *cmd = getCmd(sc, &n);
        for(int i = 0; i < n; i++)
        {
            if(cmd[i].tipCmd == '.')
            {
                state = discover(cmd[i].row-1, nc(cmd[i].col), h, w, t, s);
                if(state == -1) break;
            }
            else mark(cmd[i].row-1, nc(cmd[i].col), h, w, s);
        }
        printf("\e[1;1H\e[2J");
        ndCel = player_view(h, w, t, s);
        free(cmd);
        free(sc);
        sc = malloc(sizeof(char) * 100);
    }
    if(state == -1)printf("\033[1;31mAI ATINS O BOMBA!\033[0;37m");
    else printf("\033[1;32mAI CASTIGAT!\033[0;37m");
    return 0;
}
