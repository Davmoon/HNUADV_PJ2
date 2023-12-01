#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP      0
#define DIR_DOWN   1
#define DIR_LEFT   2
#define DIR_RIGHT   3

void ng_init();
void ngmv_random(int);
bool ngmv_manual(key_t);
void nightgame();

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX], itmx[PLAYER_MAX], itmy[PLAYER_MAX];

void ng_init(void) {
    map_init(15, 40);//#���� �ѷ����� sample.c�� ���� �÷��� �� �κ�

    int x, y;
    for (int i = 0; i < n_player; i++) {
        // ���� �ڸ��� ������ �ٽ� ����
        if (player[i].is_alive == true) {
            do {
                x = randint(1, N_ROW - 2);
                y = randint(1, N_COL - 2);
            } while (!placable(x, y));
            px[i] = x;
            py[i] = y;
            period[i] = randint(100, 500);

            back_buf[px[i]][py[i]] = '0' + i;  // (0 .. n_player-1)
        }
    }

    //item ���� ��ġ
    for (int i = 0; i < n_player - 1; i++) {
        do {
            x = randint(1, N_ROW - 2);
            y = randint(1, N_COL - 2);
        } while (!placable(x, y));
        itmx[i] = x;
        itmy[i] = y;

        back_buf[itmx[i]][itmy[i]] = 'I';  // �������� �׻� i�� ǥ��
    }

    tick = 0;
}


bool ngmv_manual(key_t key) {
    // �� �������� ������ �� x, y�� delta
    static int dx[4] = { -1, 1, 0, 0 };
    static int dy[4] = { 0, 0, -1, 1 };

    int dir;  // ������ ����(0~3)
    switch (key) {
    case K_UP: dir = DIR_UP; break;
    case K_DOWN: dir = DIR_DOWN; break;
    case K_LEFT: dir = DIR_LEFT; break;
    case K_RIGHT: dir = DIR_RIGHT; break;
    default: return;
    }

    // �������� ���� �ڸ�
    int nx, ny;
    nx = px[0] + dx[dir];
    ny = py[0] + dy[dir];
    if (!placable(nx, ny)) {
        return false;
    }

    move_tail(0, nx, ny);
    return true;
}

// ���� ����� ��ǥ ã��
void find_nearest_target(int p, int* target_x, int* target_y) {
    int min_dist = INT_MAX;
    int dist, tx, ty;

    // ������ ��ġ Ȯ��
    for (int i = 0; i < n_item; i++) {
        tx = itmx[i];
        ty = itmy[i];
        dist = abs(px[p] - tx) + abs(py[p] - ty);

        if (dist < min_dist) {
            min_dist = dist;
            *target_x = tx;
            *target_y = ty;
        }
    }

    // �ٸ� �÷��̾ ���� ������ ��ġ Ȯ��
    for (int i = 0; i < n_player; i++) {
        if (i != p && player[i].hasitem) {
            tx = px[i];
            ty = py[i];
            dist = abs(px[p] - tx) + abs(py[p] - ty);

            if (dist < min_dist) {
                min_dist = dist;
                *target_x = tx;
                *target_y = ty;
            }
        }
    }
}


//�̵� �ֱ� �� ���� ������Ʈ
void ngmv_random(int p) {
    int target_x, target_y;
    find_nearest_target(p, &target_x, &target_y);

    // ���� ����� ��ǥ�� ���� �̵�
    int nx = px[p], ny = py[p];
    if (px[p] < target_x) nx++;
    else if (px[p] > target_x) nx--;

    if (py[p] < target_y) ny++;
    else if (py[p] > target_y) ny--;

    // �̵� ���� ���� Ȯ��
    if (placable(nx, ny)) {
        move_tail(p, nx, ny);
    }
}

//������ ȹ�� ����
void pickup_item(int player_id) {
    for (int i = 0; i < n_item; i++) {
        if (px[player_id] == itmx[i] && py[player_id] == itmy[i] && !player[player_id].hasitem) {
            player[player_id].item = item[i];
            player[player_id].hasitem = true;

            // �������� �ʿ��� ����
            back_buf[itmx[i]][itmy[i]] = '0' + player_id; // �÷��̾��� ��ȣ�� �ٽ� ǥ��
            itmx[i] = -1;
            itmy[i] = -1;

            // ������ ȹ�� ���̾�α� ǥ��
            char message[100];
            sprintf_s(message, sizeof(message), "Player %d: %s�� ȹ���߽��ϴ�.", player_id, item[i].name);
            dialog(message);
            break;
        }
    }
}

//������ ��ȯ ����
void exchange_item(int player_id) {
    for (int i = 0; i < n_item; i++) {
        // �÷��̾�� �������� ���� ��ġ�� �ִ��� Ȯ��
        if (px[player_id] == itmx[i] && py[player_id] == itmy[i] && player[player_id].hasitem) {
            bool exchange = false;

            // �÷��̾� 0�̸� Ű���� �Է����� ����
            if (player_id == 0) {
                printf("�������� ��ȯ�Ͻðڽ��ϱ�? (y/n): ");
                char choice = getchar();
                exchange = (choice == 'y' || choice == 'Y');
                getchar();
            }
            else {
                // �ٸ� �÷��̾�� 50% Ȯ���� ����
                exchange = (randint(0, 1) == 1);
            }

            if (exchange) {
                // ���ο� ������ ��ġ ����
                int new_item_x, new_item_y;
                do {
                    new_item_x = randint(1, N_ROW - 2);
                    new_item_y = randint(1, N_COL - 2);
                } while (!placable(new_item_x, new_item_y));

                // ������ ��ȯ
                ITEM temp = player[player_id].item;
                player[player_id].item = item[i];
                item[i] = temp;

                // ���� ������ ��ġ�� �� ������ ��ġ
                itmx[i] = new_item_x;
                itmy[i] = new_item_y;
                back_buf[new_item_x][new_item_y] = 'I';

                // ��ȯ ���̾�α� ǥ��
                char message[100];
                sprintf_s(message, sizeof(message), "Player %d: %s �� %s�� ��ȯ�߽��ϴ�.", player_id, temp.name, player[player_id].item.name);
                dialog(message);
            }
            break;
        }
    }
}
void reset_interactions() {
    for (int i = 0; i < n_player; i++) {
        player[i].interacted = false;
    }
}

//��Ż�õ� ����
void perform_robbery(int player_id, int other_player_id) {
    if (player[player_id].str > player[other_player_id].str) {
        // ���� ����
        printf("��Ż ����!\n");
        // ������ ��ȯ ����
        if (player[other_player_id].hasitem) {
            ITEM temp = player[player_id].item;
            player[player_id].item = player[other_player_id].item;
            player[other_player_id].item = temp;
            player[player_id].hasitem = true;
            player[other_player_id].hasitem = false;
        }
        // ���¹̳� ����
        player[player_id].stamina = max(0, player[player_id].stamina - 40);
    }
    else {
        // ���� ����
        printf("��Ż ����...\n");
        // ���¹̳� ���� ����
        player[player_id].stamina = max(0, player[player_id].stamina - 60);
    }
}

//ȸ���õ� ����
void perform_persuasion(int player_id, int other_player_id) {
    if (player[player_id].intel > player[other_player_id].intel) {
        // ���� ����
        printf("ȸ�� ����!\n");
        // ������ ��ȯ ����(�������� �ִٸ�)
        if (player[other_player_id].hasitem) {
            ITEM temp = player[player_id].item;
            player[player_id].item = player[other_player_id].item;
            player[other_player_id].item = temp;
            player[player_id].hasitem = true;
            player[other_player_id].hasitem = false;
        }
        // ���¹̳� ���� ����
        player[player_id].stamina = max(0, player[player_id].stamina - 20);
    }
    else {
        // ȸ�� ���� ����
        printf("ȸ�� ����...\n");
        // ���¹̳� ���� ����
        player[player_id].stamina = max(0, player[player_id].stamina - 40);
    }
}


void handle_interaction(int player_id, int other_player_id) {
    // �÷��̾� 0���� ��ȣ�ۿ�
    if (player_id == 0 || other_player_id == 0) {
        printf("Player %d �� Player%d ���� ��ȣ�ۿ��� �մϴ�. \n", player_id, other_player_id);

        // ��ȣ�ۿ� ������ ����
        printf("1) ��Ż�õ�\n");
        printf("2) ȸ���õ�\n");
        printf("3) ����\n");
        printf("����: ");
        int choice;
        scanf_s("%d", &choice);
        getchar(); // �Է� ���� ����

        switch (choice) {
        case 1: // ��Ż�õ�
            perform_robbery(player_id, other_player_id);
            break;
        case 2: // ȸ���õ�
            perform_persuasion(player_id, other_player_id);
            break;
        case 3: // ����
            printf("��ȣ�ۿ��� �����մϴ�.\n");
            break;
        default:
            printf("�߸��� �����Դϴ�.\n");
        }
    } else {
        // �÷��̾� 0�� �ƴ� �÷��̾�� ���� ��ȣ�ۿ�
        int action = randint(1, 3); // 1: ��Ż, 2: ȸ��, 3: ����

        switch (action) {
        case 1:
            perform_robbery(player_id, other_player_id);
            break;
        case 2:
            perform_persuasion(player_id, other_player_id);
            break;
        case 3:
            printf("Player %d�� Player %d�� ��ȣ�ۿ��� ���� �ʽ��ϴ�.\n", player_id, other_player_id);
            break;
        }
    }
}

void player_interaction(int player_id) {
    // �̹� ��ȣ�ۿ������� �ǳʶ�
    if (player[player_id].interacted) return;

    // �÷��̾� �ֺ��� ���� ĭ �˻�
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue; // �ڱ� �ڽ��� �ǳʶٱ�
            int nx = px[player_id] + dx;
            int ny = py[player_id] + dy;

            for (int i = 0; i < n_player; i++) {
                if (i != player_id && px[i] == nx && py[i] == ny && player[i].is_alive) {
                    // ��ȣ�ۿ� ����
                    handle_interaction(player_id, i);

                    // ��ȣ�ۿ� �߻� �� �÷��� ����
                    player[player_id].interacted = true;
                    player[i].interacted = true;

                    return; // �� �Ͽ� �ϳ��� ��ȣ�ۿ븸 ó��
                }
            }
        }
    }
}

void nightgame(void) {
    system("cls");
    ng_init();
    display();
    while (1) {
        // player 0�� ������ ������(4����)
        key_t key = get_key();
        if (key == K_QUIT) {
            break;
        }
        else if (key != K_UNDEFINED) {

            move_manual(key);

        }
        // player 1�� ������
        for (int i = 1; i < n_player; i++) {
            if (tick % period[i] == 0) {
                ngmv_random(i, -1);
            }
        }
        // ������ ��ȣ�ۿ� ����
        for (int i = 0; i < n_player; i++) {
            if (player[i].is_alive) {
                if (player[i].hasitem) {
                    exchange_item(i);
                }
                else {
                    pickup_item(i);
                }
                // �÷��̾� �� ��ȣ�ۿ�
                player_interaction(i);
            }
        }

        display();
        Sleep(10);
        tick += 10;
    }
}