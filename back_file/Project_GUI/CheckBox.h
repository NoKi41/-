#ifndef CHECKBOX_H
#define CHECKBOX_H

class CheckBox
{
public:
    CheckBox(float x, float y, float size, bool checked = false);

    void toggle();
    void drawBox() const;
    bool isChecked() const;

private:
    float x, y, size;
    bool checked;
};

#endif // CHECKBOX_H
