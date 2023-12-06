#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

enum SwitchButtonState { OFF, HOLD, ON };

class SwitchButton
{
public:
    float x1, y1, x2, y2;
    const char* label;
    void(*action)();

    SwitchButtonState state; // 将state作为类的成员变量
    int flag = -1; // 开关状态

    float offColor[3];       // 开关断开时的颜色
    float holdColor[3];  // 鼠标悬停时的颜色
    float onColor[3];// 开关闭合时的颜色

    // 构造函数
    SwitchButton(float x1, float y1, float x2, float y2, const char* l, void(*a)())
    : x1(x1), y1(y1), x2(x2), y2(y2), label(l), action(a), state(OFF) // 在这里初始化state
    {
        offColor[0] = 0.9f; // 开关断开时的颜色
        offColor[1] = 0.9f;
        offColor[2] = 0.9f;

        holdColor[0] = 0.7f; // 鼠标悬停时的颜色
        holdColor[1] = 0.7f;
        holdColor[2] = 0.7f;

        onColor[0] = 0.5f; // 开关闭合时的颜色
        onColor[1] = 0.5f;
        onColor[2] = 0.5f;
    }

    bool isInsideSwitchButton(float x, float y) const // 检查点是否在开关内
    {
        return x >= x1 && x <= x2 && y >= y1 && y <= y2;
    }

    void drawSwitchButton() const;
};

#endif



