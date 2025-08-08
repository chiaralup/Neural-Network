#ifndef Hopfield_HPP
#define Hopfield_HPP
#include <SFML/Graphics.hpp>
#include <cassert>
#include <fstream>
#include <iostream>

struct Pixel {
  unsigned int pr;
  unsigned int pg;
  unsigned int pb;
};

class Hopfield {
 private:
  sf::Image image1_;
  sf::Image image2_;
  sf::Texture texture1;
  sf::Texture texture2;
  
  unsigned int width_{150};
  unsigned int height_{200};

 public:
  Hopfield(const sf::Image& image1, const sf::Image& image2);
  sf::Image image1() { return image1_; }
  sf::Image image2() { return image2_; }
  auto resizeimage(const sf::Image& image);
  auto pattern1(const sf::Image& image); //FORSE NON SERVE
  std::vector<int> pattern(const sf::Image& image);
  auto blackandwhite(const auto& pattern);

  // unsigned int getWidth() const { return width; }
  // unsigned int getHeight() const { return height; }
};

#endif