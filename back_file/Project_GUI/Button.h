#ifndef BUTTON_H
#define BUTTON_H

enum ButtonState { NORMAL, HOVER, PRESSED };

class Button
{
public:
    float x1, y1, x2, y2;
    const char* label;
    void(*action)();

    ButtonState state; // ��state��Ϊ��ĳ�Ա����

    float color[3];       // ��ť��Ĭ����ɫ
    float hoverColor[3];  // �����ͣʱ����ɫ
    float pressedColor[3];// ��갴��ʱ����ɫ

    // ���캯��
    Button(float x1, float y1, float x2, float y2, const char* l, void(*a)())
        : x1(x1), y1(y1), x2(x2), y2(y2), label(l), action(a), state(NORMAL) // �������ʼ��state
    {
        color[0] = 0.9f; // Ĭ����ɫ
        color[1] = 0.9f;
        color[2] = 0.9f;

        hoverColor[0] = 0.7f; // �����ͣʱ����ɫ
        hoverColor[1] = 0.7f;
        hoverColor[2] = 0.7f;

        pressedColor[0] = 0.5f; // ��갴��ʱ����ɫ
        pressedColor[1] = 0.5f;
        pressedColor[2] = 0.5f;
    }

    bool isInside(float x, float y) const // �����Ƿ��ڰ�ť��
    {
        return x >= x1 && x <= x2 && y >= y1 && y <= y2;
    }

    void drawButton() const;
};

#endif



