#define _CRT_SECURE_NO_WARNINGS

#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>
#include<string.h>
#include <stdbool.h>
#include <Windows.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3


#define PLAYER_MAX		6
#define ITEM_MAX	10

void team_mate();
void player_list();
void gamemaster();
void power();
void map();

void map() {
	for (int i = 0; i < 29; i++) {
		if (i == 14) {
			printf(" ");
		}
		else {
			printf("#");
		}
	}
	printf("\n");
	
	printf("\n");
	for (int i = 0; i < 29; i++) {
		if (i == 14) {
			printf(" ");
		}
		else {
			printf("#");
		}
	}
}


void gamemaster() {

}



//리스트 생성
void player_list() {


	printf("no. of player left : %d\n", sizeof(player) / sizeof(PLAYER));
	for (int i = 0; i < PLAYER_MAX; i++) {
		printf("player %d: ", i);
		if (player[i].is_alive == true) {
			printf("alive\n");
		}
		else {
			player[i].hasitem = false; //탈락한 플레이어의 아이템 제거
			printf("alive*\n");
		}

	}
}

//팀 설정
void team_mate() {
	int temp;
	int a[2][PLAYER_MAX / 2], x = 0, y = 0;

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


	for (int i = 0;i < PLAYER_MAX / 2; i++) {
		printf("%d", a[0][i]);
	}

}




void juldarigi() {
	
	//player_list();

	team_mate();
	//map();
}