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

struct Display {
  Drawable initial;
  Drawable blackandwhite;
  Drawable corrupted;
};

class Hopfield {
 private:
  unsigned int width_{400};
  unsigned int height_{500};
  unsigned int N_{width_ * height_};

  std::vector<std::string> files_{"Pillars.jpg", "Earring.png"};

 public:
  sf::Image loadImage(const std::string& filename);
  Drawable loadSprite(const std::string& filename);
  std::vector<Pixel> resizeimage(const sf::Image& image);
  auto pattern1(const sf::Image& image);  // FORSE NON SERVE
  std::vector<int> pattern(const sf::Image& image);
  Drawable blackandwhite(const std::vector<int>& image);
  std::vector<int> corruption(const std::vector<int>& pattern);
  auto loadPatterns();
  auto matrix();
  Display display(const std::string& filename);

  // unsigned int getWidth() const { return width; }
  // unsigned int getHeight() const { return height; }
};

#endif