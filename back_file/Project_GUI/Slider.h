#ifndef SLIDER_H
#define SLIDER_H

class Slider
{
public:
    float x1, y1, x2, y2; // 滑块的位置和尺寸
    float minVal, maxVal; // 最小值和最大值
    float currentValue;   // 当前值

    Slider(float x1, float y1, float x2, float y2, float minVal, float maxVal)
        : x1(x1), y1(y1), x2(x2), y2(y2), minVal(minVal), maxVal(maxVal), currentValue((maxVal + minVal)/2) {}

    void setValue(float value); // 设置滑块的值
    void drawSlider() const;    // 绘制滑块
    bool isInside(float x, float y) const; // 检查点是否在滑块内
    void moveSlider(float x, float y); // 移动滑块
};

#endif // SLIDER_H

