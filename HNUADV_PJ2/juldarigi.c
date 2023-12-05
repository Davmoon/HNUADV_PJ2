#define _CRT_SECURE_NO_WARNINGS

#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>
#include<string.h>
#include <stdbool.h>
#include <Windows.h>
#include <conio.h> 
#include <time.h>


#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

#define	DATA_FILE	"jjuggumi.dat"


void team_mate(); //������
void player_list(); //�÷��̾� ����Ʈ 
void gamemaster(); //���� ����
void game(); //���� �÷���
void power(); //�� ���
void map(); //�� ����
void player_spawn(); //�迭�ȿ� �÷��̾� ����
void player_change();
void print_list(); //����Ʈ ���
void end(); //���� ������ + ������ ����


int px[PLAYER_MAX], py[PLAYER_MAX], jy[3];

int a[2][PLAYER_MAX / 2]; //�÷��̾� ����Ʈ ���������� ����
char maps[3][29];
float strs; // ��
PLAYER dead[6]; //���� ����� ��ȣ�� ���� �迭
int dead_count = 0; //���� ����� ������� ī��Ʈ �ϴ� �迭
int dead_player = PLAYER_MAX + 1;
float m_pw = 0;
bool b_LLiedown = false;
bool b_RLiedown = false;
bool b_masteradd = false;
char msg1[50];
bool b_IsEnd = false;

void map() { //�� ����
	int row = 3;
	int col = 29;
	if (col % 2 != 1) {
		col++;
	}
	juldarigi_map_init(row, col);

	player_spawn();
}

void player_spawn() { //����
	int x = 0, y = 0, z = (29 / 2 - (n_player / 2 + 1)); //x�� 1�����迭, y�� 2���� �迭�� ������, z�� ���� ����ֱ����� ī��Ʈ
	for (int i = 0; i < n_player; i++) {
		if (y >= n_player / 2) {
			y = 0;
			x++;
			for (int j = 0; j < 3; j++) {
				jy[j] = i + z + j;
				back_buf[1][jy[j]] = '-';
			}
			z += 3;
			if (x == 1 && y == 0) {
				px[i] = 1;
				py[a[x][y]] = i + z;
				back_buf[px[i]][py[a[x][y]]] = a[x][y] + '0';
				y++;
			}
		}
		else {
			px[i] = 1;
			py[a[x][y]] = i + z;
			back_buf[px[i]][py[a[x][y]]] = a[x][y] + '0';
			y++;
		}
	}
}

void player_change() {
	if (b_RLiedown || b_LLiedown)
	{
		for (int count = 0; count < 2; count++) {
			if (strs < 0) { // ���� �����϶�
				if (back_buf[1][(29 / 2 + 29 % 2)] == ' ') {
					b_IsEnd = true;
					break;
				}
				else {
					if (back_buf[1][(29 / 2 + 29 % 2)] != '-')
					{

						dead[dead_count] = player[(back_buf[1][(29 / 2 + 29 % 2)] - '0')];
						dead_count++;
						dead_player = back_buf[1][(29 / 2 + 29 % 2)] - '0';

						for (int i = 0; i < n_player; i++) {
							if (i % 2 != 0) {
								back_buf[px[i]][py[i]] = ' ';
								if (back_buf[px[i]][py[i] - 1] != '-' && placable(px[i], py[i] - 1)) {
									py[i] = py[i] - 1;
									back_buf[px[i]][py[i]] = i + '0';
								}
							}
						}
					}
					else {
						for (int i = 0; i < n_player; i++) {
							if (back_buf[px[i]][py[i]] == i + '0')
							{
								back_buf[px[i]][py[i]] = ' ';
							}
							py[i] = py[i] - 1;
							back_buf[px[i]][py[i]] = i + '0';
						}
						for (int j = 0; j < 3; j++) {
							if (back_buf[1][jy[j]] == '-')
							{
								back_buf[1][jy[j]] = ' ';
							}
							jy[j] = jy[j] - 1;
							back_buf[1][jy[j]] = '-';
						}
					}
				}
			}
			else if (strs > 0) { //���� ����϶�
				if (back_buf[1][(29 / 2 - 29 % 2)] == ' ') {
					b_IsEnd = true;
					break;
				}
				else {
					if (back_buf[1][(29 / 2 - 29 % 2)] != '-')
					{

						dead[dead_count] = player[(back_buf[1][(29 / 2 - 29 % 2)] - '0')];
						dead_count++;
						dead_player = back_buf[1][(29 / 2 - 29 % 2)] - '0';

						for (int i = 0; i < n_player; i++) {
							if (i % 2 == 0) {
								back_buf[px[i]][py[i]] = ' ';
								if (back_buf[px[i]][py[i] + 1] != '-' && placable(px[i], py[i] + 1)) {
									py[i] = py[i] + 1;
									back_buf[px[i]][py[i]] = i + '0';
								}
							}
						}
					}
					else {
						for (int i = 0; i < n_player; i++) {
							if (back_buf[px[i]][py[i]] == i + '0')
							{
								back_buf[px[i]][py[i]] = ' ';
							}
							py[i] = py[i] + 1;
							back_buf[px[i]][py[i]] = i + '0';
						}
						for (int j = 2; j >= 0; j--) {
							if (back_buf[1][jy[j]] == '-')
							{
								back_buf[1][jy[j]] = ' ';
							}
							jy[j] = jy[j] + 1;
							back_buf[1][jy[j]] = '-';
						}
					}
				}
			}
			else { //���� 0�϶�

			}
		}
	}
	else
	{
		if (strs < 0) { // ���� �����϶�
			if (back_buf[1][(29 / 2 + 29 % 2)] == ' ') {
				b_IsEnd = true;
			}
			else {
				if (back_buf[1][(29 / 2 + 29 % 2)] != '-')
				{

					dead[dead_count] = player[(back_buf[1][(29 / 2 + 29 % 2)] - '0')];
					dead_count++;
					dead_player = back_buf[1][(29 / 2 + 29 % 2)] - '0';

					for (int i = 0; i < n_player; i++) {
						if (i % 2 != 0) {
							back_buf[px[i]][py[i]] = ' ';
							if (back_buf[px[i]][py[i] - 1] != '-' && placable(px[i], py[i] - 1)) {
								py[i] = py[i] - 1;
								back_buf[px[i]][py[i]] = i + '0';
							}
						}
					}
				}
				else {
					for (int i = 0; i < n_player; i++) {
						if (back_buf[px[i]][py[i]] == i + '0')
						{
							back_buf[px[i]][py[i]] = ' ';
						}
						py[i] = py[i] - 1;
						back_buf[px[i]][py[i]] = i + '0';
					}
					for (int j = 0; j < 3; j++) {
						if (back_buf[1][jy[j]] == '-')
						{
							back_buf[1][jy[j]] = ' ';
						}
						jy[j] = jy[j] - 1;
						back_buf[1][jy[j]] = '-';
					}
				}
			}
		}
		else if (strs > 0) { //���� ����϶�
			if (back_buf[1][(29 / 2 - 29 % 2)] == ' ') {
				b_IsEnd = true;
			}
			else {
				if (back_buf[1][(29 / 2 - 29 % 2)] != '-')
				{

					dead[dead_count] = player[(back_buf[1][(29 / 2 - 29 % 2)] - '0')];
					dead_count++;
					dead_player = back_buf[1][(29 / 2 - 29 % 2)] - '0';

					for (int i = 0; i < n_player; i++) {
						if (i % 2 == 0) {
							back_buf[px[i]][py[i]] = ' ';
							if (back_buf[px[i]][py[i] + 1] != '-' && placable(px[i], py[i] + 1)) {
								py[i] = py[i] + 1;
								back_buf[px[i]][py[i]] = i + '0';
							}
						}
					}
				}
				else {
					for (int i = 0; i < n_player; i++) {
						if (back_buf[px[i]][py[i]] == i + '0')
						{
							back_buf[px[i]][py[i]] = ' ';
						}
						py[i] = py[i] + 1;
						back_buf[px[i]][py[i]] = i + '0';
					}
					for (int j = 2; j >= 0; j--) {
						if (back_buf[1][jy[j]] == '-')
						{
							back_buf[1][jy[j]] = ' ';
						}
						jy[j] = jy[j] + 1;
						back_buf[1][jy[j]] = '-';
					}
				}
			}
		}
		else { //���� 0�϶�

		}
	}
}




void gamemaster() {
	char ch;


	if (_kbhit()) {  // Ű �Է��� �ִ��� Ȯ��
		ch = _getch();  // Ű �Է��� ����ϰ� �Էµ� Ű�� ������

		if (ch == 'z' || ch == 'Z') {
			b_masteradd = true;
			m_pw--;
		}
		else if (ch == '/') {
			b_masteradd = true;
			m_pw++;
		}
		else if (ch == 'q' || ch == 'Q') {
			juldarigi_dialog(" \"���Ӹ����Ͱ� �ٴٸ��⸦ �����Ͽ����ϴ�.\"");
			b_IsEnd = true;
		}
		else if (ch == 'x' || ch == 'X') {
			juldarigi_dialog(" \"�������� ���⸦ ����Ͽ����ϴ�.\"");
			b_LLiedown = true;
			for (int i = 0; i < n_player / 2; i++) {
				if (player[a[0][i]].stamina > 0) {
					player[a[0][i]].stamina = (int)player[a[0][i]].stamina * 0.7;
				}
			}
		}
		else if (ch == '.') {
			juldarigi_dialog(" \"���������� ���⸦ ����Ͽ����ϴ�.\"");
			b_RLiedown = true;
			for (int i = 0; i < n_player / 2; i++) {
				if (player[a[1][i]].stamina > 0) {
					player[a[1][i]].stamina = (int)player[a[1][i]].stamina * 0.7;
				}
			}
		}
	}


}

void end() {
	for (int i = 0; i < n_player; i++) {
		if (player[i].status == 2 || player[i].status == 4) {
			player[i].is_alive = true; //Ż������ ���� ����� ��Ƴ��ų� Ż���ߴ� ����� ��Ƴ����� 
			//alive ���°� true
		}
		else if (player[i].status == 1) {
			//Ż������ ���� ����� ������ ������ ���, ����, ���� ���� �϶�
			player[i].hasitem = false; //�������� ���� ����ؾ� �⺻���� �϶���
			player[i].intel = player[i].intel / 2;
			player[i].str = player[i].str / 2;
		}
		else if (player[i].status == 3) {
			player[i].is_alive = false; //Ż���� ����� �̹����ӿ��� Ż���߱� ������ ��¥ ���ó��
		}
	}
}


void game() { // ���� ����, �÷���
	char key;
	clock_t start_time, current_time;
	double elapsed_time;

	// ���� �ð� ���
	start_time = clock();
	while (1) {
		// ���� �ð� ���
		current_time = clock();
		char ch;
		// ��� �ð� ��� (����: ��)
		elapsed_time = (double)(current_time - start_time) / CLOCKS_PER_SEC;
		float key1 = 0, key2 = 0;
		// 1�� ��� �ÿ� ���ϴ� ���� ����
		if (elapsed_time >= 1.0) {

			player_list(); //����Ʈ ���� 

			if (b_IsEnd) {
				break;
			}

			juldarigi_display();

			power();

			player_change();

			b_LLiedown = false;
			b_RLiedown = false;

			if (dead_player != PLAYER_MAX + 1) {
				sprintf(msg1, "%d %s", dead_player, "�� �÷��̾� Ż��!");
				juldarigi_dialog(msg1);
				dead_player = PLAYER_MAX + 1;
			}

			// �ٽ� ���� �ð��� ���� �ð����� ������Ʈ
			start_time = clock();
		}
		gamemaster();

	}
}

void power() { // �� ���
	float powers[2] = { 0.0, 0.0 };

	for (int i = 0; i < n_player / 2; i++) {
		if (!(player[a[0][i]].status == 1 || player[a[0][i]].status == 3))
		{
			if (b_LLiedown) {
				if (player[a[0][i]].hasitem) {
					powers[0] += (player[a[0][i]].str + player[a[0][i]].item.str_buf) * 2.0;
				}
				else
				{
					powers[0] += player[a[0][i]].str * 2.0;
				}
			}
			else {
				if (player[a[0][i]].hasitem) {
					powers[0] += player[a[0][i]].str + player[a[0][i]].item.str_buf;
				}
				else
				{
					powers[0] += player[a[0][i]].str;
				}
			}
		}
	}

	for (int i = 0; i < n_player / 2; i++) {
		if (!(player[a[1][i]].status == 1 || player[a[1][i]].status == 3))
		{
			if (b_RLiedown) {
				if (player[a[1][i]].hasitem) {
					powers[1] += (player[a[1][i]].str + player[a[1][i]].item.str_buf) * 2.0;
				}
				else
				{
					powers[1] += player[a[1][i]].str * 2.0;
				}
			}
			else {
				if (player[a[1][i]].hasitem) {
					powers[1] += player[a[1][i]].str + player[a[1][i]].item.str_buf;
				}
				else
				{
					powers[1] += player[a[1][i]].str;
				}
			}
		}
	}

	if (b_masteradd) {
		strs = (powers[1] - powers[0]) + m_pw;
		m_pw = 0;
	}
	else {
		strs = powers[1] - powers[0];
	}
	printf("str: %.1f\n", strs);
}


//����Ʈ ����
void player_list() {
	for (int i = 0; i < n_player; i++) { //��� �÷��̾ ���� �׾����� ��Ҵ��� Ȯ���ϴ� �ݺ���
		if (player[i].is_alive == true) { //���� ��Ҵ� ��������
			player[i].status = 2;
			for (int j = 0; j < dead_count; j++) { //����Ʈ ���� �÷��̾�� ���� �÷��̾ ������ 
				//�÷��̾ ���� ���� ī��Ʈ�� �þ
				if (!strcmp(dead[j].name, player[i].name)) {
					//���� �ݺ��� �ȿ� �ִ� �÷��̾�� �ǽð����� ���ŵǴ� ���� �÷��̾�� ����
					//���� ���� �÷��̾�� ���� �÷��̾��� �̸��� ���ٸ� 
					//���ó���� �ؾ�������, ���� �÷��̾�� ���� ���ӿ��� �����߱� ������ 
					player[i].status = 1; //Ż������ ���� �÷��̾�� �̱⿡ **ǥ�� ��ũ
				}
			}
		}
		else { //�׾��� �������� Ȯ��
			player[i].status = 4;
			//�׾��� ���¿��� ���� �ݺ����� �ѹ��� üũ
			for (int j = 0; j < dead_count; j++) {
				if (!strcmp(dead[j].name, player[i].name)) { // ���� �̷��� �����鼭 Ż���ߴٸ�
					player[i].status = 3; //��¥ Ż��
				}
			}
		}
	}
}

//�� ����
void team_mate() {
	int temp;
	int x = 0, y = 0;

	for (int j = 0; j < n_player; j++) {
		if (j == 0 || j % 2 == 0) {
			a[0][0 + x] = j;
			x++;
		}
		else {
			a[1][0 + y] = j;
			y++;
		}
	} //���� ������ �ֱ�

	for (int i = 0; i < n_player / 2; i++) { //���� �� ����
		for (int j = 0; j < ((n_player / 2) - 1) - i; j++) {
			if (a[0][j] < a[0][j + 1]) {	// ���� ���� ���
				temp = a[0][j];
				a[0][j] = a[0][j + 1];
				a[0][j + 1] = temp;
			}
		}
	}
}

int juldarigi_init(void) {

	FILE* fp;
	fopen_s(&fp, DATA_FILE, "r");
	if (fp == NULL) {
		return -1;
	}

	fscanf_s(fp, "%d", &n_player);

	n_alive = n_player; //����ִ� �÷��̾� = ó�� �÷��̾� ��

	for (int i = 0; i < n_player; i++) {
		// ���� �� ��� ����(����ü ������, ��������������)
		PLAYER* p = &player[i];

		fscanf_s(fp, "%s%d%d", p->name, (unsigned int)sizeof(p->name), &(p->intel), &(p->str));
	}

	fscanf_s(fp, "%d", &n_item);
	for (int i = 0; i < n_item; i++) {
		fscanf_s(fp, "%s%d%d%d", item[i].name, (unsigned int)sizeof(item[i].name), &(item[i].intel_buf), &(item[i].str_buf), &(item[i].stamina_buf));
	}

	fclose(fp);
	return 0;
}

void juldarigi() {
	juldarigi_init();

	team_mate();

	map();


	game();
	end();
}