#include <stdio.h>
#include <stdlib.h>

// ���̱߽���
#define BOUNDARY(x, y) ((x) >= 1 && (x) <= 8 && (y) >= 1 && (y) <= 8)

// ���ֽ���
int introduce()
{
    printf("       [��Ϸ����]\n\n");
    printf("[����] ��ֿ�ʼʱ������λ��E4��D5������λ��D4��E5���ڷ����У�˫���������塣\n\n");
    printf("[����] �ڿ�λ����һö���ӣ����ڸ����ӵĺᡢ����б�˸�������һö�������ӣ���\n");
    printf("       �����м��ȫ���Է����ӷ�ת��Ϊ�������ӡ����ң�ֻ���ڿ��Է�ת���ӵĵ�\n");
    printf("       �����ӡ���û�п�����λ�ã�����һ��ֻ����Ȩ��\n\n");
    printf("[����] ��˫������������ʱ����Ϸ���������Ӷ��һ����ʤ��\n\n");
    printf("[����] ����ʱ��������ĸ+������(�磺D3)�����ֵ��Է�����ʱ������ĸ��R�����壻\n");
    printf("       ������ĸ��P���鿴��Ϸ����\n\n");
    system("pause");
    return 0;
}

// ���ò�����������λ��
int search(int *const piece, const int color)
{
    int exist = 0;
    for (int i = 0; i <= 63; i++)
    {
        if (piece[i] == 1 || piece[i] == -1)
            continue; // ��������
        if (piece[i] == 2 * color || piece[i] == -2 * color)
            piece[i] = 0;                // ���ñ��
        for (int dx = -1; dx <= 1; dx++) // ��������
            for (int dy = -1; dy <= 1; dy++)
            {
                if (dx == 0 && dy == 0)
                    continue;
                for (int row = i / 8 + 1 + dx, col = i % 8 + 1 + dy; BOUNDARY(row, col); row += dx, col += dy)
                    if (piece[row * 8 + col - 9] != -color)
                        break; // ���Ƿ�ɫ���Ӿ��˳�
                    else if (BOUNDARY(row + dx, col + dy) && piece[(row + dx) * 8 + col + dy - 9] == color)
                    { // ����ɫ������һ����ͬɫ,��õ������;�����޲���
                        piece[i] = 2 * color;
                        exist = 1;
                        goto finish;
                    }
            }
    finish:;
    }
    return exist; // ���ڿ�����λ���򷵻�1,�������򷵻�0
}

// ������̽���
int gui(int *const piece)
{
    system("clear");
    int score_black = 0, score_white = 0;
    printf("\033[0m\n        [�ڰ���]\n\n    1 2 3 4 5 6 7 8\n  ��������������������������������������\n");
    for (int i = 0; i <= 63; i++)
    {
        if (i % 8 == 0)
            printf(" %c�� ", i / 8 + 'A');
        switch (piece[i])
        {
        case 1:
            printf("�� "); // ����
            score_black++;
            break;
        case -1:
            printf("�� "); // ����
            score_white++;
            break;
        case 2:
        case -2:
            printf("\033[31m+ \033[0m");
            break; // ��ʾλ��
        default:
            printf("+ "); // ��λ
        }
        if (i % 8 == 7)
            printf("��\n");
    }
    printf("  ��������������������������������������\n   ����:%2d | ����:%2d \n", score_black, score_white);
    printf("�� �� �� �� �� �� �� �� �� �� �� ��\n");
    return score_black == score_white ? 0 : (score_black > score_white ? 1 : -1);
}

// �����Ϣ,��gui()��
int info(int *const piece, const int sign, const int color)
{
    static int cnt[3] = {0};
    for (int i = 0; i < 3; i++)
        if (cnt[i] > 10)
        {
            printf("����ͻ���\n");
            cnt[i] = 0;
            return 0;
        }
    switch (sign >= 64 ? sign - 80 : sign)
    {
    case -1:
        printf("%s������!\n", color == 1 ? "��" : "��");
        break;
    case -2:
        printf("��Ч����!\n");
        cnt[0]++;
        break;
    case -3:
        printf("��λ����������!\n");
        cnt[1]++;
        break;
    case -4:
        printf("��λ�ò�������!\n");
        cnt[1]++;
        break;
    case -5:
        printf("���ֲ��ܻ���Ŷ!\n");
        cnt[0]++;
        break;
    case -6:
        introduce();
        gui(piece);
        cnt[2]++;
        break;
    default: // sign>=0
        if (sign < 64)
            printf("%s����[%c%c]����\n", color == 1 ? "��" : "��", sign / 8 + 'A', sign % 8 + '1');
        else
            printf("%s����[%c%c]����\n%s���޿�����λ��!\n", color == 1 ? "��" : "��", sign / 8 - 10 + 'A', sign % 8 + '1', color == 1 ? "��" : "��");
        cnt[0] = cnt[1] = cnt[2] = 0;
    }
    return 0;
}

// ��������,���ڻ���
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

// ���Ӳ���,����ǰ��������
int place(int *const piece, int *const piece_last, const int color) // ����,��������,��ɫ
{
    int row = 0, col = 0;
    char ch;
    printf("��%s������:\n", color == 1 ? "��" : "��");
    fflush(stdin);
    while ((ch = getchar()) == '\n' || ch == ' ')
        ; // ��һ������
    if (ch == 'R' || ch == 'r')
        return -1; // ����
    if (ch == 'P' || ch == 'p')
        return -6; // ��Ϸ����
    if (ch >= 'A' && ch <= 'H')
        row = ch + 1 - 'A';
    else if (ch >= 'a' && ch <= 'h')
        row = ch + 1 - 'a';
    else if (ch >= '1' && ch <= '8')
        row = ch - '0';
    while ((ch = getchar()) == '\n' || ch == ' ')
        ; // �ڶ�������
    if (ch >= 'A' && ch <= 'H')
        col = ch + 1 - 'A';
    else if (ch >= 'a' && ch <= 'h')
        col = ch + 1 - 'a';
    else if (ch >= '1' && ch <= '8')
        col = ch - '0';
    if (!BOUNDARY(row, col))
        return -2;               // ��Ч����
    int loc = row * 8 + col - 9; // �����ʽλ�� loc=0~63
    if (piece[loc] == 1 || piece[loc] == -1)
        return -3; // ��������
    if (piece[loc] == 0 || piece[loc] == -2 * color)
        return -4; // ��������
    retract(piece_last, piece);
    piece[loc] = color;              // ���Ӳ���
    for (int dx = -1; dx <= 1; dx++) // ��������,��ת����
        for (int dy = -1; dy <= 1; dy++)
        {
            if (dx == 0 && dy == 0)
                continue;
            for (int rowp = row + dx, colp = col + dy; BOUNDARY(rowp, colp); rowp += dx, colp += dy)
                if (piece[rowp * 8 + colp - 9] != -color)
                    break; // ���Ƿ�ɫ���˳�
                else if (BOUNDARY(rowp + dx, colp + dy) && piece[(rowp + dx) * 8 + colp + dy - 9] == color)
                    for (; rowp != row || colp != col; rowp -= dx, colp -= dy)
                        piece[rowp * 8 + colp - 9] = color; // ���ڷ�ɫ��������Ѱͬɫ,����ת�м�����з�ɫ����
        }
    return loc; // ��������λ��
}

int main()
{
    int piece[64] = {0}, piece_last[64], color = 1, sign = -1;
    piece[27] = piece[36] = -1;
    piece[28] = piece[35] = 1; // Ԥ������λ��
    gui(piece);
    introduce();
start:
    piece[63] = 0;
    piece[28] = piece[35] = color = 1;
    piece[27] = piece[36] = sign = -1;
    search(piece, color); // �״�����
    do
    {
        gui(piece);
        if (sign <= -2)
            info(piece, sign, color);
        sign = place(piece, piece_last, color);
        if (sign == -1)
            sign = -5;
    } while (sign <= -1);                                                           // ֱ�����ӳɹ�,signΪλ��,���˳�
    while (1)                                                                       // ��������
        if (search(piece, color *= -1) || (sign += 80, search(piece, color *= -1))) // �ж��Ƿ������
            do
            {
                gui(piece);
                info(piece, sign, color);
                sign = place(piece, piece_last, color);
                if (sign == -1)
                    sign = retract(piece, piece_last);
            } while (sign <= -2);
        else
            break; // ˫�����������������;��һ��������,ͬ������˫������������
    int outcome = gui(piece);
    printf("%s����[%c%c]����\n", color == 1 ? "��" : "��", sign / 8 + 'A', sign % 8 + '1'); // �ڰ�Ҫ�ߵ�һ��
    printf("��Ϸ����, %s\n����һ����? (Y|N �� 1|0)\n", outcome == 0 ? "ƽ��!" : (outcome == 1 ? "�ڷ�ʤ!" : "�׷�ʤ!"));
    char ch;
    for (fflush(stdin); (ch = getchar()) != '0' && ch != 'n' && ch != 'N';)
        if (ch == '1' || ch == 'y' || ch == 'Y')
            goto start;
    system("pause");
    return 0;
}
