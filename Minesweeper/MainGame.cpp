#include<iostream>
#include<string>
#include<ctime>
#include"acllib.h"
using namespace std;

class Cube {
public:
	//-1Ϊ��������
	//������ֵΪ��Χ8���׵�����
	int mineNumber;
	//��¼���������
	int x;
	int y;
	//��¼�Ƿ񱻵㿪
	bool isOpen;
	//��¼����Ƿ��ڷ�����
	bool isMouseOn;
	//���캯��
	Cube() {
	}
	Cube(int mineNumber, bool isOpen, bool isMouseOn, int x, int y) {
		this->mineNumber = mineNumber;
		this->isOpen = isOpen;
		this->isMouseOn = isMouseOn;
		this->x = x;
		this->y = y;
	}
	//��������
	void PaintSelf() {
		//������û�д�ʱִ��
		if (!isOpen) {
			//��������ڵķ������Ϊ����ɫ
			if (!isMouseOn) {
				setBrushColor(RGB(0, 205, 102));
				beginPaint();
				roundrect(y, x, y + 40, x + 40, 5, 5);
				endPaint();
			}
			//�������������Ϊ��ɫ
			else {
				setBrushColor(RGB(204, 255, 0));
				beginPaint();
				roundrect(y, x, y + 40, x + 40, 5, 5);
				endPaint();
			}
			//���״��룬ʹ�ÿ�������ը���ķ�����ʾB
			//if (mineNumber == -1) {
			//	setTextBkColor(EMPTY);
			//	setTextSize(30);
			//	setTextColor(RGB(139, 87, 66));
			//	beginPaint();
			//	paintText(y + 12, x + 5, "B");
			//	endPaint();
			//}
		}
		//�������ʱִ��
		else {
			//������Ϊը��ʱִ��
			//��ը���������Ϊ��ɫ������ʧ�ܽ���ʱ��ʾ
			if (mineNumber == -1) {
				setBrushColor(RGB(255, 0, 0));
				beginPaint();
				roundrect(y, x, y + 40, x + 40, 5, 5);
				endPaint();
			}
			//�����鲻Ϊը��ʱִ��
			//���������Ϊ��ɫ��������ʾ��ǰ�����¼����Χը����Ŀ
			else {
				setBrushColor(RGB(173, 173, 173));
				beginPaint();
				roundrect(y, x, y + 40, x + 40, 5, 5);
				if (mineNumber > 0)
				{
					setTextBkColor(EMPTY);
					setTextSize(30);
					setTextColor(RGB(69, 69, 69));
					paintText(y + 12, x + 5, to_string(mineNumber).c_str());
				}
				endPaint();
			}
		}
	}
};

//����������λ����
int dx[8] = { -1,-1,0,1,1,1,0,-1 };
int dy[8] = { 0,1,1,1,0,-1,-1,-1 };
//��ŷ���ľ���
Cube Map[16][16];
//-1Ϊʧ�ܣ�1Ϊʤ����0Ϊ������
int gameState;

void GameInit();
void GameEnd();
void TimerEvent(int timer_id);
void MouseEvent(int x, int y, int button, int event);

int Setup() {
	//����ͼ�δ���
	initWindow("Winesweeper", 0, 0, 700, 700);
	//��������¼��ص��������ʱ���ص�����
	registerMouseEvent(MouseEvent);
	registerTimerEvent(TimerEvent);
	//��Ϸ��ʼ��
	GameInit();
	return 0;
}

//���Ѻ���
void dfs(int x, int y) {
	//��ǰ������Χ������ĿΪ0���򿪵�ǰ�飬��������������
	if (Map[x][y].mineNumber == 0) {
		Map[x][y].isOpen = true;
		for (int k = 0; k < 8; ++k) {
			int nx = x + dx[k];
			int ny = y + dy[k];
			if (nx >= 0 && nx < 16 && ny >= 0 && ny < 16)
				if (!Map[nx][ny].isOpen) dfs(nx, ny);
		}
	}
	//��ǰ������Χ������ĿΪ0���򿪵�ǰ�飬ֹͣ����
	else if (Map[x][y].mineNumber > 0) {
	Map[x][y].isOpen = true;
	}
}
//��Ϸ��ʼ��
void GameInit() {
	//����ֵ
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			Map[i][j] = Cube(0, false, false, 30 + 40 * j, 30 + 40 * i);
		}
	}
	//��ʼ���������
	srand(time(NULL));
	//���õ���λ�ã���40��
	for (int i = 0; i < 40; ++i) {
		int x = rand() % 16;
		int y = rand() % 16;
		if (Map[x][y].mineNumber == -1) {
			i--;
		}
		else {
			Map[x][y].mineNumber = -1;
		}
	}
	//����ÿ����Χ�м�������
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			if (Map[i][j].mineNumber != -1) {
				for (int k = 0; k < 8; ++k) {
					int x = i + dx[k];
					int y = j + dy[k];
					if (x >= 0 && x < 16 && y >= 0 && y < 16
						&& Map[x][y].mineNumber == -1)
						Map[i][j].mineNumber++;
				}
			}
		}
	}
	//��Ϸ״̬����Ϊ������
	gameState = 0;
	//������ʱ��0
	startTimer(0, 10);
}
//��Ϸ����
void GameEnd() {
	//ʤ�������ʤ��
	if (gameState == 1) {
		cancelTimer(0);
		setTextSize(300);
		setTextBkColor(EMPTY);
		setTextColor(RGB(238, 106, 80));
		beginPaint();
		paintText(50, 180, "ʤ��");
		endPaint();
	}
	//ʧ�������ʧ��
	else if (gameState == -1) {
		cancelTimer(0);
		beginPaint();
		setTextSize(300);
		setTextBkColor(EMPTY);
		setTextColor(RGB(100, 100, 100));
		paintText(50, 180, "ʧ��");
		endPaint();
	}
}
//����¼��ص�����
void MouseEvent(int x, int y, int button, int event) {
	//������ƶ�ʱִ��
	if (event == MOUSEMOVE) {
		//�������м�¼�����λ�����
		for (int i = 0; i < 16; ++i) {
			for (int j = 0; j < 16; ++j) {
				Map[i][j].isMouseOn = false;
			}
		}
		//��������������ڵķ���
		for (int i = 0; i < 16; ++i) {
			for (int j = 0; j < 16; ++j) {
				if (!Map[i][j].isOpen) {
					if (y<Map[i][j].x + 40 && y>Map[i][j].x
						&& x<Map[i][j].y + 40 && x>Map[i][j].y) {
						Map[i][j].isMouseOn = true;
						return;
					}
				}
			}
		}
	}
	//����갴������ʱִ�У�����м��Ҽ��Կɣ�
	else if (event == BUTTON_UP) {
		//���ڽ�������¿�ʼ
		if (gameState == 1 || gameState == -1)
		{
			GameInit();
		}
		//����������ķ��飬�򿪣��ж��Ƿ�ȵ�����
		//δ�ȵ������DFS���ȵ������е��׷���򿪣�
		//������Ϸ״̬����Ϊ-1
		else {
			for (int i = 0; i < 16; ++i) {
				for (int j = 0; j < 16; ++j) {
					if (!Map[i][j].isOpen &&y<Map[i][j].x + 40 && y>Map[i][j].x
						&& x<Map[i][j].y + 40 && x>Map[i][j].y) {
						if (Map[i][j].mineNumber == -1) {
							gameState = -1;
							for (int ii = 0; ii < 16; ++ii) {
								for (int jj = 0; jj < 16; ++jj) {
									if (Map[ii][jj].mineNumber == -1)
										Map[ii][jj].isOpen = true;
								}
							}
						}
						else {
							dfs(i, j);
						}
						return;
					}
				}
			}
		}
		
	}
}
//�������ص�����
//ÿ��10ms����һ�Σ�����ȷ����Լʱ�䣩
//���ڻ���û��֡
void TimerEvent(int timer_id) {
	//����
	beginPaint();
	clearDevice();
	endPaint();
	//������IDΪ0ʱִ��
	//��������
	if (timer_id == 0) {
		for (int i = 0; i < 16; ++i) {
			for (int j = 0; j < 16; ++j) {
				Map[i][j].PaintSelf();
			}
		}
	}
	//����Ƿ�ʤ��
	int cnt = 0;
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			if (!Map[i][j].isOpen) cnt++;
		}
	}
	//��ʤ��������Ϸ״̬����Ϊ1
	if (cnt == 40) gameState = 1;
	//�ж��Ƿ����
	GameEnd();
}