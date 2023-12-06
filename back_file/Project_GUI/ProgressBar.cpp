#include "ProgressBar.h"
#include <gl/GLUT.h>

ProgressBar::ProgressBar(float x, float y, float width, float height, float min, float max)
    : x(x), y(y), width(width), height(height), min(min), max(max), currentValue(min) {}

void ProgressBar::setValue(float value)
{
    currentValue = value;
    if (currentValue < min) currentValue = min;
    if (currentValue > max) currentValue = max;
}

void ProgressBar::drawProgressBar() const
{
    // 绘制背景
    glColor3f(0.9f, 0.9f, 0.9f);
    glRectf(x, y, x + width, y + height);

    // 绘制前景（进度）
    float progress = (currentValue - min) / (max - min);
    glColor3f(0.0f, 1.0f, 0.0f);
    glRectf(x, y, x + width * progress, y + height);
}
