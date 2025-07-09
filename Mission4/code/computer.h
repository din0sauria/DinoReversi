#include <string.h>
#include "../include/playerbase.h"
#include <stdio.h>
#include <iostream>
#define MY_PIECE 'O'
#define OPPONENT_PIECE 'o'

int point_x[1000];
int point_y[1000];
int point_count = 0;
int side_x[100];
int side_y[100];
int side_count = 0;
int grade[1000];
int map[30][30];
int start_point[4][2];
int start_count;

void init(struct Player *player)
{
    // This function will be executed at the begin of each game, only once.
    start_count = 0;
    memset(start_point, 0, sizeof(start_point));
    for (int i = 0; i < player->row_cnt; i++)
        for (int j = 0; j < player->col_cnt; j++)
        {
            if (player->mat[i][j] == MY_PIECE || player->mat[i][j] == OPPONENT_PIECE)
            {
                start_point[start_count][0] = i;
                start_point[start_count][1] = j;
                start_count++;
            }
        }
}

int in(struct Player *player, int x, int y)
{
    if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt)
        return 0;
    else
        return 1;
}
// 检测格子是否符合下棋标准

int is_valid(int row_cnt, int col_cnt, char mat[][30], int posx, int posy, char my_sign, char his_sign)
{
    if (posx < 0 || posx >= row_cnt || posy < 0 || posy >= col_cnt)
    {
        return false;
    }
    if (mat[posx][posy] == his_sign || mat[posx][posy] == my_sign)
    {
        return false;
    }
    int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
    for (int dir = 0; dir < 8; dir++)
    {
        int x = posx + step[dir][0];
        int y = posy + step[dir][1];
        if (x < 0 || x >= row_cnt || y < 0 || y >= col_cnt)
        {
            continue;
        }
        if (mat[x][y] != his_sign)
        {
            continue;
        }
        while (true)
        {
            x += step[dir][0];
            y += step[dir][1];
            if (x < 0 || x >= row_cnt || y < 0 || y >= col_cnt || (mat[x][y] >= '1' && mat[x][y] <= '9'))
            {
                break;
            }
            if (mat[x][y] == my_sign)
            {
                return true;
            }
        }
    }
    return false;
}

struct Point judge_corner(struct Player *player)
{
    struct Point tmp;
    char my_sign = MY_PIECE;
    char his_sign = OPPONENT_PIECE;
    char copy_map[30][30];
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
            copy_map[i][j] = player->mat[i][j];
    }

    if (is_valid(player->row_cnt, player->col_cnt, copy_map, 0, 0, my_sign, his_sign))
    {
        tmp.X = 0;
        tmp.Y = 0;
        return tmp;
    }
    if (is_valid(player->row_cnt, player->col_cnt, copy_map, 0, player->col_cnt - 1, my_sign, his_sign))
    {
        tmp.X = 0;
        tmp.Y = player->col_cnt - 1;
        return tmp;
    }
    if (is_valid(player->row_cnt, player->col_cnt, copy_map, player->row_cnt - 1, 0, my_sign, his_sign))
    {
        tmp.X = player->row_cnt - 1;
        tmp.Y = 0;
        return tmp;
    }
    if (is_valid(player->row_cnt, player->col_cnt, copy_map, player->row_cnt - 1, player->col_cnt - 1, my_sign, his_sign))
    {
        tmp.X = player->row_cnt - 1;
        tmp.Y = player->col_cnt - 1;
        return tmp;
    }
    tmp.X = -1;
    tmp.Y = -1;
    return tmp;
}

struct Point judge_side(struct Player *player)
{
    struct Point tmp;
    for (int i = 0; i < side_count; i++)
    {
        if (side_x[i] == 0 || side_x[i] == (player->row_cnt - 1))
        {
            int j;
            for (j = 0; j < side_y[i]; j++)
            {
                if (player->mat[side_x[i]][j] == MY_PIECE)
                    continue;
                else
                    break;
            }
            if (j >= side_y[i])
            {
                tmp.X = side_x[i];
                tmp.Y = side_y[i];
                return tmp;
            }

            for (j = (player->col_cnt - 1); j > side_y[i]; j--)
            {
                if (player->mat[side_x[i]][j] == MY_PIECE)
                    continue;
                else
                    break;
            }
            if (j <= side_y[i])
            {
                tmp.X = side_x[i];
                tmp.Y = side_y[i];
                return tmp;
            }
            else
                continue;
        }
        if (side_y[i] == 0 || side_y[i] == (player->col_cnt - 1))
        {
            int m;
            for (m = 0; m < side_x[i]; m++)
            {
                if (player->mat[m][side_y[i]] == MY_PIECE)
                    continue;
                else
                    break;
            }
            if (m >= side_x[i])
            {
                tmp.X = side_x[i];
                tmp.Y = side_y[i];
                return tmp;
            }

            for (m = (player->row_cnt - 1); m > side_x[i]; m--)
            {
                if (player->mat[m][side_y[i]] == MY_PIECE)
                    continue;
                else
                    break;
            }
            if (m <= side_x[i])
            {
                tmp.X = side_x[i];
                tmp.Y = side_y[i];
                return tmp;
            }
            else
                continue;
        }
    }
    tmp.X = -1;
    tmp.Y = -1;
    return tmp;
}

int no_start(int x, int y)
{
    for (int i = 0; i < start_count; i++)
    {
        if (x == start_point[i][0] && y == start_point[i][1])
            return 0;
    }

    return 1;
}
void calc_grade(struct Player *player)
{
    int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
    for (int i = 0; i < point_count; i++)
    {
        for (int dir = 0; dir < 8; dir++)
        {
            int x = point_x[i] + step[dir][0];
            int y = point_y[i] + step[dir][1];
            if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt)
            {
                continue;
            }
            if (player->mat[x][y] != OPPONENT_PIECE)
            {
                continue;
            }

            int grid_grade;

            if (no_start(x, y))
            {
                grid_grade = player->mat[x][y] - 48;
                grade[i] += grid_grade;
            }

            // grade[i]+=1;
            while (true)
            {
                x += step[dir][0];
                y += step[dir][1];
                if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt || (player->mat[x][y] >= '1' && player->mat[x][y] <= '9'))
                {
                    break;
                }
                if (player->mat[x][y] == OPPONENT_PIECE)
                {
                    if (no_start(x, y))
                    {
                        grid_grade = player->mat[x][y] - 48;
                        grade[i] += grid_grade;
                    }
                    // grade[i]+=1;
                }
                if (player->mat[x][y] == MY_PIECE)
                {
                    break;
                }
            }
        }
    }
}

int partition(int s, int t)
{
    int i = s, j = t;
    int tmp1 = grade[i];
    int tmp2 = point_x[i];
    int tmp3 = point_y[i];
    while (i < j)
    {
        while (j > i && grade[j] >= tmp1)
            j--;
        grade[i] = grade[j];
        point_x[i] = point_x[j];
        point_y[i] = point_y[j];
        while (i < j && grade[i] <= tmp1)
            i++;
        grade[j] = grade[i];
        point_x[j] = point_x[i];
        point_y[j] = point_y[i];
    }
    grade[i] = tmp1;
    point_x[i] = tmp2;
    point_y[i] = tmp3;
    return i;
}

void QuickSort(int s, int t)
{
    int i;
    if (s < t)
    {
        i = partition(s, t);
        QuickSort(s, i - 1);
        QuickSort(i + 1, t);
    }
}

int exclude_position(struct Player *player, int x, int y)
{
    int play_side1 = player->row_cnt - 1;
    int play_side2 = player->row_cnt - 2;
    int exclude_x[12] = {0, 1, 1, 0, 1, play_side2, play_side1, play_side2, play_side2, play_side1, play_side2};
    int exclude_y[12] = {1, 0, 1, play_side2, play_side1, 0, 1, 1, play_side1, play_side2, play_side2};

    if (x == 1 || x == player->row_cnt - 2 || y == 1 || y == player->col_cnt - 2)
        return 1;

    for (int i = 0; i < 12; i++)
    {
        if (x == exclude_x[i] && y == exclude_y[i])
            return 1;
    }

    return 0;
}

void judge_stable(struct Player *player)
{
    int flag = 0;
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            if (player->mat[0][0] != MY_PIECE)
            {
                flag = 1;
                break;
            }
            if (player->mat[i][j] == MY_PIECE)
            {
                if (i == 0 && j == 0)
                {
                    map[i][j] = 1;
                    continue;
                }
                if (j == 0)
                {
                    if (map[i - 1][j] == 1)
                        map[i][j] = 1;
                    else
                    {
                        flag = 1;
                        break;
                    }
                }
                else if (i == 0)
                {
                    if (map[i][j - 1] == 1)
                        map[i][j] = 1;
                    else
                        break;
                }
                else
                {
                    if (map[i][j - 1] == 1 && map[i - 1][j] == 1)
                    {
                        map[i][j] = 1;
                    }
                    else
                        break;
                }
            }
            else
                break;
        }
        if (flag == 1)
        {
            break;
        }
    }

    flag = 0;

    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = player->col_cnt - 1; j >= 0; j--)
        {
            if (player->mat[0][player->col_cnt - 1] != MY_PIECE)
            {
                flag = 1;
                break;
            }
            if (player->mat[i][j] == MY_PIECE)
            {
                if (i == 0 && j == (player->col_cnt - 1))
                {
                    map[i][j] = 1;
                    continue;
                }
                if (j == (player->col_cnt - 1))
                {
                    if (map[i - 1][j] == 1)
                        map[i][j] = 1;
                    else
                    {
                        flag = 1;
                        break;
                    }
                }
                else if (i == 0)
                {
                    if (map[i][j + 1] == 1)
                        map[i][j] = 1;
                    else
                        break;
                }
                else
                {
                    if (map[i][j + 1] == 1 && map[i - 1][j] == 1)
                    {
                        map[i][j] = 1;
                    }
                    else
                        break;
                }
            }
            else
                break;
        }
        if (flag == 1)
        {
            break;
        }
    }

    flag = 0;
    for (int i = player->row_cnt - 1; i >= 0; i--)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            if (player->mat[player->row_cnt - 1][0] != MY_PIECE)
            {
                flag = 1;
                break;
            }
            if (player->mat[i][j] == MY_PIECE)
            {
                if (i == (player->row_cnt - 1) && j == 0)
                {
                    map[i][j] = 1;
                    continue;
                }
                if (j == 0)
                {
                    if (map[i + 1][j] == 1)
                        map[i][j] = 1;
                    else
                    {
                        flag = 1;
                        break;
                    }
                }
                else if (i == (player->row_cnt - 1))
                {
                    if (map[i][j - 1] == 1)
                        map[i][j] = 1;
                    else
                        break;
                }
                else
                {
                    if (map[i][j - 1] == 1 && map[i + 1][j] == 1)
                    {
                        map[i][j] = 1;
                    }
                    else
                        break;
                }
            }
            else
                break;
        }
        if (flag == 1)
        {
            break;
        }
    }

    flag = 0;
    for (int i = player->row_cnt - 1; i >= 0; i--)
    {
        for (int j = player->col_cnt - 1; j >= 0; j--)
        {
            if (player->mat[player->row_cnt - 1][player->col_cnt - 1] != MY_PIECE)
            {
                flag = 1;
                break;
            }
            if (player->mat[i][j] == MY_PIECE)
            {
                if (i == (player->row_cnt - 1) && j == (player->col_cnt - 1))
                {
                    map[i][j] = 1;
                    continue;
                }
                if (j == (player->col_cnt - 1))
                {
                    if (map[i + 1][j] == 1)
                        map[i][j] = 1;
                    else
                    {
                        flag = 1;
                        break;
                    }
                }
                else if (i == (player->col_cnt - 1))
                {
                    if (map[i][j + 1] == 1)
                        map[i][j] = 1;
                    else
                        break;
                }
                else
                {
                    if (map[i][j + 1] == 1 && map[i + 1][j] == 1)
                    {
                        map[i][j] = 1;
                    }
                    else
                        break;
                }
            }
            else
                break;
        }
    }
}

struct Point select_point(struct Player *player)
{
    int i;
    struct Point tmp;
    judge_stable(player);

    char my_sign = MY_PIECE;
    char his_sign = OPPONENT_PIECE;
    char copy_map[30][30];
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
            copy_map[i][j] = player->mat[i][j];
    }

    for (i = point_count - 1; i >= 0; i--)
    {
        int x = point_x[i];
        int y = point_y[i];

        if ((in(player, x, y - 1) && in(player, x - 1, y) && map[x][y - 1] == 1 && map[x - 1][y] == 1) || (in(player, x, y + 1) && in(player, x - 1, y) && map[x][y + 1] == 1 && map[x - 1][y] == 1) || (in(player, x, y - 1) && in(player, x + 1, y) && map[x][y - 1] == 1 && map[x + 1][y] == 1) || (in(player, x, y + 1) && in(player, x + 1, y) && map[x][y + 1] == 1 && map[x + 1][y] == 1))
        {
            tmp.X = x;
            tmp.Y = y;
            return tmp;
        }
        if (exclude_position(player, x, y))
        {
            continue;
        }

        char ch = copy_map[x][y];
        copy_map[x][y] = MY_PIECE;
        if (is_valid(player->row_cnt, player->col_cnt, copy_map, 0, 0, his_sign, my_sign) || is_valid(player->row_cnt, player->col_cnt, copy_map, 0, player->col_cnt - 1, his_sign, my_sign) || is_valid(player->row_cnt, player->col_cnt, copy_map, player->row_cnt - 1, 0, his_sign, my_sign) || is_valid(player->row_cnt, player->col_cnt, copy_map, player->row_cnt - 1, player->col_cnt - 1, his_sign, my_sign))
        {
            copy_map[x][y] = ch;
            continue;
        }

        copy_map[x][y] = ch;

        tmp.X = x;
        tmp.Y = y;
        return tmp;
    }

    for (i = point_count - 1; i >= 0; i--)
    {
        int x = point_x[i];
        int y = point_y[i];

        char ch = copy_map[x][y];
        copy_map[x][y] = MY_PIECE;
        if (is_valid(player->row_cnt, player->col_cnt, copy_map, 0, 0, his_sign, my_sign) || is_valid(player->row_cnt, player->col_cnt, copy_map, 0, player->col_cnt - 1, his_sign, my_sign) || is_valid(player->row_cnt, player->col_cnt, copy_map, player->row_cnt - 1, 0, his_sign, my_sign) || is_valid(player->row_cnt, player->col_cnt, copy_map, player->row_cnt - 1, player->col_cnt - 1, his_sign, my_sign))
        {
            copy_map[x][y] = ch;
            continue;
        }
        copy_map[x][y] = ch;

        tmp.X = x;
        tmp.Y = y;
        return tmp;
    }

    if (point_count >= 1)
    {
        tmp.X = point_x[point_count - 1];
        tmp.Y = point_y[point_count - 1];
        return tmp;
    }

    tmp.X = -1;
    tmp.Y = -1;
    return tmp;
}
struct Point place(struct Player *player)
{
    memset(point_x, 0, sizeof(point_x));
    memset(point_y, 0, sizeof(point_y));
    memset(side_x, 0, sizeof(side_x));
    memset(side_y, 0, sizeof(side_y));
    memset(grade, 0, sizeof(grade));
    memset(map, 0, sizeof(map));
    point_count = 0;
    side_count = 0;
    struct Point tmp;
    tmp = judge_corner(player); // 检测四个角，这是最高的优先级
    // printf("chenggong\n");
    if (!(tmp.X == -1 && tmp.Y == -1))
        return initPoint(tmp.X, tmp.Y);

    char my_sign = MY_PIECE;
    char his_sign = OPPONENT_PIECE;

    char copy_map[30][30];
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
            copy_map[i][j] = player->mat[i][j];
    }

    for (int i = player->row_cnt - 1; i >= player->row_cnt - i - 1; i--)
    {
        int ti = i;
        for (int j = player->col_cnt - 1; j >= player->col_cnt - j - 1; j--)
        {
            int tj = j;
            if (is_valid(player->row_cnt, player->col_cnt, copy_map, ti, tj, my_sign, his_sign))
            {
                point_x[point_count] = ti;
                point_y[point_count] = tj;
                point_count++;
            }
            tj = player->col_cnt - j - 1;
            if (is_valid(player->row_cnt, player->col_cnt, copy_map, ti, tj, my_sign, his_sign))
            {
                point_x[point_count] = ti;
                point_y[point_count] = tj;
                point_count++;
            }
        }
        ti = player->row_cnt - i - 1;
        for (int j = player->col_cnt - 1; j >= player->col_cnt - j - 1; j--)
        {
            int tj = j;
            if (is_valid(player->row_cnt, player->col_cnt, copy_map, ti, tj, my_sign, his_sign))
            {
                point_x[point_count] = ti;
                point_y[point_count] = tj;
                point_count++;
            }
            tj = player->col_cnt - j - 1;
            if (is_valid(player->row_cnt, player->col_cnt, copy_map, ti, tj, my_sign, his_sign))
            {
                point_x[point_count] = ti;
                point_y[point_count] = tj;
                point_count++;
            }
        }
    }

    tmp = judge_side(player);
    if (!(tmp.X == -1 && tmp.Y == -1))
        return initPoint(tmp.X, tmp.Y);

    calc_grade(player);
    QuickSort(0, point_count - 1);

    tmp = select_point(player);
    if (!(tmp.X == -1 && tmp.Y == -1))
        return initPoint(tmp.X, tmp.Y);
    return initPoint(-1, -1);
}
