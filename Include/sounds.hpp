#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <iostream>

using namespace std;

class Sounds
{
    ALLEGRO_SAMPLE *sound;

public:
    Sounds(const char *_sound);
    ~Sounds();
    void playSound();
};

