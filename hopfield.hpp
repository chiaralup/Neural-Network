#ifndef HOPFIELD_HPP
#define HOPFIELD_HPP
#include <SFML/Graphics.hpp>
#include <cassert>
#include <fstream>
#include <iostream>

struct Pixel {
  unsigned int pr;
  unsigned int pg;
  unsigned int pb;
};

struct Drawable {
  sf::Image image;
  sf::Texture texture;
  sf::Sprite sprite;
};

class Hopfield {
 private:
  unsigned int width_{150};
  unsigned int height_{200};
  unsigned int N_{width_ * height_};

  //sf::Texture texture_;
  //sf::Sprite sprite_;
  //std::vector<sf::Texture> texture_;
  //std::vector<sf::Sprite> sprite_;

 public:
  Hopfield() {
  }  // Costruttore un po' inutile(?). Che variabili dovrebbe avere??
  Drawable loadImage(const std::string& filename);
  auto resizeimage(const sf::Image& image);
  auto pattern1(const sf::Image& image);  // FORSE NON SERVE
  std::vector<int> pattern(const sf::Image& image);
  Drawable blackandwhite(const std::vector<int>& pattern);
  std::vector<int> corruption(const std::vector<int>& pattern);

  // unsigned int getWidth() const { return width; }
  // unsigned int getHeight() const { return height; }
};

#endif