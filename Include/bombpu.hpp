#include "powerup.hpp"
#include "finalScreen.hpp"
#include "sounds.hpp"

class BombPU : public PowerUp
{
    Sounds *explosion_sound;
    FinalScreen *finalscreen;

public:
    void applyPowerUp(Player* player, Bot** bots, int* clockpu, int time, int* shovelpu);
    BombPU(FinalScreen *_finalscreen);
    ~BombPU();
};

