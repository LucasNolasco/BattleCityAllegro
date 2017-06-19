#include "../Include/finalScreen.hpp"

FinalScreen::FinalScreen()
{
    //Carrega as imagens de pontução que aparecem quando bots so destruidos.
    points[0] = al_load_bitmap("References/100points.png");
    if(!points[0])
        std::cout << "Failed to load 100points" << std::endl;

    points[1] = al_load_bitmap("References/200points.png");
    if(!points[1])
        std::cout << "Failed to load 200points" << std::endl;

    //Zera os vetores do tipo 1.
    for(int i = 0; i < 2; i++)
    {
        highscore[i].type1 = 0; //Vetor que guarda o highscore de cada fase.
        bots_destroyed[i].type1 = 0; //Vetor que guarda os bots destruidos pro cada player em cada fase.
    }
    //Zera os vetores do tipo 2.
    for(int i = 0; i < 2; i++)
    {
        highscore[i].type2 = 0;
        bots_destroyed[i].type2 = 0;
    }

    //Zera os vetores que guardam o highscore total.
    highscore_total[0] = 0;
    highscore_total[1] = 0;

    //Fonte usada pra desenhar a tela final de cada level.
    font = new Fonts(20);

    for(int i = 0; i < 3; i++)
        nome_score[i] = 'A';

    nome_score[3] = '\0';

    pos_char = 0;
    save = 0;

    seta = al_load_bitmap("References/seta.png"); // Carrega a imagem da seta

    font2 = new Fonts(30);
    font3 = new Fonts(10);
}

FinalScreen::~FinalScreen()
{
    //Destroi as imagens de pontuação.
    for(int i = 0; i < 2; i++)
        al_destroy_bitmap(points[i]);
}

void FinalScreen::saveScore(Keyboard* keyboard, ALLEGRO_EVENT* ev)
{
    if(ev->type == ALLEGRO_EVENT_KEY_DOWN || ev->type == ALLEGRO_EVENT_KEY_UP)
    {
        if(keyboard->keys[keyboard->KEY_LEFT]) // Caso a tecla da seta esquerda tenha sido pressionada
            pos_char = (pos_char + 2) % 3; // Manda o pos_char uma posição para a esquerda

        else if(keyboard->keys[keyboard->KEY_RIGHT]) // Caso a tecla da seta direita tenha sido pressionada
            pos_char = (pos_char + 1) % 3; // Manda o pos_char uma posição para a direita

        else if(keyboard->keys[keyboard->KEY_UP]) // Caso a tecla da seta cima tenha sido pressionada
            nome_score[pos_char] = ((nome_score[pos_char] - 'A' + 1) % 26) + 'A'; // Avança para a próxima letra do alfabeto

        else if(keyboard->keys[keyboard->KEY_DOWN]) // Caso a tecla da seta baixo tenha sido pressionada
            nome_score[pos_char] = ((nome_score[pos_char] - 'A' + 25) % 26) + 'A'; // Retorna para a letra anterior do alfabeto

        else if(keyboard->keys[keyboard->KEY_ENTER]) // Confere se a tecla enter foi pressionada
        {
            FILE* arquivo;
            arquivo = fopen("References/Files/Highscores.txt", "a+"); // Abre o arquivo onde as pontuações são salvas

            for(int i = 0; i < 3; i++)
                fprintf(arquivo, "%c", nome_score[i]); // Escreve o nome escolhido

            for(int i = 0; i < 3; i++)
                fprintf(arquivo, "%c", '-'); // Escreve as --- para separar o nome da pontuacao

            fprintf(arquivo, "%d", highscore_total[0] + highscore_total[1]); // Escreve a pontuação total
            fprintf(arquivo, "%c", '\n');
            save = 1; // Marca a pontuação como salva

            fclose(arquivo); // Fecha o arquivo
        }
    }

    if(!save) // Confere se a pontuação ainda não foi salva
    {
        char aux[20];
        sprintf(aux, "%d", highscore_total[0] + highscore_total[1]); // Transforma a pontuação em string

        al_clear_to_color(al_map_rgb(0, 0, 0)); // Pinta o fundo de preto

        font->drawFont(260, 60, 255, 255, 255, "SAVE YOUR SCORE"); // Escreve na tela

        font2->drawFont(200, SCREEN_H/2, 255, 255, 255, nome_score); // Escreve o nome que está sendo escolihdo na tela
        font2->drawFont(350, SCREEN_H/2, 255, 255, 255, aux); // Escreve a pontuação total

        font3->drawFont(SCREEN_W/2, SCREEN_H-50, 255, 255, 255, "PRESS 'ENTER' TO CONTINUE");

        al_draw_scaled_rotated_bitmap(seta, 0, 0, 268 + pos_char * 45, 360, MAP_MULT*1.5, MAP_MULT*1.5, PI/2, 0); // Desenha a seta que mostra qual character está sendo modificado

        al_flip_display(); // Exibe o que foi passado pra tela
    }
}


//Metodo que desenha a pontuação quando um bot é destruido.
void FinalScreen::drawPoint(int score, Position position)
{
    //Desenha de acordo com o score do bot.
    if(score == 100)
    {
        //Se passar da metada da tela desenha a esquerda do bot, se não a direita.
        if(position.x > (SCREEN_W*MAP_MULT)/2 )
            al_draw_scaled_rotated_bitmap(points[0], 6, 3, position.x - BOT_TAM*MAP_MULT*2, position.y, MAP_MULT*1.5, MAP_MULT*1.5, 0, 0);
        else
            al_draw_scaled_rotated_bitmap(points[0], 6, 3, position.x + BOT_TAM*MAP_MULT*2, position.y, MAP_MULT*1.5, MAP_MULT*1.5, 0, 0);

    }
    else if(score == 200)
    {
        //Se passar da metada da tela desenha a esquerda do bot, se não a direita.
        if(position.x > (SCREEN_W*MAP_MULT)/2 )
            al_draw_scaled_rotated_bitmap(points[1], 6, 3, position.x - BOT_TAM*MAP_MULT*2, position.y, MAP_MULT*1.5, MAP_MULT*1.5, 0, 0);
        else
            al_draw_scaled_rotated_bitmap(points[1], 6, 3, position.x + BOT_TAM*MAP_MULT*2, position.y, MAP_MULT*1.5, MAP_MULT*1.5, 0, 0);
    }
}

//Atualiza o highscore quando os bots são destruidos.
void FinalScreen::addHighscore(int score, int player)
{
    //Se foi o player 1 que destruiu.
    if(player == 1)
    {
        //Atualiza o highscore e a qauntidade de bots destruido de acordo com o score e tipo do bot.
        if(score == 100)
        {
            highscore[0].type1 += score;
            bots_destroyed[0].type1++;
        }
        else
        {
            highscore[0].type2 += score;
            bots_destroyed[0].type2++;
        }
        //Atualiza o highscore total.
        highscore_total[0] += score;
    }
    //Se o player 2 que destruiu.
    else if(player == 2)
    {
        //Atualiza o highscore e a qauntidade de bots destruido de acordo com o score e tipo do bot.
        if(score == 100)
        {
            highscore[1].type1 += score;
            bots_destroyed[1].type1++;
        }
        else
        {
            highscore[1].type2 += score;
            bots_destroyed[1].type2++;
        }
        //Atualiza o highscore total.
        highscore_total[1] += score;
    }
}

//Metodo que desenha a tela final de cada level.
void FinalScreen::drawFinalScreen(int stage)
{
    //Transforma a variavel inteiro em um char pra desenhar na tela.
    char level[10];
    sprintf(level, "STAGE %d", stage);

    //Deixa o fundo preto.
    al_clear_to_color(al_map_rgb(0, 0, 0));
    //Se for o ultimo level desenha FINAL STAGE ao invés de STAGE 9
    if(stage == 9)
        font->drawFont(SCREEN_W/2, 50, 255, 255, 255, "FINAL STAGE");
    else
        font->drawFont(SCREEN_W/2, 50, 255, 255, 255, level);

    //Escreve player 1 e 2 na tela.
    font->drawFont(120, 130, 255, 255, 255, "PLAYER 1");
    font->drawFont(400, 130, 255, 255, 255, "PLAYER 2");

    //Transforma o highscore total do player 1 em char e escreve na tela.
    char aux[20];
    sprintf(aux, "%d", highscore_total[0]);
    font->drawFont(120, 160, 255, 255, 255, aux);

    //Transforma o highscore total do player 2 em char e escreve na tela.
    sprintf(aux, "%d", highscore_total[1]);
    font->drawFont(400, 160, 255, 255, 255, aux);

    //Carrega a imagem do bot normal.
    ALLEGRO_BITMAP *bot_image = al_load_bitmap("References/bot1-1.png");
    if(!bot_image)
        printf("Can not load bot_image !!!");

    //Carrega a imagem do bot vermelho.
    ALLEGRO_BITMAP *bot_red_image = al_load_bitmap("References/botRed1-1.png");
    if(!bot_red_image)
        printf("Can not load bot_image !!!");

    //Carrega a imagem da seta.
    ALLEGRO_BITMAP *seta = al_load_bitmap("References/seta.png");
    if(!seta)
        printf("Can not load seta !!!");

    //Desenha o bot normal no meio com as duas setas do lado.
    al_draw_scaled_rotated_bitmap(bot_image, 7, 7, (SCREEN_W*MAP_MULT)/2, 230*MAP_MULT, MAP_MULT*2, MAP_MULT*2, 0, 0);
    al_draw_scaled_rotated_bitmap(seta, 7, 7, (SCREEN_W*MAP_MULT)/2 - 50, 230*MAP_MULT, MAP_MULT, MAP_MULT, 0, 0);
    al_draw_scaled_rotated_bitmap(seta, 7, 7, (SCREEN_W*MAP_MULT)/2 + 40, 230*MAP_MULT, MAP_MULT, MAP_MULT, PI, 0);

    //Transfomra os highscores e os numeros de bots normais destruidos da fase em char e escreve na tela pros 2 players.
    sprintf(aux, "%d PTS", highscore[0].type1);
    font->drawFont(100, 220, 255, 255, 255, aux);

    sprintf(aux, "%d PTS", highscore[1].type1);
    font->drawFont(420, 220, 255, 255, 255, aux);

    sprintf(aux, "%d", bots_destroyed[0].type1);
    font->drawFont(205, 220, 255, 255, 255, aux);

    sprintf(aux, "%d", bots_destroyed[1].type1);
    font->drawFont(310, 220, 255, 255, 255, aux);


    //Desenha o bot vermelho no meio com as duas setas do lado.
    al_draw_scaled_rotated_bitmap(bot_red_image, 7, 7, (SCREEN_W*MAP_MULT)/2, 280*MAP_MULT, MAP_MULT*2, MAP_MULT*2, 0, 0);
    al_draw_scaled_rotated_bitmap(seta, 7, 7, (SCREEN_W*MAP_MULT)/2 - 50, 280*MAP_MULT, MAP_MULT, MAP_MULT, 0, 0);
    al_draw_scaled_rotated_bitmap(seta, 7, 7, (SCREEN_W*MAP_MULT)/2 + 40, 280*MAP_MULT, MAP_MULT, MAP_MULT, PI, 0);


    //Transfomra os highscores e os numeros de bots vermelhos destruidos da fase em char e escreve na tela pros 2 players.
    sprintf(aux, "%d PTS", highscore[0].type2);
    font->drawFont(100, 270, 255, 255, 255, aux);

    sprintf(aux, "%d PTS", highscore[1].type2);
    font->drawFont(420, 270, 255, 255, 255, aux);

    sprintf(aux, "%d", bots_destroyed[0].type2);
    font->drawFont(205, 270, 255, 255, 255, aux);

    sprintf(aux, "%d", bots_destroyed[1].type2);
    font->drawFont(310, 270, 255, 255, 255, aux);

    font->drawFont(SCREEN_W/2, 350, 255, 255, 255, "PRESS ESC TO EXIT");
}

//Metodo que reinicia os valores de highscore e numero de bots, para a passagem de level.
void FinalScreen::restartValues()
{
    //Zeram os vetores.
    for(int i = 0; i < 2; i++)
    {
        highscore[i].type1 = 0;
        bots_destroyed[i].type1 = 0;
    }
    for(int i = 0; i < 2; i++)
    {
        highscore[i].type2 = 0;
        bots_destroyed[i].type2 = 0;
    }
}
