#pragma once

#include <fstream>
#include <iostream>
#include <stdio.h>
#include "hq.hpp"
#include "tank.hpp"
#include "fonts.hpp"

class Blocks;
class HQ;
class Player;
class Fonts;

class Mapa{
    std::ifstream arquivo_nivel;
    Blocks *blocks;
    ALLEGRO_BITMAP *inf_background;
    ALLEGRO_BITMAP *icon_tank;
    ALLEGRO_BITMAP *icon_flag;
    ALLEGRO_BITMAP *icon_bot;
    int level;
    char** positions;

public:
    HQ *hq;

    Mapa(int _level);
    ~Mapa();
    HQ* getHQ();
    char** draw(Player *player1, Player *player2, Fonts *font, int num_bots);
    void drawBush();
};

class Blocks{
public:
    ALLEGRO_BITMAP* brick;
    ALLEGRO_BITMAP* block;
    ALLEGRO_BITMAP* water;
    ALLEGRO_BITMAP* bush;
    Blocks();
    ~Blocks();
};

