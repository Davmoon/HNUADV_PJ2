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


#define PLAYER_MAX		6
#define ITEM_MAX	10

void team_mate(); //������
void player_list(); //�÷��̾� ����Ʈ 
void gamemaster(); //���� ����
void game(); //���� �÷���
void power(); //�� ���
void map(); //�� ����
void player_spawn(); //�迭�ȿ� �÷��̾� ����
void player_change();
void print_player_list();




int a[2][PLAYER_MAX / 2]; //�÷��̾� ����Ʈ ���������� ����
char maps[3][29];
float strs; // ��
int z = (29 / 2) - ((PLAYER_MAX / 2) + 1); // �������� ��ƴ�涧�� ������
int z2 = (29 / 2) + ((PLAYER_MAX / 2) + 3); //���������� ��ƴ�涧�� ������
PLAYER dead[PLAYER_MAX]; //���� ����� ��ȣ�� ���� �迭
int dead_count = 0; //���� ����� ������� ī��Ʈ �ϴ� �迭
float m_pw = 0;


void map() { //�� ����


	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 29; j++) {
			if (i == 0 || i == 2) {
				if (j == 29 / 2)
					maps[i][j] = ' ';
				else
				{
					maps[i][j] = '#';
				}
			}
			else if ((i == 1 && j == 0) || (i == 1 && j == 28)) {
				maps[i][j] = '#';
			}
			else {
				maps[i][j] = ' ';
			}
		}// �� ����
	}

	player_spawn();



}

void player_spawn() { //����
	int x = 0, y = 0, z = (29 / 2 - (PLAYER_MAX - 2)); //x�� 1�����迭, y�� 2���� �迭�� ������, z�� ���� ����ֱ����� ī��Ʈ
	for (int i = 0; i < PLAYER_MAX; i++) {
		if (y >= PLAYER_MAX / 2) {
			y = 0;
			x++;
			for (int j = 0; j < 3; j++) {
				maps[1][i + z + j] = '-';
			}
			z += 3;
			if (x == 1 && y == 0) {
				maps[1][i + z] = a[x][y] + '0';
				y++;
			}
		}
		else {
			maps[1][i + z] = a[x][y] + '0';
			y++;
		}
	}
}

void player_change() {
	if (strs < 0) { // ���� �����϶�

		if (maps[1][15] != '-')
		{
			dead[dead_count] = player[atoi(&maps[1][15])];
			dead_count++;
			for (int i = 14; i < 14 + ((PLAYER_MAX / 2) + 1); i++) {

				maps[1][i + 1] = maps[1][i + 2];

			}

		}
		else {
			for (int i = z; i < z + (PLAYER_MAX + 4); i++) {
				maps[1][i - 1] = maps[1][i];

			}
			z--; //������ ����
		}
	}
	else if (strs > 0) { //���� ����϶�
		if (maps[1][13] != '-')
		{
			dead[dead_count] = player[atoi(&maps[1][13])];
			dead_count++;
			for (int i = 14; i > 14 - ((PLAYER_MAX / 2) + 1); i--) {
				maps[1][i - 1] = maps[1][i - 2];
			}
		}
		else {
			for (int i = z2; i > z2 - (PLAYER_MAX + 4); i--) {
				maps[1][i - 1] = maps[1][i - 2];
			}
			z2++;//������ ����
		}
	}
	else { //���� 0�϶�
		maps[3][29];
	}
}




void gamemaster() {
	char ch;


	if (_kbhit()) {  // Ű �Է��� �ִ��� Ȯ��
		ch = _getch();  // Ű �Է��� ����ϰ� �Էµ� Ű�� ������

		if (ch == 'z' || ch == 'Z') {
			m_pw--;
		}
		else if (ch == '/') {
			m_pw++;
		}
		else if (ch == 'q' || ch == 'Q') {
			printf("\nQuitting the program.\n");

		}
	}


}

//void print_player_list() {
//	int x = 0;
//	printf("no. of player left : %d\n", sizeof(player) / sizeof(PLAYER));
//	for (int i = 0; i < PLAYER_MAX/2; i++) {
//		
//		for (int j = 0; j < 2; j++) {
//			printf("player %d: ",x);
//			x++;
//
//			
//		}
//	}
//}


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


			system("cls");

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 29; j++) {
					printf("%c", maps[i][j]);

				}
				printf("\n");
			}



			power();
			player_list();
			player_change();

			// �ٽ� ���� �ð��� ���� �ð����� ������Ʈ
			start_time = clock();
		}
		gamemaster();
	}

}

void power() { // �� ���
	int powers[2];

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < PLAYER_MAX / 2; j++) {
			if (player[a[i][j]].hasitem) {
				powers[i] += player[a[i][j]].str + player[a[i][j]].item.str_buf;
			}
			else
			{
				powers[i] += player[a[i][j]].str;
			}
		}
	}
	strs = (powers[0] - powers[1]) + m_pw;

	printf("str: %.1f\n", strs);
}


//����Ʈ ����
void player_list() {



	printf("no. of player left : %d\n", sizeof(player) / sizeof(PLAYER));
	for (int i = 0; i < PLAYER_MAX; i++) {
		printf("player %d: ", i);
		if (player[i].is_alive == true) {
			char a = dead[i].name;
			char b = player[i].name;
			if (strcmp(dead[i].name, player[i].name) == 0) {
				printf("alive\n"); //�̹� ��Ҵ� �÷��̾�� ���� ���� ��� **���� ǥ��
				player[i].hasitem = false; //�÷��̾��� ������ ����
				player[i].intel = player[i].intel / 2; // �÷��̾��� ���� ���丷
			}
			else {
				printf("alive\n");
			}

		}
		else {
			player[i].hasitem = false; //Ż���� �÷��̾��� ������ ����
			char a = dead[i].name;
			char b = player[i].name;
			if (strcmp(dead[i].name, player[i].name) == 0) {
				printf("DEAD\n");
			}
			else {
				printf("alive*\n"); //�׾��� �÷��̾�� *�� ǥ��
				player[i].is_alive == true; //�׾��� �÷��̾� ��Ȱ
			}


		}

	}
}

//�� ����
void team_mate() {
	int temp;
	int x = 0, y = 0;

	for (int j = 0; j < PLAYER_MAX; j++) {
		if (j == 0 || j % 2 == 0) {
			a[0][0 + x] = j;
			x++;
		}
		else {
			a[1][0 + y] = j;
			y++;
		}
	} //���� ������ �ֱ�

	for (int i = 0; i < PLAYER_MAX / 2; i++) { //���� �� ����
		for (int j = 0; j < ((PLAYER_MAX / 2) - 1) - i; j++) {
			if (a[0][j] < a[0][j + 1]) {	// ���� ���� ���
				temp = a[0][j];
				a[0][j] = a[0][j + 1];
				a[0][j + 1] = temp;
			}
		}
	}




}




void juldarigi() {
	player[0].hasitem = true;
	player[0].item.str_buf = 2;
	player_list();
	team_mate();

	map();


	game();

}