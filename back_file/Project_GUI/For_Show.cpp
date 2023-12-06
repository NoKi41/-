#include<gl/GLUT.H>
#include "Button.h"
#include <algorithm>


#define _USE_MATH_DEFINES
#include <math.h>


int direction = 1; // 用于确定风车的旋转方向
float rotationAngle = 0.0f;  // 用于跟踪风车扇叶的旋转角度
float rotationSpeed = 0.01f; // 风车的旋转速度
const float MAX_SPEED = 0.2f;
const float MIN_SPEED = 0.001f;

// 图窗大小

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;

// 按钮数量默认颜色

int button_num = 3;
float ButtondefaultColor[3] = { 0.8f, 0.8f, 0.8f };

// 按钮操作函数

void toggleDirection()
{
    direction *= -1;
}

void increaseSpeed()
{
    rotationSpeed += 0.01f;
    if (rotationSpeed > MAX_SPEED) {
        rotationSpeed = MAX_SPEED;
    }
}

void decreaseSpeed()
{
    rotationSpeed -= 0.01f;
    if (rotationSpeed < MIN_SPEED)
    {
        rotationSpeed = MIN_SPEED;
    }
}

// 实例化按钮
// 这个地方的数字不同步

Button buttons[3] = { 
    Button(10, 10, 150, 40, "Toggle Direction", toggleDirection),
    Button(10, 60, 150, 90, "Increase Speed", increaseSpeed),
    Button(10, 110, 150, 140, "Decrease Speed", decreaseSpeed)
};

void mouseMove(int x, int y)
{
    y = WINDOW_HEIGHT - y;
    for (int i = 0; i < button_num; i++)
    {
        if (buttons[i].isInside(x, y))
        {
            std::copy(std::begin(buttons[i].hoverColor), std::end(buttons[i].hoverColor), std::begin(buttons[i].color));
        }
        else
        {
            std::copy(std::begin(ButtondefaultColor), std::end(ButtondefaultColor), std::begin(buttons[i].color));
        }
    }
    glutPostRedisplay();// 请求重绘
}

void mouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) // 注意这里的状态是GLUT_DOWN，表示鼠标按钮被按下
    {
        y = WINDOW_HEIGHT - y;
        for (int i = 0; i < button_num; i++)
        {
            if (buttons[i].isInside(x, y))
            {
                std::copy(std::begin(buttons[i].pressedColor), std::end(buttons[i].pressedColor), std::begin(buttons[i].color)); // 设置为按下时的颜色
                break;
            }
        }
        glutPostRedisplay();
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        y = WINDOW_HEIGHT - y;
        for (int i = 0; i < button_num; i++)
        {
            if (buttons[i].isInside(x, y))
            {
                buttons[i].action(); // 执行按钮动作
                std::copy(std::begin(buttons[i].hoverColor), std::end(buttons[i].hoverColor), std::begin(buttons[i].color)); // 恢复为悬停颜色
                break;
            }
        }
        glutPostRedisplay();
    }
}

void Initial(void)//初始化函数
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//白色背景，前3个是RGB，最后是Alpha值，用来控制透明，1.0表示完全不透明
    glMatrixMode(GL_PROJECTION);//OpenGL按照三维方式来处理图像，所以需要一个投影变换将三维图形投影到显示器的二维空间中
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);//指定使用正投影将一个x坐标在0~500，y坐标0~500范围内的矩形坐标区域投影到显示器窗口
}

void drawFanBlade(float x, float y, float angle, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y); // 中心点
    glVertex2f(x + 100 * cos(angle + rotationAngle), y + 100 * sin(angle + rotationAngle));
    glVertex2f(x + 100 * cos(angle + M_PI / 4 + rotationAngle), y + 100 * sin(angle + M_PI / 4 + rotationAngle));
    glEnd();
}

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    float centerX = WINDOW_WIDTH / 2;
    float centerY = WINDOW_HEIGHT / 2;

    // 绘制风车的四个扇叶
    drawFanBlade(centerX, centerY, 0, 1.0f, 0.0f, 0.0f);   // 红色
    drawFanBlade(centerX, centerY, M_PI / 2, 0.0f, 1.0f, 0.0f); // 绿色
    drawFanBlade(centerX, centerY, M_PI, 0.0f, 0.0f, 1.0f); // 蓝色
    drawFanBlade(centerX, centerY, 3 * M_PI / 2, 1.0f, 1.0f, 0.0f); // 黄色

    for (int i = 0; i < button_num; i++)
    {
        buttons[i].drawButton();
    }


    //glFlush();
    glutSwapBuffers();
}

void timer(int value)
{
    rotationAngle += direction * rotationSpeed;  // 每次旋转5度
    glutPostRedisplay();     // 请求重绘
    glutTimerFunc(17, timer, 0); // 每17毫秒调用一次timer函数，即近60帧/秒
}

//int main(int argc, char* argv[])//这是使用glut库函数进行窗口管理
//{
//    glutInit(&argc, argv);//使用glut库需要进行初始化
//    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);//设定窗口显示模式，颜色模型和缓存，这里是RGB颜色模型和双缓存（平滑动画）
//    glutInitWindowPosition(100, 100);//设定窗口的初始位置，屏幕左上角为原点，单位为像素
//    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);//设定窗口的大小
//    glutCreateWindow("彩色旋转风车OpenGL程序——顾笑铭");//创建一个窗口，参数是窗口标题名
//    glutDisplayFunc(&myDisplay);//将myDisplay指定为当前窗口的显示内容函数
//    Initial();
//    glutTimerFunc(0, timer, 0);  // 启动计时器
//    glutMouseFunc(mouseClick);
//    glutPassiveMotionFunc(mouseMove);
//    glutMainLoop();//使窗口框架运行起来，使显示回调函数开始工作
//    return 0;
//}
