#include "../Include/hq.hpp"

HQ::HQ()
{
    //CArrega a imagem do HQ.
    hq = al_load_bitmap("References/hq.png");
    if(!hq)
        cout << "Failed to load hq1" << endl;

    //Deixa ele com 100 de vida.
    hp = 100;
    gameover = false; //Seta a flag de gameover como falsa.
}
HQ::~HQ()
{
    //Destroi a iamgem do HQ.
    al_destroy_bitmap(hq);
}

void HQ::takeDamage()
{
    //Dois tiros são necessários para destruir o hq, então a cada tiro ele perde metade da vida.
    hp -= 50;
    if(hp == 0)
    {
        //Quando o hp chega a zero a imaagem dele é destruida e substituida pela imagem destruida.
        al_destroy_bitmap(hq);
        hq = al_load_bitmap("References/hqDestroyed.png"); //Quartel destruido;
        if(!hq)
            cout << "Failed to load gameover" << endl;
        gameover = true; //Flag de game over é setada como true.
    }
}
