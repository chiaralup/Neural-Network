#ifndef HOPFIELD_HPP
#define HOPFIELD_HPP
#include <SFML/Graphics.hpp>
#include <cassert>
#include <fstream>
#include <iostream>

using Matrix = std::vector<std::vector<double>>;

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
  unsigned int width_{50};
  unsigned int height_{60};
  unsigned int N_{width_ * height_};

  std::vector<std::string> files_{"Pillars.jpg", "Earring.png"};

 public:
  sf::Image loadImage(const std::string& filename);
  Drawable loadSprite(const std::string& filename);
  std::vector<Pixel> resizeimage(const sf::Image& image);
  std::vector<int> pattern(const sf::Image& image);
  Drawable blackandwhite(const std::vector<int>& image);
  std::vector<int> corruption(const std::vector<int>& pattern);
  auto loadPatterns();
  Display screen(const std::string& filename);
  Matrix matrix();
  bool update(std::vector<int>& corr_pattern, const Matrix& W);

  // intgetWidth() const { return width; }
  // intgetHeight() const { return height; }
};

#endif