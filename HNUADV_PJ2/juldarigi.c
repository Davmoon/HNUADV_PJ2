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

void team_mate(); //팀설정
void player_list(); //플레이어 리스트 
void gamemaster(); //게임 조작
void game(); //게임 플레이
void power(); //힘 계산
void map(); //맵 생성
void player_spawn(); //배열안에 플레이어 생성
void player_change();
void print_player_list();




int a[2][PLAYER_MAX / 2]; //플레이어 리스트 전역변수로 선언
char maps[3][29];
float strs; // 힘
int z = (29 / 2) - ((PLAYER_MAX / 2) + 1); // 왼쪽으로 잡아당길때의 기준점
int z2 = (29 / 2) + ((PLAYER_MAX / 2) + 3); //오른쪽으로 잡아당길때의 기준점
PLAYER dead[PLAYER_MAX]; //죽은 사람의 번호를 넣을 배열
int dead_count = 0; //죽은 사람이 몇명인지 카운트 하는 배열
float m_pw = 0;


void map() { //맵 생성


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
		}// 맵 생성
	}

	player_spawn();



}

void player_spawn() { //스폰
	int x = 0, y = 0, z = (29 / 2 - (PLAYER_MAX - 2)); //x는 1차원배열, y가 2차원 배열의 미지수, z는 줄을 집어넣기위한 카운트
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
	if (strs < 0) { // 힘이 음수일때

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
			z--; //기준점 갱신
		}
	}
	else if (strs > 0) { //힘이 양수일때
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
			z2++;//기준점 갱신
		}
	}
	else { //힘이 0일때
		maps[3][29];
	}
}




void gamemaster() {
	char ch;


	if (_kbhit()) {  // 키 입력이 있는지 확인
		ch = _getch();  // 키 입력을 대기하고 입력된 키를 가져옴

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


void game() { // 게임 조작, 플레이
	char key;
	clock_t start_time, current_time;
	double elapsed_time;

	// 현재 시간 기록
	start_time = clock();
	while (1) {
		// 현재 시간 기록
		current_time = clock();
		char ch;
		// 경과 시간 계산 (단위: 초)
		elapsed_time = (double)(current_time - start_time) / CLOCKS_PER_SEC;
		float key1 = 0, key2 = 0;
		// 1초 경과 시에 원하는 동작 수행
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

			// 다시 시작 시간을 현재 시간으로 업데이트
			start_time = clock();
		}
		gamemaster();
	}

}

void power() { // 힘 계산
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


//리스트 생성
void player_list() {



	printf("no. of player left : %d\n", sizeof(player) / sizeof(PLAYER));
	for (int i = 0; i < PLAYER_MAX; i++) {
		printf("player %d: ", i);
		if (player[i].is_alive == true) {
			char a = dead[i].name;
			char b = player[i].name;
			if (strcmp(dead[i].name, player[i].name) == 0) {
				printf("alive\n"); //이미 살았던 플레이어는 죽지 않음 대신 **개로 표시
				player[i].hasitem = false; //플레이어의 아이템 몰수
				player[i].intel = player[i].intel / 2; // 플레이어의 지능 반토막
			}
			else {
				printf("alive\n");
			}

		}
		else {
			player[i].hasitem = false; //탈락한 플레이어의 아이템 제거
			char a = dead[i].name;
			char b = player[i].name;
			if (strcmp(dead[i].name, player[i].name) == 0) {
				printf("DEAD\n");
			}
			else {
				printf("alive*\n"); //죽었던 플레이어는 *로 표시
				player[i].is_alive == true; //죽었던 플레이어 부활
			}


		}

	}
}

//팀 설정
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
	} //유저 데이터 넣기

	for (int i = 0; i < PLAYER_MAX / 2; i++) { //왼쪽 팀 정렬
		for (int j = 0; j < ((PLAYER_MAX / 2) - 1) - i; j++) {
			if (a[0][j] < a[0][j + 1]) {	// 버블 정렬 사용
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