#include "Labirinth.h"

Labirinth::Labirinth(int screenWidth, int screenHeight, int escala){
  g = new grid(screenWidth/escala, screenHeight/escala);
  srand((int)time(0) * 10);
  montaLabirinto();
}

Labirinth::~Labirinth(){
    delete g;
}

