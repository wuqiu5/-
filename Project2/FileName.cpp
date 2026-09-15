#pragma comment(lib, "EasyXa.lib")
#define _CRT_SECURE_NO_WARNINGS		//去掉scanf_s 问题
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <graphics.h>//图形插件
#include <conio.h>//getch()头文件
#include <Windows.h>
#include <wingdi.h>
#include <mmsystem.h>// 多媒体系统库
#include <thread>//用于创建和管理线程

int count = 100; // 经验值满100升一级
int 升级经验[3] = { 400, 800, 1200 };
int currentMap[13][13][7]; // 当前地图层级的数组
int currentLayer = 0; // 当前地图层级
int savedMap[7][13][13];//保存地图
int stairsPositions[7][13][13];//楼梯的位置

// 地图设计
int map[7][13][13] =
{
    {
        {0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 13, 1, 15, 16, 15, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 6, 1, 19, 3, 1, 0, 7, 5, 6, 0, 1, 0},
        {0, 5, 18, 5, 0, 1, 0, 6, 5, 6, 0, 1, 0},
        {0, 0, 3, 0, 0, 1, 0, 0, 0, 20, 0, 1, 0},
        {0, 5, 19, 1, 0, 1, 3, 17, 15, 16, 0, 1, 0},
        {0, 7, 1, 4, 0, 1, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 3, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 21, 1, 0, 0, 3, 0, 0, 0, 3, 0, 0},
        {0, 6, 8, 9, 0, 5, 1, 5, 0, 5, 18, 4, 0},
        {0, 6, 10, 9, 0, 1, 12, 1, 0, 8, 7, 8, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,7,20,5,0,1,25,1,0,1,20,1,0},
        {0,20,4,1,0,1,1,1,0,1,0,1,0},
        {0,5,17,1,0,0,11,0,0,1,0,1,0},
        {0,0,3,0,0,1,19,1,0,1,0,1,0},
        {0,1,1,1,0,0,0,18,0,5,0,22,0},
        {0,15,0,1,1,17,1,1,0,5,0,22,0},
        {0,17,0,1,15,16,15,1,0,16,0,6,0},
        {0,15,0,0,0,0,0,1,1,1,0,1,0},
        {0,1,1,1,1,1,0,0,3,0,0,1,0},
        {0,0,0,0,0,20,0,16,1,16,0,1},
        {0,1,1,1,1,1,0,1,22,1,0,1,0},
        {0,14,12,0,0,0,0,8,7,9,0,13,0}
    },
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 31, 1, 1, 1, 0, 1, 30, 1, 0},
        {0, 1, 0, 0, 0, 0, 0, 3, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 1, 30, 1, 7, 0, 1, 0, 1, 0},
        {0, 7, 0, 2, 0, 0, 0, 1, 0, 8, 0, 1, 0},
        {0, 34, 0, 1, 1, 30, 0, 31, 0, 7, 0, 1, 0},
        {0, 21, 0, 1, 34, 27, 0, 31, 0, 8, 0, 9, 0},
        {0, 21, 0, 30, 25, 10, 0, 21, 0, 1, 0, 9, 0},
        {0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 9, 0},
        {0, 1, 19, 1, 0, 1, 1, 1, 18, 1, 0, 1, 0},
        {0, 0, 0, 1, 0, 7, 0, 0, 0, 0, 0, 1, 0},
        {0, 14, 12, 1, 2, 1, 13, 0, 26, 32, 3, 1, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 23, 5, 1, 0, 0, 0, 1, 1, 1, 0, 6, 0},
        {0, 5, 1, 32, 3, 1, 1, 1, 0, 1, 3, 22, 0},
        {0, 0, 3, 0, 0, 1, 0, 0, 0, 1, 0, 8, 0},
        {0, 1, 1, 1, 0, 1, 0, 12, 1, 1, 0, 9, 0},
        {0, 1, 1, 1, 2, 1, 0, 14, 0, 1, 0, 9, 0},
        {0, 0, 2, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
        {0, 8, 34, 9, 0, 1, 0, 13, 0, 1, 0, 8, 0},
        {0, 0, 3, 0, 0, 1, 1, 1, 3, 1, 0, 8, 0},
        {0, 34, 6, 34, 0, 0, 2, 0, 0, 1, 0, 9, 0},
        {0, 4, 30, 6, 0, 31, 21, 31, 0, 1, 3, 31, 0},
        {0, 27, 5, 30, 3, 7, 8, 7, 0, 1, 0, 7, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 5, 0, 6, 0, 7, 33, 1, 1, 34, 5, 4, 0},
        {0, 1, 0, 8, 0, 30, 1, 1, 1, 1, 30, 5, 0},
        {0, 30, 0, 1, 0, 34, 1, 0, 0, 3, 0, 0, 0},
        {0, 1, 3, 34, 0, 25, 32, 0, 1, 34, 21, 8, 0},
        {0, 30, 0, 1, 0, 0, 0, 0, 1, 1, 1, 31, 0},
        {0, 8, 0, 1, 1, 1, 34, 1, 1, 1, 1, 1, 0},
        {0, 9, 0, 0, 30, 0, 0, 0, 0, 1, 1, 1, 0},
        {0, 1, 10, 0, 30, 0, 7, 4, 1, 31, 33, 1, 0},
        {0, 0, 0, 0, 31, 0, 3, 0, 2, 0, 3, 0, 0},
        {0, 1, 1, 0, 1, 0, 21, 0, 9, 3, 1, 1, 0},
        {0, 14, 12, 21, 1, 1, 1, 0, 8, 0, 1, 13, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    {
        {29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29},
        {29, 33, 1, 6, 6, 1, 14, 12, 1, 7, 7, 33, 29},
        {29, 1, 29, 29, 29, 29, 29, 29, 29, 29, 29, 1, 29},
        {29, 1, 29, 29, 29, 29, 29, 29, 29, 29, 29, 1, 29},
        {29, 1, 29, 29, 29, 0, 0, 0, 29, 29, 29, 1, 29},
        {29, 1, 29, 29, 0, 0, 28, 0, 0, 29, 29, 1, 29},
        {29, 1, 29, 29, 0, 0, 1, 0, 0, 29, 29, 1, 29},
        {29, 1, 29, 29, 0, 0, 11, 0, 0, 29, 29, 1, 29},
        {29, 1, 29, 29, 29, 0, 35, 0, 29, 29, 29, 1, 29},
        {29, 1, 29, 29, 29, 29, 33, 29, 29, 29, 29, 1, 29},
        {29, 2, 29, 29, 29, 29, 33, 29, 29, 29, 29, 2, 29},
        {29, 23, 1, 1, 1, 1, 1, 1, 1, 1, 1, 24, 29},
        {29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29}
    }
};

// 角色信息
struct gameRole {
    char 角色名[20];
    int 血量;
    int 蓝量;
    int 防御;
    int 攻击;
    int 蓝钥匙;
    int 黄钥匙;
    int 等级;
    int 经验;
    int lastX; // 记录上一次下楼时的X位置
    int lastY; // 记录上一次下楼时的Y位置
} role;

// 怪物信息
enum MonsterType {
    Monster_小蝙蝠,
    Monster_初级法师,
    Monster_绿毛球,
    Monster_红毛球,
    Monster_骷髅人,
    Monster_黑毛球,
    Monster_骷髅士兵,
    Monster_兽面人,
    Monster_初级士兵,
    Monster_红蝙蝠,
    Monster_白衣武士,
    Monster_金卫士,
    Monster_金队长,
    Monster_大魔王
};

struct monster {
    int 血量;
    int 攻击;
    int 防御;
    int 经验;
    int 伤害值;
    MonsterType 类型;  // 添加一个类型字段
    int calculateDamage() const {
        return 攻击 - role.防御;
    }
};

struct monster 绿毛球 = { 50, 18 , 1 ,2, Monster_绿毛球 };
struct monster 红毛球 = { 70, 20 , 1, 4, Monster_红毛球 };
struct monster 小蝙蝠 = { 100, 20, 5, 6, Monster_小蝙蝠 };
struct monster 骷髅人 = { 120,30,5,10, Monster_骷髅人 };
struct monster 黑毛球 = { 150,40,15,15,Monster_黑毛球 };
struct monster 初级法师 = { 180, 50, 20, 17, Monster_初级法师 };
struct monster 骷髅士兵 = { 250,60,30,20,Monster_骷髅士兵 };
struct monster 兽面人 = { 300,120,50,25,Monster_兽面人 };
struct monster 初级士兵 = { 450,150,80,30,Monster_初级士兵 };
struct monster 红蝙蝠 = { 550,160,90,35,Monster_红蝙蝠 };
struct monster 白衣武士 = { 1300,300,150,50,Monster_白衣武士 };
struct monster 金卫士 = { 850,350,200,55,Monster_金卫士 };
struct monster 金队长 = { 2000,500,200,100,Monster_金队长 };
struct monster 大魔王 = { 800,500,350,0,Monster_大魔王 };

IMAGE img[40], roleInfo;//图片
HWND hwnd;		//用来显示战斗时候一个消息框

// 初始化用户信息
void inputRoleName() {
    strcpy(role.角色名, "勇士");
    role.攻击 = 10;
    role.防御 = 10;
    role.等级 = 0;
    role.黄钥匙 = 0;
    role.蓝钥匙 = 0;
    role.蓝量 = 250;
    role.血量 = 1000;
    role.经验 = 0;
    role.lastX = 0; // 初始化下楼位置
    role.lastY = 0; // 初始化下楼位置
}

// 加载资源:批量加载
void loadResource() {
    loadimage(&roleInfo, "info.jpg");
    for (int i = 0; i < 40; i++) {
        char fileName[20] = "";
        sprintf(fileName, "%d.jpg", i);
        loadimage(img + i, fileName, 60, 60);
    }
}

// 把整形转换为字符串 ：类型转换
char str[20] = "";
char* intToWcahr(int xNum) {
    sprintf(str, "%d", xNum);
    return str;
}

void PlaySoundEffect(const char* soundFileName) {
    // 播放.wav文件，并设置为循环播放
    PlaySound(soundFileName, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

// 欢迎界面
void drawWelcomeScreen() {
    setbkcolor(BLACK);//背景
    cleardevice();
    settextcolor(WHITE);//字体颜色
    settextstyle(48, 0, _T("Arial"));//字体大小

    int width = textwidth(_T("魔塔"));
    int x = (getmaxx() - width) / 2;//文字居中计算
    int y = (getmaxy() / 2) - 50;
    outtextxy(x, y, _T("魔塔"));

    settextstyle(24, 0, _T("Arial"));
    settextcolor(LIGHTGRAY);

    const char* options[2] = { "开始游戏", "游戏玩法" };
    int optionY[2] = { getmaxy() / 2 + 50, getmaxy() / 2 + 90 };

    for (int i = 0; i < 2; ++i) {
        x = (getmaxx() - textwidth(options[i])) / 2;
        outtextxy(x, optionY[i], options[i]);
    }
}

// 绘制箭头光标
void drawCursor(int posY) {
    setfillcolor(WHITE);
    setlinecolor(WHITE);
    float width = textwidth(_T("开始游戏")) + 10; // 光标宽度
    float height = textheight(_T("开始游戏")) + 4; // 光标高度
    float x = (getmaxx() - width) / 2 - 20; // 光标在文字左边
    float y = posY - height / 2;
    // 绘制箭头
    int arrowSize = 10; // 箭头大小
    int arrowX = x + 5; // 箭头位置
    int arrowY = posY + height / 2;
    // 绘制箭头三角形
    int points[6] = { arrowX, arrowY, arrowX - arrowSize, arrowY - arrowSize, arrowX - arrowSize, arrowY + arrowSize };
    fillpoly(3, points);
}

// 游戏玩法
void showGameplayTips() {
    setbkcolor(BLACK);
    cleardevice();
    settextcolor(WHITE);
    settextstyle(24, 0, _T("Arial"));
    const char* tips[] = {
        "使用方向键或者“w”“s”“a”“d”来进行角色移动",
        "按Q可查看怪物图鉴",
        "温馨提示：请注意自己的血量和钥匙数量",
        "祝你好运！"
    };
    int screenHeight = getmaxy();
    int tipY = screenHeight / 3; // 从屏幕的1/3位置开始显示文本
    for (int i = 0; i < 4; ++i) {
        int tipX = (getmaxx() - textwidth(tips[i])) / 2; // 计算文本的X坐标以居中显示
        outtextxy(tipX, tipY + i * 30, tips[i]); // 显示提示文本
    }
    _getch(); // 等待用户按键
    cleardevice(); // 清空设备，准备返回游戏
}

//游戏背景
void showStory(HWND hwnd) {
	MessageBox(hwnd,
		"恶魔降临在这个国家中，公主被恶魔首领附身逃到魔塔中，国王下了悬赏。一位异国勇士扬言一周内将公主救出。\n"
		"勇士却没想到他再也没有回头路。",
		"游戏背景",
		MB_OK | MB_ICONINFORMATION);
}

// 自定义弹窗的窗口过程
LRESULT CALLBACK CustomMsgBoxProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_TIMER:
        if (wParam == 1) {
            KillTimer(hwnd, 1); // 停止定时器
            DestroyWindow(hwnd); // 销毁窗口
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// 创建自定义弹窗
void ShowCustomMessageBox(HWND parent, const char* text) {
    static int msgBoxCount = 0; // 用于生成唯一的窗口类名
    std::string className = "CustomMsgBoxClass" + std::to_string(msgBoxCount++);

    // 注册窗口类
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = CustomMsgBoxProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = className.c_str();
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "窗口类注册失败", "错误", MB_OK | MB_ICONERROR);
        return;
    }

    // 获取父窗口的尺寸
    RECT parentRect;
    GetClientRect(parent, &parentRect);
    int parentWidth = parentRect.right - parentRect.left;
    int parentHeight = parentRect.bottom - parentRect.top;

    // 计算弹窗的位置
    int msgBoxWidth = 300;
    int msgBoxHeight = 100;
    int x = (parentWidth - msgBoxWidth) / 2;
    int y = (parentHeight - msgBoxHeight) / 2; // 修改为1/1，使弹窗在垂直方向上居中

    // 创建窗口
    HWND hwnd = CreateWindow(className.c_str(), "提示", WS_POPUP | WS_VISIBLE,
        x, y, msgBoxWidth, msgBoxHeight, parent, NULL, GetModuleHandle(NULL), NULL);
    if (!hwnd) {
        MessageBox(NULL, "窗口创建失败", "错误", MB_OK | MB_ICONERROR);
        return;
    }

    // 创建静态文本控件
    HWND hStatic = CreateWindow("STATIC", text, WS_VISIBLE | WS_CHILD | SS_CENTER,
        10, 10, 280, 80, hwnd, NULL, GetModuleHandle(NULL), NULL);

    // 设置字体
    HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
    SendMessage(hStatic, WM_SETFONT, (WPARAM)hFont, TRUE);

    // 设置定时器
    SetTimer(hwnd, 1, 800, NULL); // 设置一个2秒的定时器

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

//绘制地图
void drawMap() {
    int i, j;
    int x, y;
    settextstyle(16, 0, _T("Arial"));
    for (i = 0; i < 13; i++) {
        for (j = 0; j < 13; j++) {
            x = 60 * j;
            y = 60 * i;
            putimage(x, y, &img[currentMap[i][j][currentLayer]]);
        }
    }
    putimage(60 * 13, 0, &roleInfo);
    outtextxy(60 * 13 + 12, 100, role.角色名);
    //角色信息变化
    while (role.经验 >= count) {
        role.经验 -= count;
        role.等级++;
        role.血量 += 1000;
        role.攻击 += 10;
        role.防御 += 10;

        char message[100];
        sprintf(message, "恭喜你升级了！当前等级：%d", role.等级);
        ShowCustomMessageBox(hwnd, message);
    }

    outtextxy(60 * 13 + 12, 180, intToWcahr(role.等级));
    outtextxy(60 * 13 + 12, 230, intToWcahr(role.经验));
    outtextxy(60 * 13.2 + 12, 275, "Q");
    outtextxy(60 * 12.9 + 6.5, 295, "怪物图鉴");
    outtextxy(60 * 13 + 12, 362, intToWcahr(role.血量));
    outtextxy(60 * 13 + 12, 415, intToWcahr(role.蓝量));
    outtextxy(60 * 13 + 12, 517, intToWcahr(role.攻击));
    outtextxy(60 * 13 + 12, 567, intToWcahr(role.防御));
    outtextxy(60 * 13 + 12, 689, intToWcahr(role.黄钥匙));
    outtextxy(60 * 13 + 12, 759, intToWcahr(role.蓝钥匙));
}

//加载当前地图
void loadMap(int layer) {
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            currentMap[i][j][layer] = map[layer][i][j];
        }
    }
}

//初始化地图 保存初始地图和楼梯位置
void initializeMapState() {
    for (int layer = 0; layer < 7; ++layer) {
        for (int i = 0; i < 13; ++i) {
            for (int j = 0; j < 13; ++j) {
                savedMap[layer][i][j] = map[layer][i][j];
                stairsPositions[layer][i][j] = map[layer][i][j]; // 初始化楼梯位置
            }
        }
    }
}

//保存当前层地图 记录楼梯位置
void saveLayerState(int layer) {
    for (int x = 0; x < 13; x++) {
        for (int y = 0; y < 13; y++) {
            savedMap[layer][x][y] = currentMap[x][y][layer];
            if (currentMap[x][y][layer] == 13 || currentMap[x][y][layer] == 14) {
                stairsPositions[layer][x][y] = currentMap[x][y][layer];
            }
        }
    }
}

//恢复地图状态和楼梯位置
void restoreLayerState(int layer) {
    for (int x = 0; x < 13; x++) {
        for (int y = 0; y < 13; y++) {
            currentMap[x][y][layer] = savedMap[layer][x][y];
            if (stairsPositions[layer][x][y] == 13 || stairsPositions[layer][x][y] == 14) {
                currentMap[x][y][layer] = stairsPositions[layer][x][y];
            }
        }
    }
}

//进入楼层
void enterLayer(int layer) {
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            currentMap[i][j][layer] = map[layer][i][j];
        }
    }
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 13; ++j) {
            savedMap[layer][i][j] = map[layer][i][j];
        }
    }
}

//角色和怪物攻击原理
bool bloodPK(struct monster& mons) {
    int monsterDamage = mons.calculateDamage();//怪物对角色的伤害
    if (monsterDamage < 0) monsterDamage = 0; // 确保伤害值不为负
    if (monsterDamage >= role.血量) {
        role.血量 -= monsterDamage; 
        return false; 
    }

    int roleDamage = role.攻击 - mons.防御;//角色对怪物的伤害
    if (roleDamage < 0) roleDamage = 0;
    role.血量 -= monsterDamage;
    mons.血量 -= roleDamage;
    if (mons.血量 <= 0) {
        role.经验 += mons.经验; // 怪物死亡，角色获得经验
        return true;
    }
    return true; // 战斗继续
}

//角色属性变化
void PickItem(int itemType, int x, int y, int i, int j) {
    currentMap[x][y][currentLayer] = 12; // 将物品位置设置为已拾取
    currentMap[i][j][currentLayer] = 1;   // 将角色位置设置为移动后的状态
    char message[100];
    switch (itemType) {
    case 4: role.蓝钥匙++; break;
    case 5: role.黄钥匙++; break;
    case 8:
        role.攻击 += 3;
        ShowCustomMessageBox(hwnd, "攻击增加 3 点");
        break;
    case 9:
        role.防御 += 3;
        ShowCustomMessageBox(hwnd, "防御增加 3 点");
        break;
    case 6:
        role.血量 += 200;
        ShowCustomMessageBox(hwnd, "血量增加 200 点");
        break;
    case 7:
        role.血量 += 500;
        ShowCustomMessageBox(hwnd, "血量增加 500 点");
        break;
    case 25:
        role.攻击 += 10;
        ShowCustomMessageBox(hwnd, "攻击增加 10 点");
        break;
    case 10:
        role.等级++;
        role.血量 += 1000;
        role.攻击 += 10;
        role.防御 += 10;
        sprintf(message, "恭喜你升级了！当前等级：%d", role.等级);
        ShowCustomMessageBox(hwnd, message);
        break;
    case 27:
        role.防御 += 15;
        ShowCustomMessageBox(hwnd, "防御增加 15 点");
        break;
    case 23:
        role.攻击 += 20;
        ShowCustomMessageBox(hwnd, "攻击增加 20 点");
        break;
    case 24:
        role.防御 += 20;
        ShowCustomMessageBox(hwnd, "防御增加 20 点");
        break;
    case 26:
        role.等级 += 3;
        role.血量 += 3000;
        role.攻击 += 30;
        role.防御 += 30;
        sprintf(message, "恭喜你升级了！当前等级：%d", role.等级);
        ShowCustomMessageBox(hwnd, message);
        break;
    }
}

//使用钥匙
void UseKey(int keyType, int x, int y, int i, int j) {
    if (keyType == 2) {
        role.蓝钥匙--;
    }
    else if (keyType == 3) {
        role.黄钥匙--;
    }
    currentMap[x][y][currentLayer] = 12; // 将门的位置设置为已使用钥匙
    currentMap[i][j][currentLayer] = 1;   // 将角色位置设置为移动后的状态
}

//怪物名称
std::string GetMonsterName(MonsterType monsType) {
    switch (monsType) {
    case Monster_绿毛球: return "绿毛球";
    case Monster_红毛球: return "红毛球";
    case Monster_小蝙蝠: return "小蝙蝠";
    case Monster_初级法师: return "初级法师";
    case Monster_骷髅人: return "骷髅人";
    case Monster_黑毛球: return "黑毛球";
    case Monster_骷髅士兵: return "骷髅士兵";
    case Monster_兽面人: return "兽面人";
    case Monster_初级士兵: return "初级士兵";
    case Monster_红蝙蝠: return "红蝙蝠";
    case Monster_金卫士: return "金卫士";
    case Monster_金队长: return "金队长";
    case Monster_白衣武士: return "白衣武士";
    case Monster_大魔王:return "大魔王";
    default: return "未知怪物";
    }
}

//攻击怪物过程
void FightMonster(MonsterType monsType, int x, int y, int i, int j) {
    monster mons;
    switch (monsType) {
    case Monster_绿毛球: mons = 绿毛球; break;
    case Monster_红毛球: mons = 红毛球; break;
    case Monster_小蝙蝠: mons = 小蝙蝠; break;
    case Monster_初级法师: mons = 初级法师; break;
    case Monster_骷髅人: mons = 骷髅人; break;
    case Monster_黑毛球: mons = 黑毛球; break;
    case Monster_骷髅士兵: mons = 骷髅士兵; break;
    case Monster_兽面人: mons = 兽面人; break;
    case Monster_初级士兵: mons = 初级士兵; break;
    case Monster_红蝙蝠: mons = 红蝙蝠; break;
    case Monster_金卫士: mons = 金卫士; break;
    case Monster_金队长: mons = 金队长; break;
    case Monster_白衣武士: mons = 白衣武士; break;
    case Monster_大魔王:mons = 大魔王; break;
    }

    std::string monsterName = GetMonsterName(monsType);
    char attackMessage[100];
    sprintf(attackMessage, "正在攻击 %s", monsterName.c_str());
    ShowCustomMessageBox(hwnd, attackMessage);
    if (role.攻击 <= mons.防御) {//角色攻击小于怪物防御无法对怪物造成伤害
        sprintf(attackMessage, "无法战胜 %s", monsterName.c_str());
        ShowCustomMessageBox(hwnd, attackMessage);
        return; // 直接退出战斗
    }
    int totalDamage = 0;//怪物对角色的总伤害值
    int roleHP = role.血量;
    int monsHP = mons.血量;
    while (roleHP > 0 && monsHP > 0) {
        int monsterDamage = mons.calculateDamage();
        if (monsterDamage < 0) monsterDamage = 0; 
        roleHP -= monsterDamage;
        totalDamage += monsterDamage;

        int roleDamage = role.攻击 - mons.防御;
        if (roleDamage < 0) roleDamage = 0; 
        monsHP -= roleDamage;
    }
    if (roleHP <= 0) {
        sprintf(attackMessage, "无法战胜 %s", monsterName.c_str());
        ShowCustomMessageBox(hwnd, attackMessage);
        return; // 直接退出战斗
    }
    while (true) {
        if (bloodPK(mons) == true) {
            if (mons.血量 <= 0) {
                currentMap[i][j][currentLayer] = 1; 
                currentMap[x][y][currentLayer] = 12; 
                break; // 怪物死亡，结束战斗
            }
        }
    }
}

//角色移动
void MoveCharacter(int dx, int dy, int i, int j) {
    int newX = i + dx;
    int newY = j + dy;
    if (newX >= 0 && newX < 13 && newY >= 0 && newY < 13) {
        if (currentMap[newX][newY][currentLayer] == 13 && currentLayer < 7) { // 跳转到下一层
            saveLayerState(currentLayer); // 保存当前层地图状态
            role.lastX = newX; // 更新下楼位置
            role.lastY = newY; // 更新下楼位置
            currentLayer++;
            if (currentLayer == 7) { // 检查是否是最后一层
	            MessageBox(hwnd,"没想到你竟然能来到这里\n"
		            "不过又是一个不自量力的家伙前来送死。",
		            "大魔王",
		        MB_OK | MB_ICONINFORMATION);
            }
            restoreLayerState(currentLayer); // 恢复新层的状态
            drawMap(); // 重绘地图，显示下一层
        }
        else if (currentMap[newX][newY][currentLayer] == 14 && currentLayer > 0) { // 返回上一层
            saveLayerState(currentLayer); // 保存当前层地图状态
            currentLayer--;
            restoreLayerState(currentLayer); // 恢复上一层的地图状态
            drawMap(); // 重绘地图，显示上一层
            if (stairsPositions[currentLayer][role.lastX][role.lastY] == 13) {
                currentMap[role.lastX][role.lastY][currentLayer] = 13; // 恢复上楼的图块
            }
        }
        else if (currentMap[newX][newY][currentLayer] != 0&& currentMap[newX][newY][currentLayer] != 29) {//墙不能覆盖
            switch (currentMap[newX][newY][currentLayer]) {
            case 4: // 蓝钥匙
                PickItem(4, newX, newY, i, j);
                break;
            case 5: // 黄钥匙
                PickItem(5, newX, newY, i, j);
                break;
            case 2: // 需要蓝钥匙的门
                if (role.蓝钥匙 > 0) {
                    UseKey(2, newX, newY, i, j);
                }
                break;
            case 3: // 需要黄钥匙的门
                if (role.黄钥匙 > 0) {
                    UseKey(3, newX, newY, i, j);
                }
                break;
            case 8: // 攻击增加
                PickItem(8, newX, newY, i, j);
                break;
            case 9: // 防御增加
                PickItem(9, newX, newY, i, j);
                break;
            case 6: // 血量增加
                PickItem(6, newX, newY, i, j);
                break;
            case 7: // 蓝量增加
                PickItem(7, newX, newY, i, j);
                break;
            case 10: // 等级增加
                PickItem(10, newX, newY, i, j);
                break;
            case 23://攻击增加
                PickItem(23, newX, newY, i, j);
                break;
            case 24://防御增加
                PickItem(24, newX, newY, i, j);
                break;
            case 25://攻击增加
                PickItem(25, newX, newY, i, j);
                break;
            case 26://等级增加
                PickItem(26, newX, newY, i, j);
                break;
            case 27://防御增加
                PickItem(27, newX, newY, i, j);
                break;
            case 15: // 绿毛球
                FightMonster(Monster_绿毛球, newX, newY, i, j);
                break;
            case 16: // 红毛球
                FightMonster(Monster_红毛球, newX, newY, i, j);
                break;
            case 17: // 小蝙蝠
                FightMonster(Monster_小蝙蝠, newX, newY, i, j);
                break;
            case 18: // 大法师
                FightMonster(Monster_初级法师, newX, newY, i, j);
                break;
            case 19: // 骷髅士兵
                FightMonster(Monster_骷髅士兵, newX, newY, i, j);
                break;
            case 20: // 骷髅人
                FightMonster(Monster_骷髅人, newX, newY, i, j);
                break;
            case 21: // 兽面人
                FightMonster(Monster_兽面人, newX, newY, i, j);
                break;
            case 22: // 黑毛球
                FightMonster(Monster_黑毛球, newX, newY, i, j);
                break;
            case 30: // 红蝙蝠
                FightMonster(Monster_红蝙蝠, newX, newY, i, j);
                break;
            case 31: // 初级士兵
                FightMonster(Monster_初级士兵, newX, newY, i, j);
                break;
            case 32: // 金卫士
                FightMonster(Monster_金卫士, newX, newY, i, j);
                break;
            case 33: // 金队长
                FightMonster(Monster_金队长, newX, newY, i, j);
                break;
            case 34: // 白衣武士
                FightMonster(Monster_白衣武士, newX, newY, i, j);
                break;
            case 35: // 大魔王
                FightMonster(Monster_大魔王, newX, newY, i, j);
                break;
            case 28: // 显示游戏背景故事
               	MessageBox(hwnd,"勇士，非常感谢你凭借着智慧与勇气打败了大魔王，拯救了人民和我。\n"
            		"我将代表国家给予你至高无上的荣誉，并且给予你无数财富作为奖励。",
               		"公主",
            		MB_OK | MB_ICONINFORMATION);
               	MessageBox(hwnd,"你已获取胜利",
            		"游戏结束",
            		MB_OK | MB_ICONINFORMATION);
                break;
            default:
                currentMap[i][j][currentLayer] = 1; // 将旧位置设置为空地
                currentMap[newX][newY][currentLayer] = 12; // 将新位置设置为角色
                break;
            }
            if (currentMap[i][j][currentLayer] == 14) {
                currentMap[i][j][currentLayer] = 13;
            }
            drawMap(); // 重绘地图
        }
    }
}

//怪物图鉴
void displayMonsterGallery() {
    int screenWidth = getmaxx();
    int screenHeight = getmaxy();

    char monsterInfo[14][256];
    IMAGE monsterImages[14]; // 用于存储怪物图片

    loadimage(&monsterImages[0], "15.jpg", 60, 60);
    loadimage(&monsterImages[1], "16.jpg", 60, 60);
    loadimage(&monsterImages[2], "17.jpg", 60, 60);
    loadimage(&monsterImages[3], "20.jpg", 60, 60);
    loadimage(&monsterImages[4], "22.jpg", 60, 60);
    loadimage(&monsterImages[5], "18.jpg", 60, 60);
    loadimage(&monsterImages[6], "19.jpg", 60, 60);
    loadimage(&monsterImages[7], "21.jpg", 60, 60);
    loadimage(&monsterImages[8], "31.jpg", 60, 60);
    loadimage(&monsterImages[9], "30.jpg", 60, 60);
    loadimage(&monsterImages[10], "34.jpg", 60, 60);
    loadimage(&monsterImages[11], "32.jpg", 60, 60);
    loadimage(&monsterImages[12], "33.jpg", 60, 60);
    loadimage(&monsterImages[13], "35.jpg", 60, 60);

    char buffer[14][2048]; // 缓冲区

    for (int i = 0; i < 14; i++) {
        struct monster* mons = nullptr;
        MonsterType monsType;
        switch (i) {
        case 0: mons = &绿毛球; monsType = Monster_绿毛球; break;
        case 1: mons = &红毛球; monsType = Monster_红毛球; break;
        case 2: mons = &小蝙蝠; monsType = Monster_小蝙蝠; break;
        case 3: mons = &骷髅人; monsType = Monster_骷髅人; break;
        case 4: mons = &黑毛球; monsType = Monster_黑毛球; break;
        case 5: mons = &初级法师; monsType = Monster_初级法师; break;
        case 6: mons = &骷髅士兵; monsType = Monster_骷髅士兵; break;
        case 7: mons = &兽面人; monsType = Monster_兽面人; break;
        case 8: mons = &初级士兵; monsType = Monster_初级士兵; break;
        case 9: mons = &红蝙蝠; monsType = Monster_红蝙蝠; break;
        case 10: mons = &白衣武士; monsType = Monster_白衣武士; break;
        case 11: mons = &金卫士; monsType = Monster_金卫士; break;
        case 12: mons = &金队长; monsType = Monster_金队长; break;
        case 13: mons = &大魔王; monsType = Monster_大魔王; break;
        default: break;
        }
        if (mons) {
            int totalDamage = 0;
            int roleHP = role.血量;
            int monsHP = mons->血量;
            if (role.攻击 <= mons->防御) {
                snprintf(monsterInfo[i], sizeof(monsterInfo[i]), "名称: %s   血量: %d   攻击: %d   防御: %d   经验: %d   伤害值: 不可战胜     ",
                    GetMonsterName(monsType).c_str(), mons->血量, mons->攻击, mons->防御, mons->经验);
            }
            else {
                int monsterDamage = mons->calculateDamage();
                int roleDamage = role.攻击 - mons->防御;
                while (roleHP > 0 && monsHP > 0) {
                    if (monsterDamage < 0) monsterDamage = 0; // 确保伤害值不为负
                    roleHP -= monsterDamage;
                    totalDamage += monsterDamage;
                    if (roleDamage < 0) roleDamage = 0; // 确保伤害值不为负
                    monsHP -= roleDamage;
                }
                if (roleHP <= 0) {
                    while (monsHP > 0) {
                        totalDamage += mons->calculateDamage(); // 如果角色血量降到零，继续计算剩余伤害
                        monsHP -= roleDamage;
                    }
                }
                sprintf(monsterInfo[i], "名称: %s   血量: %d   攻击: %d   防御: %d   经验: %d   伤害值: %d     ",
                    GetMonsterName(monsType).c_str(), mons->血量, mons->攻击, mons->防御, mons->经验, totalDamage);
            }
            sprintf(buffer[i], "%s\n\n", monsterInfo[i]); // 将怪物信息添加到缓冲区
        }
    }
    setfillcolor(BLACK); // 设置填充颜色为黑色
    bar(0, 0, screenWidth, screenHeight);
    for (int i = 0; i < 14; i++) {
        putimage(40, i * 55, &monsterImages[i]); // 显示怪物图片
        outtextxy(120, i * 55 + 12, buffer[i]); // 显示怪物信息
    }
    _getch(); 
}

//方向键
void keyDown() {
    char userKey;
    int i = 0, j = 0;
    for (i = 0; i < 13; i++) {
        for (j = 0; j < 13; j++) {
            if (currentMap[i][j][currentLayer] == 12) {
                break;
            }
        }
        if (currentMap[i][j][currentLayer] == 12) {
            break;
        }
    }
    userKey = _getch();
    switch (userKey) {
    case 'A':
    case 'a':
    case 75: MoveCharacter(0, -1, i, j); break;
    case 'D':
    case 'd':
    case 77: MoveCharacter(0, 1, i, j); break;
    case 'W':
    case 'w':
    case 72: MoveCharacter(-1, 0, i, j); break;
    case 'S':
    case 's':
    case 80: MoveCharacter(1, 0, i, j); break;
    case 'Q':
    case 'q': displayMonsterGallery(); break;
    default: break;
    }
}

int main() {
    inputRoleName();
    loadResource();
    initializeMapState();
    loadMap(0);
    hwnd = initgraph(60 * 14, 60 * 13);  // 初始化图形库

    drawWelcomeScreen();  // 绘制欢迎界面
    drawCursor(getmaxy() / 2 + 50);  // 绘制光标箭头，初始时指向第一个选项
    int selectedOption = 0;  // 用于跟踪选项的变量
    PlaySoundEffect("魔塔.wav");//背景音乐

    while (true) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'W' || key == 'w' || key == 72) { // Up arrow key
                selectedOption = 1 - selectedOption;
                cleardevice(); // 清除屏幕
                drawWelcomeScreen();  // 重新绘制欢迎界面
                drawCursor(selectedOption == 0 ? getmaxy() / 2 + 50 : getmaxy() / 2 + 90); // 绘制光标
            }
            else if (key == 'S' || key == 's' || key == 80) { // Down arrow key
                selectedOption = 1 - selectedOption;
                cleardevice(); // 清除屏幕
                drawWelcomeScreen();  // 重新绘制欢迎界面
                drawCursor(selectedOption == 0 ? getmaxy() / 2 + 50 : getmaxy() / 2 + 90); // 绘制光标
            }
            else if (key == 13) { // Enter key
                if (selectedOption == 0) {
                    showStory(hwnd);  // 显示游戏背景故事
                    while (1) {
                        drawMap();
                        saveLayerState(currentLayer); // 在角色移动前保存当前地图状态
                        keyDown();
                    }
                }
                else {
                    showGameplayTips();  // 显示游戏玩法提示
                    cleardevice();
                    drawWelcomeScreen();  // 重新绘制欢迎界面
                    drawCursor(selectedOption == 0 ? getmaxy() / 2 + 50 : getmaxy() / 2 + 90); // 绘制光标
                }
            }
        }
    }
    closegraph();  // 关闭图形库
    return 0;
}