#include "../Include/tank.hpp"

Player::Player(int _num_player, int vel, float _rotation, int x, int y, int _max_bullets, Physics* _fisica, ALLEGRO_TIMER *_timer, Bot** _bots, Bullet** _orf_bullets) : Tank(vel, _rotation, x ,y, _max_bullets, _timer,  _orf_bullets), Sounds("References/sounds/fire.ogg")
{
    num_player = _num_player;
    fisica = _fisica;
    bots = _bots;
    type = 0;

    if(num_player == 1)
    {
        img[0] = al_load_bitmap("References/Tank1-1.png"); //Imagem 1 player 1.
        img[1] = al_load_bitmap("References/Tank1-2.png"); //Imagem 2 player 1.
    }

    else if(num_player == 2)
    {
        img[0]= al_load_bitmap("References/Tank2-1.png"); //Imagem player 2.
        img[1]= al_load_bitmap("References/Tank2-2.png"); //Imagem player 2.
    }

    if(!img[0])
        cout << "Failed to create a tank" << endl;

    if(!img[1])
        cout << "Failed to create a tank" << endl;

    img_imortal[0] = al_load_bitmap("References/tankImortal1-1.png");
    if(!img_imortal[0])
        cout << "Failed to load tank imortal1-1" << endl;

    img_imortal[1] = al_load_bitmap("References/tankImortal1-2.png");
    if(!img_imortal[1])
        cout << "Failed to load tank imortal1-2" << endl;

    currFrame = 0;
    lifes = 2; //Numero de vidas do tank.
}

void Player::playerKeyboard(Keyboard *keyboard, char**posicoes, Player *_player, int ident)
{
    if(ident == 1) // Confere se é o player1
    {
        if(keyboard->keys[keyboard->KEY_W] == 1) //Move para cima
            mover(0, -velocidade, posicoes, 0, _player);

        else if(keyboard->keys[keyboard->KEY_S] == 1)//Move para baixo
            mover(0, velocidade, posicoes, PI, _player);

        else if(keyboard->keys[keyboard->KEY_A] == 1)//Move para esquerda
            mover(-velocidade ,0, posicoes, (3*PI)/2, _player);

        else if(keyboard->keys[keyboard->KEY_D] == 1 ) //Move para direita
            mover(velocidade , 0, posicoes, PI/2, _player);
    }
    else if (ident == 2) // Confere se é o player2
    {
        if(keyboard->keys[keyboard->KEY_UP] == 1) //Move para cima
            mover(0, -velocidade, posicoes, 0, _player);

        else if(keyboard->keys[keyboard->KEY_DOWN] == 1)//Move para baixo
            mover(0, velocidade, posicoes, PI, _player);

        else if(keyboard->keys[keyboard->KEY_LEFT] == 1)//Move para esquerda
            mover(-velocidade ,0, posicoes, (3*PI)/2, _player);

        else if(keyboard->keys[keyboard->KEY_RIGHT] == 1 ) //Move para direita
            mover(velocidade , 0, posicoes, PI/2, _player);
    }
}

void Player::mover(int x, int y, char **matriz, float graus, Player *_player)
{
    if(!died) // Confere se o player está vivo
    {
        //Se ainda está no tempo de spawn o player não pode se mover.
        if(al_get_timer_count(timer) >= dt_spawn)
        {
            rotation = graus; // Configura a nova rotação

            position.x += x; // Move o player
            position.y += y;

            if(!multiplayer) // Verifica a existência do player2
            {
                // Confere uma possível colisão
                if(fisica->collisionTankMap(&position, matriz, TANK_TAM) ||
                   fisica->collisionTankBots(&position, bots))
                {
                    position.x -= x; // Caso haja alguma colisão o player volta para a posição anterior
                    position.y -= y;
                }
            }

            if(multiplayer && _player != NULL) //Caso esteja no modo multiplayer, detecta a colisão entre players
            {
                //Colisão entre players.
                if(fisica->collisionTankMap(&position, matriz, TANK_TAM) ||
                   fisica->collisionTankTank(&position, &(_player->position)) ||
                   fisica->collisionTankBots(&position, bots))
                {
                    position.x -= x; // Caso haja alguma colisão o player volta para a posição anterior
                    position.y -= y;
                }
            }

            // Faz o efeito de animação do movimento
            if(!sprites)
                sprites = 1;

            else if(sprites)
                sprites = 0;
        }
    }
}

void Player::draw(char**posicoes)
{

    if(!died)
    {
        //Se ainda está no tempo de spawn o player não é desenhado.
        if(al_get_timer_count(timer) >= dt_spawn)
        {
            //Muda os sprites a cada movimentação do tank, para dar sensação de movimento.
                al_draw_scaled_rotated_bitmap(img[sprites], 7, 7, position.x, position.y, MAP_MULT*1.5, MAP_MULT*1.5, rotation, 0);
        }
        else
            spawnTank();
    }

    giveImortality(); //Confere se o player deve estar imortal.
    die(); //Confere se o player morreu.
}

void Player::atirar(Player *_player)
{
    if(!died)
    {
        if(al_get_timer_count(timer) >= dt_spawn) // Não permite que o player atire antes de spawnar
        {
            for(int i = 0; i < max_bullets; i++)
            {
                if(num_bullets[i] == 0) // Procura uma posição vazia no vetor de balas
                {
                    bullets[i] = new Bullet(position.x, position.y, rotation, _player, i, fisica); // Cria uma nova bala nessa posição
                    bullets[i]->multiplayer = multiplayer;
                    num_bullets[i] = 1; // Avisa que existe uma bala na posição
                    playSound(); // Toca o som do tiro
                    break;
                }
            }
        }
    }
}

void Player::giveImortality()
{
    //Se está dentro do tempo pre determinado e depois do tempo de spawn o tanque fica imortal.
    if(al_get_timer_count(timer) <= dt_imortal && al_get_timer_count(timer) >= dt_spawn)
    {
        imortal = true;

        //Se o timer for divisivel por 8, que é um valor determinado, trocamos o frame.
        if(al_get_timer_count(timer)%8 == 0)
        {
            if(currFrame)
                currFrame = 0;

            else if(!currFrame)
                currFrame = 1;
        }

        //Mostra o frame de imortalidade de acordo com frame selecionado anteriormente.
        al_draw_scaled_rotated_bitmap(img_imortal[currFrame], 8, 8, position.x, position.y, MAP_MULT*1.5, MAP_MULT*1.5, rotation, 0);
    }

    //Se não e se ja passou do tempo de spawn ele pode ser destruido.
    else if(al_get_timer_count(timer) >= dt_spawn)
        imortal = false;
}

void Player::die()
{
    if(died && lifes != -1) //Se o player tomou dano e morreu suas condições são reiniciadas e não era sua ultima vida.
    {
        if(explodeTank())
        {
            // Passa todas as balas para o vetor de balas órfãs
            for(int i = 0; i < max_bullets; i++)
            {
                if(bullets[i] != NULL)
                {
                    for(int j = 0; j < TAM_ORF; j++)
                    {
                        if(orf_bullets[j] == NULL)
                        {
                            bullets[i]->tank = NULL;
                            orf_bullets[j] = bullets[i];
                            break;
                        }
                    }
                }
            }

            restart(bots, orf_bullets); // Reinicia o player
            died = 0; //E a flag volta a ser zero.
        }
    }

}

void Player::restart(Bot** _bots, Bullet** _orf_bullets)
{
    bots = _bots;
    orf_bullets = _orf_bullets;

     //Timers são recomeçados.
    dt_spawn = al_get_timer_count(timer) + 40;
    dt_imortal = al_get_timer_count(timer) + 80;

    //Posições e rotações são colocados novamente na condição inicial.
    position.y = (SCREEN_H*MAP_MULT - 15*MAP_MULT);
    rotation = 0;

    // Limpa todo o vetor de balas
    for(int i = 0; i < max_bullets; i++)
    {
        bullets[i] = NULL;
        num_bullets[i] = 0;
    }

    // Configura a posição em x do spawn dependendo de qual player
    if(num_player == 1)
        position.x = (160*MAP_MULT);

    else if(num_player == 2)
        position.x = (256*MAP_MULT);
}

Player::~Player()
{
    // Desenha a animação de explosão
    al_draw_scaled_rotated_bitmap(explosion[0], 8, 8, position.x, position.y, MAP_MULT, MAP_MULT, rotation, 0);
    al_draw_scaled_rotated_bitmap(explosion[1], 8, 8, position.x, position.y, MAP_MULT, MAP_MULT, rotation, 0);

    al_destroy_bitmap(img[0]);
    al_destroy_bitmap(img[1]);

    delete[] bullets;
}

