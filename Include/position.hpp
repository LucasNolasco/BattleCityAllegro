#pragma once

#include <iostream>

class Position
{
public:
    int x;
    int y;
    ~Position(){}

    //Template para deixar o tipo passado genérico.
    template<class Gen> //ou template<typename VarGen>
    Position operator+(Gen p)
    {
        Position posicao;

        posicao.x = x;
        posicao.y = y + p;

        return posicao;
    }

    //Sobrecarga no operador de soma, para ser possivel somar duas posições.
    Position operator+(Position p)
    {
        Position posicao;

        posicao.x = x + p.x;
        posicao.y = y + p.y;

        return posicao;
    }
    //Sobrecarga no operador de subtração, para ser possivel subtrair duas posições.
    Position operator-(Position p)
    {
        Position posicao;

        posicao.x = x - p.x;
        posicao.y = y - p.y;

        return posicao;
    }

    //Template para deixar o tipo passado genérico.
    template<class Gen> //ou template<typename VarGen>
    Position operator-(Gen p)
    {
        Position posicao;

        posicao.x = x;
        posicao.y = y - p;

        return posicao;
    }
    //Sobrecarga no operador de diferença;
    int operator!=(Position p)
    {
        if(x != p.x || y != p.y)
            return 1;

        return 0;
    }
};
