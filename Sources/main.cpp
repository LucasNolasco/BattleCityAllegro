#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <ctime>

#include "../Include/tank.hpp"
#include "../Include/bullet.hpp"
#include "../Include/hq.hpp"
#include "../Include/inGameMenu.hpp"
#include "../Include/interfaceMenu.hpp"
#include "../Include/keyboard.hpp"
#include "../Include/mapa.hpp"
#include "../Include/menu.hpp"
#include "../Include/position.hpp"
#include "../Include/starter.hpp"
#include "../Include/physics.hpp"
#include "../Include/controler.hpp"
#include "../Include/sounds.hpp"
#include "../Include/powerup.hpp"


#define PI 3.1415

using namespace std;

int main(int argc, char **argv)
{
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;

    Starter inicio(&display, &event_queue, &timer); //Starta o jogo inicialzando as coisas basicas.

    if(inicio.start() == -1) // Se não startar direito, termina a aplicação.
        return -1;

    Controler controler(timer);

    al_start_timer(timer);

    // Inicia o jogo desenho tanto o menu quanto a opção inicial dele.
    controler.menu->drawMenu();

    ALLEGRO_EVENT ev;

    while(ev.type != ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
        al_wait_for_event(event_queue, &ev);

        if(controler.gameControlsScreen)
        {
            controler.screenGameControl(&ev);
        }
        else if(controler.highscoreScreen)
        {
            controler.screenHighscore(&ev);
            al_flush_event_queue(event_queue);
        }
        //Abre o menu incial caso não tenha ninguem jogando.
        else if(!controler.jogo)
        {
            if(ev.type == ALLEGRO_EVENT_KEY_DOWN || ev.type == ALLEGRO_EVENT_KEY_UP)
            {
                if(controler.startMenu(&ev))
                    break;
            }
        }

        //Abre o menu dentro do jogo, caso as flags sejam verdadeiras.
        else if(controler.jogo && controler.game_menu->menu_in_game)
        {
            if(ev.type == ALLEGRO_EVENT_KEY_DOWN || ev.type == ALLEGRO_EVENT_KEY_UP)
            {
                controler.menuInGame(&ev);
            }
        }
        //Atualiza o jogo se assim for escolhido.
        else if(controler.jogo)
        {

            controler.update(&ev);//Inicia o jogo e atualiza tanto o mapra quanto as ações dos players

            //Quando um determinado tempo for alcançado a imagem dos objetos na tela são atualizadas.
            if(ev.type == ALLEGRO_EVENT_TIMER)
            {
                controler.draw(); //Desenha os itens na tela

                controler.deletePlayers(); //Caso alguem tenha morrido deleta o mesmo

                controler.nextLevel(event_queue); //Passa de nivel se for o momento certo

                al_flip_display();
            }
        }
    }

    return 0;
}

