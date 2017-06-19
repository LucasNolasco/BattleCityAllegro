#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

#include "keyboard.hpp"
#include "physics.hpp"
#include "fonts.hpp"

typedef struct{
    int type1;
    int type2;
}TankTypes;

class FinalScreen
{
    ALLEGRO_BITMAP *points[2];

    Fonts *font, *font2, *font3;
    TankTypes highscore[2];
    TankTypes bots_destroyed[2];
    int highscore_total[2];
    char nome_score[4];
    int pos_char;
    ALLEGRO_BITMAP* seta;

public:

    int save;
    FinalScreen();
    ~FinalScreen();
    void saveScore(Keyboard* keyboard, ALLEGRO_EVENT* ev);
    void drawPoint(int points, Position position);
    void addHighscore(int score, int player);
    void drawFinalScreen(int stage);
    void restartValues();
};
