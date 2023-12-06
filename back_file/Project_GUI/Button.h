#ifndef BUTTON_H
#define BUTTON_H

enum ButtonState { NORMAL, HOVER, PRESSED };

class Button
{
public:
    float x1, y1, x2, y2;
    const char* label;
    void(*action)();

    ButtonState state; // 将state作为类的成员变量

    float color[3];       // 按钮的默认颜色
    float hoverColor[3];  // 鼠标悬停时的颜色
    float pressedColor[3];// 鼠标按下时的颜色

    // 构造函数
    Button(float x1, float y1, float x2, float y2, const char* l, void(*a)())
        : x1(x1), y1(y1), x2(x2), y2(y2), label(l), action(a), state(NORMAL) // 在这里初始化state
    {
        color[0] = 0.9f; // 默认颜色
        color[1] = 0.9f;
        color[2] = 0.9f;

        hoverColor[0] = 0.7f; // 鼠标悬停时的颜色
        hoverColor[1] = 0.7f;
        hoverColor[2] = 0.7f;

        pressedColor[0] = 0.5f; // 鼠标按下时的颜色
        pressedColor[1] = 0.5f;
        pressedColor[2] = 0.5f;
    }

    bool isInside(float x, float y) const // 检查点是否在按钮内
    {
        return x >= x1 && x <= x2 && y >= y1 && y <= y2;
    }

    void drawButton() const;
};

#endif



