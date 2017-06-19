#include "../Include/shovelpu.hpp"

ShovelPU::ShovelPU() : PowerUp("References/shovelpu.png")
{

}

ShovelPU::~ShovelPU()
{

}

void ShovelPU::applyPowerUp(Player* player, Bot** bots, int* ShovelPU, int time, int* shovelpu)
{
    *shovelpu = time + 150; // Ajusta a variavel para o tempo atual + 15 segundos
}
