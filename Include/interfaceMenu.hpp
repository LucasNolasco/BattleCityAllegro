#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include "physics.hpp"
#include "keyboard.hpp"

using namespace std;

class InterfaceMenu
{
public:
    virtual void drawMenu() = 0;
    virtual int enterOption() = 0;
};

