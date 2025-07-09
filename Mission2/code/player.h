/**
 * @file player.h
 * @author yangboyang@jisuanke.com
 * @copyright jisuanke.com
 * @date 2021-07-01
 */

#include <string.h>
#include "../include/playerbase.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
typedef struct PW
{
    struct Point point;
    int weight; // 权重
    int value;  // 价值：翻转棋子个数
} P;
int Map[20][20];
int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
// 定义稳定子标志
char stable_mat[20][20] = {0}; 
void init_Map(struct Player *player)
{
    int row = player->row_cnt - 1, col = player->col_cnt - 1;
    Map[0][0] = 550 * player->mat[0][0];
    Map[0][col] = 550 * player->mat[0][col];
    Map[row][0] = 550 * player->mat[row][0];
    Map[row][col] = 550 * player->mat[row][col];
    Map[0][1] = Map[0][col - 1] = Map[1][0] = Map[1][col] = Map[row - 1][0] = Map[row - 1][col] = Map[row][1] = Map[row][col - 1] = -25;
    for (int i = 1; i <= col - 1; i++)Map[1][i] = Map[row - 1][i] = -1;
    for (int i = 1; i <= row - 1; i++)Map[i][1] = Map[i][col - 1] = -1;
    Map[1][1] = Map[1][col - 1] = Map[row - 1][1] = Map[row - 1][col - 1] = -45;
    int midx = row / 2, midy = col / 2;
    for (int i = midx; i <= midx + 1; i++)Map[i][midy - 1] = Map[i][midy + 2] = 2;
    for (int i = midy; i <= midy + 1; i++)Map[midx - 1][i] = Map[midx + 2][i] = 2;
    Map[midx - 1][midy - 1] = Map[midx - 1][midy + 2] = Map[midx + 2][midy - 1] = Map[midx + 2][midy + 2] = 3;
    for (int i = 3; i <= row - 3; i++)Map[i][0] = Map[i][col] = 5;
    for (int i = 3; i <= col - 3; i++)Map[0][i] = Map[row][i] = 5;
    Map[0][2] = Map[0][col - 2] = Map[2][0] = Map[2][col] = Map[row - 2][0] = Map[row - 2][col] = Map[row][col - 2] = Map[row][2] = 10;
}
void init(struct Player *player) {
    init_Map(player);
    srand(time(0));
}
int is_valid(struct Player *player, int posx, int posy, int *value)
{
    if (posx < 0 || posx >= player->row_cnt || posy < 0 || posy >= player->col_cnt)
    {
        return 0;
    }
    if (player->mat[posx][posy] == 'o' || player->mat[posx][posy] == 'O')
    {
        return 0;
    }
    for (int dir = 0; dir < 8; dir++)
    {
        int x = posx + step[dir][0];
        int y = posy + step[dir][1];
        if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt)
            continue;
        if (player->mat[x][y] != 'o')
            continue;
        int cnt = 1;
        while (1)
        {
            x += step[dir][0];
            y += step[dir][1];
            if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt || player->mat[x][y] >= '1' && player->mat[x][y] <= '9')
                break;
            cnt++;
            if (player->mat[x][y] == 'O')
            {
                *value = cnt;
                return 1;
            }
        }
    }
    return 0;
}
int is_corn(struct Player *player, int x, int y)
{
    if ((x == 0 || x == player->row_cnt - 1) && (y == 0 || y == player->col_cnt - 1))
        return 1;
    return 0;
}
int update_fun(struct Player *player, int i, int j)
{
    int row = player->row_cnt - 1, col = player->col_cnt - 1;
    if (Map[i][j] == 200)
        return 0;
    if (i == 0 && j == 0)
    {

        for (int k = j; k <= col - 2; k++)
            Map[i][k] = 200;
        for (int k = i; k <= row - 2; k++)
            Map[k][j] = 200;
        Map[1][1] = 200;
    }
    else if (i == 0 && j == col)
    {
        for (int k = 2; k <= j; k++)
            Map[i][k] = 200;
        for (int k = i; k <= row - 2; k++)
            Map[k][j] = 200;
        Map[1][col - 1] = 200;
    }
    else if (i == row && j == 0)
    {
        for (int k = j; k <= col - 2; k++)
            Map[i][k] = 200;
        for (int k = 2; k <= i; k++)
            Map[k][j] = 200;
        Map[row - 1][1] = 200;
    }
    else if (i == row && j == col)
    {
        for (int k = 2; k <= j; k++)
            Map[i][k] = 200;
        for (int k = 2; k <= i; k++)
            Map[k][j] = 200;
        Map[row - 1][col - 1] = 200;
    }
    return 1;
}
void dfs(struct Player *player, int i, int j, int vis[][20])
{
    if (vis[i][j] || i < 0 || j < 0 || i >= player->row_cnt || j >= player->col_cnt)
        return;
    vis[i][j] = 1;
    if (player->mat[i][j] != 'O')
    {
        Map[i][j] = 200;
        return;
    }
    for (int dir = 0; dir < 8; dir++)
    {
        int xx = i + step[dir][0];
        int yy = j + step[dir][1];
        dfs(player, xx, yy, vis);
    }
}
void update_map(struct Player *player)
{
    int hang = player->row_cnt - 1, lie = player->col_cnt - 1;
    int vis[20][20] = {0};
    for (int i = 0; i <= hang; i += hang)
    {
        for (int j = 0; j <= lie; j += lie)
        {
            if (is_corn(player, i, j) && player->mat[i][j] == 'O')
            {
                if (!update_fun(player, i, j))
                    dfs(player, i, j, vis);
            }
        }
    }
}
int SensPoint(struct Player *player, int x, int y)
{
    return (x == 0 && y == 1) || (x == 1 && y == 0) || (x == 1 && y == 1) || (x == 1 && y == player->col_cnt - 2) || (x == 1 && y == player->col_cnt - 1) || (x == 0 && y == player->col_cnt - 2) || (x == player->row_cnt - 2 && y == 1) || (x == player->row_cnt - 2 && y == 0) || (x == player->row_cnt - 1 && y == 1) || (x == player->row_cnt - 2 && y == player->col_cnt - 2) || (x == player->row_cnt - 1 && y == player->col_cnt - 2) || (x == player->row_cnt - 2 && y == player->col_cnt - 1);
}
void ChangeMat(struct Player *player, int i, int j)
{
    player->mat[i][j] = 'O';
    int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
    int flag; // flag用来判断一个方向上是否有子要翻转
    for (int dir = 0; dir < 8; dir++)
    {
        flag = 0; // 初始为0，可以翻转为1 ，然后进行翻转。
        int x = i + step[dir][0];
        int y = j + step[dir][1];
        while (x >= 0 && x < player->row_cnt && y >= 0 && y < player->col_cnt)
        {
            if (player->mat[x][y] == 'O')
            {
                flag = 1;
                break;
            }
            x += step[dir][0];
            y += step[dir][1];
        }
        if (flag)
        {
            x = i + step[dir][0];
            y = j + step[dir][1];
            while (x >= 0 && x < player->row_cnt && y >= 0 && y < player->col_cnt)
            {
                if (player->mat[x][y] == 'o')
                {
                    player->mat[x][y] = 'O';
                }
                else
                {
                    break;
                }
                x += step[dir][0];
                y += step[dir][1];
            }
        }
    }
    return;
}
/*struct Point {
    int X, Y;
};
typedef struct PW
{
    struct Point point;
    int weight; // 权重
    int value;  // 价值：翻转棋子个数
} P;
*/

bool is_stable_in_direction(struct Player *player, int x, int y, int dx, int dy) {
    int nx = x + dx;
    int ny = y + dy;

    // 该方向上一边接边界或己方的稳定子
    if (nx < 0 || ny < 0 || nx >= player->row_cnt || ny >= player->col_cnt ||stable_mat[nx][ny] == 'S') {
        return true;
    }

    return false;
}

// 检查某个位置是否为稳定子
bool is_stable(struct Player *player, int x, int y) {
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    for (int i = 0; i < 4; i++) {
        if (!is_stable_in_direction(player, x, y, directions[i][0], directions[i][1])) {
            return false;
        }
    }
    return true;
}

// 更新棋盘上的稳定子
void updatestably(struct Player *player) {
    memset(stable_mat, 0, sizeof(stable_mat));
    for (int i = 0; i < player->row_cnt; i++) {
        for (int j = 0; j < player->col_cnt; j++) {
            if (player->mat[i][j] != 0) {
                if (is_stable(player, i, j)) {
                    stable_mat[i][j] = 'S';  // 标记为稳定子
                } else {
                    stable_mat[i][j] = 0;  // 未标记为稳定子
                }
            }
        }
    }
}


int opponent_valid(struct Player *player, int posx, int posy)
{
    if (posx < 0 || posx >= player->row_cnt || posy < 0 || posy >= player->col_cnt)
    {
        return false;
    }
    if (player->mat[posx][posy] == 'O' || player->mat[posx][posy] == 'o')
    {
        return false;
    }
    int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};

    for (int dir = 0; dir < 8; dir++)
    {
        int x = posx + step[dir][0];
        int y = posy + step[dir][1];
        if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt)
        {
            continue;
        }
        if (player->mat[x][y] != 'O')
        {
            continue;
        }
        while (true)
        {
            x += step[dir][0];
            y += step[dir][1];
            if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt || (player->mat[x][y] >= '1' && player->mat[x][y] <= '9'))
            {
                break;
            }
            if (player->mat[x][y] == 'o')
            {
                return true;
            }
        }
    }
    return false;
}
int exclude_position(struct Player *player,int x,int y)
{
    int play_side1=player->row_cnt-1;
    int play_side2=player->row_cnt-2;
	int exclude_x[12]={0,1,1,0,1,play_side2,play_side1,play_side2,play_side2,play_side1,play_side2};
    int exclude_y[12]={1,0,1,play_side2,play_side1,0,1,1,play_side1,play_side2,play_side2};
	
	if(x==1 || x==player->row_cnt-2 || y==1 || y==player->col_cnt-2)
	return 1;
	
	for(int i=0;i<12;i++)
	{
		if(x==exclude_x[i]&&y==exclude_y[i])
		return 1;
	}
	
	return 0;
}
struct Point place(struct Player *player)
{
    update_map(player);
    updatestably(player); // 更新稳定子
    int chess_cnt = player->your_score + player->opponent_score;
    int sum = (player->col_cnt) * (player->row_cnt);
    struct PW *ok_point = (struct PW *)malloc((player->row_cnt * player->col_cnt) * sizeof(struct PW));
    int ok_cnt = 0; // 可以走的数量
    int Max_weight = -1000;
    int Min_value = 100;
    int Max_value = 0;
    int value = 0;
    int Min_mob = 1000; // 最小行动力
    char temp_Mat[20][20];
    struct Point point = initPoint(-1, -1);
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            if (is_valid(player, i, j, &value))
            {
                ok_point[ok_cnt++].point = initPoint(i, j);
                ok_point[ok_cnt].weight = Map[i][j];
				
                if (is_stable(player, i, j)) {
                    ok_point[ok_cnt].weight += player->row_cnt;
                }
                if ( exclude_position(player, i, j))
                {
                    ok_point[ok_cnt].weight -= 1;
                }
                if (Map[i][j] > Max_weight)
                {
                    point.X = i;
                    point.Y = j;
                    Max_weight = Map[i][j];
                }
                if (Map[i][j] == Max_weight)
                {
                    for (int ii = 0; ii < player->row_cnt; ii++)
                        for (int jj = 0; jj < player->col_cnt; jj++)
                            temp_Mat[ii][jj] = player->mat[ii][jj];

                    ChangeMat(player, i, j);
                    int num = 0; // 统计行动力
                    for (int pp = 0; pp < player->row_cnt; pp++)
                        for (int qq = 0; qq < player->col_cnt; qq++)
                            if (opponent_valid(player, pp, qq) && !SensPoint(player, pp, qq))
                                num++;

                    if (num < Min_mob)
                    {
                        point.X = i;
                        point.Y = j;
                        Min_mob = num;
                    }

                    for (int ii = 0; ii < player->row_cnt; ii++)
                        for (int jj = 0; jj < player->col_cnt; jj++)
                            player->mat[ii][jj] = temp_Mat[ii][jj];
                }
            }
        }
    }
    free(ok_point);
    return point;
}
