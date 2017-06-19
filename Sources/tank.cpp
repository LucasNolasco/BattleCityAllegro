#include "../Include/tank.hpp"

bool Tank::multiplayer = false;

Tank::Tank(int vel, float _rotation, int x, int y, int _max_bullets, ALLEGRO_TIMER *_timer, Bullet** _orf_bullet ) : max_bullets(_max_bullets)
{
    spawn[0] = al_load_bitmap("References/spawnTank1-1.png");
    if(!spawn[0])
        cout << "Failed to load tank spawn 1-1" << endl;

    spawn[1] = al_load_bitmap("References/spawnTank1-2.png");
    if(!spawn[1])
        cout << "Failed to load tank spawn 1-2" << endl;

    spawn[2] = al_load_bitmap("References/spawnTank1-3.png");
    if(!spawn[2])
        cout << "Failed to load tank spawn 1-3" << endl;

    explosion[0] = al_load_bitmap("References/tankExplosion1-1.png");
    if(!explosion[0])
        cout << "Failed to load tank explosion" << endl;

    explosion[1] = al_load_bitmap("References/tankExplosion1-2.png");
    if(!explosion[1])
        cout << "Failed to load tank explosion" << endl;

    sprites = 0; //A flag sprites é iniciada, e serve para mudar a imagem do tanque que é desenhada a cada vez.
    //Os atributos bascios são iniciados.
    velocidade = vel*MAP_MULT;
    position.x = x;
    position.y = y;
    rotation = _rotation;
    timer = _timer;
    frame = 0;
    frame2 = 0;
    died = false;
    imortal = 0;

    num_bullets = new int[max_bullets]; //Aloca o vetor que vai controlar se as balas serão ou não criadas.

    bullets = new Bullet*[max_bullets]; //Aloca o vetor que vai guardar as balas.


    for(int i = 0; i < max_bullets; i++) //Aponta cada bala para NULL, para que não apontem para o nada.
        bullets[i] = NULL;

    for(int i = 0; i < max_bullets; i++) //Zera o vetor num_bullets para sabermos que ainda não existem balas.
        num_bullets[i] = 0;

    orf_bullets = _orf_bullet;
}

void Tank::bulletDestroyer(int index)
{
    delete bullets[index]; //Deleta a bala na posição passada, pra desalocar a memoria.
    bullets[index] = NULL; //Aponta a bala pro nada pra ficar mais controlável.
    num_bullets[index] = 0; //Zera o vetor de controle de balas naquela posição.
}

void Tank::spawnTank()
{
    //Se está dentro do tempo pre determinado o tanque fica imortal.
    if(al_get_timer_count(timer) <= dt_spawn)
    {
        imortal = true; //Enquanto o tanque está nascendo ele fica imortal.

        //Se o timer for divisivel por 5, que é um valor determinado, trocamos o frame.
        if(al_get_timer_count(timer) % 5 == 0)
        {
            if(frame == 0)
                frame = 1;
            else if(frame == 1)
                frame = 2;
            else if(frame == 2)
                frame = 0;
        }
        //Mostra o frame de imortalidade de acordo com frame selecionado anteriormente.
        al_draw_scaled_rotated_bitmap(spawn[frame], 8, 8, position.x+2*MAP_MULT, position.y+2*MAP_MULT, MAP_MULT*1.5, MAP_MULT*1.5, rotation, 0);
    }
    else
        imortal = false;

}


void Tank::takeDamage()
{
    //Se o tank não estiver imortal ele perde uma vida e a flag morreu recebe 1;
    if(!imortal)
    {
        lifes--;
        died = true;
        dt_explode = al_get_timer_count(timer) + 15;
    }
}

int Tank::explodeTank()
{
    //Controla a anamação do explosão do tank.
    if(al_get_timer_count(timer) <= dt_explode)
    {
        imortal = true;
        //Se o timer for divisivel por 8, que é um valor determinado, trocamos o frame.
        if(al_get_timer_count(timer)%5 == 0)
        {
            if(frame2 == 0)
                frame2 = 1;
            else if(frame2 == 1)
                frame2= 0;
        }

        // Desenha o frame de explosão
        al_draw_scaled_rotated_bitmap(explosion[frame2], 16, 16, position.x, position.y, MAP_MULT, MAP_MULT, rotation, 0);

        return 0;
    }

    else
        return 1;
}

int Tank::explodeTank(FinalScreen *finalscreen)
{
    //Controla a anamação do explosão do tank.
    if(al_get_timer_count(timer) <= dt_explode && lifes >= -1)
    {
        //Se o timer for divisivel por 8, que é um valor determinado, trocamos o frame.
        if(al_get_timer_count(timer)%5 == 0)
        {
            if(frame2 == 0)
                frame2 = 1;

            else if(frame2 == 1)
                frame2= 0;
        }

        // Desenha o frame de explosão
        al_draw_scaled_rotated_bitmap(explosion[frame2], 16, 16, position.x, position.y, MAP_MULT, MAP_MULT, rotation, 0);

        // Confere se o tanque é do tipo que pisca
        if(type)
            finalscreen->drawPoint(200, position); // Desenha a pontuação na tela

        else
            finalscreen->drawPoint(100, position); // Desenha a pontuação na tela

        return 0;
    }

    else
        return 1;
}

Tank::~Tank()
{
    //Desenha os sprites de explosão dando um efeito de animação
    for(int i = 0; i < 3; i++)
        al_destroy_bitmap(spawn[i]);

    al_destroy_bitmap(explosion[0]);
    al_destroy_bitmap(explosion[1]);
}
