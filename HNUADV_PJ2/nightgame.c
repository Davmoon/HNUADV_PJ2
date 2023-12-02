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
	for (int i = 0; i < n_alive - 1; i++) {
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
	ck_near_itm(pnum);


	int nx, ny;  // �������� ������ ���� �ڸ�

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
		// �����۰� �÷��̾� ��ǥ�� ������� �� Ŭ�� �𸣱� ������ abs()���
		int dx = abs(itmx[i] - px[pnum]); int dy = abs(itmy[i] - py[pnum]);

		int x = dx * dx; int y = dy * dy;
		double lena = sqrt(x + y);

		//len�� ���� ���� ã�� ���̱� ������ 0�� ���� ��� 0�̱� ����.
		if (i == 0) { len = lena; continue; }

		// ���̰� ª���� lena�� ��ü, �׸��� itm ��ȣ�� 
		if (lena < len) { len = lena; itm_num = i; }
	}

	for (int i = 0; i < n_player; i++) {
		if (i != player[i].hasitem && i != pnum && player[i].is_alive == true ) {
			// �� �÷��̾� ��ǥ�� ������� �� Ŭ�� �𸣱� ������ abs()���
			int dx = abs(px[i] - px[pnum]); int dy = abs(py[i] - py[pnum]);

			int x = dx * dx; int y = dy * dy;
			double lena = sqrt(x + y);

			//len�� ���� ���� ã�� ���̱� ������ 0�� ���� ��� 0�̱� ����.
			if (i == 0) { len = lena; continue; }

			// ���̰� ª���� lena�� ��ü, �׸��� itm ��ȣ�� 
			if (lena < len) { len = lena; player_itm_num = i; itmT_or_playerF = false; }
		}
	}

	// itm�� 
	if (itmT_or_playerF == true) {

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