#ifndef TEXTUREHOLDER_HPP
#define TEXTUREHOLDER_HPP

#include <SFML/Graphics.hpp>
#include <map>

using namespace sf;


class TextureHolder {
  public:
    TextureHolder();
    static Texture& GetTexture(std::string const& filename);

  private:
      // A map holding pairs of String and Texture
      std::map<std::string, sf::Texture> m_Textures;


    // Pointer to the singleton
    static TextureHolder* m_s_Instance;
};

#endif // TEXTUREHOLDER_H
