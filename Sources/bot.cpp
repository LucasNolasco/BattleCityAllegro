#include "../Include/tank.hpp"
#include <math.h>
#include <time.h>

int Bot::cont_bots = 0;
int Bot::num_bots = 5;
int Bot::max_bots = 3;
int Bot::dt_spawn_bots = 0;
int Bot::spawnRed = 2;
int Bot::totalBots = 5;

Bot::Bot(int _type, int vel, float _rotation, int x, int y, int _max_bullets, Physics* _fisica, ALLEGRO_TIMER *_timer, Bot** _bots, Bullet** _orf_bullets) : Tank(vel, _rotation, x ,y, _max_bullets, _timer, _orf_bullets), Sounds("References/sounds/fire.ogg")
{
    srand(time(NULL)); // Inicia a seed para o rand

    objetivo = rand() % 3; // Configura o objetivo inicial do bot
    type = _type;

    objetivo_aleatorio.x = rand() % (int)(384 * MAP_MULT); // Define uma posição aleatória no mapa
    objetivo_aleatorio.y = rand() % (int)(184 * MAP_MULT);

    bots = _bots;

    fisica = _fisica;
    lifes = 0; //Ou seja ele não pode renascer.
    imortal = 0; //Ele é sempre mortal.

    img[0] = al_load_bitmap("References/bot1-1.png");

    if(!img[0])
        cout << "Failed to create a bot" << endl;

    img[1] = al_load_bitmap("References/bot1-2.png");

    if(!img[1])
        cout << "Failed to create a bot" << endl;

    if(type==1)
    {
        sprites_red[0] = al_load_bitmap("References/botRed1-1.png");
        if(!sprites_red[0])
            cout << "Failed to create a bot" << endl;

        sprites_red[1] = al_load_bitmap("References/botRed1-2.png");
        if(!sprites_red[1])
            cout << "Failed to create a bot" << endl;

        lifes = 1;
    }

    histerese =  2 * BOT_TAM * MAP_MULT;
    parado = 0;

}

void Bot::correcaoCaminho()
{
    // Confere se o bot está parado
    if(parado >= MAX_PARADO)
    {
        objetivo = (objetivo + 1) % 4; // Configura um novo objetivo para o bot
        parado = 0;

        if(objetivo == ALEATORIO) // Confere se o objetivo é aleatório
        {
            objetivo_aleatorio.x = rand() % (int)(384 * MAP_MULT); // Gera uma nova posição aleatória no mapa
            objetivo_aleatorio.y = rand() % (int)(184 * MAP_MULT);
        }
    }

    else // Caso não esteja parado
        parado++; // Adiciona 1 na variável parado
}

int Bot::correcaoGiro()
{
    // Acerta a rotação do bot para virar na direção do objetivo
    if(vetor.x == 0 && vetor.y < 0)
        rotation = 0;

    else if(vetor.x == 0 && vetor.y > 0)
        rotation = PI;

    else if(vetor.x < 0 && vetor.y == 0)
        rotation = (3*PI)/2;

    else if(vetor.x > 0 && vetor.y == 0)
        rotation = PI/2;

    return 1;
}

int Bot::acertaPlayer(Player* _player1, Player* _player2)
{
    int rotacao = rotation;

    // Verifica a existência do player1
    if(_player1 != NULL)
    {
        // Verifica se o bot está na mesma coluna do player
        if(abs(position.x - _player1->position.x) < BOT_TAM)
        {
            // Caso esteja na mesma linha e com a rotação correta retorna 1 para atirar
            if(position.y > _player1->position.y && rotacao == 0)
                return 1;

            else if(position.y < _player1->position.y && rotacao == 3)
                return 1;
        }

        // Verifica se o bot está na mesma linha do player
        if(abs(position.y - _player1->position.y) < BOT_TAM)
        {
            // Caso esteja na mesma coluna e com a rotação correta retorna 1 para atirar
            if(position.x > _player1->position.x && rotacao == 4)
                return 1;

            else if(position.x < _player1->position.x && rotacao == 1)
                return 1;
        }
    }

    // Verifica a existência do player2
    if(_player2 != NULL)
    {
        // Verifica se o bot está na mesma coluna do player
        if(abs(position.x - _player2->position.x) < BOT_TAM)
        {
            // Caso esteja na mesma linha e com a rotação correta retorna 1 para atirar
            if(position.y > _player2->position.y && rotacao == 0)
                return 1;

            else if(position.y < _player2->position.y && rotacao == 3)
                return 1;
        }

        // Verifica se o bot está na mesma linha do player
        if(abs(position.y - _player2->position.y) < BOT_TAM)
        {
            // Caso esteja na mesma coluna e com a rotação correta retorna 1 para atirar
            if(position.x > _player2->position.x && rotacao == 4)
                return 1;

            else if(position.x < _player2->position.x && rotacao == 1)
                return 1;
        }
    }

    return 0;
}

int Bot::reduzDistanciaY(Player* _player1, Player* _player2, char** matriz)
{
    Position velocidade_x, posicao;

    velocidade_x.x = velocidade; // Define a velocidade em x
    velocidade_x.y = 0;

    // Confere se pode mover na direção positiva de y
    if(!fisica->collisionTankMap(&(posicao = position + velocidade), matriz, BOT_TAM) &&
       !fisica->collisionTankBots(&position, bots) && vetor.y > 0)
    {
            mover(0, velocidade, matriz, PI, _player1, _player2); // Move na direção positiva de y
            return 1;
    }

    // Confere se pode mover na direção negativa de y
    else if(!fisica->collisionTankMap(&(posicao = position - velocidade), matriz, BOT_TAM) &&
            !fisica->collisionTankBots(&position, bots) && vetor.y < 0)
    {
            mover(0, -velocidade, matriz, 0, _player1, _player2); // Move na direção negativa de y
            return 1;
    }

    return 0;
}

int Bot::reduzDistanciaX(Player* _player1, Player* _player2, char** matriz)
{
    Position velocidade_x, posicao;

    velocidade_x.x = velocidade; // Define a velocidade em x
    velocidade_x.y = 0;

    // Confere se pode mover na direção negativa de x
    if(!fisica->collisionTankMap(&(posicao = position - velocidade_x), matriz, BOT_TAM) &&
       !fisica->collisionTankBots(&position, bots) && vetor.x < 0)
    {
        mover(-velocidade, 0, matriz, (3*PI)/2, _player1, _player2); // Move na direção negativa de x
        return 1;
    }

    // Confere se pode mover na direção positiva de x
    else if(!fisica->collisionTankMap(&(posicao = position + velocidade_x), matriz, BOT_TAM) &&
            !fisica->collisionTankBots(&position, bots) && vetor.x > 0)
    {
        mover(velocidade, 0, matriz, PI/2, _player1, _player2); // Move na direção positiva de x
        return 1;
    }

    return 0;
}

int Bot::calculaMovimento(char** matriz, Player *_player1, Player *_player2)
{
    if(type == 1 && al_get_timer_count(timer) % 30 == 5)
        dt_pisca = al_get_timer_count(timer) + 15;

    if(lifes >= 0) //Só tenta se mover caso não esteja morto
    {
        if(_player1 != NULL) // Verifica se o player1 existe
            while((_player1->died && objetivo == JOGADOR1))
                objetivo = rand() % 4; // Caso o player1 exista e o objetivo seja o player1 um novo objetivo é gerado

        if(_player2 != NULL) // Verifica se o player1 existe
            while(_player2->died && objetivo == JOGADOR2)
                objetivo = rand() % 4; // Caso o player2 exista e o objetivo seja o player2 um novo objetivo é gerado

        // Calcula o novo vetor pra movimentação com base no objetivo do bot
        if(objetivo == JOGADOR1 && _player1 != NULL)
            vetor = _player1->position - position;

        else if(objetivo == JOGADOR2 && _player2 != NULL)
            vetor = _player2->position - position;

        else if(objetivo == CASTELO)
        {
            vetor.x = 208*MAP_MULT - position.x;
            vetor.y = 400*MAP_MULT - position.y;
        }

        else if(objetivo == ALEATORIO)
            vetor = objetivo_aleatorio - position;

        // Verifica se o bot se moveu
        if(position != last_position)
            parado = 0; // Caso tenha se movido a variavel parado vai para 0

        // Caso não tenha se movido a variavel recebe mais 1
        else
            parado++;

        last_position = position; // Salva a antiga posição

        // Confere se há algum possível problema com a movimentação
        if(vetor.x == 0 || vetor.y == 0 || parado >= MAX_PARADO)
            correcaoCaminho(); // Corrige o caminho tomado pelo bot

        int reduziu; // Variavel para identificar a tentativa de movimentação do bot

        // Confere se a maior distância é no eixo X
        if(abs(vetor.x) >= abs(vetor.y) + histerese)
        {
            // Reajusta a histerese
            if(histerese > 0)
                histerese *= -1;

            reduziu = reduzDistanciaX(_player1, _player2, matriz); // Tenta reduzir a distância em X

            if(!reduziu) // Caso não tenha reduzido em X tenta reduzir em Y
                reduziu = reduzDistanciaY(_player1, _player2, matriz);
        }

        // Caso a distância em Y seja maior
        else
        {
            // Reajusta a histerese
            if(histerese < 0)
                histerese *= -1;

            reduziu = reduzDistanciaY(_player1, _player2, matriz); // Tenta reduzir a distância em Y

            if(!reduziu) // Caso não tenha reduzido em Y, tenta reduzir em X
                reduziu = reduzDistanciaX(_player1, _player2, matriz);
        }

        // Caso não tenha reduzido e esteja na mesma direção do player
        if(!reduziu && (vetor.x == 0 || vetor.y == 0))
            return correcaoGiro(); // Ajusta a rotação do bot para ficar na direção do player

        // Caso não tenha reduzido e nem esteja na direção do player
        if(!reduziu && (vetor.x != 0 || vetor.y != 0))
            return 1; // Atira para destruir um possível obstáculo

        if(acertaPlayer(_player1, _player2)) // Confere se está na posição para acertar algum dos players
            return 1; // Atira para tentar acertar algum dos players
    }

    return 0;
}

void Bot::mover(int x, int y, char **matriz, float graus, Player *_player1, Player *_player2)
{
    //Se ainda está no tempo de spawn o bot não pode se mover.
    if(al_get_timer_count(timer) >= dt_spawn)
    {
        rotation = graus; // Atualiza a rotação

        position.x += x; // Move o bot em x
        position.y += y; // Move o bot em y


        if(_player1 != NULL) // Confere se existe player1
        {
            if(_player2 == NULL) // Confere se não existe player2
            {
                // Confere se há colisão
                if(fisica->collisionTankMap(&position, matriz, BOT_TAM) ||
                   fisica->collisionTankTank(&position, &(_player1->position)) ||
                   fisica->collisionTankBots(&position, bots))
                {
                    position.x -= x; // Caso haja colisão volta para a posição inicial
                    position.y -= y;
                }
            }
        }

        if(_player2 != NULL) // Confere se existe player2
        {
            if(_player1 == NULL) // Confere se não existe player1
            {
                // Confere se há colisão
                if(fisica->collisionTankMap(&position, matriz, BOT_TAM) ||
                   fisica->collisionTankTank(&position, &(_player1->position)) ||
                   fisica->collisionTankBots(&position, bots))
                {
                    position.x -= x; // Caso haja colisão volta para a posição inicial
                    position.y -= y;
                }
            }
        }

        // Confere se player1 e player2 existem
        if(_player2 != NULL && _player1 != NULL)
        {
            // Confere se há colisão
            if(fisica->collisionTankMap(&position, matriz, BOT_TAM) ||
               fisica->collisionTankTank(&position, &(_player1->position)) ||
               fisica->collisionTankTank(&position, &(_player2->position)) ||
               fisica->collisionTankBots(&position, bots))
            {
                position.x -= x; // Caso haja colisão o bot volta para a posição inicial
                position.y -= y;
            }
        }

        // Cria a animação de movimentação
        if(!sprites)
            sprites = 1;

        else if(sprites)
            sprites = 0;
    }

    // Caso ainda esteja no tempo de spawn o bot não é considerado parado
    else
        parado = 0;
}

void Bot::draw()
{
    //Se ainda está no tempo de spawn o bot não é desenhado.
    if(al_get_timer_count(timer) >= dt_spawn)
    {
        // Desenha o bot na tela
        if(type == 1 && al_get_timer_count(timer) <= dt_pisca)
        {
            al_draw_scaled_rotated_bitmap(sprites_red[sprites], 7, 7, position.x, position.y, MAP_MULT*1.5, MAP_MULT*1.5, rotation, 0);
        }
        else
            al_draw_scaled_rotated_bitmap(img[sprites], 7, 7, position.x, position.y, MAP_MULT*1.5, MAP_MULT*1.5, rotation, 0);
    }

    spawnTank();
}

void Bot::atirar(Bot *_bot)
{
    if(lifes >= 0 && al_get_timer_count(timer) >= dt_spawn)//Só pode atirar se não estivar morto.
    {
        for(int i = 0; i < max_bullets; i++)
        {
            if(num_bullets[i] == 0) // Procura uma posição vazia no vetor de balas
            {
                bullets[i] = new Bullet(position.x, position.y, rotation, _bot, i, fisica); // Cria uma nova bala
                num_bullets[i] = 1; // Avisa a existência de uma bala na posição
                break;
            }
        }
    }
}

Bot::~Bot()
{
    // Passa todas as balas do bot para o vetor de balas órfãs
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

    al_destroy_bitmap(img[0]);
    al_destroy_bitmap(img[1]);

    if(type)
    {
        al_destroy_bitmap(sprites_red[0]);
        al_destroy_bitmap(sprites_red[1]);
    }

    delete[] bullets;
}

