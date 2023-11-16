#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void jebi_init(void);
void jebi(void);
void j_move_manual(key_t key);
void print_jebi(void);
void move_jebi(int, int, int);

// ��÷ ���� ���� �뵵 0��: �÷��̾� ����, 1�� �������, 2�� ���� �̱�
int px[PLAYER_MAX], sel_jebi[PLAYER_MAX]; 

void jebi_init(void) {
	map_init(9, 24);
	
	//����ִ� �÷��̾� sel_jebi ��� �Ҵ�
	int len = 0;
	for (int i = 0; i < PLAYER_MAX; i++) {
		if (player[i].is_alive) {
			sel_jebi[len] = 1;
			len++;
		}
		else {
			sel_jebi[len] = 0;
		}
	}

	print_jebi();
}

void print_jebi(void) {
	for (int i = 0; i < PLAYER_MAX; i++) {
		if (sel_jebi[i] == (1 || 2)) {
			back_buf[4][i * 2 + 2] = '?';
		}
		px[i] = 0; //ó�� sel ��ǥ 0���� �ʱ�ȭ
	}
}

void j_move_manual(key_t key) {
	// �� �������� ������ �� x, y�� delta
	static int dx[2] = { -1, 1};

	int dir;  // ������ ����(0~3)
	switch (key) {
	case K_LEFT: dir = DIR_LEFT;
		break;
	case K_RIGHT: dir = DIR_RIGHT;
		break;
	default: return;
	}

	// �������� ���� �ڸ�
	int nx;
	nx = px[0] + dx[dir];

	move_jebi(0, nx, 4);
}

void move_jebi(int player, int nx, int ny) {
	int p = player;  // �̸��� ��...
	back_buf[nx][ny] = back_buf[px[p]][4];
	back_buf[px[p]][4] = '@';
	px[p] = nx;
}

void jebi(void) {
	jebi_init();
	system("cls");
	display();

	//dialog("\"����̱�\"");
	int round = 1;
	
	while (1) {

		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			j_move_manual(key);
		}

	}
}