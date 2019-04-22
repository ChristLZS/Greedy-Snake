/**************************************************************************************************

									文件名称：贪吃蛇.cpp
									编译环境：VS2012  and  EasyX
									作者相关：沈成宇
									编写日期：2019-04-05
									最后修改：2019-04-06

****************************************************************************************************/

#include "stdafx.h"
#include<stdio.h>
#include<graphics.h>     //图形库头文件
#include<mmsystem.h>     //多媒体设备接口
#include<conio.h>        //Console Input/Output
#include<time.h>
#include<fstream>        //文件输入输出的头文件
#include<istream>
#include<iostream>
#pragma comment(lib,"winmm.lib")  //添加库文件
#define SNAKE_MAX_LENGTH  100     //蛇的最大长度
#define SNAKE_SIZE  10            //蛇的大小（宽度）
using namespace std;

void InitSnake();
void MoveSnake();
void DrawSnake();
void GetKey();
void CoorFood();
void DrawFood();
bool EatFood();
void Break();
void GameOver();
int Level();


//键盘输入的枚举
enum DIRECT {
    up = 72,
    down = 80,
    Left = 75,   //使用 std 之后，要对left ,right 重命名
    Right = 77,
    space = 32
};

//坐标结构体
struct COOR {
    int x;
    int y;
} coor;

//蛇的结构体
struct SNAKE {
    COOR szb[SNAKE_MAX_LENGTH];  //每一个蛇的结点的坐标
    int n;                       //蛇的当前长度
    DIRECT c;                    //蛇的方向
} snake;

//食物结构体
struct FOOD {
    COOR fzb;   //食物坐标
    bool flag;  //true 表示食物没有被吃掉， false 表示食物被吃掉
} food;

int IntervalTime;      //蛇每两次移动之间的间隔，表示难度等级用的

int main()
{
    InitSnake();
    while (1)
    {
        while (!kbhit())        //如果没有接收键盘
        {
            if (!EatFood())     //如果食物被吃掉，就画出新的食物来
            {
                CoorFood();
                DrawFood();
            }
            Break();            //先检查再移动
            MoveSnake();
            IntervalTime = Level() * 60;
            Sleep(IntervalTime);
        }
        GetKey();
    }
    getchar();  //卡屏
    return 0;
}

//初始化函数
void InitSnake()
{
    initgraph(640, 480, NOMINIMIZE);  //初始化绘图环境,创建画板

    //加载图片
    loadimage(NULL, L"E:\\贪吃蛇\\贪吃蛇\\pic.jpg", 640, 480);
    Sleep(1000);

    //加载音乐
    mciSendString(L"open 谓风.mp3  alias bk", 0, 0, 0);
    mciSendString(L"play bk repeat", 0, 0, 0);

    setbkcolor(RGB(220, 20, 150));   //用 RGB 宏合成颜色
    cleardevice();    //用背景色清空屏幕,并将当前点移至 (0, 0)

    //初始化一条小蛇
    setfillcolor(CYAN);  //设置填充颜色，蛇头是青色
    solidrectangle(0, 0, SNAKE_SIZE, SNAKE_SIZE);
    snake.szb[0].x = 0;
    snake.szb[0].y = 0;
    snake.n = 1;
    snake.c = Right;

    //初始化食物
    food.flag = true;
}

//蛇的移动函数
void MoveSnake()
{
    //蛇的后一个结点保存前一个结点的坐标，DrawSnake()之后就实现了每个结点坐标的更新
    //同时，更新之前的最后一个结点还保留在，所以更新后会多出一个结点，要把它删去
    for (int i = snake.n; i > 0; i--)
        //这里 i=snake.n 初始化，就会只有一个结点
    {
        snake.szb[i].x = snake.szb[i - 1].x;
        snake.szb[i].y = snake.szb[i - 1].y;
    }
    //对蛇的方向进行判断
    switch (snake.c)
    {
    case up:
        snake.szb[0].y -= SNAKE_SIZE;
        break;
    case down:
        snake.szb[0].y += SNAKE_SIZE;
        break;
    case Left:
        snake.szb[0].x -= SNAKE_SIZE;
        break;
    case Right:
        snake.szb[0].x += SNAKE_SIZE;
        break;
    }
    DrawSnake();  //移动完之后重新画蛇
}

//在画板上画蛇
void DrawSnake()
{
    for (int i = snake.n - 1; i > 0; i--)
    {
        setfillcolor(BLUE);   //这里是画蛇的身体，蓝色
        solidrectangle(snake.szb[i].x, snake.szb[i].y, snake.szb[i].x + SNAKE_SIZE,
                       snake.szb[i].y + SNAKE_SIZE);
    }
    //蛇头颜色不一样，单独拿出来画
    setfillcolor(CYAN);
    solidrectangle(snake.szb[0].x, snake.szb[0].y, snake.szb[0].x + SNAKE_SIZE,
                   snake.szb[0].y + SNAKE_SIZE);
    //隐藏尾巴
    setfillcolor(RGB(220, 20, 150));   //设置当前绘图前景色
    //用和当前背景色相同的绘图前景色来绘制多出的结点，就把它隐藏了
    solidrectangle(snake.szb[snake.n].x, snake.szb[snake.n].y,
                   snake.szb[snake.n].x + SNAKE_SIZE, snake.szb[snake.n].y + SNAKE_SIZE);
    //setcolor(WHITE);  //隐藏最后一个结点之后，别忘了恢复绘图色
}

//接收键盘
void GetKey()
{
    int key;
    key = getch();
    switch (key)
    {
    case up:   //这里应该是在枚举里面转换过了
        if (snake.c != down)
        {
            snake.c = up;
        }
        break;
    case down:
        if (snake.c != up)
        {
            snake.c = down;
        }
        break;
    case Left:
        if (snake.c != Right)
        {
            snake.c = Left;
        }
        break;
    case Right:
        if (snake.c != Left)
        {
            snake.c = Right;
        }
        break;
    //实现游戏暂停功能，按空格键继续游戏
    case space:
        do
        {
            mciSendString(L"pause bk", 0, 0, 0); //暂停音乐
            key = getch();
        } while (key != space);
        mciSendString(L"resume bk", 0, 0, 0);    //再次按下空格键恢复音乐
        break;
    }
}

//画食物
void DrawFood()
{
    setfillcolor(GREEN);  //食物是绿色填充
    solidroundrect(food.fzb.x, food.fzb.y, food.fzb.x + SNAKE_SIZE,
                   food.fzb.y + SNAKE_SIZE, SNAKE_SIZE, SNAKE_SIZE);
    food.flag = true; //新画的食物还没有被吃掉
}

//计算食物坐标
void CoorFood()
{
    srand(unsigned(time(NULL)));
    //这种求余方法是为了使食物位于 10 的整数倍的坐标上
    food.fzb.x = rand() % (640 / SNAKE_SIZE) * SNAKE_SIZE;
    food.fzb.y = rand() % (480 / SNAKE_SIZE) * SNAKE_SIZE;
}

//吃食物函数，返回食物有没有被吃掉
bool EatFood()
{
    if (snake.szb[0].x == food.fzb.x && snake.szb[0].y == food.fzb.y)
    {
        snake.n++;
        //将食物结点删掉
        setfillcolor(RGB(220, 20, 150));
        solidroundrect(food.fzb.x, food.fzb.y, food.fzb.x + SNAKE_SIZE,
                       food.fzb.y + SNAKE_SIZE, SNAKE_SIZE, SNAKE_SIZE);

        food.flag = false; //食物已经被吃掉
    }
    return food.flag;
}

//判断有没有撞到自己或者撞到墙
void Break()
{
    //撞到墙
    if (snake.szb[0].x < 0 || snake.szb[0].x > 630 || snake.szb[0].y < 0 ||
            snake.szb[0].y > 470)
    {
        GameOver();
    }
    //撞到自己
    //因为某个 bug ，这里必须要从 2 开始
    for (int i = 2; i < snake.n; i++)
    {
        if (snake.szb[0].x == snake.szb[i].x && snake.szb[0].y == snake.szb[i].y)
        {
            GameOver();
        }
    }
}

//游戏结束
void GameOver()
{
    cleardevice();

	//先获取当前历史最高分
    ifstream fin("历史最高分.txt");
    int tmp;
    fin >> tmp;

    //输出游戏结束的画面
    setcolor(YELLOW);
    settextstyle(50, 20, L"宋体");
    outtextxy(100, 100, L"亲爱的，游戏结束啦！");
    outtextxy(100, 200, L"您的得分是：");
	//输出数字的话，要先将数字格式化成字符串
    TCHAR s[5];
    _stprintf(s, _T("%d"), snake.n);
    outtextxy(340, 200, s);
    outtextxy(400, 200, L"分！");

    if (snake.n > tmp) //超过了最高分
	{
        outtextxy(100, 300, L"恭喜超过了当前最高分！");
		fstream file("历史最高分.txt", ios::out);  //清空文件内容
        ofstream fout("历史最高分.txt");
        fout << snake.n;
    }
    else   //没有超过最高分
    {
        outtextxy(100, 300, L"历史最高分是：");
        TCHAR t[5];
	    _stprintf(t, _T("%d"), tmp);
		outtextxy(400, 300, t);
		outtextxy(460, 300, L"分！");
    }

    //关闭音乐
    mciSendString(L"close bk", 0, 0, 0);

    //手动关闭游戏窗口
    getchar();
    closegraph();
}

//游戏难度等级，表现为蛇的移动速度越来越快
int Level()
{
    int level;
    if (snake.n < 15)
    {
        level = 5;
    }
    else if (snake.n < 30)
    {
        level = 4;
    }
    else if (snake.n < 40)
    {
        level = 3;
    }
    else if (snake.n < 50)
    {
        level = 2;
    }
    else
    {
        level = 1;
    }
    return level;
}