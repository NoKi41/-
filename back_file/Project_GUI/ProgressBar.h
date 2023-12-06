#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

class ProgressBar
{
public:
    ProgressBar(float x, float y, float width, float height, float min, float max);

    void setValue(float value);
    void drawProgressBar() const;
    float max;

private:
    float x, y, width, height;
    float min, currentValue;
};

#endif // PROGRESSBAR_H
