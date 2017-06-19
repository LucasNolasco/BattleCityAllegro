#include "powerup.hpp"

class ClockPU : public PowerUp
{
    public:
        void applyPowerUp(Player* player, Bot** bots, int* clockpu, int time, int* shovelpu);
        ClockPU();
        ~ClockPU();
};

