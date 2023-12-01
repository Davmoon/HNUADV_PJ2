
#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void ng_init();
void ngmv_random(int, int);
bool ngmv_manual(key_t);
void nightgame();

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX], itmx[PLAYER_MAX], itmy[PLAYER_MAX], curr_itmx[PLAYER_MAX], curr_itmy[PLAYER_MAX];

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
	for (int i = 0; i < n_player; i++) {
		do {
			x = randint(1, N_ROW - 2);
			y = randint(1, N_COL - 2);
		} while (!placable(x, y));
		itmx[i] = x;
		itmy[i] = y;

		back_buf[itmx[i]][itmy[i]] = 'I';  // 아이템은 항상 i로 표기
	}
	for (int i = 0; i < n_player; i++) {
		curr_itmx[i] = -1;
		curr_itmy[i] = -1;
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


void nightgame(void) {
	sample_init();
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
		for (int i = 0; i < n_player; i++) {
			for (int j = 0; j < n_item; j++) {
				// 플레이어가 아이템과 인접한지 확인
				if (abs(px[i] - itmx[j]) + abs(py[i] - itmy[j]) == 1 && itmx[j] != -1) {
					if (!player[i].hasitem) {
						player[i].hasitem = true;
						curr_itmx[i] = itmx[j];
						curr_itmy[i] = itmy[j];
						// 아이템 획득
						printf("Player %d acquired item at (%d, %d)\n", i, itmx[j], itmy[j]);
						// 아이템을 맵에서 제거
						back_buf[itmx[j]][itmy[j]] = ' '; // 아이템 위치를 공백으로 변경
						itmx[j] = -1; // 아이템 위치를 유효하지 않은 값으로 설정
						itmy[j] = -1;
					}else {
						// 교환 로직 실행
						bool exchange = false;
						if (i == 0) { // 플레이어 0
							printf("교환하려면 Y를, 그렇지 않으면 N을 누르세요: ");



							//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
							//!!!!!!!!!!! 여기를 교수님 sample 코드처럼 get key로 바꾸세요.
							char decision = getchar();
							if (decision == 'Y' || decision == 'y') {
								exchange = true;
							}
							else if (decision == 'N' || decision == 'n') {
								//무시하는 코드 추가
								printf("Player %d ignored item at (%d, %d)\n", i, itmx[j], itmy[j]);
								continue;
							}
						} else {
							if (randint(0, 1)) {
								exchange = true;
							}

						}
						if (exchange) {
							back_buf[curr_itmx[i]][curr_itmy[i]] = 'I'; // 이전 아이템 위치에 아이템 표시
							// 새 아이템의 위치를 현재 아이템 위치로 업데이트
							curr_itmx[i] = itmx[j];
							curr_itmy[i] = itmy[j];
							printf("Player %d exchanged item at (%d, %d)\n", i, itmx[j], itmy[j]);
							back_buf[itmx[j]][itmy[j]] = ' '; // 아이템 위치를 공백으로 변경
							itmx[j] = -1; // 아이템 위치를 유효하지 않은 값으로 설정
							itmy[j] = -1;
						}
					}
				}
			}
		}

		display();
		Sleep(10);
		tick += 10;
	}
}