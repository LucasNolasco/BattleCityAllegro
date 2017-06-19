#pragma once

#include "interfaceMenu.hpp"

class InGameMenu : public InterfaceMenu
{
    int option;
    ALLEGRO_BITMAP *background;
    ALLEGRO_BITMAP *cursor;
    Keyboard *keyboard;
    Fonts* font;

public:

    bool menu_in_game;

    InGameMenu(Keyboard *_keyboard);
    virtual ~InGameMenu();
    void drawMenu();
    int enterOption();
};
