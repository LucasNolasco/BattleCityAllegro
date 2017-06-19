#pragma once

#include "physics.hpp"
#include "position.hpp"
#include "mapa.hpp"
#include "bullet.hpp"
#include "keyboard.hpp"
#include "sounds.hpp"
#include "finalScreen.hpp"


class Bullet;
class Mapa;
class Physics;
class Bot;

class Tank{
protected:
    ALLEGRO_BITMAP *img[2];
    ALLEGRO_BITMAP *explosion[2];
    ALLEGRO_TIMER *timer;
    int *num_bullets;
    Bot** bots;
    Bullet** orf_bullets;

public:
    ALLEGRO_BITMAP *spawn[3];
    int frame;
    int frame2;
    int dt_spawn;
    int dt_explode;
    bool imortal;
    bool died;
    int lifes;
    int type;
    int velocidade, sprites;

    static bool multiplayer;
    const int max_bullets;
    Position position;
    float rotation;
    Bullet **bullets;

    Tank(int vel, float _rotation, int x, int y, int _max_bullets, ALLEGRO_TIMER *timer, Bullet** _orf_bullets);
    virtual ~Tank();
    void bulletDestroyer(int index);
    void spawnTank();
    int explodeTank();
    int explodeTank(FinalScreen *finalScreen);
    virtual void takeDamage();
};


class Player : public Tank, public Sounds
{
    ALLEGRO_BITMAP *img_imortal[2];
    Physics* fisica;
    int highscore;

public:
    int currFrame;
    int dt_imortal;
    int num_player;

    Player(int _num_player, int vel, float _rotation, int x, int y, int _max_bullets, Physics* _fisica, ALLEGRO_TIMER *_timer, Bot** bots, Bullet** _orf_bullet);
    ~Player();
    void playerKeyboard(Keyboard *keyboard, char**posicoes, Player *_player, int ident);
    void mover(int x, int y, char **matriz, float graus, Player *_player);
    void draw(char**posicoes);
    void giveImortality();
    void atirar(Player *_player);
    void die();
    void restart(Bot** _bots, Bullet** _orf_bullets);
};

class Bot : public Tank, public Sounds
{
    ALLEGRO_BITMAP *sprites_red[2];
    Position vetor, objetivo_aleatorio, last_position;
    Physics* fisica;
    int objetivo;
    int histerese;
    int parado;

    enum OBJETIVOS{
        CASTELO,
        JOGADOR1,
        JOGADOR2,
        ALEATORIO
    };

public:

    static int max_bots;
    static int num_bots;
    static int cont_bots;
    static int dt_spawn_bots;
    static int spawnRed;
    static int totalBots;
    int dt_pisca;

    Bot(int _type, int vel, float _rotation, int x, int y, int _max_bullets, Physics* _fisica, ALLEGRO_TIMER *timer, Bot** _bots, Bullet** _orf_bullets);
    ~Bot();
    int correcaoGiro();
    void correcaoCaminho();
    int acertaPlayer(Player* _player1, Player* _player2);
    int reduzDistanciaY(Player* _player1, Player* _player2, char** matriz);
    int reduzDistanciaX(Player* _player1, Player* _player2, char** matriz);
    void mover(int x, int y, char **matriz, float graus, Player *_player, Player *_player2);
    int calculaMovimento(char** matriz, Player *_player1, Player *_player2);
    void draw();
    void atirar(Bot *_bot);
};


