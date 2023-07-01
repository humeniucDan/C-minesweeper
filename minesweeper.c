#include "minesweeper.h"

int vx[] = {0, 0, 1, 1, 1, -1, -1, -1},
    vy[] = {1, -1, 0, -1, 1, 0, 1, -1};

comanda* getCmd(char *s, int *nrC)
{
    int i = 0, j = 0;
    comanda *cmd = malloc(sizeof(comanda)), cc;
    while(s[i] != '\0')
    {
        if (s[i] == '!' &&
            (s[i+1] !='\0'&& (('A' <= s[i+1] && s[i+1] <= 'Z') || ('a' <= s[i+1] && s[i+1] <= 'z')) &&
            '1' <= s[i+2] && s[i+2] <= '9'))
        {
            i++;
            cc.tipCmd = '!';
            cc.col = s[i];
            i++;
            cc.row = 0;
            while('0' <= s[i] && s[i] <= '9')
            {
                cc.row = cc.row * 10 + (s[i]-'0');
                i++;
            }
            j++;
            cmd = realloc(cmd, sizeof(comanda) * j);
            cmd[j-1] = cc;
        }
        else if((('A' <= s[i] && s[i] <= 'Z') || ('a' <= s[i] && s[i] <= 'z')) &&
                '1' <= s[i+1] && s[i+1] <= '9')
        {
            cc.tipCmd = '.';
            cc.col = s[i];
            i++;
            cc.row = 0;
            while('0' <= s[i] && s[i] <= '9')
            {
                cc.row = cc.row * 10 + (s[i]-'0');
                i++;
            }
            j++;
            cmd = realloc(cmd, sizeof(comanda) * j);
            cmd[j-1] = cc;
        } else i++;
    }
    *nrC = j;
    return cmd;
}

void mark(int i, int j, int h, int w, char** s)
{
    if(!(0 <= i && i < h && 0 <= j && j < w)) return;
    if(s[i][j] == '!') s[i][j] = '?';
    else if(s[i][j] == '?') s[i][j] = '!';
}

int player_view(int h, int w, char t[h][w], char** s)
{
    int ndCel = 0;
    colN(w);
    for(int i = 0; i < h; i++)
    {
        printf("%2i ", i+1);
        for(int j = 0; j < w; j++)
        {
            if(s[i][j] == '.') printc(t[i][j]);
            else
            {
                printf("%c ", s[i][j]);
                ndCel++;
            }
        }
        printf("%-2i \n", i+1);
    }
    colN(w);
    return ndCel;
}

int nc(char a)  //1
{
    if(a <= 'Z') return a - 'A';
    return a - 'a' + 26;
}

void zm(int h, int w, char t[][w])  //2
{
    for(int i = 0; i < h; i++) for(int j = 0; j < w; j++) t[i][j] = 0;
}

void colN(int w)  //3
{
    printf("  ");
    if(w + 'A' - 'Z' <= 0)
    {
        for(int i = 0; i < w; i++) printf(" %c", i+ 'A');
        printf("\n");
    }
    else
    {
        for(char i = 'A'; i <= 'Z'; i++) printf(" %c", i);
        for(int i = 0; i < w - 26; i++) printf(" %c", i + 'a');
        printf("\n");
    }
}

void print(int h, int w, char t[h][w])  //4
{
    colN(w);
    for(int i = 0; i < h; i++)
    {
        printf("%2i ", i+1);
        for(int j = 0; j < w; j++) printf("%c ", t[i][j]);
        printf("%-2i \n", i+1);
    }
    colN(w);
}

void create(int nr_bombs, char cols[], int rows[], int h, int w, char t[h][w])  //5
{
    int r, c;
    zm(h, w, t);
    for(int i = 0; i < nr_bombs; i++)
    {
        r = rows[i] - 1;
        c = nc(cols[i]);
        t[r][c] = 'X';
        if(0 < r && r < h-1 && 0 < c && c < w-1)
        {
            for(int k = -1; k <= 1; k++)
                for(int l = -1; l <= 1; l++)
                    if(t[r+k][c+l] != 'X')
                        t[r+k][c+l]++;
        }
        else
        {
            for(int k = 0; k < 8; k++)
                if((0 <= r+vx[k] && r+vx[k] <= h-1) &&
                        (0 <= c+vy[k] && c+vy[k] <= w-1) &&
                        t[r+vx[k]][c+vy[k]] != 'X')
                    t[r+vx[k]][c+vy[k]]++;
        }
    }
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            if(t[i][j] == 0) t[i][j] = '.';
            else if(t[i][j] != 'X')t[i][j] += '0';
        }
    }
}

char** init_state(int h, int w)
{
    char **s = malloc(h * sizeof(char*));
    for(int i = 0; i < h; i++)
    {
        s[i] = malloc(w * sizeof(char));
        for(int j = 0; j < w; j++) s[i][j] = '?';
    }
    return s;
}

int discover(int i, int j, int h, int w, char t[h][w], char** s)
{
    if(i < 0 || h <= i || j < 0 || w <= j) return -2;
    else if(t[i][j] == 'X') return -1;
    else if(s[i][j] == '.') return 0;
    else if('1' <= t[i][j] && t[i][j] <= '8')
    {
        s[i][j] = '.';
        //s[i][j] = t[i][j];
        return 1;
    }
    else if(t[i][j] == '.')
    {
        s[i][j] = '.';
        for(int k = 0; k < 8; k++) discover(i+vx[k], j+vy[k], h, w, t, s);
        return 2;
    }
    return 3;
}

void prins(int h, int w, char **s)
{
    colN(w);
    for(int i = 0; i < h; i++)
    {
        printf("%2i ", i+1);
        for(int j = 0; j < w; j++) printf("%c ", s[i][j]);
        printf("%-2i \n", i+1);
    }
    colN(w);
}

void printc(char c)
{
    switch(c){
    case '1': printf("\033[1;34m"); break;
    case '2': printf("\033[1;32m"); break;
    case '3': printf("\033[1;31m"); break;
    case '4': printf("\033[1;35m"); break;
    case '5': printf("\033[1;33m"); break;
    case '6': printf("\033[1;34m"); break;
    case '7': printf("\033[1;33m"); break;
    case '8': printf("\033[1;31m"); break;
    default: break;
    }
    printf("%c ", c);
    printf("\033[0;37m");
}

void title()
{
    printf("\033[1;37m");
    printf("  __  __   _                                   \n");
    printf(" |  \\/  | (_)  _ _    ___                      \n");
    printf(" | |\\/| | | | | ' \\  / -_)                     \n");
    printf(" |_|  |_| |_| |_||_| \\___|                     \n");
    printf("                                               \n");
    printf("  ___  __      __  ___   ___   ___   ___   ___ \n");
    printf(" / __| \\ \\    / / | __| | __| | _ \\ | __| | _ \\\n");
    printf(" \\__ \\  \\ \\/\\/ /  | _|  | _|  |  _/ | _|  |   /\n");
    printf(" |___/   \\_/\\_/   |___| |___| |_|   |___| |_|_\\\n");
    printf("\033[0;37m");
}
