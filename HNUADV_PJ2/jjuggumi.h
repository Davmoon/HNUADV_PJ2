#ifndef _JJUGGUMI_H_ // if no define으로 자세한건 https://blog.naver.com/clown7942/110123052710 참고
#define _JJUGGUMI_H_

#include <Windows.h>
#include <stdbool.h>

#define PLAYER_MAX		10
#define ITEM_MAX	10

typedef struct {
	char name[100];
	int intel_buf, str_buf, stamina_buf;
}ITEM;

typedef struct {
	int id;
	char name[100];

	// 능력치: 지능, 힘, 스태미나
	int intel, str, stamina;

	bool is_alive;
	bool hasitem;
	ITEM item;
}PLAYER;

PLAYER player[PLAYER_MAX];
ITEM item[ITEM_MAX];
bool pass[PLAYER_MAX]; // 기본값 false, 통과하면 true
int itm_take[PLAYER_MAX]; //존재 true, 비활성화 && 먹음 false 구분

int n_player, n_alive, n_item;
int tick;  // 시계

// 미니게임
void sample(void);
void mugunghwa(void);
//void nightgame(void);
//void juldarigi(void);
void jebi(void);

int randint(int low, int high);

#endif
