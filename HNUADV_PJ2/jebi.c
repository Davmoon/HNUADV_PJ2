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

int sel_jebi[PLAYER_MAX]; // 당첨 제비 구분 용도 0번: 플레이어 제외, 1번 살아있음, 2번 죽음 뽑기
int round; //라운드 저장

void jebi_init(void) {
	map_init(5, 24);
	
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

	//print_jebi();
}

void print_jebi(void) {
	for (int i = 0; i < PLAYER_MAX; i++) {
		//move_tail()
	}
}

void j_move_manual(key_t key) {
	// 각 방향으로 움직일 때 x, y값 delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // 움직일 방향(0~3)
	switch (key) {
	case K_LEFT: dir = DIR_LEFT;
		break;
	case K_RIGHT: dir = DIR_RIGHT;
		break;
	default: return;
	}

	// 움직여서 놓일 자리
	int nx, ny, select;
	nx = px[0] + dx[dir];
	ny = py[0] + dy[dir];
	//if (select == (0 || 4)) {
		return;
	//}

	//move_tail(0, nx, ny);
}

void draw_jebi(void) {
	for (int i = 0; i < 4; i++) {

	}
}

void jebi(void) {
	jebi_init();
	system("cls");
	display();
	
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