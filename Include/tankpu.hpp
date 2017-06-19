#include "powerup.hpp"

class TankPU : public PowerUp
{
    public:
        TankPU();
        ~TankPU();
        void applyPowerUp(Player* player, Bot** bots, int* clockpu, int time, int* shovelpu);
};
