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


void team_mate(); //팀설정
void player_list(); //플레이어 리스트 
void gamemaster(); //게임 조작
void game(); //게임 플레이
void power(); //힘 계산
void map(); //맵 생성
void player_spawn(); //배열안에 플레이어 생성
void player_change();
void print_list(); //리스트 출력

int a[2][PLAYER_MAX / 2]; //플레이어 리스트 전역변수로 선언
char maps[3][29];
float strs; // 힘
int z; // 왼쪽으로 잡아당길때의 기준점
int z2; //오른쪽으로 잡아당길때의 기준점
PLAYER dead[6]; //죽은 사람의 번호를 넣을 배열
int dead_count = 0; //죽은 사람이 몇명인지 카운트 하는 배열
float m_pw = 0;
bool b_LLiedown = false;
bool b_RLiedown = false;
bool b_masteradd = false;


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
	int x = 0, y = 0, z = (29 / 2 - (n_player / 2 + 1)); //x는 1차원배열, y가 2차원 배열의 미지수, z는 줄을 집어넣기위한 카운트
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
			if (strs < 0) { // 힘이 음수일때
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
					z--; //기준점 갱신
				}
			}
			else if (strs > 0) { //힘이 양수일때
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
					z2++;//기준점 갱신
				}
			}
			else { //힘이 0일때
				maps[3][29];
			}
		}
	}
	else
	{
		if (strs < 0) { // 힘이 음수일때
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
				z--; //기준점 갱신
			}
		}
		else if (strs > 0) { //힘이 양수일때
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
				z2++;//기준점 갱신
			}
		}
		else { //힘이 0일때
			maps[3][29];
		}
	}
}




void gamemaster() {
	char ch;


	if (_kbhit()) {  // 키 입력이 있는지 확인
		ch = _getch();  // 키 입력을 대기하고 입력된 키를 가져옴

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
			printf("\n왼쪽 팀 눕기 사용.\n");
			b_LLiedown = true;
			for (int i = 0; i < n_player / 2; i++) {
				if (player[a[0][i]].stamina > 0) {
					player[a[0][i]].stamina = (int)player[a[0][i]].stamina * 0.7;
				}
			}
		}
		else if (ch == '.') {
			printf("\n오른쪽 팀 눕기 사용.\n");
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

			player_list(); //리스트 생성 

			power();

			player_change();

			b_LLiedown = false;
			b_RLiedown = false;

			// 다시 시작 시간을 현재 시간으로 업데이트
			start_time = clock();
		}
		gamemaster();

	}

}

void power() { // 힘 계산
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


//리스트 생성
void player_list() {


	printf("no. of player left : %d\n", sizeof(player) / sizeof(PLAYER));
	for (int i = 0; i < n_player; i++) { //모든 플레이어가 과거 죽었는지 살았는지 확인하는 반복문
		if (player[i].is_alive == true) { //과거 살았던 상태인지
			player[i].status = 2;
			for (int j = 0; j < dead_count; j++) { //리스트 내의 플레이어와 죽은 플레이어를 대조함 
				//플레이어가 죽을 수록 카운트는 늘어남
				if (!strcmp(dead[j].name, player[i].name)) {
					//현재 반복문 안에 있는 플레이어와 실시간으로 갱신되는 죽은 플레이어와 대조
					//만약 현재 플레이어와 죽은 플레이어의 이름이 같다면 
					//사망처리를 해야하지만, 현재 플레이어는 과거 게임에서 생존했기 때문에 
					player[i].status = 1; //탈락한적 없는 플레이어는 이기에 **표시 마크
				}
			}
		}
		else { //죽었던 상태인지 확인
			player[i].status = 4;
			//죽었던 상태에서 위의 반복문을 한번더 체크
			for (int j = 0; j < dead_count; j++) {
				if (!strcmp(dead[j].name, player[i].name)) { // 죽은 이력이 있으면서 탈락했다면
					player[i].status = 3; //진짜 탈락
				}
			}
		}
	}
	print_list(); //리스트 출력



}

void print_list() {
	for (int i = 0; i < n_player; i++) {

		switch (player[i].status) {
		case 1:
			printf("player %d: alive**\n", i); //탈락하지 않은 플레이어 죽으면 **
			break;
		case 2:
			printf("player %d: alive\n", i); //살아있는 상태
			break;
		case 3:
			printf("player %d: DEAD\n", i); //플레이어 사망
			break;
		case 4:
			printf("player %d: alive*\n", i); //탈락했던 사람
			break;

		default:
			break;

		}
	}

}

//팀 설정
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
	} //유저 데이터 넣기

	for (int i = 0; i < n_player / 2; i++) { //왼쪽 팀 정렬
		for (int j = 0; j < ((n_player / 2) - 1) - i; j++) {
			if (a[0][j] < a[0][j + 1]) {	// 버블 정렬 사용
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

	n_alive = n_player; //살아있는 플레이어 = 처음 플레이어 수

	for (int i = 0; i < n_player; i++) {
		// 아직 안 배운 문법(구조체 포인터, 간접참조연산자)
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