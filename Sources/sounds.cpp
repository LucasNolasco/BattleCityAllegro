#include "../Include/sounds.hpp"

Sounds::Sounds(const char* _sound)
{
    //Carrega o som na variavel.
    sound = al_load_sample(_sound);
}

Sounds::~Sounds()
{
    //Destroi o som.
    al_destroy_sample(sound);
}

void Sounds::playSound()
{
    //Toca o som uma só vez com volumes originais.
    al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
}
