#pragma once

#include "position.hpp"
#include "physics.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class Player;
class Bot;

class PowerUp
{
    ALLEGRO_BITMAP *img;

    enum {TANK, BOMB, SHIELD, CLOCK, SHOVEL};

public:
    Position position;
    static int max_powerups;

    PowerUp(const char* imagem);
    virtual ~PowerUp();
    void draw();
    virtual void applyPowerUp(Player* player, Bot** bots, int* clockpu, int time, int* shovelpu) = 0;
};
