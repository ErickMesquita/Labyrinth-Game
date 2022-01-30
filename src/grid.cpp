#include "grid.h"

#include <SFML/Graphics.hpp>
using namespace sf;

grid::grid(int w, int h) {
    this->scale = 100;

    // check parameters
    if (w <= 0) {
        throw std::runtime_error("Invalid value for width");
        this->w = 1;
    } else {
        this->w = w;
    }

    if (h <= 0) {
        throw std::runtime_error("Invalid value for height");
        this->h = 1;
    } else {
        this->h = h;
    }

    readTextures();

    // Begin building the labyrinth
    buildLabyrinth();
}

grid::grid(int screenWidth, int screenHeight, int scale) {

    // check parameters
    if (scale <= 0) {
        throw "Invalid value for scale";
        this->scale = 1;
    }else
        this->scale = scale;

    if (screenWidth <= 0) {
        throw "Invalid value for width";
        this->w = 1;
    } else {
        this->w = screenWidth/this->scale;
    }

    if (screenHeight <= 0) {
        throw "Invalid value for height";
        this->h = 1;
    } else {
        this->h = screenHeight/this->scale;
    }

    readTextures();

    // Begin building the labyrinth
    buildLabyrinth();
}

void grid::readTextures(){
    this->m_texture_wall_left.loadFromFile("Resources/graphics/wall_left.png");
    this->m_texture_wall_right.loadFromFile("Resources/graphics/wall_right.png");
    this->m_texture_wall_up.loadFromFile("Resources/graphics/wall_top.png");
    this->m_texture_wall_down.loadFromFile("Resources/graphics/wall_down.png");
}

grid::~grid(){
    for (int i = 0; i < this->w; i++){
      for (int j = 0; j < this->h; j++){
        delete this->nos[i][j];
      }
    }

    for (int c = 0; c < w; c++){
        delete this->nos[c];
    }

    delete this->nos;
}

void grid::buildLabyrinth() {

    // instantiate pointer to 'vector of w vectors of pointers to node'
    this->nos = new node**[this->w];

    // instantiate each one of the w vectors of 'h vectors of pointers to node'
    for (int c = 0; c < this->w; c++) {
        this->nos[c] = new node*[this->h];
    }

    // number of unseen nodes
    int nVistos = this->h*this->w;

    // instantiate each one of the pointers to node
    for (int i = 0; i < this->w; i++) {
        for (int j = 0; j < this->h; j++) {
            this->nos[i][j] = new node(i, j, false, false, false, false);
        }
    }
    std::cout << "width: " << this->w << "\nheight: " << this->h << std::endl;

    int x = 0, y = 0;
    Direction veio = Direction::UP;
    int prox = 2;
    //0 -> de cima
    //1 -> da direita
    //2 -> de baixo
    //3 -> da esquerda

    while(nVistos > 1) {

        //Se for um nó novo
        if(this->nos[x][y]->visto == false) {
            this->nos[x][y]->visto = true;//marca
            nVistos--;
            anteriores.push_back(veio);
        }
        std::cout << "node: " << x << ", " << y << "\tveio de: " << (int)veio << "\tnVistos: " << nVistos << std::endl;


        //verifica em volta
        if (testaEmVolta(x, y) == true) { //TEM SAÍDA

            //Dentre as direções possíveis, onde não viu ainda, sorteia a próxima
            bool podeIr = false;
            while(!podeIr) {
                prox = int(rand()%4);
                switch (prox) {
                case 0: //pra cima
                    if(y >= 1 && !(this->nos[x][y-1]->visto)) {
                        this->nos[x][y]->up = true;
                        this->nos[x][y-1]->down = true;
                        podeIr = true;
                    }
                    break;
                case 1: //pra direita
                    if(x < this->w - 1 && !(this->nos[x+1][y]->visto)) {
                        this->nos[x][y]->right = true;
                        this->nos[x+1][y]->left = true;
                        podeIr = true;
                    }
                    break;
                case 2: //pra baixo
                    if(y < this->h - 1 && !(this->nos[x][y+1]->visto)) {
                        this->nos[x][y]->down = true;
                        this->nos[x][y+1]->up = true;
                        podeIr = true;
                    }
                    break;
                case 3: //pra esquerda
                    if(x >= 1 && !(this->nos[x-1][y]->visto)) {
                        this->nos[x][y]->left = true;
                        this->nos[x-1][y]->right = true;
                        podeIr = true;
                    }
                    break;
                default:
                    //throw std::runtime_error("bad direction");
                    break;
                }
            }

            //passa pro próximo
            switch (prox) {
            case 0: //pra cima
                veio = Direction::DOWN;
                y--;
                break;
            case 1: //pra direita
                veio = Direction::LEFT;
                x++;
                break;
            case 2: //pra baixo
                veio = Direction::UP;
                y++;
                break;
            case 3: //pra esquerda
                veio = Direction::RIGHT;
                x--;
                break;
            default:
                break;
            }
        } else { //NÃO TEM SAÍDA
            std::cout << "Sem saida em " << x << ", " << y << std::endl;

            while(testaEmVolta(x, y) != true) {
                std::cout << "Voltei para: " << x << ", " << y << std::endl;
                if  ( x != 0 || y != 0){



                veio = this->anteriores.back();
                if(this->anteriores.size() != 0)
                    anteriores.pop_back();

                //volta pra trás
                switch (veio) {
                case Direction::UP: //de cima
                    if (y > 0)
                        y--;
                    break;
                case Direction::RIGHT: //da direita
                    if (x < this->w - 1)
                    x++;
                    break;
                case Direction::DOWN: //de baixo
                    if (y < this->h - 1)
                    y++;
                    break;
                case Direction::LEFT: //da esquerda
                    if (x > 0)
                        x--;
                    break;
                default:
                    //throw std::runtime_error("bad return direction");
                    break;
                }
            }else{
                nVistos = 0;
                break;
            }
            }
        }
    }

    for (int i = 0; i < this->w; i++) {
        for (int j = 0; j < this->h; j++) {
            if (!(this->nos[i][j]->left) && !(this->nos[i][j]->up) && !(this->nos[i][j]->right) && !(this->nos[i][j]->down)) {

                std::cout << "Faltou: i: " << i << "\tj: " << j << std::endl;

                bool podeIr = false;
                while(!podeIr) {
                    prox = int(rand()%4);
                    switch (prox) {
                    case 0: //UP
                        if(j >= 1) {
                            this->nos[i][j]->up = true;
                            this->nos[i][j-1]->down = true;
                            podeIr = true;
                        }
                        break;
                    case 1: //pra direita
                        if(i < this->w - 1) {
                            this->nos[i][j]->right = true;
                            this->nos[i+1][j]->left = true;
                            podeIr = true;
                        }
                        break;
                    case 2: //pra baixo
                        if(j < this->h - 1) {
                            this->nos[i][j]->down = true;
                            this->nos[i][j+1]->up = true;
                            podeIr = true;
                        }
                        break;
                    case 3: //pra esquerda
                        if(i >= 1) {
                            this->nos[i][j]->left = true;
                            this->nos[i-1][j]->right = true;
                            podeIr = true;
                        }
                        break;
                    default:
                        //throw std::runtime_error("bad direction");
                        break;
                    }
                }
            }
        }
    }
    // sets a sprite for each wall in each node
    for (int i = 0; i < this->w; i++) {
        for (int j = 0; j < this->h; j++) {
            this->nos[i][j]->setSprites(m_texture_wall_left, m_texture_wall_up, m_texture_wall_right, m_texture_wall_down, scale);
        }
    }

}

void grid::draw(RenderWindow& window) {
    sf::RectangleShape rectangle;

    //Background
    rectangle.setSize(Vector2f(w*scale, h*scale)); //full screen


    rectangle.setFillColor(sf::Color(239, 228, 176, 255));
    window.draw(rectangle);

    //Start Area
    rectangle.setSize(Vector2f(scale, scale)); //full screen
    rectangle.setFillColor(sf::Color(0, 255, 0, 255)); //Green
    window.draw(rectangle);
    //noStroke();
    //fill(0,255,0); //quadrado verde
    //rect(0, 0, scale, scale);


    //Finish Area
    rectangle.setPosition((w-1)*scale, (h-1)*scale);
    rectangle.setSize(Vector2f(scale, scale)); //full screen
    rectangle.setFillColor(sf::Color(0, 125, 126, 255)); //Blue
    window.draw(rectangle);

    //stroke(0);
    //strokeWeight(scale/6);

    for (int j = 0; j < this->h; j++) {
        for (int i = 0; i < this->w; i++) {
            this->nos[i][j]->draw(window);
        }
    }
}

bool grid::testaEmVolta(int x, int y) {

    if(this->nos[x][y] == nullptr) {
        throw std::runtime_error("node not initialized");
        return false;
    }
    if(x >= 1){ if(this->nos[x-1][y]->visto == false) { //esquerda
        return true;
    }}

    if(x < this->w - 1){ if(this->nos[x+1][y]->visto == false) { //direita
        return true;
    }}

    if(y >= 1){ if(this->nos[x][y-1]->visto == false) { //cima
        return true;
    }}

    if(y < this->h - 1){ if(this->nos[x][y+1]->visto == false) { //baixo
        return true;
    }}
    return false;
}

int grid::getScale(){
    return this->scale;
}

IntRect grid::getArena(){
    return IntRect(0, 0, this->w*this->scale, this->h*this->scale);
}

Vector2i grid::getHW(){
    return Vector2i(this->w, this->h);
}

bool grid::checkColision(FloatRect playerPosition){

    //Checks if parameters are OK
    if (playerPosition.height <= 0 || playerPosition.width <= 0){
        throw std::runtime_error("Player position's size is negative");
    } else if (playerPosition.height >= this->scale || playerPosition.width >= this->scale){
        throw std::runtime_error("Scale too small for player to fit in");
    }

    // Player's current tile coordinates: Center of rectangle/scale
    int x = (playerPosition.left + (playerPosition.width)/2)/this->scale;
    int y = (playerPosition.top + (playerPosition.height)/2)/this->scale;


    //It is not necessary to compare with player's sprite with all walls' sprites, just
    //with current tile's, and the adjacent ones'.
    for (int i = -1; i <= 1; i++){
        if (i == 1 && x == w - 1){
            // do nothing
        }else if (i >= 0 && x == w){
            // do nothing
        }else if (x > w){
            // do nothing
        }else if (i == -1 && x == 0){
            // do nothing
        }else{
            for (int j = -1; j <= 1; j++){
                if (j == 1 && y == h - 1){
                    // do nothing
                }else if (j >= 0 && y == h){
                    // do nothing
                }else if (y > h){
                    // do nothing
                }else if (j == -1 && y == 0){
                    // do nothing
                }else{
                    for (std::vector<Sprite>::const_iterator s = this->nos[x + i][y + j]->spriteWalls->begin(); s != this->nos[x + i][y + j]->spriteWalls->end(); s++){

                        //Simple Rectangle-Rectangle collision detection
                        /*
                        if (playerPosition.intersects((*s).getGlobalBounds())){
                            return true;
                        }*/


                        //Circle-Rectangle Collision Detection
                        //Created by Jeffrey Thompson
                        //Obtained from:  http://www.jeffreythompson.org/collision-detection/circle-rect.php

                        // Circle's center
                        int cx = playerPosition.left + playerPosition.width/2;
                        int cy = playerPosition.top + playerPosition.height/2;

                        //Wall's square:
                        int rx = (*s).getGlobalBounds().left;
                        int ry = (*s).getGlobalBounds().top;

                        int rw = (*s).getGlobalBounds().width;
                        int rh = (*s).getGlobalBounds().height;

                        int testX = cx;
                        int testY = cy;

                        // Circle's center is inside rectangle
                        if ((*s).getGlobalBounds().contains(cx, cy)){
                            return true;

                        }else{
                            if (cx < rx)         testX = rx;        // left edge
                            else if (cx > rx+rw) testX = rx+rw;     // right edge

                            if (cy < ry)         testY = ry;        // top edge
                            else if (cy > ry+rh) testY = ry+rh;     // bottom edge

                            float distX = cx-testX;
                            float distY = cy-testY;
                            float distance = sqrt( (distX*distX) + (distY*distY) );

                            if (distance <= playerPosition.width/2) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

