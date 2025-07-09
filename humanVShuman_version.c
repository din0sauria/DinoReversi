#include <stdio.h>
#include <stdlib.h>

// 棋盘边界检测
#define BOUNDARY(x, y) ((x) >= 1 && (x) <= 8 && (y) >= 1 && (y) <= 8)

// 开局介绍
int introduce()
{
    printf("       [游戏规则]\n\n");
    printf("[开局] 棋局开始时，黑棋位于E4和D5，白棋位于D4和E5。黑方先行，双方交替下棋。\n\n");
    printf("[落子] 在空位落下一枚棋子，且在该棋子的横、竖、斜八个方向有一枚己方棋子，则\n");
    printf("       夹在中间的全部对方棋子翻转成为己方棋子。并且，只能在可以翻转棋子的地\n");
    printf("       方落子。若没有可落子位置，则这一轮只能弃权。\n\n");
    printf("[结束] 当双方都不能落子时，游戏结束，棋子多的一方获胜。\n\n");
    printf("[操作] 落子时输入行字母+列数字(如：D3)；在轮到对方落子时输入字母“R”悔棋；\n");
    printf("       输入字母“P”查看游戏规则。\n\n");
    system("pause");
    return 0;
}

// 重置并搜索可落子位置
int search(int *const piece, const int color)
{
    int exist = 0;
    for (int i = 0; i <= 63; i++)
    {
        if (piece[i] == 1 || piece[i] == -1)
            continue; // 有子跳过
        if (piece[i] == 2 * color || piece[i] == -2 * color)
            piece[i] = 0;                // 重置标记
        for (int dx = -1; dx <= 1; dx++) // 八向搜索
            for (int dy = -1; dy <= 1; dy++)
            {
                if (dx == 0 && dy == 0)
                    continue;
                for (int row = i / 8 + 1 + dx, col = i % 8 + 1 + dy; BOUNDARY(row, col); row += dx, col += dy)
                    if (piece[row * 8 + col - 9] != -color)
                        break; // 不是反色棋子就退出
                    else if (BOUNDARY(row + dx, col + dy) && piece[(row + dx) * 8 + col + dy - 9] == color)
                    { // 若反色棋子下一格是同色,则该点可落子;否则无操作
                        piece[i] = 2 * color;
                        exist = 1;
                        goto finish;
                    }
            }
    finish:;
    }
    return exist; // 存在可落子位置则返回1,不存在则返回0
}

// 输出棋盘界面
int gui(int *const piece)
{
    system("clear");
    int score_black = 0, score_white = 0;
    printf("\033[0m\n        [黑白棋]\n\n    1 2 3 4 5 6 7 8\n  ┌─────────────────┐\n");
    for (int i = 0; i <= 63; i++)
    {
        if (i % 8 == 0)
            printf(" %c│ ", i / 8 + 'A');
        switch (piece[i])
        {
        case 1:
            printf("○ "); // 黑棋
            score_black++;
            break;
        case -1:
            printf("● "); // 白棋
            score_white++;
            break;
        case 2:
        case -2:
            printf("\033[31m+ \033[0m");
            break; // 提示位置
        default:
            printf("+ "); // 空位
        }
        if (i % 8 == 7)
            printf("│\n");
    }
    printf("  └─────────────────┘\n   黑棋:%2d | 白棋:%2d \n", score_black, score_white);
    printf("─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─\n");
    return score_black == score_white ? 0 : (score_black > score_white ? 1 : -1);
}

// 输出信息,在gui()后
int info(int *const piece, const int sign, const int color)
{
    static int cnt[3] = {0};
    for (int i = 0; i < 3; i++)
        if (cnt[i] > 10)
        {
            printf("再玩就坏了\n");
            cnt[i] = 0;
            return 0;
        }
    switch (sign >= 64 ? sign - 80 : sign)
    {
    case -1:
        printf("%s方悔棋!\n", color == 1 ? "黑" : "白");
        break;
    case -2:
        printf("无效操作!\n");
        cnt[0]++;
        break;
    case -3:
        printf("该位置已有棋子!\n");
        cnt[1]++;
        break;
    case -4:
        printf("该位置不可落子!\n");
        cnt[1]++;
        break;
    case -5:
        printf("开局不能悔棋哦!\n");
        cnt[0]++;
        break;
    case -6:
        introduce();
        gui(piece);
        cnt[2]++;
        break;
    default: // sign>=0
        if (sign < 64)
            printf("%s方在[%c%c]落子\n", color == 1 ? "白" : "黑", sign / 8 + 'A', sign % 8 + '1');
        else
            printf("%s方在[%c%c]落子\n%s方无可落子位置!\n", color == 1 ? "黑" : "白", sign / 8 - 10 + 'A', sign % 8 + '1', color == 1 ? "白" : "黑");
        cnt[0] = cnt[1] = cnt[2] = 0;
    }
    return 0;
}

// 复制棋盘,用于悔棋
int retract(int *const piece, int *const piece_last)
{
    int sign = -2;
    for (int i = 0; i < 64; i++)
    {
        if (piece[i] != piece_last[i])
            sign = -1;
        piece[i] = piece_last[i];
    }

    return sign;
}

// 落子操作,落子前备份棋盘
int place(int *const piece, int *const piece_last, const int color) // 棋盘,备份棋盘,颜色
{
    int row = 0, col = 0;
    char ch;
    printf("请%s方落子:\n", color == 1 ? "黑" : "白");
    fflush(stdin);
    while ((ch = getchar()) == '\n' || ch == ' ')
        ; // 第一次输入
    if (ch == 'R' || ch == 'r')
        return -1; // 悔棋
    if (ch == 'P' || ch == 'p')
        return -6; // 游戏规则
    if (ch >= 'A' && ch <= 'H')
        row = ch + 1 - 'A';
    else if (ch >= 'a' && ch <= 'h')
        row = ch + 1 - 'a';
    else if (ch >= '1' && ch <= '8')
        row = ch - '0';
    while ((ch = getchar()) == '\n' || ch == ' ')
        ; // 第二次输入
    if (ch >= 'A' && ch <= 'H')
        col = ch + 1 - 'A';
    else if (ch >= 'a' && ch <= 'h')
        col = ch + 1 - 'a';
    else if (ch >= '1' && ch <= '8')
        col = ch - '0';
    if (!BOUNDARY(row, col))
        return -2;               // 无效操作
    int loc = row * 8 + col - 9; // 数组格式位置 loc=0~63
    if (piece[loc] == 1 || piece[loc] == -1)
        return -3; // 已有棋子
    if (piece[loc] == 0 || piece[loc] == -2 * color)
        return -4; // 不可落子
    retract(piece_last, piece);
    piece[loc] = color;              // 落子操作
    for (int dx = -1; dx <= 1; dx++) // 八向搜索,翻转棋子
        for (int dy = -1; dy <= 1; dy++)
        {
            if (dx == 0 && dy == 0)
                continue;
            for (int rowp = row + dx, colp = col + dy; BOUNDARY(rowp, colp); rowp += dx, colp += dy)
                if (piece[rowp * 8 + colp - 9] != -color)
                    break; // 不是反色就退出
                else if (BOUNDARY(rowp + dx, colp + dy) && piece[(rowp + dx) * 8 + colp + dy - 9] == color)
                    for (; rowp != row || colp != col; rowp -= dx, colp -= dy)
                        piece[rowp * 8 + colp - 9] = color; // 存在反色棋子则搜寻同色,并翻转中间的所有反色棋子
        }
    return loc; // 返回落子位置
}

int main()
{
    int piece[64] = {0}, piece_last[64], color = 1, sign = -1;
    piece[27] = piece[36] = -1;
    piece[28] = piece[35] = 1; // 预设棋子位置
    gui(piece);
    introduce();
start:
    piece[63] = 0;
    piece[28] = piece[35] = color = 1;
    piece[27] = piece[36] = sign = -1;
    search(piece, color); // 首次落子
    do
    {
        gui(piece);
        if (sign <= -2)
            info(piece, sign, color);
        sign = place(piece, piece_last, color);
        if (sign == -1)
            sign = -5;
    } while (sign <= -1);                                                           // 直到落子成功,sign为位置,才退出
    while (1)                                                                       // 后续落子
        if (search(piece, color *= -1) || (sign += 80, search(piece, color *= -1))) // 判断是否可落子
            do
            {
                gui(piece);
                info(piece, sign, color);
                sign = place(piece, piece_last, color);
                if (sign == -1)
                    sign = retract(piece, piece_last);
            } while (sign <= -2);
        else
            break; // 双方都不可落子则结束;若一方无棋子,同样满足双方都不可落子
    int outcome = gui(piece);
    printf("%s方在[%c%c]落子\n", color == 1 ? "黑" : "白", sign / 8 + 'A', sign % 8 + '1'); // 黑白要颠倒一下
    printf("游戏结束, %s\n再来一局吗? (Y|N 或 1|0)\n", outcome == 0 ? "平局!" : (outcome == 1 ? "黑方胜!" : "白方胜!"));
    char ch;
    for (fflush(stdin); (ch = getchar()) != '0' && ch != 'n' && ch != 'N';)
        if (ch == '1' || ch == 'y' || ch == 'Y')
            goto start;
    system("pause");
    return 0;
}
