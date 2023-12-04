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
void cg_player_itm(int, int);
void nightgame();
void cg_item(int, int);
bool ck_near_itm(int, int*);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX], itmx[PLAYER_MAX], itmy[PLAYER_MAX];

void ng_init(void) {
	map_init(15, 40);//#���� �ѷ����� sample.c�� ���� �÷��� �� �κ�
	//int period_set[] = { 800,800,800,800,800,800,800,800,800,800 };
	int period_set[] = { 420, 430, 440, 400, 410, 440, 440, 430, 430, 410 };

	int x, y;
	for (int i = 0; i < PLAYER_MAX; i++) {
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
		itmx[i] = itmy[i] = -1; //��� ������ ��ǥ�� ����
	}

	//item ���� ��ġ ���� item ������ ����ȭ �߰�
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
void ngmv_random(int p) {
	int itm_or_player_num;
	int nx = px[p], ny = py[p];  // �������� ������ ���� �ڸ�
	int target_x, target_y;
	bool counter = true;
	bool check = ck_near_itm(p, &itm_or_player_num);

	if (check) {
		//item index�� ���
		target_x = itmx[itm_or_player_num]; target_y = itmy[itm_or_player_num];
	}
	else {
		// �÷��̾� index�� ���
		target_x = px[itm_or_player_num]; target_y = py[itm_or_player_num];
	}
	

	if (px[p] < target_x) { nx++; counter = false; }
	else if (px[p] > target_x) { nx--; counter = false; }

	if (counter) {
		if (py[p] < target_y) ny++;
		else if (py[p] > target_y) ny--;
	}

	if (p != 0 && placable(nx, ny) ) {
		move_tail(p, nx, ny);
	}
	else if (nx == target_x && ny == target_y) {
		itraction(check, itm_or_player_num, p, nx, ny);
	}

}

void itraction(bool check, int itm_or_player_num, int p, int nx, int ny) {

	//�����ۿ� �����ϴ� ���
	if (check) {
		if (player[p].hasitem == false) {
			back_buf[itmx[itm_or_player_num]][itmy[itm_or_player_num]] = ' ';
			itmx[itm_or_player_num] = -1; itmy[itm_or_player_num] = -1;
			player[p].hasitem = true;
			player[p].item = item[itm_or_player_num]; // ������ �ּҰ����� ��������.
			//char msgp[30];
			//sprintf_s(msgp, sizeof(msgp), "player %d ������ ȹ��", p);
			//dialog(msgp);
			gotoxy(N_ROW, 0);
			printf("                                                    ");
			gotoxy(N_ROW, 0);
			printf("player %d ������ ȹ��", p);
		}
		else {
			cg_item(p, itm_or_player_num);
		}

	}
	// ������ ���� �÷��̾ �����ϴ� ���
	else {
		cg_player_itm(p, itm_or_player_num);
	}
}

void cg_player_itm(int p, int itm_pnum) {
	int select;

	if (p == 0) {
		gotoxy(N_ROW, 0);
		printf("                                                    ");
		gotoxy(N_ROW, 0);
		printf("%d���� 1:��Ż 2:ȸ�� 3:����", itm_pnum);
		while (1) {
			key_t key = get_key();
			if (key == K_first) { select = 1; break; }
			if (key == K_second) { select = 2; break;}
			if (key == K_third) {select = 3; break;}
		}
		gotoxy(N_ROW, 0);
		printf("                                                    ");
		gotoxy(N_ROW, 0);
		printf("����� ������ ���ڴ� : %d", select);
	}
	else {
		select = randint(1, 3);
	}
	
	switch (select) {
	case 1: //�÷��̾ ���׹̳��� ���� ��
		if (player[p].str > 0 && player[p].str > player[itm_pnum].str) {
			gotoxy(N_ROW, 0);
			printf("                                                    ");
			gotoxy(N_ROW, 0);
			printf("%d���� %d���� ���� ��Ż ����", p, itm_pnum);
			//Sleep(1000);

			//�÷��̾ �������� ���� ��
			if (player[p].hasitem) {
				ITEM temp = player[p].item;
				player[p].item = player[itm_pnum].item;
				player[itm_pnum].item = temp;
			}
			else {
				ITEM temp = player[p].item;
				player[p].item = player[itm_pnum].item;
				player[itm_pnum].item = temp;

				player[itm_pnum].hasitem = false;
				player[p].hasitem = true;
			}
			player[p].stamina = ((player[p].stamina / 10) * 6);
		}
		else {
			gotoxy(N_ROW, 0);
			printf("                                                    ");
			gotoxy(N_ROW, 0);
			printf("%d���� %d���� ���� ��Ż ����", p, itm_pnum);
			//Sleep(1000);

			player[p].stamina = ((player[p].stamina / 10) * 4);
		}
		break;
	case 2: //ȸ���ϴ� ���
		if (player[p].str > 0 && player[p].intel > player[itm_pnum].intel) {
			gotoxy(N_ROW, 0);
			printf("                                                    ");
			gotoxy(N_ROW, 0);
			printf("%d���� %d���� ���� ȸ�� ����", p, itm_pnum);
			//Sleep(1000);

			//�÷��̾ �������� ���� ��
			if (player[p].hasitem) {
				ITEM temp = player[p].item;
				player[p].item = player[itm_pnum].item;
				player[itm_pnum].item = temp;
			}
			else {
				ITEM temp = player[p].item;
				player[p].item = player[itm_pnum].item;
				player[itm_pnum].item = temp;

				player[itm_pnum].hasitem = false;
				player[p].hasitem = true;
			}
			player[p].stamina = ((player[p].stamina / 10) * 8);
		}
		else {
			gotoxy(N_ROW, 0);
			printf("                                                    ");
			gotoxy(N_ROW, 0);
			printf("%d���� %d���� ���� ȸ�� ����", p, itm_pnum);
			//Sleep(1000);

			player[p].stamina = ((player[p].stamina / 10) * 6);
		}
		break;
	case 3: //�����ϴ� ���
		gotoxy(N_ROW, 0);
		printf("                                                    ");
		gotoxy(N_ROW, 0);
		printf("%d���� %d�� ��ȣ�ۿ� ����", p, itm_pnum);
		//Sleep(1000);

		break;
	default:
		return 0;
	}
}

void cg_item(int p, int itmnum) {
	int choose = randint(0, 1); // 0��ȯ, 1�̱�ȯ
	int nnx, nny;

	do {
		nnx = randint(1, N_ROW - 2);
		nny = randint(1, N_COL - 2);
	} while (!placable(nnx, nny));

	if (p == 0) {
		gotoxy(N_ROW, 0);
		printf("                                                    ");
		gotoxy(N_ROW, 0);
		printf("�������� ��ȯ�Ͻðڽ��ϱ�? y/n");
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

				//dialog("0�� ��ü ���, ������ ���� ��ġ");
				gotoxy(N_ROW, 0);
				printf("                                                    ");
				gotoxy(N_ROW, 0);
				printf("0�� ��ü ���, ������ ���� ��ġ");
				break;
			}
			if (key == K_NO) {
				back_buf[itmx[itmnum]][itmy[itmnum]] = ' ';

				itmx[itmnum] = nnx;
				itmy[itmnum] = nny;
				back_buf[nnx][nny] = 'I';

				gotoxy(N_ROW, 0);
				printf("                                                    ");
				gotoxy(N_ROW, 0);
				printf("0�� ��ü �ź�, ������ ���� ��ġ");

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

			//char msgp[30];
			//sprintf_s(msgp, sizeof(msgp), "player %d ������ ��ü", p);
			gotoxy(N_ROW, 0);
			printf("                                                    ");
			gotoxy(N_ROW, 0);
			printf("player %d ������ ��ü", p);
		}
		else {
			back_buf[itmx[itmnum]][itmy[itmnum]] = ' ';

			itmx[itmnum] = nnx;
			itmy[itmnum] = nny;
			back_buf[nnx][nny] = 'I';

			gotoxy(N_ROW, 0);
			printf("                                                    ");
			gotoxy(N_ROW, 0);
			printf("%d�� ��ü �ź�, ������ ���� ��ġ", p);

		}
	}
}

bool ck_near_itm(int p, int* itm_or_player_num) {
	int len = INT_MAX;
	bool itmT_or_playerF = true;
	int short_index = 0;

	for (int i = 0; i < PLAYER_MAX; i++) {
		if (itmx[i] != -1 && itmy[i] != -1) {
			// �����۰� �÷��̾� ��ǥ�� ������� �� Ŭ�� �𸣱� ������ abs() ���� ���
			int lena = abs(px[p] - itmx[i]) + abs(py[p] - itmy[i]);

			// ���̰� ª���� lena�� ��ü, �׸��� itm ��ȣ�� ����
			if (lena < len) { len = lena; short_index = i; }
		}
	}
	//if (len == INT_MAX) { return 0; }// ������ ���� �� ������ ������ �ϴ� �ӽ� ���� �ڵ�
	
	//�������� �� �ȸ����� �� ������ ���� �����ϵ��� ����
	//if (p == 0 || len == INT_MAX) {
		for (int i = 0; i < n_player; i++) {
			if (player[i].hasitem && i != p && player[i].is_alive == true) {
				// �� �÷��̾� ��ǥ�� ������� �� Ŭ�� �𸣱� ������ abs() ���� ���
				int lena = abs(px[p] - px[i]) + abs(py[p] - py[i]);

				// ���̰� ª���� lena�� ��ü, �׸��� player ��ȣ�� ����
				if (lena < len) { len = lena; short_index = i; itmT_or_playerF = false; }
			}
		}
	//}

	*itm_or_player_num = short_index;

	if (itmT_or_playerF == true) { return true; }
	else if (itmT_or_playerF == false) { return false; }
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
			ngmv_random(0);
			move_manual(key);
		}

		//0�� ������ �÷��̾� ������(Ȥ�� ������ ���� �÷��̾�)�� �̵��ϴ� �ڵ�
		for (int i = 1; i < n_player; i++) {
			if (player[i].is_alive && tick % period[i] == 0) {
				ngmv_random(i);
			}
		}

		// ��и��� ���׹̳� �߰������ �ұ�? �𸣰ڴ�...

		display();
		Sleep(10);
		tick += 10;
	}
}