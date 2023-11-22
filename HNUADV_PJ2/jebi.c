#define _CRT_SECURE_NO_WARNINGS //sprintf() ����ϱ� ���� ������ ������ �����������..
#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void jebi_init(void);
void fix_zero_cursor();
void j_move_manual(key_t key);
bool jebi_placable(int, int);
void move_jebi(int, int, int);
void print_rp(int);
void ck_kill(int*, bool);
void jebi(void);

// ��÷ ���� ���� �뵵 0��: �÷��̾� ����, 1�� �������, 2�� ���� �̱�
int px[PLAYER_MAX], py[PLAYER_MAX];
int count_r; //���� ī����
char msg[50] = { "player", };

void jebi_init(void) {
	map_init(9, 24);

	//���� �÷��̾����ŭ ����
	for (int i = 0; i < n_alive; i++) {
		back_buf[4][i * 2 + 2] = '?';
	}
	count_r = 0;
	fix_zero_cursor();
}

void fix_zero_cursor() {
	px[0] = 4; py[0] = 2;
	back_buf[px[0]][py[0]] = '@';
}

void j_move_manual(key_t key) {
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
		return;
	}

	move_jebi(0, nx, ny);
}

// ����ִ� �÷��̾� ������ �� ������ �ʵ��� count_p�� ����
bool jebi_placable(int row, int col) {
	// ?�� �ƴ� ��� �� ������
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

void print_rp(int player) {
	gotoxy(9, 0);
	printf("round %d, turn: player %d", count_r + 1, player);
}

void ck_kill(int* sel_kill, bool space) {
	//0���� ����ִ� ��� �״��� Ȯ��
	/*if (space && *sel_kill == 0) {
		n_alive--;
		player[0].is_alive = false;
		dialog("player 0 fail!");

		back_buf[px[0]][py[0]] = '?';
		back_buf[4][n_alive * 2 + 2] = ' ';
		count_r++;
		
		*sel_kill = randint(0, n_alive - 1);
	}*/
	int count = 0;
	for (int i = 0; i < PLAYER_MAX; i++) {
		if (player[i].is_alive == true && i != 0) {
			print_rp(i);
			Sleep(1000);
		}
		if (player[i].is_alive == true && count == *sel_kill) {
			n_alive--;
			player[i].is_alive = false;
			sprintf(msg, "%s %d %s ", msg, i, "dead!");
			dialog(msg);
			msg[6] = '\0';//player �� �ʱ�ȭ
	
			//if (i == 0) {
				//back_buf[px[0]][py[0]] = '?';
			//}
			//fix_zero_cursor();
			//back_buf[4][(n_alive - count) * 2 + 2] = ' ';

			count_r++;
			count++;
			*sel_kill = randint(0, n_alive - 1);
			break;
		}
		else if(player[i].is_alive == true){
			back_buf[px[0]][py[0]] = '?';
			fix_zero_cursor();
			back_buf[4][(n_alive - count - 1) * 2 + 2] = ' ';

			sprintf(msg, "%s %d %s ", msg, i, "pass!");
			dialog(msg);
			msg[6] = '\0';//player �� �ʱ�ȭ
			count++;
		}
	}
	for (int i = 0; i < PLAYER_MAX; i++) {
		back_buf[4][i * 2 + 2] = ' ';
	}
	for (int i = 0; i < n_alive; i++) {
		back_buf[4][i * 2 + 2] = '?';
	}
	fix_zero_cursor();
}

void jebi(void) {
	jebi_init();
	system("cls");
	display();
	//dialog("\"����̱�\"");
	int sel_kill = randint(0, n_alive - 1);
	

	while (1) {

		if (n_alive == 1) {
			break;
		}

		//0���� ��������� �۵�
		if (player[0].is_alive == true) {
			int i = 0;
			print_rp(i);
			key_t key = get_key();
			if (key == K_QUIT) {
				break;
			}
			else if (key == K_SPACE) {
				ck_kill(&sel_kill, true);
			}
			else if (key != K_UNDEFINED) {
				j_move_manual(key);
			}
		}
		else if (player[0].is_alive == false){
			ck_kill(&sel_kill, false);
		}

		display();
		Sleep(10);
		tick += 10;
	}
}