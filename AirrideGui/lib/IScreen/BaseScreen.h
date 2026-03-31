#ifndef BASESCREEN_H
#define BASESCREEN_H
#include "IScreen.h"

class BaseScreen : public IScreen {
public:
    BaseScreen(EScreen name, const char *path) : name(name), path(path), isActive(false) {};

    virtual ~BaseScreen() {
        for (Button *button: buttons) {
            delete button;
        }
        buttons.clear();
    }

    void HandleTouch(int touchX, int touchY) override {
        for (Button *button: buttons) {
            button->CheckTouch(touchX, touchY);
        };
    }

    void ReleaseButtons() override {
        for (Button *button: buttons) {
            button->ReleaseButton();
        }
    };

    EScreen GetName() override { return name; };

    const char *GetPath() override { return path; };

protected:
    EScreen name;
    const char *path;
    bool isActive;
    std::vector<Button *> buttons;
};

#endif //BASESCREEN_H