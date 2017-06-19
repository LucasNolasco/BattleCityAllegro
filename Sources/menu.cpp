#include "../Include/interfaceMenu.hpp"
#include "../Include/menu.hpp"

#define PI 3.1415

Menu::Menu(Keyboard *_keyboard)
{
    /* Inicializa tanto a variavel option quanto as imagens que são
    usadas no fundo/background do menu e no "cursor" do mesmo. */
    option = 1;
    keyboard = _keyboard;

    background = al_load_bitmap("References/menuBackground.png");
    if(!background)
        cout << "Failed to load Menu" << endl;

    cursor = al_load_bitmap("References/Tank1-1.png");
    if(!cursor)
        cout << "Failed to load Cursor do Menu" << endl;

    font = new Fonts(15); //Cria a fonte que será usada no menu.
}
Menu::~Menu()
{
    //Deleta os itens criados pelo menu.
    delete font;
    al_destroy_bitmap(background);
    al_destroy_bitmap(cursor);
}
//Metodo encarregado de desenhar o menu.
void Menu::drawMenu()
{
    /* Desenha a imagem de fundo, dividindo a escala dela por 4 para se ajustar a resolução da tela,
    visto que a imagem original é 4 vezes maior. */
    al_draw_scaled_rotated_bitmap(background, 0, 0, 0, 0,(MAP_MULT/4.0), (MAP_MULT/4.0), 0,  0);

    //Muda o valor da opcao dependendo da telca pressionada.
    if(keyboard->keys[keyboard->KEY_UP] == 1)
        option--;
    else if(keyboard->keys[keyboard->KEY_DOWN] == 1)
        option++;

    //Desenha as opções do menu.
    font->drawFont(SCREEN_W/2, 150*MAP_MULT, 255, 255, 255, "1 PLAYER");
    font->drawFont(SCREEN_W/2, 170*MAP_MULT, 255, 255, 255, "2 PLAYERS");
    font->drawFont(SCREEN_W/2, 190*MAP_MULT, 255, 255, 255, "HIGHSCORES");
    font->drawFont(SCREEN_W/2, 210*MAP_MULT, 255, 255, 255, "GAME CONTROLS");
    font->drawFont(SCREEN_W/2, 230*MAP_MULT, 255, 255, 255, "QUIT");

    //Mantem a opção entre 1 e 5
    if(option > 5)
        option = 5;

    if(option < 1)
        option = 1;

    //Atualiza a posicao do cursor na tela de acordo com a opção selecionada.
    if(option == 1)
        al_draw_scaled_rotated_bitmap(cursor, 0, 0, 200*MAP_MULT, 225*MAP_MULT, MAP_MULT, MAP_MULT, PI/2, 0);

    else if(option == 2)
        al_draw_scaled_rotated_bitmap(cursor, 0, 0, 190*MAP_MULT, 255*MAP_MULT, MAP_MULT, MAP_MULT, PI/2, 0);

    else if(option == 3)
        al_draw_scaled_rotated_bitmap(cursor, 0, 0, 180*MAP_MULT, 285*MAP_MULT, MAP_MULT, MAP_MULT, PI/2, 0);

    else if(option == 4)
        al_draw_scaled_rotated_bitmap(cursor, 0, 0, 160*MAP_MULT, 315*MAP_MULT, MAP_MULT, MAP_MULT, PI/2, 0);

    else if(option == 5)
        al_draw_scaled_rotated_bitmap(cursor, 0, 0, 225*MAP_MULT, 345*MAP_MULT, MAP_MULT, MAP_MULT, PI/2, 0);

    //Atualzia o display.
    al_flip_display();
}

//Metodo que retorna a opção selecionada pelo usuario.
int Menu::enterOption()
{
    // Se a tecla seleciona foi enter ele retorna o numero da opcao que está sendo selecionada no momento.
    if(keyboard->keys[keyboard->KEY_ENTER] == 1 && option == 1)
        return 1;

    else if(keyboard->keys[keyboard->KEY_ENTER] == 1 && option == 2)
        return 2;

    else if(keyboard->keys[keyboard->KEY_ENTER] == 1 && option == 3)
        return 3;

    else if(keyboard->keys[keyboard->KEY_ENTER] == 1 && option == 4)
        return 4;

    else if(keyboard->keys[keyboard->KEY_ENTER] == 1 && option == 5)
        return 5;

    else
        return 0;
}
