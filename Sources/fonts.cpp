#include "../Include/fonts.hpp"

Fonts::Fonts(int tam)
{
    //Carrega a fonte usada no jogo de acordo com o tamanho recebido.
    font = al_load_ttf_font("References/prstart.ttf",tam*MAP_MULT, 0 );
    if (!font)
        cout << "Could not load prstart.ttf" << endl;
}
Fonts::~Fonts()
{
    //Destroi a fonte.
    al_destroy_font(font);
}
void Fonts::drawFont(int x, int y, int r, int g, int b, const char *_text)
{
    //Desenha a fonte na tela com as cores o tamanho e o texto desejado, alinhada no centro.
    al_draw_text(font, al_map_rgb(r,g,b), x*MAP_MULT, y*MAP_MULT ,ALLEGRO_ALIGN_CENTRE, _text);
}
