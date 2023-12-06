#ifndef SLIDER_H
#define SLIDER_H

class Slider
{
public:
    float x1, y1, x2, y2; // �����λ�úͳߴ�
    float minVal, maxVal; // ��Сֵ�����ֵ
    float currentValue;   // ��ǰֵ

    Slider(float x1, float y1, float x2, float y2, float minVal, float maxVal)
        : x1(x1), y1(y1), x2(x2), y2(y2), minVal(minVal), maxVal(maxVal), currentValue((maxVal + minVal)/2) {}

    void setValue(float value); // ���û����ֵ
    void drawSlider() const;    // ���ƻ���
    bool isInside(float x, float y) const; // �����Ƿ��ڻ�����
    void moveSlider(float x, float y); // �ƶ�����
};

#endif // SLIDER_H

