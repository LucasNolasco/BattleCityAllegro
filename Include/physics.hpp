#pragma once

#include "sounds.hpp"
#include <iostream>
#include "position.hpp"
#include "tank.hpp"
#include "powerup.hpp"

#define SCREEN_W 520
#define SCREEN_H 416
#define MAP_MULT 1.5 //Controla o tamanho da tela.
#define FONT_TAM 30
#define MAP_TAM 16
#define BOT_TAM 15.5
#define TANK_TAM 12
#define BULLET_TAM 3
#define MAX_PARADO 30
# define TAM_ORF 3 * Bot::max_bots + 3 * 2

#define FPS 30

#define PI 3.1415

class Tank;
class Player;
class Bot;
class Bullet;
class HQ;
class PowerUp;

class Physics : Sounds
{
public:
    Physics();
    int collision(Position* object1, Position* object2, int tamanho1, int tamanho2);
    int collisionTankTank(Position* player1, Position* player2);
    int collisionTankBots(Position* player, Bot** bots);
    int collisionPowerUp(Position* player, PowerUp** powerups);
    int collisionTankMap(Position* player, char** matriz, int tamanho);
    int collisionBulletMap(Bullet* bullet, char** matriz);
    Tank* collisionBullet(Bullet* bullet, int index, Player* player1, Player* player2, Bot** bots, char** matriz, HQ* hq);
    Tank* collisionBullet(Bullet* bullet, Player* player, Bot** bots, char** matriz);
    Tank* collisionBullet(Bullet* bullet, Player* player1, Player* player2, Bot** bots, char** matriz);
};
