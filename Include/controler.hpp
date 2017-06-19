#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "physics.hpp"
#include "tank.hpp"
#include "bullet.hpp"
#include "interfaceMenu.hpp"
#include "menu.hpp"
#include "inGameMenu.hpp"
#include "sounds.hpp"
#include "fonts.hpp"
#include "finalScreen.hpp"
#include "powerup.hpp"
#include "bombpu.hpp"
#include "clockpu.hpp"
#include "tankpu.hpp"
#include "shieldpu.hpp"
#include "shovelpu.hpp"

class Controler{

    ALLEGRO_TIMER *timer;
    Fonts *font1;
    Fonts *font2;
    Sounds *gamestart_sound;
    Sounds *gameover_sound;
    Sounds *powerup_sound;
    FinalScreen *finalscreen;
    Physics* fisica;
    Mapa *mapinha;
    Keyboard *keyboard;
    Player *player1, *player2;
    Bot** bots;
    Bullet* balinha;
    Bullet** orf_bullets;
    PowerUp** powerups;
    char **posicoes; //Matriz usada ora criação e detecção de colisões no mapa.
    int curr_level, dt_clockpu, dt_shovelpu;

public:

    Menu *menu;
    bool jogo; //Flag usada pra identificar se o jogo foi iniciado no menu.
    bool multiplayer;
    bool gameControlsScreen;
    bool highscoreScreen;
    InGameMenu *game_menu;

    Controler(ALLEGRO_TIMER *_timer);
    ~Controler();
    int startMenu(ALLEGRO_EVENT *ev);
    void menuInGame(ALLEGRO_EVENT *ev);
    void startGame(bool multiplayer);
    void screenGameControl(ALLEGRO_EVENT *ev);
    void screenHighscore(ALLEGRO_EVENT *ev);
    void update(ALLEGRO_EVENT *ev);
    void updatePosicoes(char** posicoes, int dt_shovelpu);
    void draw();
    void spawnBots();
    void spawnPowerUp();
    void nextLevel(ALLEGRO_EVENT_QUEUE *event_queue);
    void deletePlayers();
    void deleteAll();
    void gameover(ALLEGRO_EVENT_QUEUE *event_queue);

};
