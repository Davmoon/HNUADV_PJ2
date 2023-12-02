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
void ck_near_itm(int);

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
void ngmv_random(int pnum) {
	int itm_or_player_num;
	ck_near_itm(pnum);


	int nx, ny;  // 움직여서 다음에 놓일 자리

	do {
		nx = randint(-1, 1);
		ny = randint(-1, 1);
	} while (!placable(nx, ny));

	move_tail(player, nx, ny);
}

void ck_near_itm(int pnum) {
	double len = 0.;
	bool itmT_or_playerF = true;
	int itm_num = 0, player_itm_num = 0;

	for (int i = 0; i < n_item; i++) {
		// 아이템과 플레이어 좌표중 어느것이 더 클지 모르기 때문에 abs()사용
		int dx = abs(itmx[i] - px[pnum]); int dy = abs(itmy[i] - py[pnum]);

		int x = dx * dx; int y = dy * dy;
		double lena = sqrt(x + y);

		//len이 작은 것을 찾는 것이기 때문에 0이 들어가면 계속 0이기 때문.
		if (i == 0) { len = lena; continue; }

		// 길이가 짧으면 lena로 교체, 그리고 itm 번호를 
		if (lena < len) { len = lena; itm_num = i; }
	}

	for (int i = 0; i < n_player; i++) {
		if (i != player[i].hasitem && i != pnum && player[i].is_alive == true ) {
			// 두 플레이어 좌표중 어느것이 더 클지 모르기 때문에 abs()사용
			int dx = abs(px[i] - px[pnum]); int dy = abs(py[i] - py[pnum]);

			int x = dx * dx; int y = dy * dy;
			double lena = sqrt(x + y);

			//len이 작은 것을 찾는 것이기 때문에 0이 들어가면 계속 0이기 때문.
			if (i == 0) { len = lena; continue; }

			// 길이가 짧으면 lena로 교체, 그리고 itm 번호를 
			if (lena < len) { len = lena; player_itm_num = i; itmT_or_playerF = false; }
		}
	}

	// itm이 
	if (itmT_or_playerF == true) {

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

		//0을 제외한 플레이어 아이템(혹은 아이템 가진 플레이어)로 이동하는 코드
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