#include "Slider.h"
#include <gl/GLUT.h>

void Slider::setValue(float value)
{
    currentValue = value;
    if (currentValue < minVal) currentValue = minVal;
    if (currentValue > maxVal) currentValue = maxVal;
}

void Slider::drawSlider() const
{
    // »æÖÆ»¬¿é¹ìµÀ
    glColor3f(0.5f, 0.5f, 0.5f);
    glRectf(x1, y1, x2, y2);

    // ¼ÆËã»¬¿éÎ»ÖÃ
    float sliderPosition = x1 + (currentValue - minVal) / (maxVal - minVal) * (x2 - x1);

    // »æÖÆ»¬¿é
    glColor3f(0.8f, 0.8f, 0.8f);
    glRectf(sliderPosition - 5, y1 - 10, sliderPosition + 5, y2 + 10);
}

bool Slider::isInside(float x, float y) const
{
    return x >= x1 && x <= x2 && y >= y1 - 10 && y <= y2 + 10;
}

void Slider::moveSlider(float x, float y)
{
    if (isInside(x, y))
    {
        float newValue = minVal + (x - x1) / (x2 - x1) * (maxVal - minVal);
        setValue(newValue);
    }
}