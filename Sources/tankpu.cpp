#include "../Include/tankpu.hpp"

TankPU::TankPU() : PowerUp("References/tankpu.png")
{

}

TankPU::~TankPU()
{

}

void TankPU::applyPowerUp(Player* player, Bot** bots, int* clockpu, int time, int* shovelpu)
{
    player->lifes += 1; // Adiciona 1 vida ao player
}
