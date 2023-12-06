#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

enum SwitchButtonState { OFF, HOLD, ON };

class SwitchButton
{
public:
    float x1, y1, x2, y2;
    const char* label;
    void(*action)();

    SwitchButtonState state; // ��state��Ϊ��ĳ�Ա����
    int flag = -1; // ����״̬

    float offColor[3];       // ���ضϿ�ʱ����ɫ
    float holdColor[3];  // �����ͣʱ����ɫ
    float onColor[3];// ���رպ�ʱ����ɫ

    // ���캯��
    SwitchButton(float x1, float y1, float x2, float y2, const char* l, void(*a)())
    : x1(x1), y1(y1), x2(x2), y2(y2), label(l), action(a), state(OFF) // �������ʼ��state
    {
        offColor[0] = 0.9f; // ���ضϿ�ʱ����ɫ
        offColor[1] = 0.9f;
        offColor[2] = 0.9f;

        holdColor[0] = 0.7f; // �����ͣʱ����ɫ
        holdColor[1] = 0.7f;
        holdColor[2] = 0.7f;

        onColor[0] = 0.5f; // ���رպ�ʱ����ɫ
        onColor[1] = 0.5f;
        onColor[2] = 0.5f;
    }

    bool isInsideSwitchButton(float x, float y) const // �����Ƿ��ڿ�����
    {
        return x >= x1 && x <= x2 && y >= y1 && y <= y2;
    }

    void drawSwitchButton() const;
};

#endif



