#pragma once

#include <allegro5/allegro.h>
#include <iostream>

#define NUM_KEYS 12

using namespace std;


class Keyboard
{
public:
    int keys[NUM_KEYS];
    enum KEYS{ KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_L, KEY_ESC, KEY_ENTER, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE};

    Keyboard();
    ~Keyboard();
    void waitKey();
    void resetKeyboard();
    void checKeyboard(ALLEGRO_EVENT *ev);
};

