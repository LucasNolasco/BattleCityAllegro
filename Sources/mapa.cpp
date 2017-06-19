#include "../Include/mapa.hpp"

Mapa::Mapa(int _level)
{
    //Trnasforma o level em um char que é usado pra abrir o arquivo do level.
    char dir_mapa[10];
    sprintf(dir_mapa, "Maps/%d", _level);

    arquivo_nivel.open(dir_mapa); //Abre o arquivo.
    string linha;
    level = _level;

    //Carrega todos os bitmaps necessarios para fazer as partes auxiliares do mapa.
    inf_background = al_load_bitmap("References/InfBackgorund.png");
    if(!inf_background)
        cout << "Failed to load inf_background" << endl;

    icon_tank = al_load_bitmap("References/iconTank.png");
    if(!icon_tank )
        cout << "Failed to load icon_tank " << endl;

    icon_flag = al_load_bitmap("References/iconFlag.png");
    if(!icon_flag)
        cout << "Failed to load icon_flag" << endl;

    icon_bot = al_load_bitmap("References/iconBot.png");
    if(!icon_bot)
        cout << "Failed to load icon_bot" << endl;

    //Cria os blocos usados no desenho do mapa.
    blocks = new Blocks;
    hq = new HQ;

    /*Aloca a matriz que sera usada pra receber as informações do arquivo
    e será usada nas colisões e na hora de desenhar o mapa.*/
    positions = new char*[26];
    for(int i = 0; i < 26; i++)
        positions[i] = new char[26];

    //Passa os caracteres do arquivo para a matriz alocada anteriromente.
    for(int i = 0; i < 26; i++)
    {
        arquivo_nivel >> linha;
        for(int j = 0; j < 26; j++)
            positions[i][j] = linha[j];
    }
}
Mapa::~Mapa()
{
    //Deleta os objetos criados pelo mapa
    delete blocks;
    delete hq;

    //Deleta a matriz alocada no construtor.
    for(int i = 0; i < 26; i++)
        delete positions[i];
    delete[] positions;
}

HQ* Mapa::getHQ()
{
    return hq;
}

//Metodo que desenha todo o mapa e suas informações.
char** Mapa::draw(Player *player1, Player *player2, Fonts *font, int num_bots)
{
    int flag = 1; //Flag usada para desenhar o HQ só uma vez

    //Percorre a matriz posiçes inteira.
    for(int i = 0; i < 26; i++)
    {
        for(int j = 0; j < 26; j++)
        {
            //De acordo com o simbolo encontrado na matriz, um objeto diferente é desenhado.
            if(positions[i][j] == '#')
                al_draw_scaled_rotated_bitmap(blocks->brick, 0, 0, j*MAP_TAM*MAP_MULT, i*MAP_TAM*MAP_MULT, MAP_MULT, MAP_MULT, 0, 0 );

            else if(positions[i][j] == '@')
                al_draw_scaled_rotated_bitmap(blocks->block, 0, 0, j*MAP_TAM*MAP_MULT, i*MAP_TAM*MAP_MULT, MAP_MULT, MAP_MULT, 0, 0 );

            else if(positions[i][j] == '~')
                al_draw_scaled_rotated_bitmap(blocks->water, 0, 0, j*MAP_TAM*MAP_MULT, i*MAP_TAM*MAP_MULT, MAP_MULT, MAP_MULT, 0, 0 );

            else if(positions[i][j] == 'F' && flag )
            {
                flag =0; //Depis de desnhar o HQ uma vez, não desenha mais.
                al_draw_scaled_rotated_bitmap(hq->hq, 0, 0, j*MAP_TAM*MAP_MULT+3, i*MAP_TAM*MAP_MULT+5, MAP_MULT*1.7, MAP_MULT*1.7, 0, 0 );
            }

        }
    }
    //Desenha o fundo cinza na parte direita da tela, onde ficaram as informações do level.
    al_draw_scaled_rotated_bitmap(inf_background,0, 0, ((SCREEN_W*MAP_MULT)/5)*4, 0 , MAP_MULT, MAP_MULT, 0, 0);

    //Variavel para transformar o inteiro em char e passar para a função de texto.
    char aux[5];

    //Se o player 1 existir.
    if(player1 != NULL)
    {
        //Aparece zero se o player estiver com "vidas" negativas,  o qeu pdoe acontecer devido a ordem com que as coisas acontecem.
        if(player1->lifes <= -1)
            sprintf(aux, "%d", 0);
        else
            sprintf(aux, "%d", player1->lifes); //Se não aparece as vidas normais dele.
    }
    else
        sprintf(aux, "%d", 0); //Caso o player não exista o numero zero aparece.

    //Imprime na tela o PLAYER 1, o numero de vidas e o iconizinho do tank.
    font->drawFont(468, 220, 0, 0, 0, "PLAYER 1");
    font->drawFont(480, 244, 0, 0, 0, aux);
    al_draw_scaled_rotated_bitmap(icon_tank, 0, 0, 450*MAP_MULT, 240*MAP_MULT , 2*MAP_MULT, 2*MAP_MULT, 0, 0);

    //Se o player 2 existir.
    if(player2 != NULL)
    {
        //Aparece zero se o player estiver com "vidas" negativas, o que pdoe acontecer devido a ordem com que as coisas acontecem.
        if(player2->lifes <= -1)
            sprintf(aux, "%d", 0);
        else
            sprintf(aux, "%d", player2->lifes);//Se não aparece as vidas normais dele.

        //Imprime na tela o PLAYER 2, o numero de vidas e o iconizinho do tank.
        font->drawFont(468, 280, 0, 0, 0, "PLAYER 2");
        font->drawFont(480, 304, 0, 0, 0, aux);
        al_draw_scaled_rotated_bitmap(icon_tank, 0, 0, 450*MAP_MULT, 300*MAP_MULT , 2*MAP_MULT, 2*MAP_MULT, 0, 0);
    }
    else if(player1->multiplayer) //Se o jogo estiver no modo multiplayer e o player ja estiver morto
    {
        sprintf(aux, "%d", 0); //O numero zero que aparece.

        //Imprime na tela o PLAYER 2, o numero de vidas e o iconizinho do tank.
        font->drawFont(468, 280, 0, 0, 0, "PLAYER 2");
        font->drawFont(480, 304, 0, 0, 0, aux);
        al_draw_scaled_rotated_bitmap(icon_tank, 0, 0, 450*MAP_MULT, 300*MAP_MULT , 2*MAP_MULT, 2*MAP_MULT, 0, 0);
    }

    sprintf(aux, "%d", level);
    //Mostra a bandeira e o numero do level do jogo.
    al_draw_scaled_rotated_bitmap(icon_flag, 0, 0, 460*MAP_MULT, 350*MAP_MULT , 2*MAP_MULT, 2*MAP_MULT, 0, 0);
    font->drawFont(476, 380, 0, 0, 0, aux);

    int row = 1, col = 1;

    //Desenha os icones dos bots na tela.
    for (int i = 0; i < num_bots; i++)
    {
        al_draw_scaled_rotated_bitmap(icon_bot, 0, 0, (422*MAP_MULT+ (row)*20*MAP_MULT), (20*MAP_MULT + col*20*MAP_MULT) , 2*MAP_MULT, 2*MAP_MULT, 0, 0);
        //Sempre que o numero de linhas for divisivel por 3, a coluna aumenta e as linhas voltam pro zero.
        if(!(row%3))
        {
            col++;
            row = 0;
        }
        row++;
    }

    return (positions); //Retorna a matriz posições, pra que o controler tenha acesso a ela.
}

void Mapa::drawBush()
{
    /*Desenha as bush pois ela tem que ser desenha depois do tank,
    como á agua tem qeu ser desenhada antes, eles não podem ser desenhados juntos no draw.*/
    for(int i = 0; i < 26; i++)
    {
        for(int j = 0; j < 26; j++)
        {
            if(positions[i][j] == '%')
                al_draw_scaled_rotated_bitmap(blocks->bush, 0, 0, j*MAP_TAM*MAP_MULT, i*MAP_TAM*MAP_MULT, MAP_MULT, MAP_MULT, 0, 0 );
        }
    }
}
