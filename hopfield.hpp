#ifndef HOPFIELD_HPP
#define HOPFIELD_HPP

#include <SFML/Graphics.hpp>  //la mettiamo qui perché serve nei test
#include <cassert>
#include <filesystem>
#include <fstream>
#include <random>

using Pattern = std::vector<int>;
using Matrix = std::vector<std::vector<double>>;

namespace nn {

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
  unsigned int width_;
  unsigned int height_;
  unsigned int N_{width_ * height_};
  Matrix W_{N_, std::vector<double>(N_, 0.)};

  std::vector<std::string> files_{
      "Avogadro.png", "Curie.png",      "Einstein.png", "Feynman.png",
      "Galileo.png",  "Heisenberg.png", "Hopfield.png", "Schrodinger.png"};

 public:
  Hopfield(unsigned int width, unsigned int height)
      : width_{width}, height_{height} {}
  auto getN() const { return N_; }
  auto getWidth() const { return width_; }
  auto getHeight() const { return height_; }
  sf::Image loadImage(
      const std::string&);  // è necessario che stiano dentro la classe?
  Drawable loadSprite(
      const std::string&);  // è necessario che stiano dentro la classe?
  std::vector<Pixel> resize_image(const sf::Image&);
  Pattern pattern(const sf::Image&);
  Drawable baw_image(const Pattern&);
  Pattern corruption(const Pattern&);
  std::vector<Pattern> loadPatterns();
  Display screen(const std::string&);
  void matrix();
  void getMatrix();
  // void update(const Pattern&);
  Pattern update(const Pattern&);
  double energy(const Pattern&);
};
}  // namespace nn

#endif