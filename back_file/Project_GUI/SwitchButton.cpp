#include "SwitchButton.h"
#include <gl\GLUT.h>
#include <cstring>

void drawSwitchText(float x, float y, const char* text, void* font = GLUT_BITMAP_HELVETICA_12)
{
    glColor3f(0.0f, 0.0f, 0.0f); // ÎÄ±¾ÑÕÉ«
    glRasterPos2f(x, y);
    while (*text)
    {
        glutBitmapCharacter(font, *text);
        text++;
    }
}

void SwitchButton::drawSwitchButton() const
{
    switch (state)
    {
    case HOLD:
        glColor3f(holdColor[0], holdColor[1], holdColor[2]);
        break;
    case ON:
        glColor3f(onColor[0], onColor[1], onColor[2]);
        break;
    default:
        glColor3f(offColor[0], offColor[1], offColor[2]);
    }

    glRectf(x1, y1, x2, y2);
    float textX = (x1 + x2) / 2 - 3 * strlen(label);
    float textY = (y1 + y2) / 2 + 3;
    drawSwitchText(textX, textY, label);
}


