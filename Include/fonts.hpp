#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "physics.hpp"

class Fonts
{
    ALLEGRO_FONT *font;

public:
    Fonts(int tam);
    ~Fonts();
    void drawFont(int x, int y,  int r, int g, int b, const char *_text);
};
