// 2023-2 고급프로그래밍 과제: 쭈꾸미 게임
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);

// low 이상 high 이하 난수를 발생시키는 함수
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
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
        p->stamina = 100; // 100%

        p->is_alive = true;
        p->hasitem = false;
    }

    fscanf_s(fp, "%d", &n_item);
    for (int i = 0; i < n_item; i++) {
        fscanf_s(fp, "%s%d%d%d", item[i].name,
            (unsigned int)sizeof(item[i].name),
            &(item[i].intel_buf),
            &(item[i].str_buf),
            &(item[i].stamina_buf));
    }

    fclose(fp);
	return 0;
}

void intro() {
    printf("   _    _                                         _ \n");
    printf("  (_)  (_)                                       (_)\n");
    Sleep(200);
    printf("   _    _  _   _   __ _   __ _  _   _  _ __ ___   _ \n");
    Sleep(200);
    printf("  | |  | || | | | / _` | / _` || | | || '_ ` _ \\ | |\n");
    Sleep(200);
    printf("  | |  | || |_| || (_| || (_| || |_| || | | | | || |\n");
    Sleep(200);
    printf("  | |  | | \\__,_| \\__, | \\__, | \\__,_||_| |_| |_||_|\n");
    Sleep(200);
    printf(" _/ | _/ |         __/ |  __/ |                     \n");
    Sleep(200);
    printf("|__/ |__/         |___/  |___/                      \n");
    Sleep(300);

    system("cls");
}

void ending() {
    // 1명만 남은 경우: 즉시 엔딩으로 넘어가서 쭈꾸미 게임을 종료
    int alive_count = 0;
    int last_alive_player = -1;
    for (int i = 0; i < n_player; i++) {
        if (player[i].is_alive) {
            alive_count++;
            last_alive_player = i;
        }
    }

    if (alive_count == 1) {
        system("cls");
        printf("게임이 종료됩니다. 단 한 명만 남았습니다.\n");
        printf("승자: 플레이어 %d\n", last_alive_player);
        exit(0);
    }

    // 모든 (미니)게임에서, 게임 마스터가 게임을 강제로 종료할 수 있음

    if (alive_count > 1) {
        system("cls");
        printf("우승자를 가리지 못했습니다.\n");
        printf("살아 남은 플레이어:\n");
        for (int i = 0; i < n_player; i++) {
            if (player[i].is_alive) {
            printf("플레이어 %d\n", i);
            }
        }
        exit(0);
    }
    
}


int main(void) {
    //intro();
	jjuggumi_init();//인원 입력 함수
    //sample();
	//mugunghwa();
	nightgame();
	//juldarigi();
	jebi();
	ending();
	return 0;
}
