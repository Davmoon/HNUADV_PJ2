#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>
#include <math.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void ng_init();
void ngmv_random(int);
void nightgame();
void ck_near_item();

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
	for (int i = 0; i < n_alive - 1; i++) {
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

// 0 <= dir < 4가 아니면 랜덤
void ngmv_random(int player) {
	int p = player;  // 이름이 길어서...
	int nx, ny;  // 움직여서 다음에 놓일 자리

	// 움직일 공간이 없는 경우는 없다고 가정(무한 루프에 빠짐)	

	do {
		nx = px[p] + randint(-1, 1);
		ny = py[p] + randint(-1, 1);
	} while (!placable(nx, ny));

	move_tail(p, nx, ny);
}

void ck_near_item() {
	double len = 0;

	for (int k= 1; k< n_player; k++) {
		if (player[k].is_alive == true) {

			//가장 긴 길이 체크
			for (int i = 0; i < n_alive - 2; i++) {
				for (int j = 1; j < n_alive - 1; j++) {
					int dx = itmx[i]; int dy = itmy[i];
					int xx = px[i]; int yy = py[i];

					int x = (dx - xx) * (dx - xx);
					int y = (dy - yy) * (dy - yy);
					double lena = sqrt(x + y);

					if (lena > len) {
						len = lena;
					}
				}
			}
		}
	}


	
}

void nightgame(void) {
	ng_init();
	system("cls");
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

		for (int i = 1; i < n_player; i++) {
			if (player[i].is_alive == true && tick % period[i] == 0) {
				ngmv_random(i);
			}
		}
		

		display();
		Sleep(10);
		tick += 10;
	}
}