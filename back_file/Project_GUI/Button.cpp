#include "Button.h"
#include <gl\GLUT.h>
#include <cstring>

void drawText(float x, float y, const char* text, void* font = GLUT_BITMAP_HELVETICA_12)
{
    glColor3f(0.0f, 0.0f, 0.0f); // ÎÄ±¾ÑÕÉ«
    glRasterPos2f(x, y);
    while (*text)
    {
        glutBitmapCharacter(font, *text);
        text++;
    }
}

void Button::drawButton() const
{
    switch (state)
    {
    case HOVER:
        glColor3f(hoverColor[0], hoverColor[1], hoverColor[2]);
        break;
    case PRESSED:
        glColor3f(pressedColor[0], pressedColor[1], pressedColor[2]);
        break;
    default:
        glColor3f(color[0], color[1], color[2]);
    }

    glRectf(x1, y1, x2, y2);
    float textX = (x1 + x2) / 2 - 3 * strlen(label);
    float textY = (y1 + y2) / 2 + 3;
    drawText(textX, textY, label);
}


