#include "../Include/bombpu.hpp"

BombPU::BombPU(FinalScreen *_finalscreen) : PowerUp("References/bombpu.png")
{
    finalscreen = _finalscreen; //Recebe a finalscreen pra atualizar o highscore.
    //Adiciona o som de explosão
    explosion_sound = new Sounds("References/sounds/explosion.ogg");
}

BombPU::~BombPU(){}

void BombPU::applyPowerUp(Player* player, Bot** bots, int* clockpu, int time, int* shovelpu)
{
    // Loop para acessar todas as posições do vetor de bots
    for(int i = 0; i < Bot::max_bots; i++)
    {
        // Confere se existe um bot nessa posição
        if(bots[i] != NULL)
        {
            bots[i]->lifes = -1; // Ajusta a vida do bot para -1
            bots[i]->dt_explode = time + 15; // Ajusta o tempo de explosão
            explosion_sound->playSound();
            if(bots[i]->type)
                finalscreen->addHighscore(200, player->num_player);
            else
                finalscreen->addHighscore(100, player->num_player);
        }
    }
}
