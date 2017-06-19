#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "physics.hpp"

using namespace std;


class Starter
{
public:
    ALLEGRO_DISPLAY** display;
    ALLEGRO_EVENT_QUEUE** event_queue;
    ALLEGRO_TIMER** timer;
    ALLEGRO_BITMAP *mapa;
    ALLEGRO_SAMPLE *som;

    Starter(ALLEGRO_DISPLAY** _display, ALLEGRO_EVENT_QUEUE** _event_queue, ALLEGRO_TIMER** _timer);
    ~Starter();
    int start();
};
