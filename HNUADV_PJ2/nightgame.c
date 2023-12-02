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
bool ck_near_itm(int, int*);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX], itmx[PLAYER_MAX], itmy[PLAYER_MAX];

void ng_init(void) {
	map_init(15, 40);//#으로 둘러쌓인 sample.c의 실제 플레이 맵 부분
	int period_set[] = { 300,300,300,300,300,300,300,300,300,300 };
	//int period_set[] = { 250, 260, 270, 280, 290, 300, 310, 320, 330, 340 };

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
			period[i] = period_set[i];

			back_buf[px[i]][py[i]] = '0' + i;  // (0 .. n_player-1)
		}
	}

	//item 랜덤 배치 추후 item 종류도 랜덤화 추가
	for (int i = 0; i < n_alive; i++) {
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
	int nx = px[pnum], ny = py[pnum];  // 움직여서 다음에 놓일 자리
	int target_x, target_y;

	if (ck_near_itm(pnum, &itm_or_player_num)) {
		//item index인 경우
		target_x = itmx[itm_or_player_num]; target_y = itmy[itm_or_player_num];
	}
	else {
		// 플레이어 index인 경우
		target_x = px[itm_or_player_num]; target_y = py[itm_or_player_num];
	}

	// 여기 코드에 문제가 있는듯. 아직 아이템 먹었을 때 처리 안해서 그런듯.
	if (px[pnum] < target_x) nx++;
	else if (px[pnum] > target_x) nx--;

	if (py[pnum] < target_y) ny++;
	else if (py[pnum] > target_y) ny--;

	if (placable(nx, ny)) {
		move_tail(pnum, nx, ny);
	}
}

bool ck_near_itm(int pnum, int* itm_or_player_num) {
	double len = 0.;
	bool itmT_or_playerF = true;
	int short_index = 0;

	for (int i = 0; i < n_item; i++) {
		// 아이템과 플레이어 좌표중 어느것이 더 클지 모르기 때문에 abs() 절댓값 사용
		int dx = abs(itmx[i] - px[pnum]); int dy = abs(itmy[i] - py[pnum]);

		int x = dx * dx; int y = dy * dy;
		double lena = sqrt(x + y);

		//len이 작은 것을 찾는 것이기 때문에 0이 들어가면 계속 0이기 때문.
		if (i == 0) { len = lena; continue; }

		// 길이가 짧으면 lena로 교체, 그리고 itm 번호를 저장
		if (lena < len) { len = lena; short_index = i; }
	}

	for (int i = 0; i < n_player; i++) {
		if (i == player[i].hasitem && i != pnum && player[i].is_alive == true ) {
			// 두 플레이어 좌표중 어느것이 더 클지 모르기 때문에 abs() 절댓값 사용
			int dx = abs(px[i] - px[pnum]); int dy = abs(py[i] - py[pnum]);

			int x = dx * dx; int y = dy * dy;
			double lena = sqrt(x + y);

			// 길이가 짧으면 lena로 교체, 그리고 player 번호를 저장
			if (lena < len) { len = lena; short_index = i; itmT_or_playerF = false; }
		}
	}

	*itm_or_player_num = short_index;

	// itm이 
	if (itmT_or_playerF == true) {
		return true;
	}
	else {
		return false;
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