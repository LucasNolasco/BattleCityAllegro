#include "../Include/keyboard.hpp"

Keyboard::Keyboard()
{
    //Reinicia o teclado.
    resetKeyboard();
}

Keyboard::~Keyboard(){}

void Keyboard::waitKey()
{
    //Cria uma fila de eventos.
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    if(event_queue)
    {
        //Regista o telcado na fila de eventos.
        al_register_event_source(event_queue,al_get_keyboard_event_source());

        //Cria um loop infinito até uma tecla ser pressioada.
        while(1)
        {
            //Cria um objeto evento para guardar a informação do evento.
            ALLEGRO_EVENT ev;
            //Espera por um evento.
            al_wait_for_event(event_queue,&ev);

            checKeyboard(&ev);
            //Se ESC for pressionado.
            if(keys[KEY_ESC] == 1)
                break;
        }
        //Destroi a fila de eventos.
        al_destroy_event_queue(event_queue);
    }
}

void Keyboard::resetKeyboard()
{
    //Zera o vetor das teclas.
    for(int i = 0; i < NUM_KEYS; i++)
        keys[i] = 0;
}

void Keyboard::checKeyboard(ALLEGRO_EVENT *ev)
{
    /*Se alguma tecla foi pressionada, analisamos
    pra ver qual foi e o vetor de teclas é atualizado */
    if(ev->type == ALLEGRO_EVENT_KEY_DOWN)
    {
        switch(ev->keyboard.keycode)
        {
        case ALLEGRO_KEY_UP:
            keys[KEY_UP] = 1;
            break;

        case ALLEGRO_KEY_DOWN:
            keys[KEY_DOWN] = 1;
            break;

        case ALLEGRO_KEY_LEFT:
            keys[KEY_LEFT] = 1;
            break;

        case ALLEGRO_KEY_RIGHT:
            keys[KEY_RIGHT] = 1;
            break;

        case ALLEGRO_KEY_L:
            keys[KEY_L] = 1;
            break;

        case ALLEGRO_KEY_ESCAPE:
            keys[KEY_ESC] = 1;
            break;

        case ALLEGRO_KEY_ENTER:
            keys[KEY_ENTER] = 1;
            break;

        case ALLEGRO_KEY_W:
            keys[KEY_W] = 1;
            break;

        case ALLEGRO_KEY_S:
            keys[KEY_S] = 1;
            break;

        case ALLEGRO_KEY_A:
            keys[KEY_A] = 1;
            break;

        case ALLEGRO_KEY_D:
            keys[KEY_D] = 1;
            break;

        case ALLEGRO_KEY_SPACE:
            keys[KEY_SPACE] = 1;
            break;

        }
    }
    /*Se alguma tecla foi "solta", analisamos pra
    ver qual foi e o vetor de teclas é atualizado */
    else if(ev->type == ALLEGRO_EVENT_KEY_UP)
    {
        switch(ev->keyboard.keycode)
        {
        case ALLEGRO_KEY_UP:
            keys[KEY_UP] = 0;
            break;

        case ALLEGRO_KEY_DOWN:
            keys[KEY_DOWN] = 0;
            break;

        case ALLEGRO_KEY_LEFT:
            keys[KEY_LEFT] = 0;
            break;

        case ALLEGRO_KEY_RIGHT:
            keys[KEY_RIGHT] = 0;
            break;

        case ALLEGRO_KEY_L:
            keys[KEY_L] = 0;
            break;

        case ALLEGRO_KEY_ESCAPE:
            keys[KEY_ESC] = 0;
            break;

        case ALLEGRO_KEY_ENTER:
            keys[KEY_ENTER] = 0;
            break;

        case ALLEGRO_KEY_W:
            keys[KEY_W] = 0;
            break;

        case ALLEGRO_KEY_S:
            keys[KEY_S] = 0;
            break;

        case ALLEGRO_KEY_A:
            keys[KEY_A] = 0;
            break;

        case ALLEGRO_KEY_D:
            keys[KEY_D] = 0;
            break;

        case ALLEGRO_KEY_SPACE:
            keys[KEY_SPACE] = 0;
            break;
        }
    }
}
