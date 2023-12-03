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

int a[2][PLAYER_MAX / 2]; //�÷��̾� ����Ʈ ���������� ����
char maps[3][29];
float strs; // ��
int z; // �������� ��ƴ�涧�� ������
int z2; //���������� ��ƴ�涧�� ������
PLAYER dead[6]; //���� ����� ��ȣ�� ���� �迭
int dead_count = 0; //���� ����� ������� ī��Ʈ �ϴ� �迭
float m_pw = 0;
bool b_LLiedown = false;
bool b_RLiedown = false;
bool b_masteradd = false;


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
	int x = 0, y = 0, z = (29 / 2 - (n_player / 2 + 1)); //x�� 1�����迭, y�� 2���� �迭�� ������, z�� ���� ����ֱ����� ī��Ʈ
	for (int i = 0; i < n_player; i++) {
		if (y >= n_player / 2) {
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
	if (b_RLiedown || b_LLiedown)
	{
		for (int count = 0; count < 2; count++) {
			if (strs < 0) { // ���� �����϶�
				if (maps[1][15] != '-')
				{

					dead[dead_count] = player[(maps[1][15] - '0')];
					dead_count++;
					for (int i = 14; i < 14 + ((n_player / 2) + 1); i++) {

						maps[1][i + 1] = maps[1][i + 2];
					}

				}
				else {
					for (int i = z; i < z + (n_player + 5); i++) {
						maps[1][i - 1] = maps[1][i];
					}
					z--; //������ ����
				}
			}
			else if (strs > 0) { //���� ����϶�
				if (maps[1][13] != '-')
				{
					dead[dead_count] = player[(maps[1][13] - '0')];
					dead_count++;
					for (int i = 14; i > 14 - ((n_player / 2) + 1); i--) {
						maps[1][i - 1] = maps[1][i - 2];
					}
				}
				else {
					for (int i = z2; i > z2 - (n_player + 4); i--) {
						maps[1][i - 1] = maps[1][i - 2];
					}
					z2++;//������ ����
				}
			}
			else { //���� 0�϶�
				maps[3][29];
			}
		}
	}
	else
	{
		if (strs < 0) { // ���� �����϶�
			if (maps[1][15] != '-')
			{

				dead[dead_count] = player[(maps[1][15] - '0')];
				dead_count++;
				for (int i = 14; i < 14 + ((n_player / 2) + 2); i++) {

					maps[1][i + 1] = maps[1][i + 2];
				}

			}
			else {
				for (int i = z; i < z + (n_player + 6); i++) {
					maps[1][i - 1] = maps[1][i];
				}
				z--; //������ ����
			}
		}
		else if (strs > 0) { //���� ����϶�
			if (maps[1][13] != '-')
			{
				dead[dead_count] = player[(maps[1][13] - '0')];
				dead_count++;
				for (int i = 14; i > 14 - ((n_player / 2) + 2); i--) {
					maps[1][i - 1] = maps[1][i - 2];
				}
			}
			else {
				for (int i = z2; i > z2 - (n_player + 5); i--) {
					maps[1][i - 1] = maps[1][i - 2];
				}
				z2++;//������ ����
			}
		}
		else { //���� 0�϶�
			maps[3][29];
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
			printf("\nQuitting the program.\n");

		}
		else if (ch == 'x' || ch == 'X') {
			printf("\n���� �� ���� ���.\n");
			b_LLiedown = true;
			for (int i = 0; i < n_player / 2; i++) {
				if (player[a[0][i]].stamina > 0) {
					player[a[0][i]].stamina = (int)player[a[0][i]].stamina * 0.7;
				}
			}
		}
		else if (ch == '.') {
			printf("\n������ �� ���� ���.\n");
			b_RLiedown = true;
			for (int i = 0; i < n_player / 2; i++) {
				if (player[a[1][i]].stamina > 0) {
					player[a[1][i]].stamina = (int)player[a[1][i]].stamina * 0.7;
				}
			}
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

			player_list(); //����Ʈ ���� 

			power();

			player_change();

			b_LLiedown = false;
			b_RLiedown = false;

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


	printf("no. of player left : %d\n", sizeof(player) / sizeof(PLAYER));
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
	print_list(); //����Ʈ ���



}

void print_list() {
	for (int i = 0; i < n_player; i++) {

		switch (player[i].status) {
		case 1:
			printf("player %d: alive**\n", i); //Ż������ ���� �÷��̾� ������ **
			break;
		case 2:
			printf("player %d: alive\n", i); //����ִ� ����
			break;
		case 3:
			printf("player %d: DEAD\n", i); //�÷��̾� ���
			break;
		case 4:
			printf("player %d: alive*\n", i); //Ż���ߴ� ���
			break;

		default:
			break;

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
	srand((unsigned int)time(NULL));

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

	z = (29 / 2) - ((n_player / 2) + 1);
	z2 = (29 / 2) + ((n_player / 2) + 3);
	return 0;
}


void juldarigi() {
	player[0].is_alive = false;

	juldarigi_init();

	team_mate();


	map();


	game();

}