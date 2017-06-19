#include "powerup.hpp"

class ShieldPU : public PowerUp
{
    public:
        void applyPowerUp(Player* player, Bot** bots, int* clockpu, int time, int* shovelpu);
        ShieldPU();
        ~ShieldPU();
};
