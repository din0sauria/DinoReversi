#include <string.h>
#include "../include/playerbase.h"
#include <stdio.h>
#include <stdbool.h>

int priority[20][20];
void set_corners(int x, int y) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            priority[x + i][y + j] = 1;
        }
    }
}
void init(struct Player *player) {
	// This function will be executed at the begin of each game, only once.
    int row = player->row_cnt;
    int col = player->col_cnt;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            priority[i][j] = 4;
        }
    }
    set_corners(0, 0), set_corners(0, col - 2);
    set_corners(row - 2, 0), set_corners(row - 2, col - 2);
    priority[0][0] = priority[0][col-1] = priority[row-1][0] = priority[row-1][col-1] = 5;
    for (int i = 2; i < col - 2; i++) {
        priority[0][i] = priority[row - 1][i] = 3;
        priority[1][i] = priority[row - 2][i] = 2;
    }
    for (int i = 2; i < row - 2; i++) {
        priority[i][0] = priority[i][col - 1] = 3;
        priority[i][1] = priority[i][col - 2] = 2;
    }
}

int is_valid(struct Player *player, int posx, int posy) {
    if (posx < 0 || posx >= player->row_cnt || posy < 0 || posy >= player->col_cnt) {
        return false;
    }
    if (player->mat[posx][posy] == 'o' || player->mat[posx][posy] == 'O') {
        return false;
    }
    int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
    for (int dir = 0;  dir < 8; dir++) {
        int x = posx + step[dir][0];
        int y = posy + step[dir][1];
        if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt) {
            continue;
        }
        if (player->mat[x][y] != 'o') {
            continue;
        }
        while (true) {
            x += step[dir][0];
            y += step[dir][1];
            if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt || (player->mat[x][y] >= '1' && player->mat[x][y] <= '9')) {
                break;
            }
            if (player->mat[x][y] == 'O') {
                return true;
            }
        }
    }
    return false;
}

struct Point place(struct Player *player) {
    struct Point point = initPoint(-1, -1);
    int max_value = -1;
    for (int i = 0; i < player->row_cnt; i++) {
        for (int j = 0; j < player->col_cnt; j++) {
            if (is_valid(player, i, j) && priority[i][j] > max_value) {
                max_value = priority[i][j];
                point = initPoint(i, j);
            }
        }
    }
	return point;
}