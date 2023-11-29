#define _CRT_SECURE_NO_WARNINGS
#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void n_init() {
    map_init(13, 35);
}

// �÷��̾�� �������� �� �� �����ϰ� ��ġ�ϴ� �Լ�
void place_players_and_items() {
    // �÷��̾� ��ġ
    for (int i = 0; i < n_player; ++i) {
        do {
            player[i].id = i;
            player[i].is_alive = true;
            player[i].hasitem = false;
            player[i].intel = randint(1, 10);
            player[i].str = randint(1, 10);
            player[i].stamina = randint(1, 10);
            player[i].item = item[randint(0, n_item - 1)];
            pass[i] = false;

            // �� �� �÷��̾ �����ϰ� ��ġ
            player[i].name[0] = 'P';
            player[i].name[1] = '0' + i;
            int row = randint(0, N_ROW - 1);
            int col = randint(0, N_COL - 1);

            if (placable(row, col)) {
                player[i].hasitem = true;
                printxy(player[i].name[1], row, col);
                break;
            }
        } while (1);
    }

    // ������ ��ġ
    for (int i = 0; i < n_item; ++i) {
        do {
            item[i].intel_buf = randint(1, 10);
            item[i].str_buf = randint(1, 10);
            item[i].stamina_buf = randint(1, 10);

            // �� �� �������� �����ϰ� ��ġ
            item[i].name[0] = 'I';
            item[i].name[1] = '0' + i;
            int row = randint(0, N_ROW - 1);
            int col = randint(0, N_COL - 1);

            if (placable(row, col)) {
                printxy(item[i].name[1], row, col);
                break;
            }
        } while (1);
    }
}

// �÷��̾��� �̵��� ó���ϴ� �Լ�, ȭ��ǥ Ű�� Player 0�� ����
void move_players() {
    for (int i = 0; i < n_player; ++i) {
        if (!player[i].is_alive) {
            continue;
        }

        int new_row = -1, new_col = -1;

        // �ش� �÷��̾ ����ڿ� ���� �����Ǵ��� Ȯ��
        if (i == 0) {
            char key = get_key();
            switch (key) {
            case K_UP:
                new_row = (player[i].id - N_COL + N_ROW * N_COL) % (N_ROW * N_COL) / N_COL;
                new_col = player[i].id % N_COL;
                break;
            case K_DOWN:
                new_row = (player[i].id + N_COL) % (N_ROW * N_COL) / N_COL;
                new_col = player[i].id % N_COL;
                break;
            case K_LEFT:
                new_row = player[i].id / N_COL;
                new_col = (player[i].id - 1 + N_COL) % N_COL;
                break;
            case K_RIGHT:
                new_row = player[i].id / N_COL;
                new_col = (player[i].id + 1) % N_COL;
                break;
            default:
                continue;  // ��ȿ���� ���� �Է��� ���, ���� �÷��̾�� ��� ����
            }
        }
        else {
            // �ٸ� �÷��̾��� ���, �������� �̵�
            int direction = randint(0, 3);
            switch (direction) {
            case DIR_UP:
                new_row = player[i].id / N_COL;
                new_col = player[i].id % N_COL;
                break;
            case DIR_DOWN:
                new_row = (player[i].id + 2) / N_COL;
                new_col = (player[i].id + 2) % N_COL;
                break;
            case DIR_LEFT:
                new_row = (player[i].id + 1) / N_COL;
                new_col = (player[i].id + 1) % N_COL;
                break;
            case DIR_RIGHT:
                new_row = (player[i].id + 3) / N_COL;
                new_col = (player[i].id + 3) % N_COL;
                break;
            }
        }

        // ���ο� ��ġ�� ��ȿ���� Ȯ���ϰ� ������Ʈ
        if (new_row >= 0 && new_row < N_ROW && new_col >= 0 && new_col < N_COL && placable(new_row, new_col)) {
            printxy(player[i].name[1], new_row, new_col);
            printxy(' ', player[i].id / N_COL, player[i].id % N_COL);  // ���� ��ġ�� �����
            player[i].id = new_row * N_COL + new_col;  // �÷��̾��� ��ġ ������Ʈ
        }
    }
}


// �÷��̾�� ������ ���� ��ȣ�ۿ��� ó���ϴ� �Լ�
void interact_with_items() {
    for (int i = 0; i < n_player; ++i) {
        if (!player[i].is_alive) {
            continue;
        }

        for (int j = 0; j < n_item; ++j) {
            // �÷��̾�� �������� ���� ��ġ�� �ִ��� Ȯ��
            if (player[i].id == item[j].name[1]) {
                // �÷��̾ �������� ������ ���� �ʴٸ� ȹ��
                if (!player[i].hasitem) {
                    player[i].item = item[j];
                    player[i].hasitem = true;
                    // ������ ������ ������Ʈ
                    item[j].name[1] = ' ';  // �������� �������� ǥ��
                }
                // �÷��̾ �������� ������ �ִٸ� ��ȯ
                else {
                    ITEM temp = player[i].item;
                    player[i].item = item[j];
                    item[j] = temp;
                }
            }
        }
    }
}

// �÷��̾� ���� ��ȣ�ۿ��� ó���ϴ� �Լ�
void interact_between_players() {
    for (int i = 0; i < n_player; ++i) {
        if (!player[i].is_alive) {
            continue;
        }

        for (int j = i + 1; j < n_player; ++j) {
            // �� �÷��̾ ���� ��ġ�� �ִ��� Ȯ��
            if (player[i].id == player[j].id) {
                // �÷��̾� i�� �÷��̾� j���� �������� ��Ż
                if (player[i].str > player[j].str) {
                    player[i].item = player[j].item;
                    player[j].hasitem = false;
                }
                // �÷��̾� j�� �÷��̾� i���� �������� ��Ż
                else if (player[j].str > player[i].str) {
                    player[j].item = player[i].item;
                    player[i].hasitem = false;
                }
            }
        }
    }
}
