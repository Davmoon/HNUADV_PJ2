#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void ng_init();
void ngmv_towards_item(int, int);
bool ngmv_manual(key_t);
void nightgame();

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
	for (int i = 0; i < n_player; i++) {
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
int find_nearest_item(int player) {
	int min_distance = INT_MAX;
	int nearest_item = -1;

	for (int i = 0; i < n_item; i++) {
		int distance = abs(itmx[i] - px[player]) + abs(itmy[i] - py[player]);
		if (distance < min_distance) {
			min_distance = distance;
			nearest_item = i;
		}
	}

	return nearest_item;
}

// �÷��̾ ������ ĭ�� �������� ���� ��ȣ�ۿ�
void interaction_with_item(int player_index, int item_index) {
	if (player[player_index].hasitem == false) {
		// �������� ȹ���Ѵ�.
		player[player_index].item = item[item_index];
		player[player_index].hasitem = true;
		printf("%s �÷��̾ %s �������� ȹ���Ͽ����ϴ�.\n", player[player_index].name, item[item_index].name);
	}
	else {
		// �������� ��ȯ�Ѵ�.
		if (player_index == 0) {
			printf("\n��ȯ�Ͻðڽ��ϱ�? (y/n)\n");
			char key = get_key();
			if (key == 'y') {
				ITEM temp = player[player_index].item;
				player[player_index].item = item[item_index];
				item[item_index] = temp;
				printf("%s �÷��̾ �������� ��ȯ�Ͽ����ϴ�.\n", player[player_index].name);
			}
		}
		else {
			int rand = randint(0, 1);
			if (rand == 1) {
				ITEM temp = player[player_index].item;
				player[player_index].item = item[item_index];
				item[item_index] = temp;
				printf("%s �÷��̾ �������� ��ȯ�Ͽ����ϴ�.\n", player[player_index].name);
			}
		}
	}
}
// �÷��̾�� ��ȣ�ۿ�
void interaction_between_players(int player1_index, int player2_index) {
	printf("�÷��̾� %s�� %s�� �������ϴ�.\n", player[player1_index].name, player[player2_index].name);
	if (player1_index == 0 || (!player[player1_index].hasitem && player[player2_index].hasitem)) {
		printf("\n1) ��Ż�õ� 2) ȸ���õ� 3) ����\n");
		char key = get_key();
		if (key == '1') {
			// ��Ż�õ� ����
		}
		else if (key == '2') {
			// ȸ���õ� ����
		}
		else {
			// ���� ����
		}
	}
	else {
		// ������ �÷��̾�鿡 ���� ����
	}
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

	// �����۰��� ��ȣ�ۿ� Ȯ��
	for (int i = 0; i < n_item; i++) {
		if (nx == itmx[i] && ny == itmy[i]) {
			// player0�� ������ ���� ������ ���
			printf("Player0 is on the item!\n");  // ����� �޽��� �߰�
			interaction_with_item(0, i);
		}
	}
	return true;
}
bool is_player_on_item(int player) {
	for (int i = 0; i < n_item; i++) {
		if (px[player] == itmx[i] && py[player] == itmy[i]) {
			return true;
		}
	}
	return false;
}


//������ �������� �����̱�
void ngmv_towards_item(int player, int item) {
	int dx = itmx[item] - px[player];
	int dy = itmy[item] - py[player];

	if (abs(dx) > abs(dy)) {
		if (dx > 0) {
			move_tail(player, px[player] + 1, py[player]);
		}
		else {
			move_tail(player, px[player] - 1, py[player]);
		}
	}
	else {
		if (dy > 0) {
			move_tail(player, px[player], py[player] + 1);
		}
		else {
			move_tail(player, px[player], py[player] - 1);
		}
	}
}


void nightgame(void) {
	sample_init();
	system("cls");
	ng_init();
	display();

	printf("%s", item[0].name);
	printf("%s", item[1].name);
	printf("%s", item[2].name);
	printf("%s", item[3].name);
	printf("%s", item[4].name);
	while (1) {
		// player 0�� ������ ������(4����)
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {

			ngmv_manual(key);

		}

		// player 1 ���ʹ� �������� ���� ������
		for (int i = 1; i < n_player; i++) {
			if (tick % period[i] == 0) {
				int nearest_item = find_nearest_item(i);
				ngmv_towards_item(i, nearest_item);
			}
		}
		// �������̳� �÷��̾���� ��ȣ�ۿ��� Ȯ��
		for (int i = 0; i < n_player; i++) {
			for (int j = 0; j < n_item; j++) {
				if (px[i] == itmx[j] && py[i] == itmy[j]) {
					// �÷��̾ ������ ĭ�� ������ ���
					interaction_with_item(i, j);
				}
			}
			for (int j = i + 1; j < n_player; j++) {
				if (px[i] == px[j] && py[i] == py[j]) {
					// �� �÷��̾ ���� ĭ�� �ִ� ���
					interaction_between_players(i, j);
				}
			}
		}

		display();
		Sleep(10);
		tick += 10;

	}
}