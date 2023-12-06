#include<gl/GLUT.H>
#include "Button.h"
#include "SwitchButton.h"
#include "ProgressBar.h"
#include "CheckBox.h"
#include "Slider.h"
#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>

const int SIDE_PANEL_WIDTH = 200; // 显示框宽度

const int ROWS = 30; // 30行
const int COLS = 10; // 10列

int movedown_speed_original = 800;
int movedown_speed = movedown_speed_original;
int score = 0;
int target = 200;
int game_status = 1;
int line_status = 1;
bool timerEnabled = true; 

// 定义方块类型
enum BlockType
{
    I, O, T, S, Z, L, J, NUM_TYPES
};

const int BLOCK_SIZE = 4; 
const int CELL_SIZE = 20; // 单元格像素数

// 声明计时器
void tetris_timer(int value); // 函数原型声明

// 定义方块形状
const bool SHAPES[NUM_TYPES][BLOCK_SIZE][BLOCK_SIZE] =
{
    // 0表示空，1表示存在单元格
    // 长条块 I
    {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },

    // 正方块 O
    {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
    },

    // 丁字块 T
    {
        {0, 0, 0, 0},
        {0, 1, 1, 1},
        {0, 0, 1, 0},
        {0, 0, 0, 0},
    },

    // 右折叠块 S
    {
        {0, 0, 0, 0},
        {0, 0, 1, 1},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
    },
    // 左折叠块 Z
    {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 1},
        {0, 0, 0, 0},
    },
    // 左转块 L
    {
        {0, 0, 0, 0},
        {0, 1, 1, 1},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
    },
    // 右转块 J
    {
        {0, 0, 0, 0},
        {0, 1, 1, 1},
        {0, 0, 0, 1},
        {0, 0, 0, 0},
    }
};

// 定义方块状态
struct Block
{
    BlockType type;
    int x, y;  // 方块的左上角位置
    int rotation; // 0: 0°, 1: 90°, 2: 180°, 3: 270°
    bool shape[BLOCK_SIZE][BLOCK_SIZE];  // 当前的方块形状
};

// 定义游戏板
int board[ROWS][COLS] = { 0 };  // 0表示空，1表示有方块

// 实例化当前方块
Block currentBlock;

// 定义操作：旋转方块
void rotateBlock()
{
    bool temp[BLOCK_SIZE][BLOCK_SIZE];

    // 转置操作
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        for (int j = 0; j < BLOCK_SIZE; j++)
        {
            temp[j][i] = currentBlock.shape[i][j];
        }
    }

    // 反转列操作
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        for (int j = 0; j < BLOCK_SIZE / 2; j++)
        {
            bool tmp = temp[i][j];
            temp[i][j] = temp[i][BLOCK_SIZE - 1 - j];
            temp[i][BLOCK_SIZE - 1 - j] = tmp;
        }
    }

    // 更新当前方块的形状
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        for (int j = 0; j < BLOCK_SIZE; j++)
        {
            currentBlock.shape[i][j] = temp[i][j];
        }
    }
}

// 碰撞规则
bool checkCollision(int newX, int newY)
{
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        for (int j = 0; j < BLOCK_SIZE; j++)
        {
            if (currentBlock.shape[i][j])
            {
                if (newX + j < 0 || newX + j >= COLS || newY + i >= ROWS || (newY + i >= 0 && board[newY + i][newX + j]))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

// 进度条创建
ProgressBar score_progress(COLS* CELL_SIZE + 20, 200, 180, 30, 0, target);

// 清除行规则
void checkAndClearRows()
{
    for (int i = ROWS - 1; i >= 0; i--)
    {
        bool full = true;
        for (int j = 0; j < COLS; j++)
        {
            if (!board[i][j])
            {
                full = false;
                break;
            }
        }

        if (full)
        {
            for (int k = i; k > 0; k--)
            {
                for (int j = 0; j < COLS; j++)
                {
                    board[k][j] = board[k - 1][j];
                }
            }
            for (int j = 0; j < COLS; j++)
            {
                board[0][j] = 0;
            }
            i++;  // 因为整体下移了一行，所以需要再次检查当前行
            score = score + 100;

            if (score >= target)
            {
                target = target * 5;
                score_progress.max = target;
            }
        }
    }
}

// 产生新方块
void spawnBlock()
{
    currentBlock.type = static_cast<BlockType>(rand() % NUM_TYPES);
    currentBlock.x = COLS / 2 - BLOCK_SIZE / 2;  // 让方块出现在屏幕的中央位置
    currentBlock.y = 0;
    currentBlock.rotation = 0;  // 新方块初始旋转为0

    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        for (int j = 0; j < BLOCK_SIZE; j++)
        {
            currentBlock.shape[i][j] = SHAPES[currentBlock.type][i][j];
        }
    }

    // 检查新方块是否立即产生碰撞，如果是，则游戏结束
    if (checkCollision(currentBlock.x, currentBlock.y))
    {
        game_status = -1 * game_status;
    }
}

// 放置方块
void placeBlock()
{
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        for (int j = 0; j < BLOCK_SIZE; j++)
        {
            if (currentBlock.shape[i][j])
            {
                // 如果方块的位置已经在最顶部，那么游戏结束
                if (currentBlock.y + i < 0) {
                    game_status = -1; // 设置游戏状态为结束
                    timerEnabled = false;
                    return; // 直接返回，不放置方块
                }
                board[currentBlock.y + i][currentBlock.x + j] = 1;
            }
        }
    }

    checkAndClearRows();
    spawnBlock();
}

// 方块下落
void moveDown()
{
    // 尝试将方块向下移动一格
    if (!checkCollision(currentBlock.x, currentBlock.y + 1))
    {
        currentBlock.y++;
    }
    else
    {
        // 如果无法向下移动，则放置方块并生成新方块
        placeBlock();
    }

    glutPostRedisplay();
}

// 定时器
void tetris_timer(int value)
{
    if (game_status == 1 && timerEnabled) // 游戏进行中且计时器启用
    {
        moveDown();
    }
    if (timerEnabled) { // 这里再次检查以避免重复调用
        glutTimerFunc(movedown_speed, tetris_timer, 0); // 重新设置计时器
    }
}

//------------------UI等---------------
// 按钮、开关功能创建
void restartGame();
void toggleBorders();

Button restartButton(COLS* CELL_SIZE + 20, 110, COLS* CELL_SIZE + 200, 140, "Restart", restartGame);
SwitchButton toggleSW(COLS* CELL_SIZE + 20, 150, COLS* CELL_SIZE + 200, 180, "Toggle Borders", toggleBorders);

void restartGame() 
{
    score = 0; // 重置分数
    game_status = 1; // 重置游戏状态，假设 1 代表游戏进行中

    // 清空游戏板
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = 0;
        }
    }

    timerEnabled = false; // 先禁用计时器
    spawnBlock(); // 生成新的方块
    timerEnabled = true; // 启用计时器
}
void toggleBorders()
{
    line_status = -line_status;
}            

// 复选框
// CheckBox checkBox(50, 80, 20);

// 滑块
Slider slider(COLS* CELL_SIZE + 20, 250, COLS* CELL_SIZE + 200, 260, 0.5, 5);


// 鼠标点击检测
void MouseClicked(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            // 鼠标按下事件，更新按钮状态为 PRESSED
            if (restartButton.isInside(x, y))
            {
                restartButton.state = PRESSED;
            }

            // 鼠标按下事件，更新开关状态为 PRESSED
            if (toggleSW.isInsideSwitchButton(x, y))
            {
                if (toggleSW.flag == 1)
                {
                    toggleSW.state = OFF;
                    toggleSW.flag = -toggleSW.flag;
                }
                else
                {
                    toggleSW.state = ON;
                    toggleSW.flag = -toggleSW.flag;
                }
            }
        }
        else if (state == GLUT_UP)
        {
            // 鼠标释放事件，如果在按钮内则调用按钮动作
            if (restartButton.isInside(x, y))
            {
                restartButton.action(); // 调用按钮动作
                restartButton.state = NORMAL;
            }

            // 鼠标释放事件，如果在开关内则调用按钮动作
            if (toggleSW.isInsideSwitchButton(x, y))
            {
                toggleSW.action(); // 调用开关动作
            }
        }
    }

    slider.moveSlider(x, y);
    movedown_speed = movedown_speed_original / slider.currentValue;

    glutPostRedisplay();
}

// 鼠标移动检测
void MouseMoved(int x, int y)
{
    restartButton.state = restartButton.isInside(x, y) ? HOVER : NORMAL;

    if (toggleSW.flag == -1)
    {
        toggleSW.state = toggleSW.isInsideSwitchButton(x, y) ? HOLD : OFF;
    }
    else
    {
        toggleSW.state = toggleSW.isInsideSwitchButton(x, y) ? HOLD : ON;
    }

    glutPostRedisplay();
}


//------------------图窗展示部分----------------

// 初始化图窗
void initOpenGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);  // 设置背景色为黑色
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int windowWidth = 10 + COLS * CELL_SIZE + SIDE_PANEL_WIDTH + 10;
    int windowHeight = 10 + ROWS * CELL_SIZE + 10;
    gluOrtho2D(0, windowWidth, windowHeight, 0); // 设置为窗口的宽高
}

// 绘制元素格
void drawBlock(int x, int y)
{
    glRecti(x * CELL_SIZE, y * CELL_SIZE, (x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE);
}

// 展示函数
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制游戏板上的方块
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (board[i][j])
            {
                drawBlock(j, i);
            }
        }
    }

    // 绘制当前方块
    glColor3f(1.0, 0.0, 0.0);
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        for (int j = 0; j < BLOCK_SIZE; j++)
        {
            if (currentBlock.shape[i][j])
            {
                drawBlock(currentBlock.x + j, currentBlock.y + i);
            }
        }
    }

    // 绘制分割线
    if (line_status == 1)
    {
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (board[i][j])
                {
                    glColor3f(0.5, 0.5, 0.5);
                    //glRectf(j * BLOCK_SIZE, i * BLOCK_SIZE, (j + 1) * BLOCK_SIZE, (i + 1) * BLOCK_SIZE);
                }

                // 绘制分割线，颜色根据设计需要进行更改
                glColor3f(1.0, 1.0, 1.0);  // 白色线
                glBegin(GL_LINES);
                glVertex2f(j * CELL_SIZE, i * CELL_SIZE);
                glVertex2f(j * CELL_SIZE, (i + 1) * CELL_SIZE);
                glVertex2f(j * CELL_SIZE, i * CELL_SIZE);
                glVertex2f((j + 1) * CELL_SIZE, i * CELL_SIZE);
                glEnd();
            }
            // 保证每行的最右边有线，每行的末尾绘制一个垂直线段
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex2f(COLS * BLOCK_SIZE, i * BLOCK_SIZE);
            glVertex2f(COLS * BLOCK_SIZE, (i + 1) * BLOCK_SIZE);
            glEnd();
        }
    }

    // 绘制右侧的分数面板
    char scoreText[50];
    sprintf_s(scoreText, sizeof(scoreText), "Score: %d", score);

    char targetText[50];
    sprintf_s(targetText, sizeof(targetText), "Target: %d", target);

    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2i(COLS * CELL_SIZE + 10, 30); // 设置实时分数显示位置

    for (char* c = scoreText; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }

    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2i(COLS * CELL_SIZE + 10, 50); // 设置目标分数显示位置

    for (char* c = targetText; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }

    // TODO: 在这里绘制其他UI元素
    restartButton.drawButton();

    toggleSW.drawSwitchButton();
    
    // checkBox.drawBox();

    score_progress.setValue(score);

    score_progress.drawProgressBar();

    slider.drawSlider();

    glutSwapBuffers();
}

// 用户键盘操作
void tetris_keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'a':
        // 向左移动方块，前提是没有碰撞
        if (!checkCollision(currentBlock.x - 1, currentBlock.y))
        {
            currentBlock.x--;
        }
        break;

    case 'd':
        // 向右移动方块
        if (!checkCollision(currentBlock.x + 1, currentBlock.y))
        {
            currentBlock.x++;
        }
        break;

    case 's':
        // 加速方块下落
        while (!checkCollision(currentBlock.x, currentBlock.y + 1))
        {
            currentBlock.y++;
        }
        break;

    case 'w':
        // 旋转方块
        rotateBlock();
        if (checkCollision(currentBlock.x, currentBlock.y))
        {
            // 如果旋转后产生碰撞，则撤销旋转
            rotateBlock();
            rotateBlock();
            rotateBlock();
        }
        break;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    int windowWidth = 10 + COLS * CELL_SIZE + SIDE_PANEL_WIDTH + CELL_SIZE;
    int windowHeight = 10 + ROWS * CELL_SIZE + 10;
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Tetris");
    initOpenGL();

    // 注册回调函数
    glutTimerFunc(500, tetris_timer, 0);
    glutDisplayFunc(display);
    glutKeyboardFunc(tetris_keyboard);

    glutPassiveMotionFunc(MouseMoved);
    glutMouseFunc(MouseClicked);

    spawnBlock();  // 生成初始方块
    glutMainLoop();

    return 0;
}
