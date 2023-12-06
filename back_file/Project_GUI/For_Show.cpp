#include<gl/GLUT.H>
#include "Button.h"
#include <algorithm>


#define _USE_MATH_DEFINES
#include <math.h>


int direction = 1; // ����ȷ���糵����ת����
float rotationAngle = 0.0f;  // ���ڸ��ٷ糵��Ҷ����ת�Ƕ�
float rotationSpeed = 0.01f; // �糵����ת�ٶ�
const float MAX_SPEED = 0.2f;
const float MIN_SPEED = 0.001f;

// ͼ����С

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;

// ��ť����Ĭ����ɫ

int button_num = 3;
float ButtondefaultColor[3] = { 0.8f, 0.8f, 0.8f };

// ��ť��������

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

// ʵ������ť
// ����ط������ֲ�ͬ��

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
    glutPostRedisplay();// �����ػ�
}

void mouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) // ע�������״̬��GLUT_DOWN����ʾ��갴ť������
    {
        y = WINDOW_HEIGHT - y;
        for (int i = 0; i < button_num; i++)
        {
            if (buttons[i].isInside(x, y))
            {
                std::copy(std::begin(buttons[i].pressedColor), std::end(buttons[i].pressedColor), std::begin(buttons[i].color)); // ����Ϊ����ʱ����ɫ
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
                buttons[i].action(); // ִ�а�ť����
                std::copy(std::begin(buttons[i].hoverColor), std::end(buttons[i].hoverColor), std::begin(buttons[i].color)); // �ָ�Ϊ��ͣ��ɫ
                break;
            }
        }
        glutPostRedisplay();
    }
}

void Initial(void)//��ʼ������
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//��ɫ������ǰ3����RGB�������Alphaֵ����������͸����1.0��ʾ��ȫ��͸��
    glMatrixMode(GL_PROJECTION);//OpenGL������ά��ʽ������ͼ��������Ҫһ��ͶӰ�任����άͼ��ͶӰ����ʾ���Ķ�ά�ռ���
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);//ָ��ʹ����ͶӰ��һ��x������0~500��y����0~500��Χ�ڵľ�����������ͶӰ����ʾ������
}

void drawFanBlade(float x, float y, float angle, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y); // ���ĵ�
    glVertex2f(x + 100 * cos(angle + rotationAngle), y + 100 * sin(angle + rotationAngle));
    glVertex2f(x + 100 * cos(angle + M_PI / 4 + rotationAngle), y + 100 * sin(angle + M_PI / 4 + rotationAngle));
    glEnd();
}

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    float centerX = WINDOW_WIDTH / 2;
    float centerY = WINDOW_HEIGHT / 2;

    // ���Ʒ糵���ĸ���Ҷ
    drawFanBlade(centerX, centerY, 0, 1.0f, 0.0f, 0.0f);   // ��ɫ
    drawFanBlade(centerX, centerY, M_PI / 2, 0.0f, 1.0f, 0.0f); // ��ɫ
    drawFanBlade(centerX, centerY, M_PI, 0.0f, 0.0f, 1.0f); // ��ɫ
    drawFanBlade(centerX, centerY, 3 * M_PI / 2, 1.0f, 1.0f, 0.0f); // ��ɫ

    for (int i = 0; i < button_num; i++)
    {
        buttons[i].drawButton();
    }


    //glFlush();
    glutSwapBuffers();
}

void timer(int value)
{
    rotationAngle += direction * rotationSpeed;  // ÿ����ת5��
    glutPostRedisplay();     // �����ػ�
    glutTimerFunc(17, timer, 0); // ÿ17�������һ��timer����������60֡/��
}

//int main(int argc, char* argv[])//����ʹ��glut�⺯�����д��ڹ���
//{
//    glutInit(&argc, argv);//ʹ��glut����Ҫ���г�ʼ��
//    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);//�趨������ʾģʽ����ɫģ�ͺͻ��棬������RGB��ɫģ�ͺ�˫���棨ƽ��������
//    glutInitWindowPosition(100, 100);//�趨���ڵĳ�ʼλ�ã���Ļ���Ͻ�Ϊԭ�㣬��λΪ����
//    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);//�趨���ڵĴ�С
//    glutCreateWindow("��ɫ��ת�糵OpenGL���򡪡���Ц��");//����һ�����ڣ������Ǵ��ڱ�����
//    glutDisplayFunc(&myDisplay);//��myDisplayָ��Ϊ��ǰ���ڵ���ʾ���ݺ���
//    Initial();
//    glutTimerFunc(0, timer, 0);  // ������ʱ��
//    glutMouseFunc(mouseClick);
//    glutPassiveMotionFunc(mouseMove);
//    glutMainLoop();//ʹ���ڿ������������ʹ��ʾ�ص�������ʼ����
//    return 0;
//}
