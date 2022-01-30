#ifndef GRID_H
#define GRID_H

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "node.h"
//#include "player.hpp"
using namespace sf;

enum class Direction { UP=0, RIGHT=1, DOWN=2, LEFT=3 };

class grid{
public:
  node* **nos; //yes, pointer to array of array of pointers to node...


  // Screen size in number of pixels.
  // Scale means the size of each tile in pixels
  grid(int screenWidth, int screenHeight, int scale);

  // If scale is not specified, Width and Height in number of tiles
  // Scale set to default value of 100
  grid(int w, int h);

  // Destructor
  virtual ~grid();

    // Self explanatory name
  void draw(RenderWindow& window);

    // Self explanatory name
  bool checkColision(FloatRect playerPosition);


  int getScale();
  IntRect getArena();

  Vector2i getHW();

private:


    // height and width in number of tiles
  int h, w;

    // size of each tile in pixels
  int scale;

    // Self explanatory name
  void buildLabyrinth();

    // Stack that records the last nodes visited
  std::vector<Direction> anteriores;

    // tests if there is any adjacent node it is possible to move to
  bool testaEmVolta(int x, int y);

    // Textures for walls:
  Texture m_texture_wall_left;
  Texture m_texture_wall_right;
  Texture m_texture_wall_up;
  Texture m_texture_wall_down;

  void readTextures();

};

#endif // GRID_H
