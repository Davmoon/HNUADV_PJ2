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

// 플레이어와 아이템을 맵 상에 랜덤하게 배치하는 함수
void place_players_and_items() {
    // 플레이어 배치
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

            // 맵 상에 플레이어를 랜덤하게 배치
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

    // 아이템 배치
    for (int i = 0; i < n_item; ++i) {
        do {
            item[i].intel_buf = randint(1, 10);
            item[i].str_buf = randint(1, 10);
            item[i].stamina_buf = randint(1, 10);

            // 맵 상에 아이템을 랜덤하게 배치
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

// 플레이어의 이동을 처리하는 함수, 화살표 키로 Player 0를 조종
void move_players() {
    for (int i = 0; i < n_player; ++i) {
        if (!player[i].is_alive) {
            continue;
        }

        int new_row = -1, new_col = -1;

        // 해당 플레이어가 사용자에 의해 조종되는지 확인
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
                continue;  // 유효하지 않은 입력일 경우, 다음 플레이어로 계속 진행
            }
        }
        else {
            // 다른 플레이어의 경우, 무작위로 이동
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

        // 새로운 위치가 유효한지 확인하고 업데이트
        if (new_row >= 0 && new_row < N_ROW && new_col >= 0 && new_col < N_COL && placable(new_row, new_col)) {
            printxy(player[i].name[1], new_row, new_col);
            printxy(' ', player[i].id / N_COL, player[i].id % N_COL);  // 이전 위치를 지우기
            player[i].id = new_row * N_COL + new_col;  // 플레이어의 위치 업데이트
        }
    }
}


// 플레이어와 아이템 간의 상호작용을 처리하는 함수
void interact_with_items() {
    for (int i = 0; i < n_player; ++i) {
        if (!player[i].is_alive) {
            continue;
        }

        for (int j = 0; j < n_item; ++j) {
            // 플레이어와 아이템이 같은 위치에 있는지 확인
            if (player[i].id == item[j].name[1]) {
                // 플레이어가 아이템을 가지고 있지 않다면 획득
                if (!player[i].hasitem) {
                    player[i].item = item[j];
                    player[i].hasitem = true;
                    // 아이템 정보를 업데이트
                    item[j].name[1] = ' ';  // 아이템이 없어짐을 표시
                }
                // 플레이어가 아이템을 가지고 있다면 교환
                else {
                    ITEM temp = player[i].item;
                    player[i].item = item[j];
                    item[j] = temp;
                }
            }
        }
    }
}

// 플레이어 간의 상호작용을 처리하는 함수
void interact_between_players() {
    for (int i = 0; i < n_player; ++i) {
        if (!player[i].is_alive) {
            continue;
        }

        for (int j = i + 1; j < n_player; ++j) {
            // 두 플레이어가 같은 위치에 있는지 확인
            if (player[i].id == player[j].id) {
                // 플레이어 i가 플레이어 j에게 아이템을 강탈
                if (player[i].str > player[j].str) {
                    player[i].item = player[j].item;
                    player[j].hasitem = false;
                }
                // 플레이어 j가 플레이어 i에게 아이템을 강탈
                else if (player[j].str > player[i].str) {
                    player[j].item = player[i].item;
                    player[i].hasitem = false;
                }
            }
        }
    }
}
