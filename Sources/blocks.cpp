#include "../Include/mapa.hpp"

Blocks::Blocks()
{
    //Carrega as imagens de todos os blocos que são usados pra desenhar o mapa.
    brick = al_load_bitmap("References/brick.png");
    if(!brick)
        cout << "Failed to create a brick";

    block = al_load_bitmap("References/block.png");
    if(!block)
        cout << "Failed to create a block";

    bush = al_load_bitmap("References/bush.png");
    if(!bush)
        cout << "Failed to create a Bush";

    water = al_load_bitmap("References/water.png");
    if(!water)
        cout << "Failed to create a Water";
}
Blocks::~Blocks()
{
    al_destroy_bitmap(brick);
    al_destroy_bitmap(block);
    al_destroy_bitmap(bush);
    al_destroy_bitmap(water);
}
