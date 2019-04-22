/**************************************************************************************************

									�ļ����ƣ�̰����.cpp
									���뻷����VS2012  and  EasyX
									������أ������
									��д���ڣ�2019-04-05
									����޸ģ�2019-04-06

****************************************************************************************************/

#include "stdafx.h"
#include<stdio.h>
#include<graphics.h>     //ͼ�ο�ͷ�ļ�
#include<mmsystem.h>     //��ý���豸�ӿ�
#include<conio.h>        //Console Input/Output
#include<time.h>
#include<fstream>        //�ļ����������ͷ�ļ�
#include<istream>
#include<iostream>
#pragma comment(lib,"winmm.lib")  //��ӿ��ļ�
#define SNAKE_MAX_LENGTH  100     //�ߵ���󳤶�
#define SNAKE_SIZE  10            //�ߵĴ�С����ȣ�
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


//���������ö��
enum DIRECT {
    up = 72,
    down = 80,
    Left = 75,   //ʹ�� std ֮��Ҫ��left ,right ������
    Right = 77,
    space = 32
};

//����ṹ��
struct COOR {
    int x;
    int y;
} coor;

//�ߵĽṹ��
struct SNAKE {
    COOR szb[SNAKE_MAX_LENGTH];  //ÿһ���ߵĽ�������
    int n;                       //�ߵĵ�ǰ����
    DIRECT c;                    //�ߵķ���
} snake;

//ʳ��ṹ��
struct FOOD {
    COOR fzb;   //ʳ������
    bool flag;  //true ��ʾʳ��û�б��Ե��� false ��ʾʳ�ﱻ�Ե�
} food;

int IntervalTime;      //��ÿ�����ƶ�֮��ļ������ʾ�Ѷȵȼ��õ�

int main()
{
    InitSnake();
    while (1)
    {
        while (!kbhit())        //���û�н��ռ���
        {
            if (!EatFood())     //���ʳ�ﱻ�Ե����ͻ����µ�ʳ����
            {
                CoorFood();
                DrawFood();
            }
            Break();            //�ȼ�����ƶ�
            MoveSnake();
            IntervalTime = Level() * 60;
            Sleep(IntervalTime);
        }
        GetKey();
    }
    getchar();  //����
    return 0;
}

//��ʼ������
void InitSnake()
{
    initgraph(640, 480, NOMINIMIZE);  //��ʼ����ͼ����,��������

    //����ͼƬ
    loadimage(NULL, L"E:\\̰����\\̰����\\pic.jpg", 640, 480);
    Sleep(1000);

    //��������
    mciSendString(L"open ν��.mp3  alias bk", 0, 0, 0);
    mciSendString(L"play bk repeat", 0, 0, 0);

    setbkcolor(RGB(220, 20, 150));   //�� RGB ��ϳ���ɫ
    cleardevice();    //�ñ���ɫ�����Ļ,������ǰ������ (0, 0)

    //��ʼ��һ��С��
    setfillcolor(CYAN);  //���������ɫ����ͷ����ɫ
    solidrectangle(0, 0, SNAKE_SIZE, SNAKE_SIZE);
    snake.szb[0].x = 0;
    snake.szb[0].y = 0;
    snake.n = 1;
    snake.c = Right;

    //��ʼ��ʳ��
    food.flag = true;
}

//�ߵ��ƶ�����
void MoveSnake()
{
    //�ߵĺ�һ����㱣��ǰһ���������꣬DrawSnake()֮���ʵ����ÿ���������ĸ���
    //ͬʱ������֮ǰ�����һ����㻹�����ڣ����Ը��º����һ����㣬Ҫ����ɾȥ
    for (int i = snake.n; i > 0; i--)
        //���� i=snake.n ��ʼ�����ͻ�ֻ��һ�����
    {
        snake.szb[i].x = snake.szb[i - 1].x;
        snake.szb[i].y = snake.szb[i - 1].y;
    }
    //���ߵķ�������ж�
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
    DrawSnake();  //�ƶ���֮�����»���
}

//�ڻ����ϻ���
void DrawSnake()
{
    for (int i = snake.n - 1; i > 0; i--)
    {
        setfillcolor(BLUE);   //�����ǻ��ߵ����壬��ɫ
        solidrectangle(snake.szb[i].x, snake.szb[i].y, snake.szb[i].x + SNAKE_SIZE,
                       snake.szb[i].y + SNAKE_SIZE);
    }
    //��ͷ��ɫ��һ���������ó�����
    setfillcolor(CYAN);
    solidrectangle(snake.szb[0].x, snake.szb[0].y, snake.szb[0].x + SNAKE_SIZE,
                   snake.szb[0].y + SNAKE_SIZE);
    //����β��
    setfillcolor(RGB(220, 20, 150));   //���õ�ǰ��ͼǰ��ɫ
    //�ú͵�ǰ����ɫ��ͬ�Ļ�ͼǰ��ɫ�����ƶ���Ľ�㣬�Ͱ���������
    solidrectangle(snake.szb[snake.n].x, snake.szb[snake.n].y,
                   snake.szb[snake.n].x + SNAKE_SIZE, snake.szb[snake.n].y + SNAKE_SIZE);
    //setcolor(WHITE);  //�������һ�����֮�󣬱����˻ָ���ͼɫ
}

//���ռ���
void GetKey()
{
    int key;
    key = getch();
    switch (key)
    {
    case up:   //����Ӧ������ö������ת������
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
    //ʵ����Ϸ��ͣ���ܣ����ո��������Ϸ
    case space:
        do
        {
            mciSendString(L"pause bk", 0, 0, 0); //��ͣ����
            key = getch();
        } while (key != space);
        mciSendString(L"resume bk", 0, 0, 0);    //�ٴΰ��¿ո���ָ�����
        break;
    }
}

//��ʳ��
void DrawFood()
{
    setfillcolor(GREEN);  //ʳ������ɫ���
    solidroundrect(food.fzb.x, food.fzb.y, food.fzb.x + SNAKE_SIZE,
                   food.fzb.y + SNAKE_SIZE, SNAKE_SIZE, SNAKE_SIZE);
    food.flag = true; //�»���ʳ�ﻹû�б��Ե�
}

//����ʳ������
void CoorFood()
{
    srand(unsigned(time(NULL)));
    //�������෽����Ϊ��ʹʳ��λ�� 10 ����������������
    food.fzb.x = rand() % (640 / SNAKE_SIZE) * SNAKE_SIZE;
    food.fzb.y = rand() % (480 / SNAKE_SIZE) * SNAKE_SIZE;
}

//��ʳ�ﺯ��������ʳ����û�б��Ե�
bool EatFood()
{
    if (snake.szb[0].x == food.fzb.x && snake.szb[0].y == food.fzb.y)
    {
        snake.n++;
        //��ʳ����ɾ��
        setfillcolor(RGB(220, 20, 150));
        solidroundrect(food.fzb.x, food.fzb.y, food.fzb.x + SNAKE_SIZE,
                       food.fzb.y + SNAKE_SIZE, SNAKE_SIZE, SNAKE_SIZE);

        food.flag = false; //ʳ���Ѿ����Ե�
    }
    return food.flag;
}

//�ж���û��ײ���Լ�����ײ��ǽ
void Break()
{
    //ײ��ǽ
    if (snake.szb[0].x < 0 || snake.szb[0].x > 630 || snake.szb[0].y < 0 ||
            snake.szb[0].y > 470)
    {
        GameOver();
    }
    //ײ���Լ�
    //��Ϊĳ�� bug ���������Ҫ�� 2 ��ʼ
    for (int i = 2; i < snake.n; i++)
    {
        if (snake.szb[0].x == snake.szb[i].x && snake.szb[0].y == snake.szb[i].y)
        {
            GameOver();
        }
    }
}

//��Ϸ����
void GameOver()
{
    cleardevice();

	//�Ȼ�ȡ��ǰ��ʷ��߷�
    ifstream fin("��ʷ��߷�.txt");
    int tmp;
    fin >> tmp;

    //�����Ϸ�����Ļ���
    setcolor(YELLOW);
    settextstyle(50, 20, L"����");
    outtextxy(100, 100, L"�װ��ģ���Ϸ��������");
    outtextxy(100, 200, L"���ĵ÷��ǣ�");
	//������ֵĻ���Ҫ�Ƚ����ָ�ʽ�����ַ���
    TCHAR s[5];
    _stprintf(s, _T("%d"), snake.n);
    outtextxy(340, 200, s);
    outtextxy(400, 200, L"�֣�");

    if (snake.n > tmp) //��������߷�
	{
        outtextxy(100, 300, L"��ϲ�����˵�ǰ��߷֣�");
		fstream file("��ʷ��߷�.txt", ios::out);  //����ļ�����
        ofstream fout("��ʷ��߷�.txt");
        fout << snake.n;
    }
    else   //û�г�����߷�
    {
        outtextxy(100, 300, L"��ʷ��߷��ǣ�");
        TCHAR t[5];
	    _stprintf(t, _T("%d"), tmp);
		outtextxy(400, 300, t);
		outtextxy(460, 300, L"�֣�");
    }

    //�ر�����
    mciSendString(L"close bk", 0, 0, 0);

    //�ֶ��ر���Ϸ����
    getchar();
    closegraph();
}

//��Ϸ�Ѷȵȼ�������Ϊ�ߵ��ƶ��ٶ�Խ��Խ��
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