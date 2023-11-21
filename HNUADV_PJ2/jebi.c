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
bool jebi_placable(int, int);
void print_jebi(void);
void move_jebi(int, int, int);

// ��÷ ���� ���� �뵵 0��: �÷��̾� ����, 1�� �������, 2�� ���� �̱�
int px[PLAYER_MAX], py[PLAYER_MAX];
int count_r; //�÷��̾� üũ, ���� üũ

void jebi_init(void) {
	map_init(9, 24);
	
	count_r = 0;
	print_jebi();

	px[0] = 4; py[0] = 2;
	back_buf[px[0]][py[0]] = '@';
}

void print_jebi(void) {
	for (int i = 0; i < n_alive; i++) {
		back_buf[4][i * 2 + 2] = '?';
	}
}

void j_move_manual(key_t key) {
	// �� �������� ������ �� x, y�� delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // ���� Ű �Է� ����
	switch (key) {
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}

	// 2ĭ�� ������ ����
	int nx, ny;
	nx = px[0];
	ny = py[0] + dy[dir] * 2;
	if (!jebi_placable(nx, ny)) {
		return false;
	}

	move_jebi(0, nx, ny);
	return true;
}

// ����ִ� �÷��̾� ������ �� ������ �ʵ��� count_p�� ����
bool jebi_placable(int row, int col) {
	if (row < 0 || row >= N_ROW ||
		col < 0 || col >= N_COL ||
		back_buf[row][col] != '?') {
		return false;
	}
	return true;
}

void move_jebi(int player, int nx, int ny) {
	int p = player;  // �̸��� ��...
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = '?';
	px[p] = nx;
	py[p] = ny;
}

void jebi(void) {
	jebi_init();
	system("cls");
	display();
	//dialog("\"����̱�\"");
	
	while (1) {

		if (n_alive == 1) {
			break;
		}

		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			j_move_manual(key);
		}

		if (player[0].is_alive == false) {

		}
		else if (player[0].is_alive == true) {

		}


		display();
	}
}