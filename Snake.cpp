/*�����Ϳ���ȡ�� Unicode ����ĺ궨�壬��������Ŀ�� MBCS ������롣
��Ϊ��outtextxyʱ�����ˡ�2��������û��һ������ת��Ϊ���в������͡�����*/
//#undef UNICODE
//#undef _UNICODE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <graphics.h>
#include <conio.h>
//#include <tchar.h>//��Ӷ�TCHAR���͵�֧�֣����unicode�����ʽ����outtextxy�޷��������������



#define MAX_LENGTH 200 // �ߵ������
#define Food_Number 3 // ʳ�������
#define Food_Points 10 // һ��ʳ��ķ���
int Game_Mode = 0; //��Ϸģʽ
int Best = 0; // ��߷�
int speed; // ���ƶ��ٶȣ�ֵԽС�ƶ�Խ��
int initspeed = 150;

/* �����Ų�ͬ��ɫ"GREEN 0x00aa00", "RED 0x0000aa", "YELLOW 0x55ffff",
"BLUE 0xAA0000", "CYAN	0xAAAA00", "MAGENTA	0xAA00AA" */
int Colors[6] = { 0x00aa00, 0X0000aa, 0x55ffff, 0xaa0000, 0xaaaa00, 0xaa00aa };//�������ȡ��ͬ����ɫ

typedef struct Coordinate
{
    /* Describe a coordiate */
    int x, y;
}coordinate;

struct Snake
{
    /* The structure of a snake */
    int num;// number of snakes
    coordinate snake_xy[MAX_LENGTH];// Maximum length of a snake
    char Position;// Direction of a snake
}snake;

struct Food
{
    /* Structure of food */
    coordinate food_xy[Food_Number];
    int score;//points of eating
    int num;// number of food
}food;

HWND hwnd = NULL;//Represents the main window

/* key values of four directions��������Ϸģʽ */
enum MovPosition {
    right = 72, left = 75, up = 80, down = 77,
    one = 49, two = 50, three = 51, four = 52
};

void initSnake();
void drawSnake();
void moveSnake();
void keyDown();
void initFood();
void drawFood();
void eatFood();
bool snakeDie();
void showScore(int x, int y);
void GameStartMenu();
void GameOverMenu();
void NewBest();
void Change_Speed();
void GameStart();//��Ϸ��ʼ��
void DrawCover();//����ʼ�˵���LOGOͼ��
void Custom();// �Զ��忪ʼ�ٶȣ��߿�ʼ���ȣ�ʳ���������Ƿ���٣��Ƿ���Χ��

int main()
{
    int gameover = 0;// �ж���Ϸ�Ƿ����
    hwnd = initgraph(640, 480);
    setbkcolor(WHITE);//������ɫ��Ϊ��ɫ
    cleardevice();//ˢ����Ļ

    while (!gameover)
    {
        NewBest();//ˢ����߷�
        GameStartMenu();//��Ϸ��ʼ�˵�
        /* ��ʼ����Ϸ���� */
        /* ��ʼ��ʳ����� */
        initSnake();//��ʼ����
        speed = initspeed;//��ϷĬ���ٶ�
        initFood();//��ʼ��ʳ��
        Game_Mode = 0;//Ĭ��Ϊ0���������κ�ģʽ
        int input = 0;//�洢�û�������ֵ
        while (_kbhit())//ֻ�а���ʱ���з���
        {
            keyDown();//����
        }

        if (Game_Mode == 4)
        {
            /* ��Ϸ���� */
            //setbkmode(TRANSPARENT);// ���ֱ���Ϊ͸��
            //settextcolor(LIGHTBLUE);// ������ɫ����ɫ
            //outtextxy(100, 100, "hello");// ���ѡ�����˳���Ϸ
            //Sleep(1000);
            gameover = true;
        }

        else if (Game_Mode == 1 || Game_Mode == 2 || Game_Mode == 3)
        {
            /* ��Ϸģʽ����Ӧģʽ�����ڿ�ʼ�˵������� */
            GameStart();
        }
    }
    GameOverMenu();//�˳���Ϸ��ʾ
    getchar();//��ֹ����

    closegraph();//�ر�ͼ��
    return 0;
}

void initSnake()
{
    /* ��ʼ���� */
    snake.num = 3;//�߳�
    snake.Position = right;//�߳�ʼ�ƶ�����

    /* �����ڵ��λ�� */
    snake.snake_xy[2].x = 0;
    snake.snake_xy[2].y = 240;

    snake.snake_xy[1].x = 10;
    snake.snake_xy[1].y = 240;

    snake.snake_xy[0].x = 20;//ͷ�ڵ�
    snake.snake_xy[0].y = 240;
}

void drawSnake()
{
    /* ���� */

    setlinecolor(BLACK);    //���α߿����ɫΪ��ɫ
    setfillcolor(Colors[rand() % 7]);    //��ͷ����ɫ
    /* ����ͷ */
    fillrectangle(snake.snake_xy[0].x, snake.snake_xy[0].y, snake.snake_xy[0].x + 10, snake.snake_xy[0].y + 10);

    for (int i = 1; i < snake.num; i++)
    {
        setlinecolor(BLACK);    //���α߿����ɫΪ��ɫ
        setfillcolor(LIGHTCYAN);    //�ߵ���ɫ
        /* ������ */
        fillrectangle(snake.snake_xy[i].x, snake.snake_xy[i].y, snake.snake_xy[i].x + 10, snake.snake_xy[i].y + 10);
    }
}

void moveSnake()
{
    for (int i = snake.num - 1; i > 0; --i)
    {
        /*����ͷ�ڵ㣬����ڵ���ƶ���ǰһ���ڵ��λ��*/
        snake.snake_xy[i].x = snake.snake_xy[i - 1].x;
        snake.snake_xy[i].y = snake.snake_xy[i - 1].y;
    }

    /* ͷ�ڵ���ƶ� */
    switch (snake.Position)
    {
    case right:
        snake.snake_xy[0].x += 10;
        break;
    case left:
        snake.snake_xy[0].x -= 10;
        break;
    case down:
        snake.snake_xy[0].y += 10;
        break;
    case up:
        snake.snake_xy[0].y -= 10;
        break;
    default:
        break;
    }
}

void initFood()
{
    /* ��ʼ��ʳ�� */
    food.score = 0;// ��ʼ��Ϊ0��
    food.num = Food_Number;// ʳ�������
    srand(time(0));// ��time()������ʼ��������ӣ�����ѭ������������һ��
    for (int i = 0; i < food.num; i++)
    {
        food.food_xy[i].x = (rand() % 64) * 10;// �������һ������������Ϊ��ʮ�ı�������֤���������640*480�ķ�Χ��
        food.food_xy[i].y = (rand() % 48) * 10;
    }
}

void drawFood()
{
    /* ��ʳ�� */
    for (int i = 0; i < food.num; i++)
    {
        setlinecolor(BLACK);    //���α߿����ɫΪ��ɫ
        setfillcolor(Colors[rand() % 7]);// ʳ�������ɫ
        fillrectangle(food.food_xy[i].x, food.food_xy[i].y, food.food_xy[i].x + 10, food.food_xy[i].y + 10);
    }
}

void eatFood()
{
    int temp = food.num;
    for (int i = 0; i < temp; i++)
    {
        if (snake.snake_xy[0].x == food.food_xy[i].x && snake.snake_xy[0].y == food.food_xy[i].y)
        {
            /* ��ʳ�� */
            food.num--;//����һ����ʳ����������1
            food.score += Food_Points;// ����һ���ӷ�
            food.food_xy[i].x = rand() % 65 * 10;
            food.food_xy[i].y = rand() % 48 * 10;
            food.num++;//����һ�����������һ��ʳ��
            //snake.num += (rand()%5 + 1);//�߱䳤
            snake.num += 1;
        }
    }
}

bool snakeDie()
{
    /* ���ײ������ */
    for (int i = 1; i < snake.num; i++)
    {
        if (snake.snake_xy[0].x == snake.snake_xy[i].x && snake.snake_xy[0].y == snake.snake_xy[i].y)
        {
            setbkmode(TRANSPARENT);// ���ֱ���Ϊ͸��
            settextcolor(LIGHTBLUE);// ������ɫ����ɫ
            outtextxy(240, 200, "�Ե��������ˣ�");
            outtextxy(230, 220, "��������˳���Ϸ");
            outtextxy(230, 240, "��������");
            showScore(294, 240);
            //MessageBox(hwnd, "Game over!");
            return true;
        }
    }

    /* �����ײ���߽磬�������Ϸģʽ�ж�����һͷ��Խ����������ײǽ */
    if (snake.snake_xy[0].x > 640 || snake.snake_xy[0].x < 0 || snake.snake_xy[0].y > 480 || snake.snake_xy[0].y < 0)
    {
        snake.snake_xy[0].x = (snake.snake_xy[0].x + 640) % 640;//��ֹ������640*480֮�������
        snake.snake_xy[0].y = (snake.snake_xy[0].y + 480) % 480;
        //outtextxy(200, 200, "��ײ��ǽ����ͷ��");
        //MessageBox(hwnd, "Game over!");
        if (Game_Mode == 1 || Game_Mode == 3)
            return false;
        if (Game_Mode == 2)
        {
            setbkmode(TRANSPARENT);// ���ֱ���Ϊ͸��
            settextcolor(LIGHTBLUE);// ������ɫ����ɫ
            outtextxy(220, 160, "��ײ��ǽ����ͷ����ײǽ��");
            outtextxy(230, 240, "��������");
            showScore(294, 240);
            outtextxy(230, 320, "��������˳���Ϸ");
            return true;
        }
    }
    return false;
}

void keyDown()
/* ����ʶ�� */
{
    char userKey = 0;
    userKey = _getch();//��ȡ������ֵ
    switch (userKey)
    {
    case right:
        if (snake.Position != down)
            snake.Position = up;
        break;
    case left:
        if (snake.Position != right)
            snake.Position = left;
        break;
    case up:
        if (snake.Position != up)
            snake.Position = down;
        break;
    case down:
        if (snake.Position != left)
            snake.Position = right;
        break;
    case one:
        Game_Mode = 1;
        break;
    case two:
        Game_Mode = 2;
        break;
    case three:
        Game_Mode = 3;
        break;
    case four:
        Game_Mode = 4;
        break;
    default:
        break;
    }
}

void showScore(int x, int y)//x �� y �Ƿ�����ʾ������
{
    char best[100] = "";
    char points[100] = "";//���ַ�����õ��ķ���
    sprintf_s(points, "%d", food.score);//��sprintf������ת��Ϊ��Ӧ���ַ���
    sprintf_s(best, "%d", Best);
    setbkmode(TRANSPARENT);// ���ֱ���Ϊ͸��
    settextcolor(LIGHTBLUE);// ������ɫ����ɫ
    outtextxy(x, y, _T("������"));
    outtextxy(x + 40, y, points);
    outtextxy(x, y + 20, _T("��ߣ�"));
    outtextxy(x + 40, y + 20, best);
}

void GameStartMenu()
{
    char best[100] = "";
    setbkmode(TRANSPARENT);// ���ֱ���Ϊ͸��
    settextcolor(BLACK);// ������ɫ����ɫ
    DrawCover();
    outtextxy(250, 100, "̰����");
    outtextxy(230, 150, "��ʷ��ߣ�");
    sprintf_s(best, "%d", Best);//��sprintf������ת��Ϊ��Ӧ���ַ���
    outtextxy(310, 150, best);
    outtextxy(230, 220, "1�����˲����ǽģʽ");
    outtextxy(230, 240, "2������Ϊ�Σ�Χ��ģʽ");
    outtextxy(230, 260, "3���仯Ī�⣺����ģʽ");
    outtextxy(230, 280, "4����Ϊ�ϼƣ��˳���Ϸ");
    outtextxy(200, 320, "��1��2��3��4ѡ�����л���Ӣ�����뷨��");
}

void GameOverMenu()
{
    cleardevice();
    setbkmode(TRANSPARENT);// ���ֱ���Ϊ͸��
    settextcolor(LIGHTBLUE);// ������ɫ����ɫ
    outtextxy(170, 150, "��˼���֪���գ���ʱ��ҹ��Ϊ�顣������ס��������ԡ�");
    outtextxy(230, 220, "�밴������˳���Ϸ");
}

void NewBest()
{
    if (food.score > Best)//�¼�¼��ԭ��¼�ָ�
    {
        Best = food.score;//ˢ�¼�¼
    }
}

void Change_Speed()
{
    if (Game_Mode == 3 && speed >= 10)
    {
        speed = initspeed - snake.num * 2;
    }
}

void GameStart()
{
    /* ��Ϸ��ʼ�� */
    while (true)
    {
        cleardevice();//ˢ����Ļ
        showScore(540, 20);//��ʾ����

        /* ���ߺ�ʳ�� */
        drawFood();
        drawSnake();

        while (_kbhit()) // kbhit����_kbhit�Ǳ���������
        {
            /*ֻ�а���ʱ�Ž��ܼ�ֵ*/
            keyDown();
        }
        moveSnake();//�ߵ��Լ��ƶ�
        Change_Speed();//����ģʽ�ı��ٶ�
        eatFood();//�ж��Ƿ�Ե���ʳ��

        Sleep(speed);//ͣ��һ�ᣬ��Ȼ�ƶ��ٶ�̫�죬�ı�ֵ���Ըı��ƶ��ٶ�

        bool flag = snakeDie();//��Ϸ����

        if (flag == true)//��Ϸ������
        {
            while (!_kbhit());// �ȴ����������������Ϸ
            break;//�����ˣ��˳�ѭ��
        }

    }
    cleardevice();//ˢ����Ļ
}

void DrawCover()
{
    setlinecolor(Colors[rand() % 7]);    //���α߿����ɫΪ��ɫ
    setfillcolor(Colors[rand() % 7]);// �����ɫ
    //fillrectangle(food.food_xy[i].x, food.food_xy[i].y, food.food_xy[i].x + 10, food.food_xy[i].y + 10);
    /* ��ĸS */
    fillrectangle(180, 20, 180 + 30, 20 + 5);//����
    fillrectangle(180, 40, 180 + 30, 40 + 5);//����
    fillrectangle(180, 60, 180 + 30, 60 + 5);//����
    fillrectangle(180, 25, 180 + 5, 25 + 15);//����
    fillrectangle(205, 45, 205 + 5, 45 + 15);//����

    /* ��ĸN */
    setlinecolor(Colors[rand() % 7]);    //���α߿����ɫΪ��ɫ
    setfillcolor(Colors[rand() % 7]);// �����ɫ
    fillrectangle(230, 20, 230 + 5, 20 + 45);//����
    fillrectangle(260, 20, 260 + 5, 20 + 45);//����
    int x = 4, y = 5;
    for (int i = 0; i < 8; i++)
        fillrectangle(230 + x * i, 20 + y * i, 230 + 4 + x * i, 20 + 5 + y * i);

    /* ��ĸA */
    setlinecolor(Colors[rand() % 7]);    //���α߿����ɫΪ��ɫ
    setfillcolor(Colors[rand() % 7]);// �����ɫ
    x = 2;
    for (int i = 0; i < 9; i++)
    {
        fillrectangle(280 + x * i, 60 - y * i, 280 + 4 + x * i, 60 + 5 - y * i);
        fillrectangle(296 + x * i, 20 + y * i, 296 + 4 + x * i, 20 + 5 + y * i);
    }
    fillrectangle(288, 40, 288 + 20, 40 + 5);//����

    /* ��ĸK */
    setlinecolor(Colors[rand() % 7]);    //���α߿����ɫΪ��ɫ
    setfillcolor(Colors[rand() % 7]);// �����ɫ
    fillrectangle(330, 20, 330 + 5, 20 + 45);//����
    x = 5;
    for (int i = 0; i < 5; i++)
    {
        fillrectangle(330 + x * i, 40 - y * i, 330 + 4 + x * i, 40 + 5 - y * i);
        fillrectangle(335 + x * i, 40 + y * i, 335 + 4 + x * i, 40 + 5 + y * i);
    }

    /* ��ĸE */
    setlinecolor(Colors[rand() % 7]);    //���α߿����ɫΪ��ɫ
    setfillcolor(Colors[rand() % 7]);// �����ɫ
    fillrectangle(380, 20, 380 + 30, 20 + 5);//����
    fillrectangle(380, 40, 380 + 30, 40 + 5);//����
    fillrectangle(380, 60, 380 + 30, 60 + 5);//����
    fillrectangle(380, 20, 380 + 5, 20 + 45);//����
}

void Custom()
{
    /* �Զ��忪ʼ�ٶȣ��߿�ʼ���ȣ�ʳ���������Ƿ���٣��Ƿ���Χ�� */

}