#include "../Include/clockpu.hpp"

ClockPU::ClockPU() : PowerUp("References/clockpu.png")
{

}

ClockPU::~ClockPU()
{

}

void ClockPU::applyPowerUp(Player* player, Bot** bots, int* clockpu, int time, int* shovelpu)
{
    *clockpu = time + 100; // Ajusta a variável para o tempo atual + 10 segundos
}
