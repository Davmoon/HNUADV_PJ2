#define _CRT_SECURE_NO_WARNINGS

#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>
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





void player_list() {
	printf("no. of player left : %d", sizeof(player));
	for (int i = 0; i < PLAYER_MAX; i++) {
		printf("player %d: ", i);
		if (player[i].is_alive == true) {
			printf("alive");
		}
		else {
			printf("alive*");
		}

	}
}


void team_mate() {

}



void juldarigi() {



}