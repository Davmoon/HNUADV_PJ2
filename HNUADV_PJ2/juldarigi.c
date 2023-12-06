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
#define DIALOG_DURATION_SEC		4

#define	DATA_FILE	"jjuggumi.dat"


void team_mate(); //팀설정
void player_list(); //플레이어 리스트 
void gamemaster(); //게임 조작
void game(); //게임 플레이
void power(); //힘 계산
void map(); //맵 생성
void player_spawn(); //배열안에 플레이어 생성
void player_change();
void end(); //게임 끝내기 + 데이터 갱신
void juldarigi_dialog(char message[]);
void juldarigi_print_status(void);
void juldarigi_display(void);
void juldarigi_map_init(int n_row, int n_col);
void juldarigi_print_status(void);


int px[PLAYER_MAX], py[PLAYER_MAX], jy[3];

int a[2][PLAYER_MAX / 2]; //플레이어 리스트 전역변수로 선언
char maps[3][29];
float strs; // 힘
PLAYER dead[6]; //죽은 사람의 번호를 넣을 배열
int dead_count = 0; //죽은 사람이 몇명인지 카운트 하는 배열
int dead_player = PLAYER_MAX + 1;
float m_pw = 0;
bool b_LLiedown = false;
bool b_RLiedown = false;
bool b_masteradd = false;
char msg1[50];
bool b_IsEnd = false;

void map() { //맵 생성
	int row = 3;
	int col = 29;
	if (col % 2 != 1) {
		col++;
	}
	juldarigi_map_init(row, col);

	player_spawn();
}

void player_spawn() { //스폰
	int x = 0, y = 0, z = (29 / 2 - (n_player / 2 + 1)); //x는 1차원배열, y가 2차원 배열의 미지수, z는 줄을 집어넣기위한 카운트
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
			if (strs < 0) { // 힘이 음수일때
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
			else if (strs > 0) { //힘이 양수일때
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
			else { //힘이 0일때

			}
		}
	}
	else
	{
		if (strs < 0) { // 힘이 음수일때
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
		else if (strs > 0) { //힘이 양수일때
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
		else { //힘이 0일때

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
			juldarigi_dialog(" \"게임마스터가 줄다리기를 종료하였습니다.\"");
			b_IsEnd = true;
		}
		else if (ch == 'x' || ch == 'X') {
			juldarigi_dialog(" \"왼쪽팀이 눕기를 사용하였습니다.\"");
			b_LLiedown = true;
			for (int i = 0; i < n_player / 2; i++) {
				if (player[a[0][i]].stamina > 0) {
					player[a[0][i]].stamina = (int)player[a[0][i]].stamina * 0.7;
				}
			}
		}
		else if (ch == '.') {
			juldarigi_dialog(" \"오른쪽팀이 눕기를 사용하였습니다.\"");
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
			player[i].is_alive = true; //탈락한적 없는 사람이 살아남거나 탈락했던 사람이 살아남으면
			n_alive++;
			//alive 상태가 true
		}
		else if (player[i].status == 1) {
			//탈락한적 없는 사람이 죽으면 아이템 상실, 지능, 힘이 절반 하락
			player[i].hasitem = false; //아이템이 먼저 상실해야 기본값이 하락함
			player[i].intel = player[i].intel / 2;
			player[i].str = player[i].str / 2;
		}
		else if (player[i].status == 3) {
			player[i].is_alive = false; //탈락한 사람이 이번게임에서 탈락했기 때문에 진짜 사망처리
			n_alive--;
		}
	}
}


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

			player_list(); //리스트 생성 

			if (b_IsEnd) {
				break;
			}

			juldarigi_display();

			power();

			player_change();

			b_LLiedown = false;
			b_RLiedown = false;

			if (dead_player != PLAYER_MAX + 1) {
				sprintf(msg1, "%d %s", dead_player, "번 플레이어 탈락!");
				juldarigi_dialog(msg1);
				dead_player = PLAYER_MAX + 1;
			}

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

void juldarigi_map_init(int n_row, int n_col) {
	// 두 버퍼를를 완전히 비우고 #을 둘레로 채움
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = front_buf[i][j] = ' ';
		}
	}

	N_ROW = n_row;
	N_COL = n_col;
	for (int i = 0; i < N_ROW; i++) {
		// 대입문 이렇게 쓸 수 있는데 일부러 안 가르쳐줬음
		back_buf[i][0] = back_buf[i][N_COL - 1] = '#'; //처음과 끝 부분 # 추가

		for (int j = 1; j < N_COL - 1; j++) {
			back_buf[i][j] = (i == 0 || i == N_ROW - 1) ? '#' : ' ';// 앞의 열부분에서 처음 끝 채웠으니 행에서는 앞뒤 한줄씩은 다 채우기 코드
		}
	}
	
	//중간 부분 구멍 추가
	back_buf[0][N_COL / 2] = ' ';
	back_buf[2][N_COL / 2] = ' '; 
}


void juldarigi_display(void) {
	draw();
	gotoxy(N_ROW + 4, 0);  // 추가로 표시할 정보가 있으면 맵과 상태창 사이의 빈 공간에 출력
	juldarigi_print_status();
}

void juldarigi_print_status(void) {

	//10일 경우 남는 칸 생김
	if (n_alive != 10) { printf("no. of players left: 0%d\n", n_alive); }
	else { printf("no. of players left: %d\n", n_alive); }
	printf("\t\t\tintl\tstr\tstm\n");
	for (int p = 0; p < n_player; p++) {
		switch (player[p].status) {
		case 1:
			//탈락하지 않은 플레이어 죽으면 **
			printf("player %2d: %7s\t%d(+%d)\t%d(+%d)\t%d%%(%d)\n", p, "alive**", player[p].intel, player[p].item.intel_buf, player[p].str, player[p].item.str_buf, player[p].stamina, player[p].item.stamina_buf);

			break;
		case 2:
			//살아있는 상태
			printf("player %2d: %7s\t%d(+%d)\t%d(+%d)\t%d%%(%d)\n", p, "alive", player[p].intel, player[p].item.intel_buf, player[p].str, player[p].item.str_buf, player[p].stamina, player[p].item.stamina_buf);

			break;
		case 3:
			//플레이어 사망
			printf("player %2d: %7s\t%d(+%d)\t%d(+%d)\t%d%%(%d)\n", p, "DEAD", player[p].intel, player[p].item.intel_buf, player[p].str, player[p].item.str_buf, player[p].stamina, player[p].item.stamina_buf);

			break;
		case 4:
			//탈락했던 사람
			printf("player %2d: %7s\t%d(+%d)\t%d(+%d)\t%d%%(%d)\n", p, "alive*", player[p].intel, player[p].item.intel_buf, player[p].str, player[p].item.str_buf, player[p].stamina, player[p].item.stamina_buf);

			break;

		default:
			break;
		}
	}
}

void juldarigi_dialog(char message[]) {

	int sec = DIALOG_DURATION_SEC + 1; // 0으로 끝나므로 1로 끝나도록 + 1
	char save_buf[ROW_MAX][COL_MAX]; //세이브 버퍼 생성, 새로운 부분 띄우고 복구하기 위함

	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			save_buf[i][j] = back_buf[i][j];//현재 출력중인 버퍼를 저장
		}
	}

	int Rmid = N_ROW / 3; //최소한으로 dialog 중앙정렬 위해 맵 세로 3등분해서 처음 + 1

	// '*'로 이루어진 dialog 테두리 출력, 상상력 부족으로 한땀한땀 해봄
	for (int i = Rmid; i < Rmid + 5; i++) {
		back_buf[i][2] = back_buf[i][COL_MAX - 45] = '*';
		back_buf[i][2] = back_buf[i][COL_MAX - 45] = '*';
		for (int j = 3; j < COL_MAX - 45; j++) {
			back_buf[i][j] = (i == Rmid || i == Rmid + 4) ? '*' : ' ';
		}
	}
	juldarigi_display();

	int y = 2; // message 출력 아래쪽으로 몇칸인지 지정
	int sec_x = 4; // sec초 출력 옆으로 몇칸인지 지정
	int mes_x = 6; // message 출력 옆으로 몇칸인지 지정

	gotoxy(Rmid + y, mes_x); // message 출력
	printf("%s", message);

	while (sec != 0) {
		gotoxy(Rmid + y, sec_x); // sec초 출력
		printf("%d", sec);

		Sleep(1000); // 1초 지연
		sec--; //초 경과 카운트

	}

	gotoxy(Rmid + y, sec_x);
	printf("                                         ");

	// front 버퍼 비움, back은 save 버퍼로 복구
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = save_buf[i][j];

		}
	}

	juldarigi_display();
	//Sleep(2000); // 정지하는 상태인지 파악하기 위한 TEST CODE
}


void juldarigi() {
	system("cls");
	team_mate();
	map();
	dialog("      \"줄다리기 게임\"    ");
	game();
	end();
}