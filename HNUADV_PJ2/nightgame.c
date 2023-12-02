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
	for (int i = 0; i < n_alive; i++) {
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

// 0 <= dir < 4�� �ƴϸ� ����
void ngmv_random(int pnum) {
	int itm_or_player_num;
	int nx = px[pnum], ny = py[pnum];  // �������� ������ ���� �ڸ�
	int target_x, target_y;

	if (ck_near_itm(pnum, &itm_or_player_num)) {
		//item index�� ���
		target_x = itmx[itm_or_player_num]; target_y = itmy[itm_or_player_num];
	}
	else {
		// �÷��̾� index�� ���
		target_x = px[itm_or_player_num]; target_y = py[itm_or_player_num];
	}

	// ���� �ڵ忡 ������ �ִµ�. ���� ������ �Ծ��� �� ó�� ���ؼ� �׷���.
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
		// �����۰� �÷��̾� ��ǥ�� ������� �� Ŭ�� �𸣱� ������ abs() ���� ���
		int dx = abs(itmx[i] - px[pnum]); int dy = abs(itmy[i] - py[pnum]);

		int x = dx * dx; int y = dy * dy;
		double lena = sqrt(x + y);

		//len�� ���� ���� ã�� ���̱� ������ 0�� ���� ��� 0�̱� ����.
		if (i == 0) { len = lena; continue; }

		// ���̰� ª���� lena�� ��ü, �׸��� itm ��ȣ�� ����
		if (lena < len) { len = lena; short_index = i; }
	}

	for (int i = 0; i < n_player; i++) {
		if (i == player[i].hasitem && i != pnum && player[i].is_alive == true ) {
			// �� �÷��̾� ��ǥ�� ������� �� Ŭ�� �𸣱� ������ abs() ���� ���
			int dx = abs(px[i] - px[pnum]); int dy = abs(py[i] - py[pnum]);

			int x = dx * dx; int y = dy * dy;
			double lena = sqrt(x + y);

			// ���̰� ª���� lena�� ��ü, �׸��� player ��ȣ�� ����
			if (lena < len) { len = lena; short_index = i; itmT_or_playerF = false; }
		}
	}

	*itm_or_player_num = short_index;

	// itm�� 
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
			if (player[i].is_alive == true && tick % period[i] == 0) {
				ngmv_random(i);
			}
		}


		
		display();
		Sleep(10);
		tick += 10;
	}
}