#include "../Include/controler.hpp"


typedef struct{
    char nome[7];
    int score;
}Pontuacao;

int compare (const void * a, const void * b)
{
  //return ( (*(Pontuacao*)a).score - (*(Pontuacao*)b).score );
  return ( ((Pontuacao*)a)->score - ((Pontuacao*)b)->score );
}


Controler::Controler(ALLEGRO_TIMER *_timer)
{
    //Inicializa e aloca os atributos do controler.
    timer = _timer;
    jogo = false;
    gameControlsScreen = false;
    highscoreScreen = false;
    curr_level = 1;

    keyboard = new Keyboard;
    menu = new Menu(keyboard);
    game_menu = new InGameMenu(keyboard);
    fisica = new Physics;
    balinha = new Bullet(fisica);
    //Duas fontes de tamanho diferente que serão usadas varias vezes.
    font1 = new Fonts(FONT_TAM);
    font2 = new Fonts(FONT_TAM*0.35);

    //Cria os son de gamestart, gameover  power up que serão usados durante o jogo.
    gameover_sound = new Sounds("References/sounds/gameover.ogg");
    gamestart_sound = new Sounds("References/sounds/gamestart.ogg");
    powerup_sound = new Sounds("References/sounds/bonus.ogg");

    //Inicializa os intervalos de tempo dos power ups.
    dt_clockpu = al_get_timer_count(timer);
    dt_shovelpu = al_get_timer_count(timer);
}

Controler::~Controler()
{
    //Deleta tudo que foi alocado no construtor.
    delete fisica;
    delete game_menu;
    delete menu;
    delete keyboard;
    delete gamestart_sound;
    delete gameover_sound;
    delete powerup_sound;
    delete font1;
    delete font2;
}

//Inicia o menu principal do jogo.
int Controler::startMenu(ALLEGRO_EVENT *ev)
{
    //Limpa a tela de fundo deixando preto.
    al_clear_to_color(al_map_rgb(0, 0, 0));

    //Atualiza a tela de menu dependendo das teclas selecionadas.
    keyboard->checKeyboard(ev);
    menu->drawMenu();
    //Para todos os sons quando voltamos pro menu.
    al_stop_samples();

    //Determina a ação de acordo com a opção escolhida no menu
    if(menu->enterOption() == 1)
        startGame(false); //Inicia o jogo de 1 player.

    else if(menu->enterOption() == 2)
        startGame(true);//Inicia o modo multiplayer

    else if(menu->enterOption() == 3)
        highscoreScreen = true;//Flag para entrar na tela de highscores.

    else if(menu->enterOption() == 4)
        gameControlsScreen = true; //Flag para entrar na tela que mostra os controles.

    else if(menu->enterOption() == 5)
        return 1; //Fechar o jogo caso seja escolhida a terceira opcao

    return 0;
}

void Controler::screenHighscore(ALLEGRO_EVENT *ev)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    char jogador[20]; //Char que vai receber o que está no arquivo.
    char aux[30]; //Char auxliar que será usado para mostrar a colocação, o nome o o highscore.

    //Desenha HIGSCORES na tela.
    font1->drawFont(SCREEN_W/2, 50, 255, 255, 255, "HIGHSCORES");
    font2->drawFont(SCREEN_W/2, SCREEN_H - 20, 255, 255, 255, "PRESS 'ESC' TO EXIT");

    FILE* arq = fopen("References/Files/Highscores.txt", "r");
    if (arq == NULL)
        printf("Erro ao abrir o arquivo!\n");

    Pontuacao pontuacoes[1000];
    char *numero; //Variavel para conversão de char para int.
    int teste; //

    int cont = 0;

    while (fscanf(arq, "%s", jogador) != EOF)
    {
        for(teste = 6; jogador[teste] != '\0'; teste++){}

        numero = new char[teste-5];

        for(teste = 6; jogador[teste] != '\0'; teste++)
            numero[teste-6] = jogador[teste];

        numero[teste-5] = '\0';

        for(teste = 0; teste < 6; teste++)
            pontuacoes[cont].nome[teste] = jogador[teste];

        pontuacoes[cont].nome[6] = '\0';
        pontuacoes[cont].score = atoi(numero);

        cont++;
    }

    fclose(arq);

    arq = fopen("References/Files/Highscores.txt", "w");

    qsort(pontuacoes, cont, sizeof(Pontuacao), compare);

    for(int var = cont-1; var >= 0; var--)
    {
        fprintf(arq, "%s", pontuacoes[var].nome);
        fprintf(arq, "%d\n", pontuacoes[var].score);
    }

    fclose(arq);

    arq = fopen("References/Files/Highscores.txt", "r");

    int j = 1;
    int i = 0;

    //Mostra os 10 melhores ou até o final do arquivo.
    while ( j <= 10 && fscanf(arq, "%s", jogador) != EOF)
    {
        sprintf(aux, "%d. ", j); //Escreve a colocação no arquivo inicial.
        strcat(aux, jogador); //Concatena as duas strings com a colocação, o nome e o score.
        font2->drawFont(SCREEN_W/2, 120+i, 255, 255, 255, aux); //Desenha na tela a string
        i += 25; //Diferença de altura entre um e outro.
        j++;//Colocação.
    }

    al_flip_display();

    keyboard->waitKey();

    //Analisa as teclas pressionadas.
    keyboard->checKeyboard(ev);

    highscoreScreen = false;

    fclose(arq);
}

//Tela do jogo que mostra os controles.
void Controler::screenGameControl(ALLEGRO_EVENT *ev)
{
    //Carrega a imagem com os controles
    ALLEGRO_BITMAP *screen = al_load_bitmap("References/gameControls.png");
    if(!screen)
        cout << "Can not load gameControls" << endl;

    //Desenha a imagem na tela.
    al_draw_scaled_rotated_bitmap(screen, 0, 0, 0, 0,MAP_MULT/1.5, MAP_MULT/1.5, 0,  0);
    al_flip_display();

    //Analisa as teclas pressionadas.
    keyboard->checKeyboard(ev);
    if(ev->type == ALLEGRO_EVENT_KEY_DOWN || ev->type == ALLEGRO_EVENT_KEY_UP)
    {
        //Se a tecla ESC foi pressionada a flag recebe falso e voltamos pro menu inicial
        if(keyboard->keys[keyboard->KEY_ESC] == 1)
            gameControlsScreen = false;
    }
}

//Faz aparecer o menu dentro do jogo.
void Controler::menuInGame(ALLEGRO_EVENT *ev)
{
    //Checa se alguma tecla foi pressionada
    keyboard->checKeyboard(ev);
    game_menu->drawMenu();// Desenha o menu

     //Se a opção escolhida for continuar a flag recebe 0 e o jogo volta ao normal.
    if(game_menu->enterOption() == 1)
        game_menu->menu_in_game = false;

    else if(game_menu->enterOption() == 2)
    {
        //Caso a opção escolhida seja sair.
        game_menu->menu_in_game = false;//Altera o valor da flag pra sair do menu dentro do jogo.
        jogo = false; //Altera o valor da flag pra siar do jogo.
        deleteAll(); //Deleta tudo qeu tem a ver com o jogo.
    }
    al_flip_display();
}

//Metodo que inicia o jogo.
void Controler::startGame(bool _multiplayer)
{
    jogo = true; //A flag jogo muda de valor, indicando que o jogo deve iniciar.
    multiplayer = _multiplayer; //Para ssabermos se o jogo é multiplayer ou não.
    mapinha = new Mapa(curr_level); //Cria o mapa do primeiro level.
    finalscreen = new FinalScreen(); //Cria a tela final que vai marcar os highscores.

    //Cria o vetor que vai controlar os bots e aponta eles para NULL, para ter melhor controle.
    bots = new Bot*[Bot::max_bots];
    for(int i = 0; i < Bot::max_bots; i++)
        bots[i] = NULL;
    //Inicia os atributos necessários para controlar os bots em cada fase.
    Bot::cont_bots = 0;
    Bot::num_bots = 5;
    Bot::totalBots = Bot::num_bots;
    Bot::max_bots = 3;
    Bot::spawnRed = 2;

    //Cria o vetor que vai controlar os power ups e aponta eles para NULL, para ter melhor controle
    powerups = new PowerUp*[PowerUp::max_powerups];
    for(int i = 0; i < PowerUp::max_powerups; i++)
        powerups[i] = NULL;

    //Cria o vetor que controla as balas de tanks que ja foram destruidos e aponta elas pra NULL.
    orf_bullets = new Bullet*[TAM_ORF];
    for(int i = 0; i < TAM_ORF; i++)
        orf_bullets[i] = NULL;

    //Cria o player 1;
    player1 = new Player(1, 2, 0, (160*MAP_MULT), (SCREEN_H*MAP_MULT - 15*MAP_MULT), 2, fisica, timer, bots, orf_bullets);
    //Separa o tempo inicial de animação do spawn do player;
    player1->dt_spawn = al_get_timer_count(timer) + 40;
    //Separa o tempo inicial em que o player não pode morrer.
    player1->dt_imortal = al_get_timer_count(timer) + 80;

    if(multiplayer)
    {
        //Cria o player 2 como foi feito com o player 1
        player2 = new Player(2, 2, 0, (256*MAP_MULT), (SCREEN_H*MAP_MULT - 15*MAP_MULT), 2, fisica, timer, bots, orf_bullets);
        player2->dt_spawn = al_get_timer_count(timer) + 40;
        player2->dt_imortal = al_get_timer_count(timer) + 80;
        player1->multiplayer = true;
    }
    else
    {
        //Ou seja, não existe player 2
        player1->multiplayer = false;
        player2 = NULL;
    }
    //Toca a musica de inicio de fase.
    gamestart_sound->playSound();
}

void Controler::update(ALLEGRO_EVENT *ev)
{
    //O mapa é desenhado e a matriz de posições passada para o main.
    posicoes = mapinha->draw(player1, player2, font2, Bot::num_bots);

    keyboard->checKeyboard(ev); //Checa o teclado paraver os botões quee foram pressionados.

    //Se alguma tecla for pressionada ou "solta" ...
    if(ev->type == ALLEGRO_EVENT_KEY_DOWN || ev->type == ALLEGRO_EVENT_KEY_UP)
    {

        //... se a tecla espaço foi pressiona o player 1 atira.
        if(keyboard->keys[keyboard->KEY_SPACE] == 1)
            if(player1 != NULL)
                player1->atirar(player1);
        //... se a tecla L foi pressiona e esta no modo multiplayer o player 2 atira.
        if(player2 != NULL)
            if(keyboard->keys[keyboard->KEY_L] == 1)
                player2->atirar(player2);
        //Se a tecla ESC for pressionada a flag no menu dentro do jogo é deixada postitiva para o menu ser aberto.
        if(keyboard->keys[keyboard->KEY_ESC] == 1)
            game_menu->menu_in_game = true;
    }
}

//Metodo pra ontrolar o power up shovel.
void Controler::updatePosicoes(char** posicoes, int dt_shovelpu)
{
    char shovel;

    //Se estiver no tempo determinado a variavel shovel recebe o char correspondente ao bloco indestrutivel.
    if(al_get_timer_count(timer) < dt_shovelpu)
        shovel = '@';
    //Se não continua normal.
    else
        shovel = '#';

    //Muda o tipo do bloco em volta do HQ pro tipo indestrutivel.
    for(int i = 25; i > 22; i--)
    {
        if(posicoes[i][11] != '.')
            posicoes[i][11] = shovel;

        if(posicoes[i][14] != '.')
            posicoes[i][14] = shovel;
    }

    if(posicoes[23][12] != '.')
        posicoes[23][12] = shovel;

    if(posicoes[23][13] != '.')
        posicoes[23][13] = shovel;
}

//Desenha os objetos na tela.
void Controler::draw()
{
    spawnBots(); //Função que controla o spawn dos bots.

    spawnPowerUp(); //Função que controla o spawn dos power ups.

    //"Limpa" o fundo deixando preto.
    al_clear_to_color(al_map_rgb(0, 0, 0));

    //pErcorre o vetor de power ups.
    for(int i = 0; i < PowerUp::max_powerups; i++)
    {
        //Se existir um power up nessa posição.
        if(powerups[i] != NULL)
        {
            if(player1 != NULL)
            {
                //Caso o player 1 "colida" com o power up.
                if(fisica->collisionTankTank(&(powerups[i]->position), &(player1->position)))
                {
                    //O power up é ativado e destruido depois.
                    powerups[i]->applyPowerUp(player1, bots, &dt_clockpu, al_get_timer_count(timer), &dt_shovelpu);
                    powerup_sound->playSound();
                    delete powerups[i];
                    powerups[i] = NULL;
                }
            }

            if(player2 != NULL && powerups[i] != NULL)
            {
                //Caso o player 2 "colida" com o power up.
                if(fisica->collisionTankTank(&(powerups[i]->position), &(player2->position)))
                {
                    //O power up é ativado e destruido depois.
                    powerups[i]->applyPowerUp(player2, bots, &dt_clockpu, al_get_timer_count(timer), &dt_shovelpu);
                    powerup_sound->playSound();
                    delete powerups[i];
                    powerups[i] = NULL;
                }
            }

            //Caso ainda exista um power up nessa posição ele é desenhado.
            if(powerups[i] != NULL)
                powerups[i]->draw();
        }
    }

    //Dependo das teclas pressionadas faz os players se moverem, se existirem.
    if(player1 != NULL)
        player1->playerKeyboard(keyboard, posicoes, player2, 1);

    if(player2 != NULL)
        player2->playerKeyboard(keyboard, posicoes, player1, 2);

    if(player1 != NULL) //Atualiza  player 1 se ele existir
        player1->draw(posicoes);

    if(player2 != NULL ) //Atualiza o player 2 se ele exitir.
        player2->draw(posicoes);

    mapinha->draw(player1, player2, font2, Bot::num_bots); //Atualiza o mapa.

    //Percorre o vetor de bots e atualiza os bots qeu existem.
    for(int i = 0; i < Bot::max_bots; i++)
    {
        if(bots[i] != NULL)
        {
            //Faz o bot se mover e atirar.
            if(al_get_timer_count(timer) > dt_clockpu)
                if(bots[i]->calculaMovimento(posicoes, player1, player2))
                    bots[i]->atirar(bots[i]);

            //Atualiza o bot.
            bots[i]->draw();

            //Se algum bot perdeu as vidas e ainda existe ele é destruido,
            if(bots[i]->lifes <= -1)
            {
                if(bots[i]->explodeTank(finalscreen))
                {
                    delete bots[i];
                    bots[i] = NULL;
                    Bot::num_bots--;//Atualiza o numero de bots no mapa.
                }
            }
        }
    }

    updatePosicoes(posicoes, dt_shovelpu);

    //Atualiza todas as balas.
    balinha->mover(player1, player2, bots, orf_bullets, posicoes, mapinha->getHQ(), finalscreen);

    //Atualiza a bush que tem que ficar separa pra ser desenhada depois das balas e antes dos players.
    mapinha->drawBush();
}

void Controler::spawnBots()
{
    //Anda pelo vetor de controle de bots.
    for(int i = 0; i < Bot::max_bots; i++)
    {
        //Se estiver em um limite de tempo, o numero de bots ainda não estiver no limite e não existir nenhum bot na posição.
        if(al_get_timer_count(timer) >= Bot::dt_spawn_bots && Bot::cont_bots != Bot::totalBots  && bots[i] == NULL)
        {
            Position spawn;

            srand(time(NULL));

            //Cria uma posição aleatoria par ao bot nascer.
            spawn.x = (rand() % (int)(384 * MAP_MULT)) + BOT_TAM;
            spawn.y = (rand() % (int)(184 * MAP_MULT)) + BOT_TAM;

            //Caso o player 1 exista, mas o player 2 não.
            if(player1 != NULL && player2 == NULL)
            {
                //Enquanto no achar uma posição sem blocos, player 1 ou bots, ele recebe novas posiçes.
                while(fisica->collisionTankMap(&spawn, posicoes, BOT_TAM) ||
                      fisica->collisionTankBots(&spawn, bots) ||
                      fisica->collisionTankTank(&spawn, &(player1->position)))
                {
                    spawn.x = (rand() % (int)(384 * MAP_MULT)) + BOT_TAM;
                    spawn.y = (rand() % (int)(184 * MAP_MULT)) + BOT_TAM;
                }
            }

            //Caso o player 2 exista e o player 1 não
            else if(player1 == NULL && player2 != NULL)
            {
                //Enquanto no achar uma posição sem blocos, player 2 ou bots, ele recebe novas posiçes.
                while(fisica->collisionTankMap(&spawn, posicoes, BOT_TAM) ||
                      fisica->collisionTankBots(&spawn, bots) ||
                      fisica->collisionTankTank(&spawn, &(player2->position)))
                {
                    spawn.x = (rand() % (int)(384 * MAP_MULT)) + BOT_TAM;
                    spawn.y = (rand() % (int)(184 * MAP_MULT)) + BOT_TAM;
                }
            }
            //Caso os dois players existam.
            else if(player1 != NULL && player2 != NULL)
            {
                //Enquanto no achar uma posição sem blocos, players ou bots, ele recebe novas posiçes.
                while(fisica->collisionTankMap(&spawn, posicoes, BOT_TAM) ||
                      fisica->collisionTankBots(&spawn, bots) ||
                      fisica->collisionTankTank(&spawn, &(player1->position)) ||
                      fisica->collisionTankTank(&spawn, &(player2->position)))
                {
                    spawn.x = (rand() % (int)(384 * MAP_MULT)) + BOT_TAM;
                    spawn.y = (rand() % (int)(184 * MAP_MULT)) + BOT_TAM;
                }
            }


            int type = rand() % 2; //Gera um tipo de bot aleatorio.

            //Caso ainda possam spawnar bots vermelhos e for um tipo vermelho diminuimos o contador.
            if(type == 1 && Bot::spawnRed > 0)
                Bot::spawnRed--;
            else //Caso não possam spawnar muda-se  o tipo para 0 (norma).
                type = 0;

            //Um novo bot é criado.
            bots[i] = new Bot(type, 1, 0, spawn.x ,spawn.y , 1, fisica, timer, bots, orf_bullets);
            bots[i]->dt_spawn = al_get_timer_count(timer) + 40;

            //O tempo para o proximo renascer é reiniciado.
            Bot::dt_spawn_bots = al_get_timer_count(timer) + 100;
            Bot::cont_bots++; //E o contador de bots é atualizado.
        }
    }
}

void Controler::spawnPowerUp()
{
    int prob = rand() % 500; //Probabilidade de um power up ser criado.

    if(prob == 0) //Caso o numero aleatorio seja 0.
    {
        //Percorre o vetor que controla os power ups.
        for(int i = 0; i < PowerUp::max_powerups; i++)
        {
            if(powerups[i] == NULL) //Caso não exista um power up nessa posição.
            {
                Position spawn;

                //Cria uma posição aleatoria.
                spawn.x = (rand() % (int)(384 * MAP_MULT)) + BOT_TAM;
                spawn.y = (rand() % (int)(184 * MAP_MULT)) + BOT_TAM;

                //Enaquanto tiver blocos, power ups ou bots essa posição ela muda.
                while(fisica->collisionTankMap(&spawn, posicoes, BOT_TAM) ||
                      fisica->collisionTankBots(&spawn, bots) ||
                      fisica->collisionPowerUp(&spawn, powerups))
                {
                    spawn.x = (rand() % (int)(384 * MAP_MULT)) + BOT_TAM;
                    spawn.y = (rand() % (int)(184 * MAP_MULT)) + BOT_TAM;
                }

                //Determina de forma aleatoria que power up vai spawnar.
                int tipo_powerup = rand() % 5;


                switch(tipo_powerup)
                {
                    case 0:
                        powerups[i] = new TankPU; //Aumentar vida.
                        break;

                    case 1:
                        powerups[i] = new BombPU(finalscreen); //Destruir bots.
                        break;

                    case 2:
                        powerups[i] = new ShieldPU; //Deixar imortal.
                        break;

                    case 3:
                        powerups[i] = new ClockPU;  //Parar o tempo.
                        break;

                    case 4:
                        powerups[i] = new ShovelPU; //Proteger o quartel.
                        break;
                }

                //Por fim o power up recebe a posição escolhida.
                powerups[i]->position = spawn;
                break;
            }
        }
    }
}

//Metodo que controla a passagem de level.
void Controler::nextLevel(ALLEGRO_EVENT_QUEUE *event_queue)
{
    //Se o numero de bots chegar a zero passamos de fase.
    if(!Bot::num_bots)
    {

        //Deleta os bots
        for(int i =0; i < Bot::max_bots; i++)
        {
            if(bots[i] != NULL)
            delete bots[i];
        }
        delete [] bots;

        //Deleta as balas orfãs.
        for(int i = 0; i < TAM_ORF; i++)
        {
            if(orf_bullets[i] != NULL)
                delete orf_bullets[i];
        }
        delete [] orf_bullets;

        curr_level++; //Atualiza a variavel que cuida do nuemro do level.

        Bot::num_bots = 3 + (2*curr_level); //O numero de bots que vai spawnar na proxima fase aumenta.
        Bot::max_bots = 2 + (curr_level); // O numero de bots que pode paracer na tela ao mesmo tempo, aumenta.
        Bot::spawnRed = 2 + (curr_level); //O numero de bots mais fortes também aumenta.
        Bot::totalBots = Bot::num_bots;
        Bot::cont_bots = 0; //O contador de bots é zerado.

        dt_shovelpu = 0;//Reseta o tempo do power up shove.

        //Recria o vetor de bots.
        bots = new Bot*[Bot::max_bots];
        for(int i = 0; i < Bot::max_bots; i++)
            bots[i] = NULL;

        //Recria o vetor de balas orfãs.
        orf_bullets = new Bullet*[TAM_ORF];
        for(int i = 0; i < TAM_ORF; i++)
            orf_bullets[i] = NULL;

        //Recria o vetor de power ups.
        for(int i = 0; i < PowerUp::max_powerups; i++)
        {
            if(powerups[i] != NULL)
            {
                delete powerups[i];
                powerups[i] = NULL;
            }
        }

        //Reinicia o player 1 se ele não foi morto.
        if(player1 != NULL)
        {
            player1->restart(bots, orf_bullets);
            player1->lifes = 2;
        }
        else
        {
            //Recria o player 1 se ele foi morto.
            player1 = new Player(1, 2, 0, (160*MAP_MULT), (SCREEN_H*MAP_MULT - 15*MAP_MULT), 2, fisica, timer, bots, orf_bullets);
            player1->dt_spawn = al_get_timer_count(timer) + 40;
            player1->dt_imortal = al_get_timer_count(timer) + 80;
        }
        //Reinicia o player 2 se ele não foi morto.
        if(player2 != NULL)
        {
            player2->restart(bots, orf_bullets);
            player2->lifes = 2;
        }
        else if(multiplayer)
        {
            //Recria o player 2 se estiver no multiplayer e ele foi morto.
            player2 = new Player(2, 2, 0, (256*MAP_MULT), (SCREEN_H*MAP_MULT - 15*MAP_MULT), 2, fisica, timer, bots, orf_bullets);
            player2->dt_spawn = al_get_timer_count(timer) + 40;
            player2->dt_imortal = al_get_timer_count(timer) + 80;
        }

        //Se ainda não chegou no ultimo level.
        if(curr_level != 10)
        {
            //Recria o mapa.
            delete mapinha;
            mapinha = new Mapa(curr_level);

            //Mostra a tela final com as pontuações.
            finalscreen->drawFinalScreen(curr_level-1);
            finalscreen->restartValues();//Reinicia as pontuações pra proxima fase.
            al_flip_display();
            keyboard->waitKey();

            //Deixa a tela cinza
            al_clear_to_color(al_map_rgb(176, 166, 166));

            //Vetor que armazena a mensagem que vai aparecer na tela;
            char level[12]= "FINAL STAGE";

            if(curr_level != 9) //Se não for o ultimo level mostra o numero do level na tela.
                sprintf(level, "STAGE %d", curr_level);

            font1->drawFont(260, 200, 0, 0, 0, level); //Desenha a mesnagem na tela,
            font2->drawFont(260, 240, 0 ,0, 0, " 'PRESS ESC TO CONTINUE' ");
            al_flip_display();
            keyboard->waitKey();

            al_flush_event_queue(event_queue);//Limpa a fila de eventos.
            gamestart_sound->playSound(); //Toca a musica de inicio de fase.
        }
    }

    gameover(event_queue); //Analisa se o jogo acabou.
}

//Metodo que controla a morte dos players.
void Controler::deletePlayers()
{
    //Se o player 1 ainda existe e morreu ele é deletado.
    if(player1 != NULL && player1->lifes <= -1)
    {
        if(player1->explodeTank())
        {
            delete player1;
            player1 = NULL;
        }
    }
     //Se o player 2 ainda existe e morreu ele é deletado.
    if(player2 != NULL && player2->lifes <= -1)
    {
        if(player2->explodeTank())
        {
            delete player2;
            player2 = NULL;
        }
    }
    //Se os dois players não existem mais o jogo é terminado.
    if( player1 == NULL && player2 == NULL)
        mapinha->hq->gameover = true;
}

//Metodo pra deletar os objetos do jogo.
void Controler::deleteAll()
{
    delete mapinha; //Deleta o mapa

    curr_level = 1; //Reinicia os leveis.

    if(player1 != NULL) //Deleta o player 1 se ele existe.
        delete player1;

    if(player2 != NULL) //Deleta o player 2 se ele existe.
        delete player2;

    //Deleta os bots e o vetor de controle.
    for(int i =0; i < Bot::max_bots; i++)
    {
        if(bots[i] != NULL)
            delete bots[i];
    }
    delete [] bots;

    //Deleta os power ups e seu vetor de controle.
    for(int i = 0; i < PowerUp::max_powerups; i++)
        if(powerups[i] != NULL)
            delete powerups[i];
    delete[] powerups;

    //Deleta as balas orfãs e seu vetor de controle.
    for(int i = 0; i < TAM_ORF; i++)
        if(orf_bullets[i] != NULL)
            delete orf_bullets[i];

    delete[] orf_bullets;

    delete finalscreen; //Deleta a tela final pra resetar os highscores.

}

//Metodo que controla o fim de jogo.
void Controler::gameover(ALLEGRO_EVENT_QUEUE *event_queue)
{
    //Caso a flag gameover do hq seja verdadeira o jogo termina.
    if(mapinha->hq->gameover)
    {
        //Toca o som de gameover.
        gameover_sound->playSound();

        //Caso o hq esteja destruido o mapa é atualizado e o jogo fnzalizado
        al_clear_to_color(al_map_rgb(0, 0, 0));
        jogo = false; //Flag que para o jogo.
        mapinha->draw(player1, player2, font2, Bot::num_bots);

        //Desenha o Game Over na tela final.
        font1->drawFont(200, 200, 255, 255, 255, "GAME OVER");
        font2->drawFont(200, 240, 255 ,255, 255, " 'PRESS ESC TO EXIT' ");
        al_flip_display();
        keyboard->waitKey();
        al_flush_event_queue(event_queue);

        //Desenha as pontuações do jogador no level.
        finalscreen->drawFinalScreen(curr_level);
        al_flip_display();
        keyboard->waitKey();
        al_flush_event_queue(event_queue);

        //Cria um objeto evento para guardar a informação do evento.
        ALLEGRO_EVENT ev;

        // Mostra a tela de salvar o HighScore enquanto ele não terminar de salvar
        while(!finalscreen->save)
        {
            //Espera por um evento.
            al_wait_for_event(event_queue,&ev);
            keyboard->checKeyboard(&ev); // Checa o teclado por novas teclas pressionadas
            finalscreen->saveScore(keyboard, &ev); // Exibe a tela de salvar o HighScore
        }

        deleteAll();

        //Limpa a tela deixando preta.
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_flip_display();
    }
    //Caso tenha chego no final do jogo.
    else if(curr_level == 10 )
    {
        //Desenha as informações finais de pontuação do(s) player(s).
        finalscreen->drawFinalScreen(curr_level-1);
        al_flip_display();
        keyboard->waitKey();
        al_flush_event_queue(event_queue);//Limpa a fila de eventos.

        //Faz aparecer a mensagem de parabenização.
        al_clear_to_color(al_map_rgb(176, 166, 166));
        font1->drawFont(260, 170, 0, 0, 0, "CONGRATULATIONS");
        font1->drawFont(260, 220, 0, 0, 0, "YOU WIN");
        font2->drawFont(260, 400, 0 ,0, 0, "PRESS ESC TO EXIT");
        al_flip_display();
        keyboard->waitKey();
        al_flush_event_queue(event_queue);

        //Cria um objeto evento para guardar a informação do evento.
        ALLEGRO_EVENT ev;

        // Mostra a tela de salvar o HighScore enquanto ele não terminar de salvar
        while(!finalscreen->save)
        {
            //Espera por um evento.
            al_wait_for_event(event_queue,&ev);
            keyboard->checKeyboard(&ev); // Checa o teclado por novas teclas pressionadas
            finalscreen->saveScore(keyboard, &ev); // Exibe a tela de salvar o HighScore
        }


        jogo = false;// Flag que diz que o jogo terminou

        deleteAll(); //deleta os objetos.

        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_flip_display();
    }
}

