/*这样就可以取消 Unicode 编码的宏定义，让整个项目以 MBCS 编码编译。
因为用outtextxy时遇到了“2个重载中没有一个可以转化为所有参数类型”问题*/
//#undef UNICODE
//#undef _UNICODE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <graphics.h>
#include <conio.h>
//#include <tchar.h>//添加对TCHAR类型的支持，解决unicode编码格式导致outtextxy无法正常输出的问题



#define MAX_LENGTH 200 // 蛇的最长长度
#define Food_Number 3 // 食物的数量
#define Food_Points 10 // 一个食物的分数
int Game_Mode = 0; //游戏模式
int Best = 0; // 最高分
int speed; // 蛇移动速度，值越小移动越快
int initspeed = 150;

/* 代表着不同颜色"GREEN 0x00aa00", "RED 0x0000aa", "YELLOW 0x55ffff",
"BLUE 0xAA0000", "CYAN	0xAAAA00", "MAGENTA	0xAA00AA" */
int Colors[6] = { 0x00aa00, 0X0000aa, 0x55ffff, 0xaa0000, 0xaaaa00, 0xaa00aa };//用于随机取不同的颜色

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

/* key values of four directions，还有游戏模式 */
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
void GameStart();//游戏开始了
void DrawCover();//画开始菜单的LOGO图形
void Custom();// 自定义开始速度，蛇开始长度，食物数量，是否变速，是否有围栏

int main()
{
    int gameover = 0;// 判断游戏是否结束
    hwnd = initgraph(640, 480);
    setbkcolor(WHITE);//窗口颜色设为白色
    cleardevice();//刷新屏幕

    while (!gameover)
    {
        NewBest();//刷新最高分
        GameStartMenu();//游戏开始菜单
        /* 初始化游戏数据 */
        /* 初始化食物和蛇 */
        initSnake();//初始化蛇
        speed = initspeed;//游戏默认速度
        initFood();//初始化食物
        Game_Mode = 0;//默认为0，不代表任何模式
        int input = 0;//存储用户按键的值
        while (_kbhit())//只有按键时才有反馈
        {
            keyDown();//按键
        }

        if (Game_Mode == 4)
        {
            /* 游戏结束 */
            //setbkmode(TRANSPARENT);// 文字背景为透明
            //settextcolor(LIGHTBLUE);// 文字颜色淡蓝色
            //outtextxy(100, 100, "hello");// 玩家选择了退出游戏
            //Sleep(1000);
            gameover = true;
        }

        else if (Game_Mode == 1 || Game_Mode == 2 || Game_Mode == 3)
        {
            /* 游戏模式，对应模式名称在开始菜单函数里 */
            GameStart();
        }
    }
    GameOverMenu();//退出游戏显示
    getchar();//防止闪屏

    closegraph();//关闭图像
    return 0;
}

void initSnake()
{
    /* 初始化蛇 */
    snake.num = 3;//蛇长
    snake.Position = right;//蛇初始移动方向

    /* 三个节点的位置 */
    snake.snake_xy[2].x = 0;
    snake.snake_xy[2].y = 240;

    snake.snake_xy[1].x = 10;
    snake.snake_xy[1].y = 240;

    snake.snake_xy[0].x = 20;//头节点
    snake.snake_xy[0].y = 240;
}

void drawSnake()
{
    /* 画蛇 */

    setlinecolor(BLACK);    //矩形边框的颜色为黑色
    setfillcolor(Colors[rand() % 7]);    //蛇头的颜色
    /* 画蛇头 */
    fillrectangle(snake.snake_xy[0].x, snake.snake_xy[0].y, snake.snake_xy[0].x + 10, snake.snake_xy[0].y + 10);

    for (int i = 1; i < snake.num; i++)
    {
        setlinecolor(BLACK);    //矩形边框的颜色为黑色
        setfillcolor(LIGHTCYAN);    //蛇的颜色
        /* 画蛇身 */
        fillrectangle(snake.snake_xy[i].x, snake.snake_xy[i].y, snake.snake_xy[i].x + 10, snake.snake_xy[i].y + 10);
    }
}

void moveSnake()
{
    for (int i = snake.num - 1; i > 0; --i)
    {
        /*除了头节点，其余节点均移动到前一个节点的位置*/
        snake.snake_xy[i].x = snake.snake_xy[i - 1].x;
        snake.snake_xy[i].y = snake.snake_xy[i - 1].y;
    }

    /* 头节点的移动 */
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
    /* 初始化食物 */
    food.score = 0;// 初始化为0分
    food.num = Food_Number;// 食物的数量
    srand(time(0));// 用time()函数初始化随机种子，避免循环里随机数结果一样
    for (int i = 0; i < food.num; i++)
    {
        food.food_xy[i].x = (rand() % 64) * 10;// 随机生成一个坐标且坐标为整十的倍数，保证这个坐标在640*480的范围内
        food.food_xy[i].y = (rand() % 48) * 10;
    }
}

void drawFood()
{
    /* 画食物 */
    for (int i = 0; i < food.num; i++)
    {
        setlinecolor(BLACK);    //矩形边框的颜色为黑色
        setfillcolor(Colors[rand() % 7]);// 食物随机颜色
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
            /* 吃食物 */
            food.num--;//吃了一个，食物数量减少1
            food.score += Food_Points;// 吃了一个加分
            food.food_xy[i].x = rand() % 65 * 10;
            food.food_xy[i].y = rand() % 48 * 10;
            food.num++;//吃了一个又随机生成一个食物
            //snake.num += (rand()%5 + 1);//蛇变长
            snake.num += 1;
        }
    }
}

bool snakeDie()
{
    /* 如果撞到蛇身 */
    for (int i = 1; i < snake.num; i++)
    {
        if (snake.snake_xy[0].x == snake.snake_xy[i].x && snake.snake_xy[0].y == snake.snake_xy[i].y)
        {
            setbkmode(TRANSPARENT);// 文字背景为透明
            settextcolor(LIGHTBLUE);// 文字颜色淡蓝色
            outtextxy(240, 200, "吃到蛇身体了！");
            outtextxy(230, 220, "按任意键退出游戏");
            outtextxy(230, 240, "您的最终");
            showScore(294, 240);
            //MessageBox(hwnd, "Game over!");
            return true;
        }
    }

    /* 如果蛇撞到边界，则根据游戏模式判断是另一头穿越过来，还是撞墙 */
    if (snake.snake_xy[0].x > 640 || snake.snake_xy[0].x < 0 || snake.snake_xy[0].y > 480 || snake.snake_xy[0].y < 0)
    {
        snake.snake_xy[0].x = (snake.snake_xy[0].x + 640) % 640;//防止出现在640*480之外的坐标
        snake.snake_xy[0].y = (snake.snake_xy[0].y + 480) % 480;
        //outtextxy(200, 200, "不撞南墙不回头！");
        //MessageBox(hwnd, "Game over!");
        if (Game_Mode == 1 || Game_Mode == 3)
            return false;
        if (Game_Mode == 2)
        {
            setbkmode(TRANSPARENT);// 文字背景为透明
            settextcolor(LIGHTBLUE);// 文字颜色淡蓝色
            outtextxy(220, 160, "不撞南墙不回头！您撞墙了");
            outtextxy(230, 240, "您的最终");
            showScore(294, 240);
            outtextxy(230, 320, "按任意键退出游戏");
            return true;
        }
    }
    return false;
}

void keyDown()
/* 按键识别 */
{
    char userKey = 0;
    userKey = _getch();//读取按键的值
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

void showScore(int x, int y)//x 和 y 是分数显示的坐标
{
    char best[100] = "";
    char points[100] = "";//用字符串存得到的分数
    sprintf_s(points, "%d", food.score);//用sprintf把数字转化为对应的字符串
    sprintf_s(best, "%d", Best);
    setbkmode(TRANSPARENT);// 文字背景为透明
    settextcolor(LIGHTBLUE);// 文字颜色淡蓝色
    outtextxy(x, y, _T("分数："));
    outtextxy(x + 40, y, points);
    outtextxy(x, y + 20, _T("最高："));
    outtextxy(x + 40, y + 20, best);
}

void GameStartMenu()
{
    char best[100] = "";
    setbkmode(TRANSPARENT);// 文字背景为透明
    settextcolor(BLACK);// 文字颜色淡蓝色
    DrawCover();
    outtextxy(250, 100, "贪吃蛇");
    outtextxy(230, 150, "历史最高：");
    sprintf_s(best, "%d", Best);//用sprintf把数字转化为对应的字符串
    outtextxy(310, 150, best);
    outtextxy(230, 220, "1、放浪不羁：无墙模式");
    outtextxy(230, 240, "2、画地为牢：围栏模式");
    outtextxy(230, 260, "3、变化莫测：变速模式");
    outtextxy(230, 280, "4、走为上计：退出游戏");
    outtextxy(200, 320, "按1、2、3、4选择（请切换成英文输入法）");
}

void GameOverMenu()
{
    cleardevice();
    setbkmode(TRANSPARENT);// 文字背景为透明
    settextcolor(LIGHTBLUE);// 文字颜色淡蓝色
    outtextxy(170, 150, "相思相见知何日？此时此夜难为情。——李白《三五七言》");
    outtextxy(230, 220, "请按任意键退出游戏");
}

void NewBest()
{
    if (food.score > Best)//新纪录比原纪录分高
    {
        Best = food.score;//刷新记录
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
    /* 游戏开始了 */
    while (true)
    {
        cleardevice();//刷新屏幕
        showScore(540, 20);//显示分数

        /* 画蛇和食物 */
        drawFood();
        drawSnake();

        while (_kbhit()) // kbhit还是_kbhit是编译器问题
        {
            /*只有按键时才接受键值*/
            keyDown();
        }
        moveSnake();//蛇的自己移动
        Change_Speed();//变速模式改变速度
        eatFood();//判断是否吃到了食物

        Sleep(speed);//停顿一会，不然移动速度太快，改变值可以改变移动速度

        bool flag = snakeDie();//游戏结束

        if (flag == true)//游戏结束了
        {
            while (!_kbhit());// 等待输入任意键结束游戏
            break;//结束了，退出循环
        }

    }
    cleardevice();//刷新屏幕
}

void DrawCover()
{
    setlinecolor(Colors[rand() % 7]);    //矩形边框的颜色为黑色
    setfillcolor(Colors[rand() % 7]);// 随机颜色
    //fillrectangle(food.food_xy[i].x, food.food_xy[i].y, food.food_xy[i].x + 10, food.food_xy[i].y + 10);
    /* 字母S */
    fillrectangle(180, 20, 180 + 30, 20 + 5);//横线
    fillrectangle(180, 40, 180 + 30, 40 + 5);//横线
    fillrectangle(180, 60, 180 + 30, 60 + 5);//横线
    fillrectangle(180, 25, 180 + 5, 25 + 15);//竖线
    fillrectangle(205, 45, 205 + 5, 45 + 15);//竖线

    /* 字母N */
    setlinecolor(Colors[rand() % 7]);    //矩形边框的颜色为黑色
    setfillcolor(Colors[rand() % 7]);// 随机颜色
    fillrectangle(230, 20, 230 + 5, 20 + 45);//竖线
    fillrectangle(260, 20, 260 + 5, 20 + 45);//竖线
    int x = 4, y = 5;
    for (int i = 0; i < 8; i++)
        fillrectangle(230 + x * i, 20 + y * i, 230 + 4 + x * i, 20 + 5 + y * i);

    /* 字母A */
    setlinecolor(Colors[rand() % 7]);    //矩形边框的颜色为黑色
    setfillcolor(Colors[rand() % 7]);// 随机颜色
    x = 2;
    for (int i = 0; i < 9; i++)
    {
        fillrectangle(280 + x * i, 60 - y * i, 280 + 4 + x * i, 60 + 5 - y * i);
        fillrectangle(296 + x * i, 20 + y * i, 296 + 4 + x * i, 20 + 5 + y * i);
    }
    fillrectangle(288, 40, 288 + 20, 40 + 5);//横线

    /* 字母K */
    setlinecolor(Colors[rand() % 7]);    //矩形边框的颜色为黑色
    setfillcolor(Colors[rand() % 7]);// 随机颜色
    fillrectangle(330, 20, 330 + 5, 20 + 45);//竖线
    x = 5;
    for (int i = 0; i < 5; i++)
    {
        fillrectangle(330 + x * i, 40 - y * i, 330 + 4 + x * i, 40 + 5 - y * i);
        fillrectangle(335 + x * i, 40 + y * i, 335 + 4 + x * i, 40 + 5 + y * i);
    }

    /* 字母E */
    setlinecolor(Colors[rand() % 7]);    //矩形边框的颜色为黑色
    setfillcolor(Colors[rand() % 7]);// 随机颜色
    fillrectangle(380, 20, 380 + 30, 20 + 5);//横线
    fillrectangle(380, 40, 380 + 30, 40 + 5);//横线
    fillrectangle(380, 60, 380 + 30, 60 + 5);//横线
    fillrectangle(380, 20, 380 + 5, 20 + 45);//竖线
}

void Custom()
{
    /* 自定义开始速度，蛇开始长度，食物数量，是否变速，是否有围栏 */

}