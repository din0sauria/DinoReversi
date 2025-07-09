#include <string.h>
#include "../include/playerbase.h"
#include <stdio.h>
#include <iostream>

void init(struct Player *player) {
	// This function will be executed at the begin of each game, only once.
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
	for (int i = player->row_cnt - 1; i >= player->row_cnt - i - 1; i--) {
        int ti = i;
		for (int j = player->col_cnt - 1; j >= player->col_cnt - j - 1; j--) {
            int tj = j;
			if (is_valid(player, ti, tj)) {
				return initPoint(ti, tj);
			}
            tj = player->col_cnt - j - 1;
            if (is_valid(player, ti, tj)) {
				return initPoint(ti, tj);
			}
		}
        ti = player->row_cnt - i - 1;
        for (int j = player->col_cnt - 1; j >= player->col_cnt - j - 1; j--) {
            int tj = j;
            if (is_valid(player, ti, tj)) {
                return initPoint(ti, tj);
            }
            tj = player->col_cnt - j - 1;
            if (is_valid(player, ti, tj)) {
                return initPoint(ti, tj);
            }
        }
	}
	return initPoint(-1, -1);   // give up
}
