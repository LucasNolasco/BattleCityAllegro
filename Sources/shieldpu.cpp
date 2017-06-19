#include "../Include/shieldpu.hpp"

ShieldPU::ShieldPU() : PowerUp("References/shieldpu.png")
{

}

ShieldPU::~ShieldPU()
{

}

void ShieldPU::applyPowerUp(Player* player, Bot** bots, int* clockpu, int time, int* shovelpu)
{
    player->dt_imortal = time + 80; // Ajusta o tempo de imortalidade para o tempo atual + 8 segundos
    player->giveImortality(); // Torna o player imortal
}
