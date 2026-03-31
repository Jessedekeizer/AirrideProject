#include "Button.h"

void Button::CheckTouch(int touchX, int touchY)
{
  if (touchX >= x && touchX <= x + width && touchY >= y && touchY <= y + height)
  {
    OnPress(true);
    return;
  }
  OnPress(false);
}

void Button::ReleaseButton()
{
  OnPress(false);
}
