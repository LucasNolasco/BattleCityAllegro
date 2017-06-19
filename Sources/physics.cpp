#include "../Include/physics.hpp"

Physics::Physics() : Sounds("References/sounds/brick.ogg"){}

/* Detecta a colisão entre dois objetos quaisquer
 *  - Retorna 1 caso ocorra a colisão
 *  - Retorna 0 caso NÂO ocorra a colisão
 */

int Physics::collision(Position* object1, Position* object2, int tamanho1, int tamanho2)
{
    Position distancia = *object1 - *object2; // Calcula o vetor posição fazendo a diferença entre as posicões de cada objeto

    // Confere se a distância calculada é uma possível colisão
    if(abs(distancia.x) <= (tamanho1 + tamanho2) && abs(distancia.y) <= (tamanho1 + tamanho2))
       return 1;

    // Caso não seja uma colisão retorna 0
    return 0;
}

/* Detecta a colisão entre dois tanques
 *  - Retorna 1 caso ocorra a colisão
 *  - Retorna 0 caso NÂO ocorra a colisão
 */

int Physics::collisionTankTank(Position* player1, Position* player2)
{
    int tamanho1 = BOT_TAM; // Define o tamanho médio como o tamanho do bot
    int tamanho2 = BOT_TAM; // Define o tamanho médio como o tamanho do bot

    Position distancia = *player1 - *player2; // Calcula o vetor distância entre os dois Tank's

    // Confere se a distância calculada é uma possível colisão
    if(abs(distancia.x) <= (tamanho1 + tamanho2) && abs(distancia.y) <= (tamanho1 + tamanho2))
        if(player1 != player2)
            return 1;

    // Caso não seja uma colisão retorna 0
    return 0;
}

/* Detecta a colisão entre um tanque player e todos os bots do mapa
 *  - Retorna 1 caso ocorra a colisão
 *  - Retorna 0 caso NÃO ocorra a colisão
 */

int Physics::collisionTankBots(Position* player, Bot** bots)
{
    // Loop para rodar todo o vetor de bots
    for(int i = 0; i < Bot::max_bots; i++)
        if(bots[i] != NULL) // Verifica se naquela posição do vetor existe um bot
            if(collisionTankTank(player, &(bots[i]->position))) // Caso existe verifica a colisão do bot nessa posiçao com o player
                return 1; // Retorna 1 caso a colisão tenha ocorrido

    return 0; // Retorna 0 caso a colisão não tenha ocorrido
}

/* Detecta a colisão entre um objeto e os powerups
 *  - Retorna 1 caso ocorra a colisão
 *  - Retorna 0 caso NÃO ocorra a colisão
 */

int Physics::collisionPowerUp(Position* player, PowerUp** powerups)
{
    // Loop para rodar todo o vetor de powerups
    for(int i = 0; i < PowerUp::max_powerups; i++)
        if(powerups[i] != NULL) // Verifica se naquela posição do vetor existe um bot
            if(collisionTankTank(player, &(powerups[i]->position))) // Caso existe verifica a colisão do powerup nessa posiçao com o player
                return 1; // Retorna 1 caso a colisão tenha ocorrido

    return 0; // Retorna 0 caso a colisão não tenha ocorrido
}

/* Detecta a colisão do tanque player com as bordas do mapa e os itens de cenário
 *  - Retorna 1 caso ocorra a colisão
 *  - Retorna 0 caso NÃO ocorra a colisão
 */

int Physics::collisionTankMap(Position* player, char** matriz, int tamanho)
{
    int map_tam = MAP_TAM * MAP_MULT;  //Auxiliares inteiros usados para acessar a matriz.
    int tank_tam = tamanho * MAP_MULT;

    // Verifica colisão com as bordas
    if (player->y  <= (tamanho) || player->y >= (401 * MAP_MULT) ||
        player->x  <= (tamanho) || player->x >= (401 * MAP_MULT))
    {
        return 1;
    }

    // Colisão com os elementos do mapa
    for(int i = - tank_tam; i < tank_tam; i++)
    {
        char obstaculos[4] = {'#', '@', 'F', '~'}; // Vetor para os caracteres que representam obstaculos no mapa
        for(int j = 0; j < 4; j++)
        {
            // Verifica se existe colisão com algum obstaculo
            if(matriz[(player->y - tank_tam) / map_tam][(player->x + i)/map_tam] == obstaculos[j] ||
               matriz[(player->y + tank_tam) / map_tam][(player->x + i)/map_tam] == obstaculos[j] ||
               matriz[(player->y + i) / map_tam][(player->x - tank_tam)/map_tam] == obstaculos[j] ||
               matriz[(player->y + i) / map_tam][(player->x + tank_tam)/map_tam] == obstaculos[j])
            {
                return 1; // Caso ocorra a colisão retorna 1
            }
        }
    }

    return 0; // Caso nenhuma colisão tenha sido detectada retorna 0
}

int Physics::collisionBulletMap(Bullet* bullet, char** matriz)
{
    int map_tam = MAP_TAM * MAP_MULT; // Tamanho do mapa ajustado ao tamanho da tela
    int bullet_tam = BULLET_TAM * MAP_MULT; // Tamanho da bala ajustada ao tamanho da tela

    // Verifica colisão com as bordas do mapa
    if (bullet->position.y  <= 2 * MAP_MULT ||
        bullet->position.y >= 406 * MAP_MULT ||
        bullet->position.x  <= 2 * MAP_MULT ||
        bullet->position.x >= 414 * MAP_MULT)
    {
        bullet->destruida = true; // Caso tenha ocorrido a colisão a bala é destruida
        playSound(); // O som de destruição da bala é tocado
        return 1;
    }

    else if(matriz[(bullet->position.y - bullet_tam) / map_tam][(bullet->position.x - bullet_tam) / map_tam] == '#')
    {
        bullet->destruida = true; // Caso tenha ocorrido a colisão a bala é destruida
        playSound(); // O som de destruição da bala é tocado
        matriz[(bullet->position.y - bullet_tam) / map_tam][(bullet->position.x - bullet_tam) / map_tam] = '.'; // Remove o obstáculo do mapa
    }

    else if(matriz[(bullet->position.y - bullet_tam) / map_tam][(bullet->position.x + bullet_tam) / map_tam] == '#')
    {
        bullet->destruida = true; // Caso tenha ocorrido a colisão a bala é destruida
        playSound(); // O som de destruição da bala é tocado
        matriz[(bullet->position.y - bullet_tam) / map_tam][(bullet->position.x + bullet_tam) / map_tam] = '.'; // Remove o obstáculo do mapa
    }

    else if(matriz[(bullet->position.y + bullet_tam) / map_tam][(bullet->position.x - bullet_tam) / map_tam] == '#')
    {
        bullet->destruida = true; // Caso tenha ocorrido a colisão a bala é destruida
        playSound(); // O som de destruição da bala é tocado
        matriz[(bullet->position.y + bullet_tam) / map_tam][(bullet->position.x - bullet_tam) / map_tam]  = '.'; // Remove o obstáculo do mapa
    }

    else if(matriz[(bullet->position.y + bullet_tam) / map_tam][(bullet->position.x + bullet_tam) / map_tam] == '#')
    {
        bullet->destruida = true; // Caso tenha ocorrido a colisão a bala é destruida
        playSound(); // O som de destruição da bala é tocado
        matriz[(bullet->position.y + bullet_tam) / map_tam][(bullet->position.x + bullet_tam) / map_tam] = '.'; // Remove o obstáculo do mapa
    }

    else if(matriz[(bullet->position.y - bullet_tam) / map_tam][(bullet->position.x - bullet_tam) / map_tam] == '@' &&
            matriz[(bullet->position.y - bullet_tam) / map_tam][(bullet->position.x + bullet_tam) / map_tam] == '@' &&
            matriz[(bullet->position.y + bullet_tam) / map_tam][(bullet->position.x - bullet_tam) / map_tam] == '@' &&
            matriz[(bullet->position.y + bullet_tam) / map_tam][(bullet->position.x + bullet_tam) / map_tam] == '@')
    {
        bullet->destruida = true; // Caso tenha ocorrido a colisão a bala é destruida
        playSound(); // O som de destruição da bala é tocado
    }
    else if(matriz[(bullet->position.y - bullet_tam) / map_tam][(bullet->position.x - bullet_tam) / map_tam] == 'F' &&
            matriz[(bullet->position.y - bullet_tam) / map_tam][(bullet->position.x + bullet_tam) / map_tam] == 'F' &&
            matriz[(bullet->position.y + bullet_tam) / map_tam][(bullet->position.x - bullet_tam) / map_tam] == 'F' &&
            matriz[(bullet->position.y + bullet_tam) / map_tam][(bullet->position.x + bullet_tam) / map_tam] == 'F')
    {
        bullet->destruida = true; // Caso tenha ocorrido a colisão a bala é destruida
        playSound(); // O som de destruição da bala é tocado
    }

    return 0;
}

/* Detecta a colisão das balas com todos os tanques do mapa (BOTS)
 *  - Retorna o endereço de memória do tanque atingido caso ocorra a colisão
 *  - Retorna NULL caso NÃO ocorra colisão
 */

Tank* Physics::collisionBullet(Bullet* bullet, int index, Player* player1, Player* player2, Bot** bots, char** matriz, HQ* hq)
{
    int map_tam = MAP_TAM * MAP_MULT; // Tamanho do mapa ajustado ao tamanho da tela
    int bullet_tam = BULLET_TAM * MAP_MULT; // Tamanho da bala ajustada ao tamanho da tela

    // Verifica se o player1 existe
    if(player1 != NULL)
    {
        // Verifica a colisão com o player1
        if(collision(&(bullet->position), &(player1->position), TANK_TAM, BULLET_TAM))
            return player1; // Caso haja a colisão retorna o player1

        // Loop para acessar todo o vetor de balas do player1
        for(int i = 0; i < player1->max_bullets; i++)
        {
            if(player1->bullets[i] != NULL) // Verifica se existe uma bala nessa posição
            {
                // Verifica se há colisão entre as balas
                if(collision(&bullet->position, &(player1->bullets[i])->position, BULLET_TAM, BULLET_TAM))
                {
                    bullet->destruida = true; // Caso haja a colisão a bala é destruida
                    player1->bullets[i]->destruida = true; // Caso haja a colisão a bala é destruida
                }
            }
        }
    }

    // Verifica se o player2 existe
    if(player2 != NULL)
    {
        // Verifica a colisão com o player2
        if(collision(&(bullet->position), &(player2->position), TANK_TAM, BULLET_TAM))
            return player2; // Caso haja a colisão retorna o player2

        // Loop para acessar todo o vetor de balas do player2
        for(int i = 0; i < player2->max_bullets; i++)
        {
            if(player2->bullets[i] != NULL) // Verifica se existe uma bala nessa posição
            {
                // Verifica se há colisão entre as balas
                if(collision(&bullet->position, &(player2->bullets[i])->position, BULLET_TAM, BULLET_TAM))
                {
                    bullet->destruida = true; // Caso haja a colisão a bala é destruida
                    player2->bullets[i]->destruida = true; // Caso haja a colisão a bala é destruida
                }
            }
        }
    }

    // Verifica a colisão com o HQ
    if(matriz[(bullet->position.y - bullet_tam) / map_tam][(bullet->position.x + bullet_tam) / map_tam] == 'F')
        hq->takeDamage(); // Caso tenha ocorrido a colisão o HQ leva o dano

    // Loop para acessar todo o vetor de bots
    for(int j = 0; j < Bot::max_bots; j++)
    {
        // Verifica se existe um bot nessa posição e se não é o mesmo bot
        if(j != index && bots[j] != NULL)
        {
            // Verifica a colisao entre a bala e o bot
            if(collision(&(bullet->position), &(bots[j]->position), BULLET_TAM, BULLET_TAM))
                bullet->destruida = true; // Destroi a bala ja que não há fogo amigo entre bots

            // Loop para acessar todas as posições do vetor de balas
            for(int k = 0; k < bots[j]->max_bullets; k++)
            {
                // Verifica a existência de uma bala nessa posição do vetor
                if(bots[j]->bullets[k] != NULL)
                {
                    // Verifica a colisão entre as balas
                    if(collision(&(bullet->position), &(bots[j]->bullets[k])->position, BULLET_TAM, BULLET_TAM))
                    {
                        bullet->destruida = true; // Destroi a bala caso tenha ocorrido a colisão
                        bots[j]->bullets[k]->destruida = true; // Destroi a bala caso tenha ocorrido a colisão
                    }
                }
            }
        }
    }

    collisionBulletMap(bullet, matriz); // Verifica a colisão da bala com o mapa

    return NULL; // Retorna NULL caso não tenha ocorrido a colisão com algum tanque
}

/* Detecta a colisão das balas com todos os tanques do mapa (PLAYER)
 *  - Retorna o endereço de memória do tanque atingido caso ocorra a colisão
 *  - Retorna NULL caso NÃO ocorra colisão
 */

Tank* Physics::collisionBullet(Bullet* bullet, Player* player, Bot** bots, char** matriz)
{
    // Verifica se o player existe
    if(player != NULL)
    {
        // Verifica a colisão com o player
        if(collision(&(bullet->position), &(player->position), TANK_TAM, BULLET_TAM))
            bullet->destruida = true; // Destroi a bala caso tenha ocorrido a colisão

        // Loop para acessar todo o vetor de balas do player
        for(int i = 0; i < player->max_bullets; i++)
        {
            // Verifica a existência de uma bala nessa posição do vetor
            if(player->bullets[i] != NULL)
            {
                // Verifica a colisão entre as balas
                if(collision(&bullet->position, &(player->bullets[i])->position, BULLET_TAM, BULLET_TAM))
                {
                    bullet->destruida = true; // Destroi a bala caso tenha ocorrido a colisão
                    player->bullets[i]->destruida = true; // Destroi a bala caso tenha ocorrido a colisão
                }
            }
        }
    }

    // Loop para acessar todo o vetor de bots
    for(int j = 0; j < Bot::max_bots; j++)
    {
        // Verifica se existe um bot nessa posição
        if(bots[j] != NULL)
        {
            // Verifica a colisao entre a bala e o bot
            if(collision(&(bullet->position), &(bots[j]->position), BOT_TAM, BULLET_TAM))
                return bots[j]; // Retorna o bot que foi atingido

            // Loop para acessar todas as posições do vetor de balas
            for(int k = 0; k < bots[j]->max_bullets; k++)
            {
                // Verifica a existência de uma bala nessa posição do vetor
                if(bots[j]->bullets[k] != NULL)
                {
                    // Verifica a colisão entre as balas
                    if(collision(&(bullet->position), &(bots[j]->bullets[k])->position, BULLET_TAM, BULLET_TAM))
                    {
                        bullet->destruida = true; // Destroi a bala caso tenha ocorrido a colisão
                        bots[j]->bullets[k]->destruida = true; // Destroi a bala caso tenha ocorrido a colisão
                    }
                }
            }
        }
    }

    collisionBulletMap(bullet, matriz); // Verifica a colisão da bala com o mapa

    return NULL; // Retorna NULL caso não tenha ocorrido a colisão com algum tanque
}

/* Detecta a colisão das balas com todos os tanques do mapa (BALAS ORFÃS)
 *  - Retorna o endereço de memória do tanque atingido caso ocorra a colisão
 *  - Retorna NULL caso NÃO ocorra colisão
 */

Tank* Physics::collisionBullet(Bullet* bullet, Player* player1, Player* player2, Bot** bots, char** matriz)
{
    // Verifica se o player1 existe
    if(player1 != NULL)
    {
        // Verifica a colisão com o player1
        if(collision(&(bullet->position), &(player1->position), TANK_TAM, BULLET_TAM))
            return player1; // Caso haja a colisão retorna o player1

        // Loop para acessar todo o vetor de balas do player
        for(int i = 0; i < player1->max_bullets; i++)
        {
            // Verifica a existência de uma bala nessa posição do vetor
            if(player1->bullets[i] != NULL)
            {
                // Verifica a colisão entre as balas
                if(collision(&bullet->position, &(player1->bullets[i])->position, BULLET_TAM, BULLET_TAM))
                {
                    bullet->destruida = true; // Destroi a bala caso tenha ocorrido a colisão
                    player1->bullets[i]->destruida = true; // Destroi a bala caso tenha ocorrido a colisão
                }
            }
        }
    }

    // Verifica se o player2 existe
    if(player2 != NULL)
    {
        // Verifica a colisão com o player1
        if(collision(&(bullet->position), &(player2->position), TANK_TAM, BULLET_TAM))
            return player2; // Caso haja a colisão retorna o player1

        // Loop para acessar todo o vetor de balas do player
        for(int i = 0; i < player2->max_bullets; i++)
        {
            // Verifica a existência de uma bala nessa posição do vetor
            if(player2->bullets[i] != NULL)
            {
                // Verifica a colisão entre as balas
                if(collision(&bullet->position, &(player2->bullets[i])->position, BULLET_TAM, BULLET_TAM))
                {
                    bullet->destruida = true; // Destroi a bala caso tenha ocorrido a colisão
                    player2->bullets[i]->destruida = true; // Destroi a bala caso tenha ocorrido a colisão
                }
            }
        }
    }

    // Loop para acessar todo o vetor de bots
    for(int j = 0; j < Bot::max_bots; j++)
    {
        // Verifica se existe um bot nessa posição
        if(bots[j] != NULL)
        {
            // Verifica a colisao entre a bala e o bot
            if(collision(&(bullet->position), &(bots[j]->position), BOT_TAM, BULLET_TAM))
            {
                bots[j]->takeDamage(); // Da dano no bot atingido
                return bots[j]; // Retorna o bot que foi atingido
            }

            // Loop para acessar todas as posições do vetor de balas
            for(int k = 0; k < bots[j]->max_bullets; k++)
            {
                // Verifica a existência de uma bala nessa posição do vetor
                if(bots[j]->bullets[k] != NULL)
                {
                    // Verifica a colisão entre as balas
                    if(collision(&(bullet->position), &(bots[j]->bullets[k])->position, BULLET_TAM, BULLET_TAM))
                    {
                        bullet->destruida = true; // Destroi a bala caso tenha ocorrido a colisão
                        bots[j]->bullets[k]->destruida = true; // Destroi a bala caso tenha ocorrido a colisão
                    }
                }
            }
        }
    }

    collisionBulletMap(bullet, matriz); // Verifica a colisão da bala com o mapa

    return NULL; // Retorna NULL caso não tenha ocorrido a colisão com algum tanque
}
