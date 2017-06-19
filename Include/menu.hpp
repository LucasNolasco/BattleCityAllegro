#pragma once

#include "interfaceMenu.hpp"

class Menu : public InterfaceMenu
{
    int option;
    ALLEGRO_BITMAP *background;
    ALLEGRO_BITMAP *cursor;
    Keyboard *keyboard;
    Fonts *font;

public:

    Menu(Keyboard *_keyboard);
    virtual ~Menu();
    void drawMenu();
    int enterOption();
};
