#include "../Include/starter.hpp"

Starter::Starter(ALLEGRO_DISPLAY** _display, ALLEGRO_EVENT_QUEUE** _event_queue, ALLEGRO_TIMER** _timer)
{
    //Passa as variaveis pra classe starter pra elas serem iniciadas.
    display = _display;
    event_queue = _event_queue;
    timer = _timer;
}
Starter::~Starter()
{
    al_destroy_timer(*timer);
    al_destroy_display(*display);
    al_destroy_event_queue(*event_queue);
}
int Starter::start()
{
    //Inicializa o allegro.
    if(!al_init())
    {
        cout << "failed to initialize allegro!\n";
        return -1;
    }

    //Instala o teclado para que sej possivel usa-lo.
    if(!al_install_keyboard())
    {
        cout << "failed to initialize the keyboard!\n";
        return -1;
    }

    //Inicializaos complemntos de fontes.
    al_init_font_addon();
    al_init_ttf_addon();

    //Inicializa o complemento de imagem.
    if(!al_init_image_addon())
    {
        cout << "failed to initialize the al_init_image_addon!\n";
        return -1;
    }

    //Inicializa os complemento de audio..
    if(!al_install_audio())
    {
        cout << "failed to initialize audio!\n" ;
        return -1;
    }
    if(!al_init_acodec_addon())
    {
        cout << "failed to initialize audio codecs!\n";
        return -1;
    }
    //Serve para tocar varios sons ao mesmo tempo.
    if (!al_reserve_samples(10))
    {
        cout << "failed to reserve samples!\n";
        return -1;
    }

    // Torna apto o uso de mouse na aplicação
    if (!al_install_mouse())
    {
        al_destroy_display(*display);
        al_destroy_timer(*timer);
        cout << "Falha ao inicializar o mouse.\n";
        return -1;
    }

    //Cria o display do jogo.
    *display = al_create_display(SCREEN_W*MAP_MULT, SCREEN_H*MAP_MULT);
    if(!display)
    {
        cout << "failed to create display!\n";
        al_destroy_timer(*timer);
        return -1;
    }

    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(*display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        al_destroy_display(*display);
        al_destroy_timer(*timer);
        cout << "Falha ao atribuir ponteiro do mouse.\n";
        return -1;
    }

    //Cria o timer do jogo, de acordo com o FPS definido na classe physics.
    *timer = al_create_timer(1.0 / FPS);
    if(!timer)
    {
        cout << "failed to create timer!\n";
        return -1;
    }

    al_set_target_bitmap(al_get_backbuffer(*display));

    //Cria a fila de eventos.
    *event_queue = al_create_event_queue();
    if(!event_queue)
    {
        cout << "failed to create event_queue!\n";
        al_destroy_display(*display);
        al_destroy_timer(*timer);
        return -1;
    }

    al_register_event_source(*event_queue, al_get_display_event_source(*display));
    al_register_event_source(*event_queue, al_get_timer_event_source(*timer));
    al_register_event_source(*event_queue, al_get_keyboard_event_source());
    al_register_event_source(*event_queue, al_get_mouse_event_source());

    return 0;
}
