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

// 당첨 제비 구분 용도 0번: 플레이어 제외, 1번 살아있음, 2번 죽음 뽑기
int px[PLAYER_MAX], py[PLAYER_MAX];
int count_r; //플레이어 체크, 라운드 체크

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
	// 각 방향으로 움직일 때 x, y값 delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // 상하 키 입력 제거
	switch (key) {
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}

	// 2칸씩 움직여 제거
	int nx, ny;
	nx = px[0];
	ny = py[0] + dy[dir] * 2;
	if (!jebi_placable(nx, ny)) {
		return false;
	}

	move_jebi(0, nx, ny);
	return true;
}

// 살아있는 플레이어 수보다 더 나가지 않도록 count_p와 검증
bool jebi_placable(int row, int col) {
	if (row < 0 || row >= N_ROW ||
		col < 0 || col >= N_COL ||
		back_buf[row][col] != '?') {
		return false;
	}
	return true;
}

void move_jebi(int player, int nx, int ny) {
	int p = player;  // 이름이 길어서...
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = '?';
	px[p] = nx;
	py[p] = ny;
}

void jebi(void) {
	jebi_init();
	system("cls");
	display();
	//dialog("\"제비뽑기\"");
	
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