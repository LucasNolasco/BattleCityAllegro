#include "../Include/powerup.hpp"

int PowerUp::max_powerups = 5; // Numero maximo de powerups aparecendo simultaneamente na tela

PowerUp::PowerUp(const char* imagem)
{
    img = al_load_bitmap(imagem); // Carrega o sprite do power up

    if(!img) // Avisa caso tenha ocorrido algum erro durante o carregamento
        cout << "Failed to load PowerUp img " << imagem << endl;
}

PowerUp::~PowerUp()
{
    al_destroy_bitmap(img); // Destroi o sprite do powerup
}

void PowerUp::draw()
{
    al_draw_scaled_rotated_bitmap(img, 7, 7, position.x, position.y, MAP_MULT * 1.5, MAP_MULT * 1.5, 0, 0); // Desenha o powerup na tela
}
