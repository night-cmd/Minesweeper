#include<iostream>
#include<string>
#include<ctime>
#include"acllib.h"
using namespace std;

class Cube {
public:
	//-1为自身是雷
	//其他数值为周围8个雷的数量
	int mineNumber;
	//记录方块的坐标
	int x;
	int y;
	//记录是否被点开
	bool isOpen;
	//记录鼠标是否在方块上
	bool isMouseOn;
	//构造函数
	Cube() {
	}
	Cube(int mineNumber, bool isOpen, bool isMouseOn, int x, int y) {
		this->mineNumber = mineNumber;
		this->isOpen = isOpen;
		this->isMouseOn = isMouseOn;
		this->x = x;
		this->y = y;
	}
	//绘制自身
	void PaintSelf() {
		//当方块没有打开时执行
		if (!isOpen) {
			//将鼠标所在的方块绘制为淡黄色
			if (!isMouseOn) {
				setBrushColor(RGB(0, 205, 102));
				beginPaint();
				roundrect(y, x, y + 40, x + 40, 5, 5);
				endPaint();
			}
			//将其他方块绘制为绿色
			else {
				setBrushColor(RGB(204, 255, 0));
				beginPaint();
				roundrect(y, x, y + 40, x + 40, 5, 5);
				endPaint();
			}
			//作弊代码，使用可以在有炸弹的方块显示B
			//if (mineNumber == -1) {
			//	setTextBkColor(EMPTY);
			//	setTextSize(30);
			//	setTextColor(RGB(139, 87, 66));
			//	beginPaint();
			//	paintText(y + 12, x + 5, "B");
			//	endPaint();
			//}
		}
		//当方块打开时执行
		else {
			//当方块为炸弹时执行
			//将炸弹方块绘制为红色，用于失败结算时显示
			if (mineNumber == -1) {
				setBrushColor(RGB(255, 0, 0));
				beginPaint();
				roundrect(y, x, y + 40, x + 40, 5, 5);
				endPaint();
			}
			//当方块不为炸弹时执行
			//将方块绘制为灰色，并且显示当前方块记录的周围炸弹数目
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

//用于搜索的位移量
int dx[8] = { -1,-1,0,1,1,1,0,-1 };
int dy[8] = { 0,1,1,1,0,-1,-1,-1 };
//存放方块的矩阵
Cube Map[16][16];
//-1为失败，1为胜利，0为进行中
int gameState;

void GameInit();
void GameEnd();
void TimerEvent(int timer_id);
void MouseEvent(int x, int y, int button, int event);

int Setup() {
	//启动图形窗口
	initWindow("Winesweeper", 0, 0, 700, 700);
	//设置鼠标事件回调函数与计时器回调函数
	registerMouseEvent(MouseEvent);
	registerTimerEvent(TimerEvent);
	//游戏初始化
	GameInit();
	return 0;
}

//深搜函数
void dfs(int x, int y) {
	//当前方块周围地雷数目为0，打开当前块，并继续向下搜索
	if (Map[x][y].mineNumber == 0) {
		Map[x][y].isOpen = true;
		for (int k = 0; k < 8; ++k) {
			int nx = x + dx[k];
			int ny = y + dy[k];
			if (nx >= 0 && nx < 16 && ny >= 0 && ny < 16)
				if (!Map[nx][ny].isOpen) dfs(nx, ny);
		}
	}
	//当前方块周围地雷数目为0，打开当前块，停止搜索
	else if (Map[x][y].mineNumber > 0) {
	Map[x][y].isOpen = true;
	}
}
//游戏初始化
void GameInit() {
	//矩阵赋值
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			Map[i][j] = Cube(0, false, false, 30 + 40 * j, 30 + 40 * i);
		}
	}
	//初始化随机种子
	srand(time(NULL));
	//设置地雷位置，共40个
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
	//计算每格周围有几个地雷
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
	//游戏状态设置为进行中
	gameState = 0;
	//开启定时器0
	startTimer(0, 10);
}
//游戏结束
void GameEnd() {
	//胜利则绘制胜利
	if (gameState == 1) {
		cancelTimer(0);
		setTextSize(300);
		setTextBkColor(EMPTY);
		setTextColor(RGB(238, 106, 80));
		beginPaint();
		paintText(50, 180, "胜利");
		endPaint();
	}
	//失败则绘制失败
	else if (gameState == -1) {
		cancelTimer(0);
		beginPaint();
		setTextSize(300);
		setTextBkColor(EMPTY);
		setTextColor(RGB(100, 100, 100));
		paintText(50, 180, "失败");
		endPaint();
	}
}
//鼠标事件回调函数
void MouseEvent(int x, int y, int button, int event) {
	//当鼠标移动时执行
	if (event == MOUSEMOVE) {
		//将矩阵中记录的鼠标位置清空
		for (int i = 0; i < 16; ++i) {
			for (int j = 0; j < 16; ++j) {
				Map[i][j].isMouseOn = false;
			}
		}
		//遍历查找鼠标所在的方块
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
	//当鼠标按键弹起时执行（左键中键右键皆可）
	else if (event == BUTTON_UP) {
		//用于结算后重新开始
		if (gameState == 1 || gameState == -1)
		{
			GameInit();
		}
		//查找鼠标点击的方块，打开，判断是否踩倒地雷
		//未踩到则进行DFS，踩到将所有地雷方块打开，
		//并将游戏状态设置为-1
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
//计数器回调函数
//每隔10ms调用一次（不精确，大约时间）
//用于绘制没有帧
void TimerEvent(int timer_id) {
	//清屏
	beginPaint();
	clearDevice();
	endPaint();
	//计算器ID为0时执行
	//遍历绘制
	if (timer_id == 0) {
		for (int i = 0; i < 16; ++i) {
			for (int j = 0; j < 16; ++j) {
				Map[i][j].PaintSelf();
			}
		}
	}
	//检测是否胜利
	int cnt = 0;
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			if (!Map[i][j].isOpen) cnt++;
		}
	}
	//若胜利，将游戏状态设置为1
	if (cnt == 40) gameState = 1;
	//判断是否结束
	GameEnd();
}