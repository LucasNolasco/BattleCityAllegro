#include "../Include/interfaceMenu.hpp"
#include "../Include/inGameMenu.hpp"

#define PI 3.1415

InGameMenu::InGameMenu(Keyboard *_keyboard)
{
     /* Inicializa tanto a variavel option quanto as imagens que são
    usadas no fundo/background do menu e no "cursor" do mesmo. */
    option = 1;
    keyboard = _keyboard;

    background = al_load_bitmap("References/inGameMenu.png");
    if(!background)
        cout << "Failed to load inGameMenu" << endl;

    cursor = al_load_bitmap("References/Tank1-1.png");
    if(!cursor)
        cout << "Failed to load Cursor do inGameMenu" << endl;

    menu_in_game = false; //Flag que vai ser usada para sabermos se o menu dentro do jogo sera aberto.
    font = new Fonts(18);
}
InGameMenu::~InGameMenu()
{
    al_destroy_bitmap(background);
    al_destroy_bitmap(cursor);
}
void InGameMenu::drawMenu()
{
    //Desenha o menu no meio da tela do jogo.
    al_draw_scaled_rotated_bitmap(background,0, 0, 70*MAP_MULT, 133*MAP_MULT , MAP_MULT, MAP_MULT, 0,  0);

    //Desenha as opções do menu.
    font->drawFont(145*MAP_MULT, 120*MAP_MULT, 255, 255, 255, "CONTINUE");
    font->drawFont(145*MAP_MULT, 145*MAP_MULT, 255, 255, 255, "QUIT");

    //Muda o valor da opcao dependendo da tecla pressionada.
    if(keyboard->keys[keyboard->KEY_UP] == 1)
        option--;
    else if(keyboard->keys[keyboard->KEY_DOWN] == 1)
        option++;

    //Mantem a opção entre 1 e 2.
    if(option > 2)
        option = 2;
    else if(option < 1)
        option = 1;

    //Mostra na tela a opcao que esta sendo selecionada
    if(option == 1)
        al_draw_scaled_rotated_bitmap(cursor, 7, 7, 130*MAP_MULT, 188*MAP_MULT, MAP_MULT+0.5, MAP_MULT+0.5, PI/2, 0);
    else if(option == 2)
        al_draw_scaled_rotated_bitmap(cursor, 7, 7, 170*MAP_MULT, 226*MAP_MULT, MAP_MULT+0.5, MAP_MULT+0.5, PI/2, 0);

}

int InGameMenu::enterOption()
{
    // Se a tecla seleciona foi enter, retonar o valor da opcao selecionada.
    if(keyboard->keys[keyboard->KEY_ENTER] == 1 && option == 1)
        return 1;

    else if(keyboard->keys[keyboard->KEY_ENTER] == 1 && option == 2)
        return 2;

    else
        return 0;
}
