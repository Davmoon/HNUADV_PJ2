#ifndef _JJUGGUMI_H_ // if no define���� �ڼ��Ѱ� https://blog.naver.com/clown7942/110123052710 ����
#define _JJUGGUMI_H_

#include <Windows.h>
#include <stdbool.h>

#define PLAYER_MAX		10

bool player[PLAYER_MAX];  // �⺻�� true, Ż���ϸ� false
bool pass[PLAYER_MAX]; // �⺻�� false, ����ϸ� true
int n_player, n_alive;
int tick;  // �ð�

// �̴ϰ���
void sample(void);
void mugunghwa(void);
//void nightgame(void);
//void juldarigi(void);
//void jebi(void);

int randint(int low, int high);

#endif
