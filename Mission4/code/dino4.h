//dino4
/* Birds are born with no shackles
Then what fetters my fate?
Blown away, the white petals
Leave me trapped in the cage.
The endless isolation
Can't wear down my illusion
Someday, I’ll make a dream unchained
Let my heart bravely spread the wings
Soaring past the night
To trace the bright moonlight
Let the clouds heal me of the stings
Gently wipe the sorrow off my life
I dream
*/
#include <string.h>
#include "../include/playerbase.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#define NUM_SIMULATIONS 10  // Number of simulations per move
#define MAX_STEPS 3         // Maximum number of steps in a simulation
typedef struct PW
{
	struct Point point;
	int weight; // 权重
	int value;	// 价值：翻转棋子个数
} P;
int Map[20][20];
int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
char stable_mat[20][20];
void init_Map(struct Player *player)
{
    int row = player->row_cnt - 1, col = player->col_cnt - 1;
    
    // 设置四个角的权重为500倍的棋盘值
    Map[0][0] = 500 * player->mat[0][0];
    Map[0][col] = 500 * player->mat[0][col];
    Map[row][0] = 500 * player->mat[row][0];
    Map[row][col] = 500 * player->mat[row][col];

    // 设置四个角邻近位置的权重为-45
    Map[0][1] = Map[0][col - 1] = Map[1][0] = Map[1][col] = -45;
    Map[row - 1][0] = Map[row - 1][col] = Map[row][1] = Map[row][col - 1] = -45;

    // 设置第一行和倒数第二行的权重为-1
    for (int i = 1; i <= col - 1; i++)
        Map[1][i] = Map[row - 1][i] = -1;

    // 设置第一列和倒数第二列的权重为-1
    for (int i = 1; i <= row - 1; i++)
        Map[i][1] = Map[i][col - 1] = -1;

    // 设置第二行第二列及其对称位置的权重为-45
    Map[1][1] = Map[1][col - 1] = Map[row - 1][1] = Map[row - 1][col - 1] = -45;

    // 计算棋盘中心位置
    int midx = row / 2, midy = col / 2;

    // 设置中心位置的左右两侧的权重为2
    for (int i = midx; i <= midx + 1; i++)
        Map[i][midy - 1] = Map[i][midy + 2] = 2;

    // 设置中心位置的上下两侧的权重为2
    for (int i = midy; i <= midy + 1; i++)
        Map[midx - 1][i] = Map[midx + 2][i] = 2;

    // 设置中心四角位置的权重为3
    Map[midx - 1][midy - 1] = Map[midx - 1][midy + 2] = Map[midx + 2][midy - 1] = Map[midx + 2][midy + 2] = 3;

    // 设置左右两侧中部位置的权重为5
    for (int i = 3; i <= row - 3; i++)
        Map[i][0] = Map[i][col] = 5;

    // 设置上下两侧中部位置的权重为5
    for (int i = 3; i <= col - 3; i++)
        Map[0][i] = Map[row][i] = 5;

    // 设置靠近边缘的中部位置的权重为100
    Map[0][2] = Map[0][col - 2] = Map[2][0] = Map[2][col] = 100;
    Map[row - 2][0] = Map[row - 2][col] = Map[row][col - 2] = Map[row][2] = 100;
}

void init(struct Player *player)
{
	// This function will be executed at the begin of each game, only once.
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
int have_corn(struct Player *player, int i, int j)
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
		Map[i][j] = 100;
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
	int row = player->row_cnt - 1, col = player->col_cnt - 1;
	int vis[20][20] = {0};
	for (int i = 0; i <= row; i += row)
	{
		for (int j = 0; j <= col; j += col)
		{
			if (is_corn(player, i, j) && player->mat[i][j] == 'O')
			{
				if (!have_corn(player, i, j))
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

int opponent_valid(struct Player *player, int posx, int posy)
{
	// 检查坐标（posx，posy）是否在棋盘内。如果坐标在棋盘外则返回false。
	if (posx < 0 || posx >= player->row_cnt || posy < 0 || posy >= player->col_cnt)
	{
		return false;
	}
	// 检查在坐标（posx，posy）的棋盘位置是否空着。如果该位置已经有棋子（'o'或'O'），则返回false。
	if (player->mat[posx][posy] == 'O' || player->mat[posx][posy] == 'o')
	{
		return false;
	}
	// 定义8个方向的坐标偏移，分别对应右，左，下，上，右下，左上，右上，左下。
	int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};

	// 遍历所有的方向。
	for (int dir = 0; dir < 8; dir++)
	{
		// 计算在当前方向上的下一步的坐标。
		int x = posx + step[dir][0];
		int y = posy + step[dir][1];
		// 如果下一步的坐标在棋盘外，则继续检查下一个方向。
		if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt)
		{
			continue;
		}
		// 如果下一步的坐标上的棋子不是'o'，则继续检查下一个方向。
		if (player->mat[x][y] != 'O')
		{
			continue;
		}
		// 当下一步的坐标在棋盘内且该位置的棋子为'o'时，继续沿当前方向前进，直到走到棋盘边缘或遇到非'o'的棋子。
		while (true)
		{
			x += step[dir][0];
			y += step[dir][1];
			if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt || (player->mat[x][y] >= '1' && player->mat[x][y] <= '9'))
			{
				break;
			}
			// 如果在这个方向上有'O'，则说明当前位置（posx，posy）是一个合法的位置，返回true。
			if (player->mat[x][y] == 'o')
			{
				return true;
			}
		}
	}
	// 如果所有的方向都被检查过仍然没有找到'O'，则说明当前位置（posx，posy）不是合法位置，返回false。
	return false;
}




struct Point place(struct Player *player)
{
    // 调用 update_map 函数，更新棋盘权重矩阵
    update_map(player);

    // 计算当前棋子总数 chess_cnt 和棋盘总格子数 sum
    int chess_cnt = player->your_score + player->opponent_score;
    int sum = (player->col_cnt) * (player->row_cnt);

    // 动态分配内存，存储可以下子的合法点 ok_point；初始化各种用于决策的变量
    struct PW *ok_point = (struct PW *)malloc((player->row_cnt * player->col_cnt) * sizeof(struct PW));
    int ok_cnt = 0; // 可以走的数量
    int Max_weight = -1000;
    int value = 0;
    int Min_num = 1000; // 最小行动力
    int corn_num = 100;
    char temp_Mat[20][20];
    struct Point point = initPoint(-1, -1);

    // 遍历棋盘的每一个格子 (i, j)，检查是否是一个合法的下子点
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            if (is_valid(player, i, j, &value))
            {
                // 如果合法，记录这个点和其权重到 ok_point 数组中
                ok_point[ok_cnt++].point = initPoint(i, j);
                ok_point[ok_cnt].weight = Map[i][j];
                // 复制当前棋盘状态到 temp_Mat
                for (int ii = 0; ii < player->row_cnt; ii++)
                    for (int jj = 0; jj < player->col_cnt; jj++)
                        temp_Mat[ii][jj] = player->mat[ii][jj];

                // 模拟在 (i, j) 位置下子，并统计对方在敏感点（角落位置）可以下子的数量 num
                ChangeMat(player, i, j);
                int num = 0;
                for (int pp = 0; pp < player->row_cnt; pp++)
                    for (int qq = 0; qq < player->col_cnt; qq++)
                        if (opponent_valid(player, pp, qq) && is_corn(player, pp, qq))
                            num++;

                // 恢复原棋盘状态
                for (int ii = 0; ii < player->row_cnt; ii++)
                    for (int jj = 0; jj < player->col_cnt; jj++)
                        player->mat[ii][jj] = temp_Mat[ii][jj];

                // 如果当前点的权重大于最大权重，或者对方下角的数量小，更新选择的下子点为 (i, j)，并更新 corn_num 和 Max_weight
                if (Map[i][j] > Max_weight || num < corn_num)
                {
                    point.X = i;
                    point.Y = j;
                    corn_num = num;
                    Max_weight = Map[i][j];
                }
                // 如果当前点的权重等于 Max_weight，则进一步检查对方的行动力
                else if (Map[i][j] == Max_weight)
                {
                    // 复制当前棋盘状态到 temp_Mat
                    for (int ii = 0; ii < player->row_cnt; ii++)
                        for (int jj = 0; jj < player->col_cnt; jj++)
                            temp_Mat[ii][jj] = player->mat[ii][jj];

                    // 模拟在 (i, j) 位置下子，并统计对方在非敏感点的可下子数量 num
                    ChangeMat(player, i, j);
                    int num = 0; // 统计行动力
                    for (int pp = 0; pp < player->row_cnt; pp++)
                        for (int qq = 0; qq < player->col_cnt; qq++)
                            if (opponent_valid(player, pp, qq) && !SensPoint(player, pp, qq))
                                num++;

                    // 如果当前 num 小于最小行动力 Min_num，更新选择的下子点为 (i, j)，并更新 Min_num
                    if (num < Min_num)
                    {
                        point.X = i;
                        point.Y = j;
                        Min_num = num;
                    }

                    // 恢复原棋盘状态
                    for (int ii = 0; ii < player->row_cnt; ii++)
                        for (int jj = 0; jj < player->col_cnt; jj++)
                            player->mat[ii][jj] = temp_Mat[ii][jj];
                }
            }
        }
    }
    printf("%d,%d chosen\n",point.X,point.Y);
    // 释放动态分配的内存，返回选择的下子点
    free(ok_point);
    return point;
}
