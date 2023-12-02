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
void cg_item(int, int);
bool ck_near_itm(int, int*);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX], itmx[PLAYER_MAX], itmy[PLAYER_MAX];

void ng_init(void) {
	map_init(15, 40);//#���� �ѷ����� sample.c�� ���� �÷��� �� �κ�
	int period_set[] = { 300,300,300,300,300,300,300,300,300,300 };
	//int period_set[] = { 250, 260, 270, 280, 290, 300, 310, 320, 330, 340 };

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
			period[i] = period_set[i];

			back_buf[px[i]][py[i]] = '0' + i;  // (0 .. n_player-1)
		}
	}

	//item ���� ��ġ ���� item ������ ����ȭ �߰�
	for (int i = 0; i < n_alive - 1; i++) {
		do {
			x = randint(1, N_ROW - 2);
			y = randint(1, N_COL - 2);
		} while (!placable(x, y));
		itmx[i] = x;
		itmy[i] = y;
		itm_take[i] = true;

		back_buf[itmx[i]][itmy[i]] = 'I';  // �������� �׻� i�� ǥ��
	}

	tick = 0;
}

// 0 <= dir < 4�� �ƴϸ� ����
void ngmv_random(int p) {
	int itm_or_player_num;
	int nx = px[p], ny = py[p];  // �������� ������ ���� �ڸ�
	int target_x, target_y;
	

	if (ck_near_itm(p, &itm_or_player_num)) {
		//item index�� ���
		target_x = itmx[itm_or_player_num]; target_y = itmy[itm_or_player_num];
	}
	else {
		// �÷��̾� index�� ���
		target_x = px[itm_or_player_num]; target_y = py[itm_or_player_num];
	}

	if (px[p] < target_x) nx++;
	else if (px[p] > target_x) nx--;

	if (py[p] < target_y) ny++;
	else if (py[p] > target_y) ny--;

	if (placable(nx, ny)) {
		move_tail(p, nx, ny);
	}

}

void itraction() {
		else if (nx == target_x && ny == target_y) {
			//�����ۿ� �����ϴ� ���
			if (check) {
				if (player[p].hasitem == false) {
					move_tail(p, nx, ny);
					itm_take[itm_or_player_num] = false;
					player[p].hasitem = true;
					player[p].item = item[itm_or_player_num]; // ������ �ּҰ����� ��������.
				}
				else {
					cg_item(p, itm_or_player_num);
				}

			}
			// ������ ���� �÷��̾ �����ϴ� ���
			else {

			}
			}
}

void cg_item(int p, int itmnum) {
	int choose = randint(0, 1);
	
	if (p == 0) {
		gotoxy(N_ROW, 0);
		printf("�������� ��ȯ�Ͻðڽ��ϱ�? y/n");
		while (1) {
			key_t key = get_key();
			if (key == K_YES) {
				player[p].item = item[itmnum];
				dialog("0�� ������ ��ü��");
				break;
			}
			if (key == K_NO) {
				break;
			}
		}
	}
	else {
		if (choose == 0) {}
		else {
			player[p].item = item[itmnum];
			char msgp[100];

			sprintf_s(msgp, sizeof(msgp), "player %d ������ ��ü", p);
			dialog(msgp);
		}
	}
}

bool ck_near_itm(int p, int* itm_or_player_num) {
	int len = INT_MAX;
	bool itmT_or_playerF = true;
	int short_index = 0;

	for (int i = 0; i < n_item; i++) {
		if (itm_take[i] == true) {
			// �����۰� �÷��̾� ��ǥ�� ������� �� Ŭ�� �𸣱� ������ abs() ���� ���
			int lena = abs(px[p] - itmx[i]) + abs(py[p] - itmy[i]);

			// ���̰� ª���� lena�� ��ü, �׸��� itm ��ȣ�� ����
			if (lena < len) { len = lena; short_index = i; }
		}
	}

	for (int i = 0; i < n_player; i++) {
		if (player[i].hasitem && i != p && player[i].is_alive == true ) {
			// �� �÷��̾� ��ǥ�� ������� �� Ŭ�� �𸣱� ������ abs() ���� ���
			int lena = abs(px[p] - px[i]) + abs(py[p] - py[i]);

			// ���̰� ª���� lena�� ��ü, �׸��� player ��ȣ�� ����
			if (lena < len) { len = lena; short_index = i; itmT_or_playerF = false; }
		}
	}

	*itm_or_player_num = short_index;

	if (itmT_or_playerF == true) { return true; }
	else if (itmT_or_playerF == false){ return false; }
}

void nightgame(void) {
	ng_init();
	system("cls");
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

		//0�� ������ �÷��̾� ������(Ȥ�� ������ ���� �÷��̾�)�� �̵��ϴ� �ڵ�
		for (int i = 1; i < n_player; i++) {
			if (player[i].is_alive && tick % period[i] == 0) {
				ngmv_random(i);
			}
		}

		// ��� �÷��̾� ����
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