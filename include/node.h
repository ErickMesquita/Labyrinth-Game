#ifndef NODE_H
#define NODE_H

#include <SFML/Graphics.hpp>
using namespace sf;

class node{
public:
  int x, y;
  bool left, right, up, down;
  bool visto;
  std::vector<Sprite>* spriteWalls;

  node(int x,int  y, bool left, bool right, bool up, bool down);
  virtual ~node();

  void draw(RenderWindow& window);
  void setSprites(Texture& textureL, Texture& textureT, Texture& textureR, Texture& textureD, int scale);

};

#endif // NODE_H
