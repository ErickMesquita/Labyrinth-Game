#include "node.h"

#include "textureHolder.hpp"

node::node(int x, int  y, bool left, bool right, bool up, bool down){
    this->x = x;
    this->y = y;
    this->left = left;
    this->right = right;
    this->up = up;
    this->down = down;
    this->visto = false;

    this->spriteWalls = new std::vector<Sprite>;
}

node::~node(){
    this->spriteWalls->clear();
    this->spriteWalls->shrink_to_fit();
    delete this->spriteWalls;
}

void node::draw(RenderWindow& window){

    for (std::vector<Sprite>::const_iterator s = this->spriteWalls->begin(); s != this->spriteWalls->end(); s++){

        window.draw(*s);
    }
}

void node::setSprites(Texture& textureL, Texture& textureT, Texture& textureR, Texture& textureD, int scale){
    if(this->up == false) {
        this->spriteWalls->push_back(Sprite(textureT));
        this->spriteWalls->back().setPosition(this->x*scale, this->y*scale);
    }
    if(this->down == false) {
        this->spriteWalls->push_back(Sprite(textureD));
        this->spriteWalls->back().setPosition(this->x*scale, (this->y + 1)*scale - scale/10);
    }
    if(this->left == false) {
        this->spriteWalls->push_back(Sprite(textureL));
        this->spriteWalls->back().setPosition(this->x*scale, this->y*scale);
    }
    if(this->right == false) {
        this->spriteWalls->push_back(Sprite(textureR));
        this->spriteWalls->back().setPosition((this->x + 1)*scale - scale/10, this->y*scale);
    }
}
