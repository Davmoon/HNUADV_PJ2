
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
	for (int i = 0; i < n_player; i++) {
		do {
			x = randint(1, N_ROW - 2);
			y = randint(1, N_COL - 2);
		} while (!placable(x, y));
		itmx[i] = x;
		itmy[i] = y;

		back_buf[itmx[i]][itmy[i]] = 'I';  // �������� �׻� i�� ǥ��
	}
	for (int i = 0; i < n_player; i++) {
		curr_itmx[i] = -1;
		curr_itmy[i] = -1;
	}

	tick = 0;
}


bool ngmv_manual(key_t key) {
	// �� �������� ������ �� x, y�� delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // ������ ����(0~3)
	switch (key) {
	case K_UP: dir = DIR_UP; break;
	case K_DOWN: dir = DIR_DOWN; break;
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}

	// �������� ���� �ڸ�
	int nx, ny;
	nx = px[0] + dx[dir];
	ny = py[0] + dy[dir];
	if (!placable(nx, ny)) {
		return false;
	}

	move_tail(0, nx, ny);
	return true;
}

// ���� ����� ��ǥ ã��
void find_nearest_target(int p, int* target_x, int* target_y) {
	int min_dist = INT_MAX;
	int dist, tx, ty;

	// ������ ��ġ Ȯ��
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

	// �ٸ� �÷��̾ ���� ������ ��ġ Ȯ��
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


//�̵� �ֱ� �� ���� ������Ʈ
void ngmv_random(int p) {
	int target_x, target_y;
	find_nearest_target(p, &target_x, &target_y);

	// ���� ����� ��ǥ�� ���� �̵�
	int nx = px[p], ny = py[p];
	if (px[p] < target_x) nx++;
	else if (px[p] > target_x) nx--;

	if (py[p] < target_y) ny++;
	else if (py[p] > target_y) ny--;

	// �̵� ���� ���� Ȯ��
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
		// player 0�� ������ ������(4����)
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {

			move_manual(key);

		}

		// player 1�� ������
		for (int i = 1; i < n_player; i++) {
			if (tick % period[i] == 0) {
				ngmv_random(i, -1);
			}
		}
		for (int i = 0; i < n_player; i++) {
			for (int j = 0; j < n_item; j++) {
				// �÷��̾ �����۰� �������� Ȯ��
				if (abs(px[i] - itmx[j]) + abs(py[i] - itmy[j]) == 1 && itmx[j] != -1) {
					if (!player[i].hasitem) {
						player[i].hasitem = true;
						curr_itmx[i] = itmx[j];
						curr_itmy[i] = itmy[j];
						// ������ ȹ��
						printf("Player %d acquired item at (%d, %d)\n", i, itmx[j], itmy[j]);
						// �������� �ʿ��� ����
						back_buf[itmx[j]][itmy[j]] = ' '; // ������ ��ġ�� �������� ����
						itmx[j] = -1; // ������ ��ġ�� ��ȿ���� ���� ������ ����
						itmy[j] = -1;
					}else {
						// ��ȯ ���� ����
						bool exchange = false;
						if (i == 0) { // �÷��̾� 0
							printf("��ȯ�Ϸ��� Y��, �׷��� ������ N�� ��������: ");



							//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
							//!!!!!!!!!!! ���⸦ ������ sample �ڵ�ó�� get key�� �ٲټ���.
							char decision = getchar();
							if (decision == 'Y' || decision == 'y') {
								exchange = true;
							}
							else if (decision == 'N' || decision == 'n') {
								//�����ϴ� �ڵ� �߰�
								printf("Player %d ignored item at (%d, %d)\n", i, itmx[j], itmy[j]);
								continue;
							}
						} else {
							if (randint(0, 1)) {
								exchange = true;
							}

						}
						if (exchange) {
							back_buf[curr_itmx[i]][curr_itmy[i]] = 'I'; // ���� ������ ��ġ�� ������ ǥ��
							// �� �������� ��ġ�� ���� ������ ��ġ�� ������Ʈ
							curr_itmx[i] = itmx[j];
							curr_itmy[i] = itmy[j];
							printf("Player %d exchanged item at (%d, %d)\n", i, itmx[j], itmy[j]);
							back_buf[itmx[j]][itmy[j]] = ' '; // ������ ��ġ�� �������� ����
							itmx[j] = -1; // ������ ��ġ�� ��ȿ���� ���� ������ ����
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