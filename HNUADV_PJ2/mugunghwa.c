#define _CRT_SECURE_NO_WARNINGS //sprintf() ����ϱ� ���� ������ ������ �����������..
#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void m_init(void);
void yh_print(int, int, int, bool); //���� ���� �Լ� x, y, ���� ��� ��, �ڵ��ƺ� O(true), X(false)
bool mv_m_random(int); // ����� �κ� https://coding-factory.tistory.com/667 ����
void pass_zone(void);
void yh_no_watch(int yh_period[]);
void mv_ten();
bool catch_mv(int, bool, key_t key);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX]; // �� �÷��̾� ��ġ, �̵� �ֱ�, �н� ����
int len = 0; //'����ȭ���� �Ǿ����ϴ�' ��µ� ���� ����
char msg1[50] = { "player",}; //dialog() �޼��� ���� sprintf https://jhnyang.tistory.com/314 ����

void m_init(void) {
	map_init(13, 35);
	yh_print(5, 1, 3, true);
	// �ֱⰡ ���� �ٸ��� tick % period[i]�� ��Ȯ�� �������� ���� �� �����Ƿ� (Lee ���� ����)
	int period_set[] = { 250, 260, 270, 280, 290, 300, 310, 320, 330, 340};

	int x, y;
	// �� �ڸ��� ��Ƴ��� �÷��̾ �����ϰ� ��ġ�Ǵ� �ڵ�
	for (int i = 0; i < n_player; i++) {
		if (player[i] == true) {
			do {
				x = randint(1, 11);
				y = 33;
			} while (!placable(x, y));
			px[i] = x;
			py[i] = y;
			period[i] = period_set[i];

			back_buf[px[i]][py[i]] = '0' + i;
		}
	}

	tick = 0; // �ʱ�ȭ
}

void yh_print(int x, int y, int num, bool look) {
	char icon = '@'; //�÷��̾ ���� �ִ� ��Ȳ

	if (!look) icon = '*'; //�� �ڷ� ���� �ִ� ��Ȳ

	for (int i = 0; i < num; i++) {
		back_buf[x + i][y] = icon;
	}
}

bool mv_m_random(int pnum) {
	int nx, ny;
	int percent[10] = { 0,0,0,0,0,0,0,1,2,3 }; // 70%, 10%, 10%, 10% ����
	int mv_stay = 0; // ���ڸ��� �ִ� ��� ī����

	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	do {
		switch (percent[randint(0, 9)]) {
			case 0:
				nx = px[pnum] + dx[2]; ny = py[pnum] + dy[2];
				break; //��������
			case 1:
				nx = px[pnum] + dx[0]; ny = py[pnum] + dy[0];
				break; //��������
			case 2:
				nx = px[pnum] + dx[1]; ny = py[pnum] + dy[1];
				break; //�Ʒ�������
			case 3:
				nx = px[pnum]; ny = py[pnum];
				back_buf[px[pnum]][py[pnum]] = ' ';//placable ��ȸ ���� ��ĭ���� ����
				mv_stay++;
				break; //���ڸ���
		}
	} while (!placable(nx, ny));

	// mv_stay�� 1�϶� 
	if (mv_stay == 1) {
		back_buf[px[pnum]][py[pnum]] = '0' + pnum; //placable ��ȸ �۾� ����
		return true;// ���ڸ��� ��� ���� ���� bool ���.
	}
	else {
		move_tail(pnum, nx, ny);
		return false; // ���� ��� �ɵ�? ���������� ���� �߰�
	}
}

void pass_zone(void) {
	//yh 3���̹Ƿ� �νĺκ��� 5��(���氡��)
	for (int i = 0; i < n_player; i++) {
		//pass ó���� false ó���� ������ ���ÿ� ó���Ǵ� ��Ȳ ����
		if (player[i] == true) {

			if ((px[i] == 4 && py[i] == 1) ||
				(px[i] == 5 && py[i] == 2) ||
				(px[i] == 6 && py[i] == 2) ||
				(px[i] == 7 && py[i] == 2) ||
				(px[i] == 8 && py[i] == 1)) {

				back_buf[px[i]][py[i]] = ' ';
				pass[i] = true;
			}
		}
	}
}

void yh_no_watch(int yh_period[]) {
	char sent[] = "����ȭ�����Ǿ����ϴ�";
	int pm_time = 30; // �������ų� �������� ���� ���ϴ� �и�sec

	if (len <= 9) {
		yh_print(5, 1, 3, false); // ���� off
		gotoxy(N_ROW, len * 2); // ��� ��ҷ� �̵�

		// '����ȭ����' ���
		if (len <= 4 && yh_period[2] % yh_period[0] == 0) {
			printf("%c%c", sent[len * 2], sent[len * 2 + 1]); //�ѱ��� 2bit, ���� %c%c ���
			len += 1;
			if (len == 4) {
				yh_period[2] = 0; //����ȭ ��� Ÿ�̸� �ʱ�ȭ
			}
		}
		// '�Ǿ����ϴ�' ���
		else if (len >= 5 && yh_period[2] % yh_period[1] == 0) {
			printf("%c%c", sent[len * 2], sent[len * 2 + 1]); //�ѱ��� 2bit, ���� %c%c ���
			len += 1;

			if (len == 10) {
				yh_period[2] = 0; //����ȭ ��� Ÿ�̸� �ʱ�ȭ
				mv_ten(); // len 10�϶��� �ѹ��� �����(���̽�!!!, 3�� ����)
			}
		}
	}
	else if (len == 10) {
		yh_print(5, 1, 3, true);
		yh_period[2] += 10; // ����ȭ ��� ���� 3�� ī��Ʈ �ؾ� �ϱ� ����

		// ���ð� ī���� �׽�Ʈ �ڵ�
		//gotoxy(N_ROW + 1, 0);
		//printf("%d �и��� ���", yh_period[2]);

		if (yh_period[2] % 3000 == 0) {
			yh_print(5, 1, 3, true); //���� on
			len = 0; //����ȭ �� ������ ����, ���ο� ������ ���� ���� �ʱ�ȭ
			yh_period[2] = 0; //����ȭ 3�� ��� Ÿ�̸� �ʱ�ȭ

			gotoxy(N_ROW, 0); // ����ȭ ����� ������ ��� (�� ���� ����� �������� ����...)
			printf("                    "); // 2bit�̹Ƿ� 20ĭ

			// �Ѹ� ���׾����� dialog() ���ð� ��ŵ
			if (msg1[6] != '\0') {
				sprintf(msg1, "%s %s", msg1, "dead!");
				dialog(msg1);
			}

			msg1[6] = '\0';//player �� �ʱ�ȭ

			// pm_time�� ���� ������ ���� �ʵ��� �ϱ� ����.
			if (yh_period[1] > pm_time) {
				yh_period[0] += pm_time;
				yh_period[1] -= pm_time;
			}
		}
	}
}

void mv_ten() {
	for (int i = 1; i < n_player; i++) {
		if (player[i] == true && pass[i] == false && randint(0, 9) == 1) {
			// catch_mv�� ������ ���ڸ����� Ȯ��, �ƴϸ� �̵��� �ڸ����� �ٽ� ���������� �˻���. (�Ӹ� ������ ���ȴ�...)
			if (!catch_mv(i, true, '\0')) {
				catch_mv(i, false, '\0');
			}
			//player[i] = false; //10% Ȯ���� �ױ⸸ �ϸ� �ȴٸ� �� �ڵ带 ���.
			//back_buf[px[i]][py[i]] = ' ';
		}
	}
}

bool catch_mv(int pnum, bool count, key_t key) {
	int al_chk_count = 0; //�÷���� ��� üũ�ؾ� ������ �� ����.
	for (int i = 0; i < n_player; i++) {

		// if���� ���� < ���� �ڱ��ڽ��� �ɷ��� (��ȣ ����)
		if (player[i] == true && pass[i] == false && py[i] < py[pnum] && px[i] == px[pnum]) {

			//�׽�Ʈ ��ǥ ���
			//gotoxy(N_ROW + 3, 0);
			//printf("%d pass ��ǥ��: %d %d | %d %d", pnum, px[i], py[i], px[pnum], py[pnum]);

			// 0�� �տ� ���� ������ ������ �� �ֵ��� �ϴ� �ڵ�
			if (pnum == 0 && count == true) {
				move_manual(key);
				return true;
			}
			break;
		}
		else { al_chk_count++; }

		if (player[pnum] == true && pass[pnum] == false && al_chk_count == n_player) {
			// ������ ������ �ٸ� �÷��̾ ������ �������� ���ϰ� ������ �ִ� �����϶�
			if (pnum == 0 && count == true && move_manual(key) == false) {
				move_manual(key);
				return false;
			}
			// ���ڸ��� �ִ� ��� ���� �ʱ�. true�� ��� �ѹ��� ������ ���ڸ��� ��� catch_mv �ٽ� �ȵ�������
			if (pnum != 0 && count == true && mv_m_random(pnum) == true) {
				return true;
			}
			//�׽�Ʈ ��ǥ ���
			//gotoxy(N_ROW + 2, 0);
			//printf("%d kill ��ǥ��: %d %d | %d %d", pnum, px[i], py[i], px[pnum], py[pnum]);
			
			
			back_buf[px[pnum]][py[pnum]] = ' ';
			player[pnum] = false;
			sprintf(msg1, "%s %d", msg1, pnum); //kill �÷��̾� ����
			
			n_alive--;
			return false; //���� ���� ���� ����
		}
	}
}

void mugunghwa(void) {
	m_init();
	system("cls");
	display();

	dialog(" \"����ȭ ���� �Ǿ����ϴ�\"");
	int yh_period[] = { 250, 250, 0 }; // ����ȭ �� t, �Ǿ����ϴ� t, ����ȭ ���� Ÿ�̸�(tick�� ������ ��������)

	while (1) {

		if (n_alive == 1) {
			break;
		}

		yh_no_watch(yh_period);
		
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {

			if (pass[0] == false && player[0] == true) {
				if (len <= 9) {
					move_manual(key);
				}
				else if (len == 10) {
					if (catch_mv(0, true, key) == true) {
						catch_mv(0, false, '\0'); //\0�� ����� �� ��� �����Ѱ���..
					}
					
				}
			}
		}

		for (int i = 1; i < n_player; i++) {
			if (tick % period[i] == 0 && player[i] == true && pass[i] == false) {
				// 10�� ���, yh_no_watch ���� ten_mv���� �۵�(10% �ߺ� �۵� ���� ����)
				if (len <= 9) {
					mv_m_random(i);
				}
			}
		}
		
		pass_zone();
		display();
		//pass, kill Ȯ���� ���� �׽�Ʈ �ڵ�
		//for (int i = 0; i < n_player; i++) {
		//	if (pass[i] == true) {
		//		gotoxy(N_ROW + 15, i * 9);
		//		printf("P%d(%d,%d)|", i, px[i], py[i]);
		//	}
		//	if (player[i] == false) {
		//		gotoxy(N_ROW + 16, i * 9);
		//		printf("K%d(%d,%d)|", i, px[i], py[i]);
		//	}
		//	//gotoxy(N_ROW + 17, i * 9);
		//	//printf("%d(%d,%d)|", i, px[i], py[i]);
		//}
		Sleep(10);
		tick += 10; 
		yh_period[2] += 10; // ����ȭ ���� Ÿ�̸�
	}
}