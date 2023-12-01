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
    map_init(15, 40);//#으로 둘러쌓인 sample.c의 실제 플레이 맵 부분

    int x, y;
    for (int i = 0; i < n_player; i++) {
        // 같은 자리가 나오면 다시 생성
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

    //item 랜덤 배치
    for (int i = 0; i < n_player - 1; i++) {
        do {
            x = randint(1, N_ROW - 2);
            y = randint(1, N_COL - 2);
        } while (!placable(x, y));
        itmx[i] = x;
        itmy[i] = y;

        back_buf[itmx[i]][itmy[i]] = 'I';  // 아이템은 항상 i로 표기
    }

    tick = 0;
}


bool ngmv_manual(key_t key) {
    // 각 방향으로 움직일 때 x, y값 delta
    static int dx[4] = { -1, 1, 0, 0 };
    static int dy[4] = { 0, 0, -1, 1 };

    int dir;  // 움직일 방향(0~3)
    switch (key) {
    case K_UP: dir = DIR_UP; break;
    case K_DOWN: dir = DIR_DOWN; break;
    case K_LEFT: dir = DIR_LEFT; break;
    case K_RIGHT: dir = DIR_RIGHT; break;
    default: return;
    }

    // 움직여서 놓일 자리
    int nx, ny;
    nx = px[0] + dx[dir];
    ny = py[0] + dy[dir];
    if (!placable(nx, ny)) {
        return false;
    }

    move_tail(0, nx, ny);
    return true;
}

// 가장 가까운 목표 찾기
void find_nearest_target(int p, int* target_x, int* target_y) {
    int min_dist = INT_MAX;
    int dist, tx, ty;

    // 아이템 위치 확인
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

    // 다른 플레이어가 가진 아이템 위치 확인
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


//이동 주기 및 로직 업데이트
void ngmv_random(int p) {
    int target_x, target_y;
    find_nearest_target(p, &target_x, &target_y);

    // 가장 가까운 목표를 향해 이동
    int nx = px[p], ny = py[p];
    if (px[p] < target_x) nx++;
    else if (px[p] > target_x) nx--;

    if (py[p] < target_y) ny++;
    else if (py[p] > target_y) ny--;

    // 이동 가능 여부 확인
    if (placable(nx, ny)) {
        move_tail(p, nx, ny);
    }
}

//아이템 획득 로직
void pickup_item(int player_id) {
    for (int i = 0; i < n_item; i++) {
        if (px[player_id] == itmx[i] && py[player_id] == itmy[i] && !player[player_id].hasitem) {
            player[player_id].item = item[i];
            player[player_id].hasitem = true;

            // 아이템을 맵에서 제거
            back_buf[itmx[i]][itmy[i]] = '0' + player_id; // 플레이어의 번호로 다시 표시
            itmx[i] = -1;
            itmy[i] = -1;

            // 아이템 획득 다이얼로그 표시
            char message[100];
            sprintf_s(message, sizeof(message), "Player %d: %s를 획득했습니다.", player_id, item[i].name);
            dialog(message);
            break;
        }
    }
}

//아이템 교환 로직
void exchange_item(int player_id) {
    for (int i = 0; i < n_item; i++) {
        // 플레이어와 아이템이 같은 위치에 있는지 확인
        if (px[player_id] == itmx[i] && py[player_id] == itmy[i] && player[player_id].hasitem) {
            bool exchange = false;

            // 플레이어 0이면 키보드 입력으로 결정
            if (player_id == 0) {
                printf("아이템을 교환하시겠습니까? (y/n): ");
                char choice = getchar();
                exchange = (choice == 'y' || choice == 'Y');
                getchar();
            }
            else {
                // 다른 플레이어는 50% 확률로 결정
                exchange = (randint(0, 1) == 1);
            }

            if (exchange) {
                // 새로운 무작위 위치 생성
                int new_item_x, new_item_y;
                do {
                    new_item_x = randint(1, N_ROW - 2);
                    new_item_y = randint(1, N_COL - 2);
                } while (!placable(new_item_x, new_item_y));

                // 아이템 교환
                ITEM temp = player[player_id].item;
                player[player_id].item = item[i];
                item[i] = temp;

                // 기존 아이템 위치에 새 아이템 배치
                itmx[i] = new_item_x;
                itmy[i] = new_item_y;
                back_buf[new_item_x][new_item_y] = 'I';

                // 교환 다이얼로그 표시
                char message[100];
                sprintf_s(message, sizeof(message), "Player %d: %s 를 %s로 교환했습니다.", player_id, temp.name, player[player_id].item.name);
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

//강탈시도 로직
void perform_robbery(int player_id, int other_player_id) {
    if (player[player_id].str > player[other_player_id].str) {
        // 성공 로직
        printf("강탈 성공!\n");
        // 아이템 교환 로직
        if (player[other_player_id].hasitem) {
            ITEM temp = player[player_id].item;
            player[player_id].item = player[other_player_id].item;
            player[other_player_id].item = temp;
            player[player_id].hasitem = true;
            player[other_player_id].hasitem = false;
        }
        // 스태미나 조정
        player[player_id].stamina = max(0, player[player_id].stamina - 40);
    }
    else {
        // 실패 로직
        printf("강탈 실패...\n");
        // 스태미나 조정 로직
        player[player_id].stamina = max(0, player[player_id].stamina - 60);
    }
}

//회유시도 로직
void perform_persuasion(int player_id, int other_player_id) {
    if (player[player_id].intel > player[other_player_id].intel) {
        // 성공 로직
        printf("회유 성공!\n");
        // 아이템 교환 로직(아이템이 있다면)
        if (player[other_player_id].hasitem) {
            ITEM temp = player[player_id].item;
            player[player_id].item = player[other_player_id].item;
            player[other_player_id].item = temp;
            player[player_id].hasitem = true;
            player[other_player_id].hasitem = false;
        }
        // 스태미나 조정 로직
        player[player_id].stamina = max(0, player[player_id].stamina - 20);
    }
    else {
        // 회유 실패 로직
        printf("회유 실패...\n");
        // 스태미나 조정 로직
        player[player_id].stamina = max(0, player[player_id].stamina - 40);
    }
}


void handle_interaction(int player_id, int other_player_id) {
    // 플레이어 0과의 상호작용
    if (player_id == 0 || other_player_id == 0) {
        printf("Player %d 가 Player%d 에게 상호작용을 합니다. \n", player_id, other_player_id);

        // 상호작용 선택지 제공
        printf("1) 강탈시도\n");
        printf("2) 회유시도\n");
        printf("3) 무시\n");
        printf("선택: ");
        int choice;
        scanf_s("%d", &choice);
        getchar(); // 입력 버퍼 비우기

        switch (choice) {
        case 1: // 강탈시도
            perform_robbery(player_id, other_player_id);
            break;
        case 2: // 회유시도
            perform_persuasion(player_id, other_player_id);
            break;
        case 3: // 무시
            printf("상호작용을 무시합니다.\n");
            break;
        default:
            printf("잘못된 선택입니다.\n");
        }
    } else {
        // 플레이어 0이 아닌 플레이어들 간의 상호작용
        int action = randint(1, 3); // 1: 강탈, 2: 회유, 3: 무시

        switch (action) {
        case 1:
            perform_robbery(player_id, other_player_id);
            break;
        case 2:
            perform_persuasion(player_id, other_player_id);
            break;
        case 3:
            printf("Player %d와 Player %d는 상호작용을 하지 않습니다.\n", player_id, other_player_id);
            break;
        }
    }
}

void player_interaction(int player_id) {
    // 이미 상호작용했으면 건너뜀
    if (player[player_id].interacted) return;

    // 플레이어 주변의 인접 칸 검사
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue; // 자기 자신은 건너뛰기
            int nx = px[player_id] + dx;
            int ny = py[player_id] + dy;

            for (int i = 0; i < n_player; i++) {
                if (i != player_id && px[i] == nx && py[i] == ny && player[i].is_alive) {
                    // 상호작용 로직
                    handle_interaction(player_id, i);

                    // 상호작용 발생 시 플래그 설정
                    player[player_id].interacted = true;
                    player[i].interacted = true;

                    return; // 한 턴에 하나의 상호작용만 처리
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
        // player 0만 손으로 움직임(4방향)
        key_t key = get_key();
        if (key == K_QUIT) {
            break;
        }
        else if (key != K_UNDEFINED) {

            move_manual(key);

        }
        // player 1의 움직임
        for (int i = 1; i < n_player; i++) {
            if (tick % period[i] == 0) {
                ngmv_random(i, -1);
            }
        }
        // 아이템 상호작용 로직
        for (int i = 0; i < n_player; i++) {
            if (player[i].is_alive) {
                if (player[i].hasitem) {
                    exchange_item(i);
                }
                else {
                    pickup_item(i);
                }
                // 플레이어 간 상호작용
                player_interaction(i);
            }
        }

        display();
        Sleep(10);
        tick += 10;
    }
}