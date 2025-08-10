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
  unsigned int width_{45};
  unsigned int height_{55};
  unsigned int N_{width_ * height_};
  Matrix W_{N_, std::vector<double>(N_, 0.)};

  std::vector<std::string> files_{"Pillars.jpg", "Earring.png"};

 public:
  auto getN() const { return N_; }
  sf::Image loadImage(const std::string& filename);
  Drawable loadSprite(const std::string& filename);
  std::vector<Pixel> resizeimage(const sf::Image& image);
  std::vector<int> pattern(const sf::Image& image);
  Drawable blackandwhite(const std::vector<int>& image);
  std::vector<int> corruption(const std::vector<int>& pattern);
  auto loadPatterns();
  Display screen(const std::string& filename);
  void matrix();
  void getMatrix();
  void update(const std::vector<int>& corr_pattern);
  std::vector<int> up(const std::vector<int>& corr_pattern);
  // intgetWidth() const { return width; }
  // intgetHeight() const { return height; }
};

#endif