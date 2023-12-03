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
void itraction(bool, int, int, int, int);
void nightgame();
void cg_item(int, int);
bool ck_near_itm(int, int*);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX], itmx[PLAYER_MAX], itmy[PLAYER_MAX];

void ng_init(void) {
	map_init(15, 40);//#으로 둘러쌓인 sample.c의 실제 플레이 맵 부분
	int period_set[] = { 800,800,800,800,800,800,800,800,800,800 };
	//int period_set[] = { 250, 260, 270, 280, 290, 300, 310, 320, 330, 340 };

	int x, y;
	for (int i = 0; i < PLAYER_MAX; i++) {
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
		itmx[i] = itmy[i] = -1;
	}

	//item 랜덤 배치 추후 item 종류도 랜덤화 추가
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
void ngmv_random(int p) {
	int itm_or_player_num;
	int nx = px[p], ny = py[p];  // 움직여서 다음에 놓일 자리
	int target_x, target_y;
	bool check = ck_near_itm(p, &itm_or_player_num);

	if (check) {
		//item index인 경우
		target_x = itmx[itm_or_player_num]; target_y = itmy[itm_or_player_num];
	}
	else {
		// 플레이어 index인 경우
		target_x = px[itm_or_player_num]; target_y = py[itm_or_player_num];
	}

	if (px[p] < target_x) nx++;
	else if (px[p] > target_x) nx--;

	if (py[p] < target_y) ny++;
	else if (py[p] > target_y) ny--;


	if (placable(nx, ny) && p != 0) {
		move_tail(p, nx, ny);
	}
	else if (nx == target_x && ny == target_y) {
		itraction(check, itm_or_player_num, p, nx, ny);
	}

}

void itraction(bool check, int itm_or_player_num, int p, int nx, int ny) {

	//아이템에 접근하는 경우
	if (check) {
		if (player[p].hasitem == false) {
			back_buf[itmx[itm_or_player_num]][itmy[itm_or_player_num]] = ' ';
			itmx[itm_or_player_num] = -1; itmy[itm_or_player_num] = -1;
			player[p].hasitem = true;
			player[p].item = item[itm_or_player_num]; // 포인터 주소값으로 연결해줌.
			char msgp[30];
			sprintf_s(msgp, sizeof(msgp), "player %d 아이템 획득", p);
			//dialog(msgp);
		}
		else {
			cg_item(p, itm_or_player_num);
		}

	}
	// 아이템 가진 플레이어에 접근하는 경우
	else {
		if (player[itm_or_player_num].hasitem) {
			cg_player_itm();
		}
		else {

		}
	}
}

void cg_player_itm() {

}

void cg_item(int p, int itmnum) {
	int choose = randint(0, 1); // 0교환, 1미교환
	int nnx, nny;

	do {
		nnx = randint(1, N_ROW - 2);
		nny = randint(1, N_COL - 2);
	} while (!placable(nnx, nny));

	if (p == 0) {
		gotoxy(N_ROW, 0);
		printf("아이템을 교환하시겠습니까? y/n");
		while (1) {
			key_t key = get_key();
			if (key == K_YES) {
				back_buf[itmx[itmnum]][itmy[itmnum]] = ' ';
				itmx[itmnum] = -1; itmy[itmnum] = -1;
				ITEM temp = player[p].item;
				player[p].item = item[itmnum];
				item[itmnum] = temp;

				itmx[itmnum] = nnx;
				itmy[itmnum] = nny;
				back_buf[nnx][nny] = 'I';

				dialog("0번 아이템 교체됨");
				gotoxy(N_ROW, 0);
				printf("                                      ");
				break;
			}
			if (key == K_NO) {
				back_buf[itmx[itmnum]][itmy[itmnum]] = ' ';
				move_tail(p, itmx[itmnum], itmy[itmnum]);
				itmx[itmnum] = nnx;
				itmy[itmnum] = nny;
				back_buf[nnx][nny] = 'I';

				tick = 0;
				gotoxy(N_ROW, 0);
				printf("%d번 교체 거부, 아이템 랜덤 배치", p);

				break;
			}
		}
	}
	else {
		if (choose == 0) {
			back_buf[itmx[itmnum]][itmy[itmnum]] = ' ';
			itmx[itmnum] = -1; itmy[itmnum] = -1;
			ITEM temp = player[p].item;
			player[p].item = item[itmnum];
			item[itmnum] = temp;

			itmx[itmnum] = nnx;
			itmy[itmnum] = nny;
			back_buf[nnx][nny] = 'I';

			char msgp[30];
			sprintf_s(msgp, sizeof(msgp), "player %d 아이템 교체", p);
			gotoxy(N_ROW, 0);
			printf("%s", msgp);
		}
		else {
			back_buf[itmx[itmnum]][itmy[itmnum]] = ' ';
			itmx[itmnum] = nnx;
			itmy[itmnum] = nny;
			back_buf[nnx][nny] = 'I';

			tick = 0;
			gotoxy(N_ROW, 0);
			printf("%d번 교체 거부, 아이템 랜덤 배치", p);

		}
	}
}

bool ck_near_itm(int p, int* itm_or_player_num) {
	int len = INT_MAX;
	bool itmT_or_playerF = true;
	int short_index = 0;

	for (int i = 0; i < n_item; i++) {
		if (itmx[i] != -1 && itmy[i] != -1) {
			// 아이템과 플레이어 좌표중 어느것이 더 클지 모르기 때문에 abs() 절댓값 사용
			int lena = abs(px[p] - itmx[i]) + abs(py[p] - itmy[i]);

			// 길이가 짧으면 lena로 교체, 그리고 itm 번호를 저장
			if (lena < len) { len = lena; short_index = i; }
		}
	}
	if (len == INT_MAX) { return 0; }// 아이템 남은 게 없으면 끝나게 하는 임시 제한 코드

	/*for (int i = 0; i < n_player; i++) {
		if (player[i].hasitem && i != p && player[i].is_alive == true) {
			// 두 플레이어 좌표중 어느것이 더 클지 모르기 때문에 abs() 절댓값 사용
			int lena = abs(px[p] - px[i]) + abs(py[p] - py[i]);

			// 길이가 짧으면 lena로 교체, 그리고 player 번호를 저장
			if (lena < len) { len = lena; short_index = i; itmT_or_playerF = false; }
		}
	}*/

	*itm_or_player_num = short_index;

	if (itmT_or_playerF == true) { return true; }
	else if (itmT_or_playerF == false) { return false; }
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
			ngmv_random(0);
			move_manual(key);
		}

		//0을 제외한 플레이어 아이템(혹은 아이템 가진 플레이어)로 이동하는 코드
		for (int i = 1; i < n_player; i++) {
			if (player[i].is_alive && tick % period[i] == 0) {
				ngmv_random(i);
			}
		}

		for (int i = 0; i < n_player; i++) {
			if (player[i].is_alive) {
				if (player[i].hasitem) {

				}
				else {

				}
			}
		}


		display();
		Sleep(10);
		tick += 10;
	}
}