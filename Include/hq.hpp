#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>

#include "physics.hpp"

using namespace std;

class HQ
{
public:
    ALLEGRO_BITMAP* hq;
    ALLEGRO_BITMAP* game_over;
    int hp;
    bool gameover;

    HQ();
    ~HQ();
    void takeDamage();
};
