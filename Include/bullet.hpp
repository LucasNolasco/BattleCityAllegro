#pragma once

#include "tank.hpp"
#include "hq.hpp"
#include "physics.hpp"
#include "keyboard.hpp"
#include "sounds.hpp"
#include "finalScreen.hpp"

class Physics;
class Tank;
class Player;
class Bot;
class HQ;
class FinalScreen;

class Bullet : public Sounds
{
    ALLEGRO_BITMAP *explosion;
    ALLEGRO_BITMAP *bullet;
    Position position;
    bool destruida;
    float rotation;
    int vel_x;
    int vel_y;
    int aux;
    Physics* fisica;

public:

    Tank *tank;
    int bullet_index;
    bool multiplayer;

    Bullet(Physics* fisica);
    Bullet(int x, int y, float _rotation, Tank *_tank, int _bullet_index, Physics* _fisica);
    ~Bullet();
    void mover(Player* player1, Player* player2, Bot** bots,  Bullet** orf_bullets, char **matriz, HQ *hq, FinalScreen *finalscreen);
    void draw();

    friend class Physics;
};

