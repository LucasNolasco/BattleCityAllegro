#include "../Include/bullet.hpp"

// Construtor Recurso Tecnico Avançado (gambiarra) para poder chamar a função mover no main
Bullet::Bullet(Physics* _fisica) : Sounds("References/sounds/explosion.ogg")
{
    fisica = _fisica;
}

Bullet::Bullet(int x, int y, float _rotation, Tank *_tank, int _bullet_index, Physics* _fisica) : Sounds("References/sounds/explosion.ogg")
{
    fisica = _fisica;

    bullet = al_load_bitmap("References/bullet.png"); // Carrega o sprite da bala
    if(!bullet)
        cout << "Failed to create a bullet" << endl;

    explosion = al_load_bitmap("References/bulletExplosion.png"); // Carrega o sprite de explosão da bala
    if(!explosion)
        cout << "Failed to create a explosion of bullet" << endl;

    rotation = _rotation;
    aux = rotation;
    destruida = false;
    tank = _tank;
    bullet_index = _bullet_index;

    //Dependendo do lado que a bala está indo ela sai de uma posição e com uma velocidade diferente.
    if(aux == 0)//Para cima.
    {
        position.x = x;
        position.y = y - BOT_TAM - 2;
        vel_x = 0;
        vel_y = -5*MAP_MULT;
    }
    else if(aux == 3)//Para baixo.
    {
        position.x = x;
        position.y = y + BOT_TAM - 2;
        vel_x = 0;
        vel_y = 5*MAP_MULT;
    }
    else if(aux == 4)//Para a esquerda.
    {
        position.x = x - BOT_TAM - 2;
        position.y = y;
        vel_x = -5*MAP_MULT;
        vel_y = 0;
    }
    else if (aux == 1)//Para a direita.
    {
        position.x = x + BOT_TAM - 2;
        position.y = y;
        vel_x = 5*MAP_MULT;
        vel_y = 0;
    }
}

Bullet::~Bullet()
{
    al_destroy_bitmap(bullet); // Destroi o bitmap da bala
    al_destroy_bitmap(explosion); // Destroi o bitmap da explosão
}

// Função para desenhar a bala na tela
void Bullet::draw()
{
    // Verifica se a bala foi destruida e é do player
    if (destruida == true && tank != NULL)
    {
        al_draw_scaled_rotated_bitmap(explosion, 5, 5, position.x, position.y, 1.5, 1.5, rotation, 0); // Desenha animação da explosão
        tank->bulletDestroyer(bullet_index); // Destroi a bala
    }

    // Verifica se a bala foi destruida e é órfã
    else if(destruida == true && tank == NULL)
    {
        al_draw_scaled_rotated_bitmap(explosion, 5, 5, position.x, position.y, 1.5, 1.5, rotation, 0); // Desenha animação da explosão
    }

    // Caso a bala não tenha sido destruida desenha o sprite da bala normalmente
    else
        al_draw_scaled_rotated_bitmap(bullet, 2, 1.5, position.x, position.y, MAP_MULT*1.5,MAP_MULT*1.5, rotation, 0);
}

// Movimenta todas as balas do jogo
void Bullet::mover(Player* player1, Player* player2, Bot** bots,  Bullet** orf_bullets, char **matriz, HQ *hq, FinalScreen *finalscreen)
{
    // Loop em todo vetor de balas do player1
    if(player1 != NULL)
    {
        for(int i = 0; i < player1->max_bullets; i++)
        {
            // Verifica se existe uma bala naquela posição do vetor
            if(player1->bullets[i] != NULL)
            {
                (player1->bullets[i])->position.x += (player1->bullets[i])->vel_x; // Move a bala em x
                (player1->bullets[i])->position.y += (player1->bullets[i])->vel_y; // Move a bala em y

                Tank* tank = fisica->collisionBullet(player1->bullets[i], player2, bots, matriz); // Verifica se colidiu com algum tanque

                if(tank != NULL) // Caso tenha ocorrido a colisão com algum tanque, a bala é destruida
                {
                    tank->takeDamage(); // Caso algum tanque seja atingido é dado o dano

                    if(tank->lifes == -1)//Para detectar a pontuação somente uma vez;
                    {
                        // Verifica se é o bot especial
                        if(tank->type)
                            finalscreen->addHighscore(200, 1); // Adiciona o ponto ao score
                        else
                            finalscreen->addHighscore(100, 1);
                    }

                    (player1->bullets[i])->destruida = true; // Tem que colocar o tanque sendo destruido aqui também
                    playSound(); // Toca o som da explosão da bala
                }

                player1->bullets[i]->draw(); // Desenha a bala do player na tela
            }
        }
    }


    // Verifica se a partida é multiplayer (se existe player2)
    if(player2 != NULL)
    {
        // Loop em todo vetor de balas de player2
        for(int i = 0; i < player2->max_bullets; i++)
        {
            // Verifica se tem uma bala na posição
            if(player2->bullets[i] != NULL)
            {
                player2->bullets[i]->position.x += (player2->bullets[i])->vel_x; // Move a bala em x
                player2->bullets[i]->position.y += (player2->bullets[i])->vel_y; // Move a bala em y

                Tank* tank = fisica->collisionBullet(player2->bullets[i], player1, bots, matriz); // Verifica se colidiu com algum tanque

                if(tank != NULL) // Caso tenha ocorrido a colisão com algum tanque, a bala é destruida
                {
                    tank->takeDamage(); // Caso algum tanque seja atingido é dado dano

                    if(tank->lifes == -1)//Para detectar a pontuação somente uma vez;
                    {
                        // Verifica se é o bot especial
                        if(tank->type)
                            finalscreen->addHighscore(200, 2); // Adiciona o ponto ao score
                        else
                            finalscreen->addHighscore(100, 2);
                    }

                    (player2->bullets[i])->destruida = true; // Tem que colocar o tanque sendo destruido aqui também
                    playSound(); // Toca o som da explosão da bala
                }

                player2->bullets[i]->draw(); // Desenha a bala do player na tela
            }
        }
    }

    // Loop em todo o vetor de bots
    for(int i = 0; i < Bot::max_bots; i++)
    {
        // Verifica se há um bot nessa posição do vetor
        if(bots[i] != NULL)
        {
            // Loop em todo o vetor de balas do bot
            for(int j = 0; j < bots[i]->max_bullets; j++)
            {
                // Verifica se existe uma bala na posição
                if(bots[i]->bullets[j] != NULL)
                {
                    (bots[i]->bullets[j])->position.x += (bots[i]->bullets[j])->vel_x; // Move a bala em x
                    (bots[i]->bullets[j])->position.y += (bots[i]->bullets[j])->vel_y; // Move a bala em y

                    Tank* tank = fisica->collisionBullet(bots[i]->bullets[j], i, player1, player2, bots, matriz, hq); // Verifica se colidiu com algum tanque
                    if(tank != NULL) // Caso tenha ocorrido a colisão com algum tanque, a bala é destruida
                    {
                        tank->takeDamage(); // Caso algum tanque seja atingido é dado dano
                        (bots[i]->bullets[j])->destruida = true; // Tem que colocar o tanque sendo destruido aqui também
                        playSound(); // Toca o som da explosão da bala
                    }

                    bots[i]->bullets[j]->draw(); // Desenha a bala do bot na tela
                }
            }
        }
    }

    // Loop para acessar todas as posições do vetor de balas órfãs
    for(int i = 0; i < TAM_ORF; i++)
    {
        // Verifica se existe uma bala naquela posição do vetor
        if(orf_bullets[i] != NULL)
        {
            (orf_bullets[i])->position.x += (orf_bullets[i])->vel_x; // Move a bala em x
            (orf_bullets[i])->position.y += (orf_bullets[i])->vel_y; // Move a bala em y

            Tank* tank = fisica->collisionBullet(orf_bullets[i], player1, player2, bots, matriz); // Verifica se colidiu com algum tanque

            if(tank != NULL) // Caso tenha ocorrido a colisão com algum tanque, a bala é destruida
            {
                tank->takeDamage(); // Caso algum tanque seja atingido é dado dano
                (orf_bullets[i])->destruida = true; // Tem que colocar o tanque sendo destruido aqui também
            }

            // Desenha a bala órfã na tela
            orf_bullets[i]->draw();

            // Verifica se a bala foi destruida
            if(orf_bullets[i]->destruida)
            {
                delete orf_bullets[i]; // Caso tenha sido destruida é desalocada
                orf_bullets[i] = NULL; // E a posição passa a apontar pra NULL
            }
        }
    }

}
