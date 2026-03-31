#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>

class Button
{
public:
  Button(int x, int y, int width, int height, int name, std::function<void(Button &)> callback = nullptr)
      : x(x), y(y), width(width), height(height), name(name), callback(callback) {};
  virtual void CheckTouch(int touchX, int touchY);
  virtual void ReleaseButton();
  int GetName() { return name; };

protected:
  virtual void OnPress(bool pressed) = 0;
  int x;
  int y;
  int width;
  int height;
  bool toggle = false;
  std::function<void(Button &)> callback;
  int name;
};

class PushButton : public Button
{
public:
  PushButton(int x, int y, int width, int height, int name, std::function<void(Button &)> callback = nullptr)
      : Button(x, y, width, height, name, callback) {};

protected:
  void OnPress(bool pressed) override
  {
    if (callback && pressed)
    {
      callback(*this);
    }
  }
};

class ToggleButton : public Button
{
public:
  ToggleButton(int x, int y, int width, int height, int name, std::function<void(Button &)> callback = nullptr)
      : Button(x, y, width, height, name, callback) {};

  bool GetState() const { return state; }

protected:
  void OnPress(bool pressed) override
  {
    if (pressed != state)
    {
      state = pressed;
      if (callback)
      {
        callback(*this);
      }
    }
  }

private:
  bool state = false;
};

#endif