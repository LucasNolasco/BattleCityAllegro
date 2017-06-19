#include "powerup.hpp"

class ShovelPU : public PowerUp
{
    public:
        void applyPowerUp(Player* player, Bot** bots, int* ShovelPU, int time, int* shovelpu);
        ShovelPU();
        ~ShovelPU();
};

