// 2023-2 ������α׷��� ����: �޲ٹ� ����
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);

// low �̻� high ���� ������ �߻���Ű�� �Լ�
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
    
    n_alive = n_player; //����ִ� �÷��̾� = ó�� �÷��̾� ��

    for (int i = 0; i < n_player; i++) {
        // ���� �� ��� ����(����ü ������, ��������������)
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
    // 1�� ���� ���: ��� �������� �Ѿ�� �޲ٹ� ������ ����
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
        printf("������ ����˴ϴ�. �� �� �� ���ҽ��ϴ�.\n");
        printf("����: �÷��̾� %d\n", last_alive_player);
        exit(0);
    }

    // ��� (�̴�)���ӿ���, ���� �����Ͱ� ������ ������ ������ �� ����

    if (alive_count > 1) {
        system("cls");
        printf("����ڸ� ������ ���߽��ϴ�.\n");
        printf("��� ���� �÷��̾�:\n");
        for (int i = 0; i < n_player; i++) {
            if (player[i].is_alive) {
            printf("�÷��̾� %d\n", i);
            }
        }
        exit(0);
    }
    
}


int main(void) {
    //intro();
	jjuggumi_init();//�ο� �Է� �Լ�
    //sample();
	//mugunghwa();
	nightgame();
	//juldarigi();
	jebi();
	ending();
	return 0;
}
