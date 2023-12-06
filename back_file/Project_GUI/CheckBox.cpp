#include "CheckBox.h"
#include <gl/GLUT.h>

CheckBox::CheckBox(float x, float y, float size, bool checked)
    : x(x), y(y), size(size), checked(checked) {}

void CheckBox::toggle()
{
    checked = !checked;
}

void CheckBox::drawBox() const
{
    // �������
    glColor3f(1.0f, 1.0f, 1.0f);
    glRectf(x, y, x + size, y + size);

    // ���ѡ�У�����ڲ�
    if (checked) {
        glColor3f(0.0f, 0.0f, 0.0f);
        glRectf(x + 2, y + 2, x + size - 2, y + size - 2);
    }
}

bool CheckBox::isChecked() const
{
    return checked;
}
