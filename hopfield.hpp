#ifndef HOPFIELD_HPP
#define HOPFIELD_HPP

#include <SFML/Graphics.hpp>  //la mettiamo qui perché serve nei test
#include <cassert>
#include <filesystem>
#include <fstream>
#include <random>

namespace nn {

using Pattern = std::vector<int>;
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
  unsigned int width_;
  unsigned int height_;
  unsigned int N_{width_ * height_};
  Matrix W_{N_, std::vector<double>(N_, 0.)};

  std::vector<std::string> files_{"Avogadro.png", "Curie.png",
                                  "Einstein.png", "Heisenberg.png",
                                  "Hopfield.png", "Schrodinger.png"};

 public:
  Hopfield(unsigned int width, unsigned int height)
      : width_{width}, height_{height} {}
  unsigned int getWidth() const;
  unsigned int getHeight() const;
  unsigned int getN() const;
  sf::Image loadImage(
      std::string const&);  // è necessario che stiano dentro la classe?
  Drawable loadSprite(
      std::string const&);  // è necessario che stiano dentro la classe?
  std::vector<Pixel> resize_image(sf::Image const&);
  Pattern pattern(sf::Image const&);
  Drawable baw_image(Pattern const&);
  Pattern corruption(Pattern const&);
  std::vector<Pattern> loadPatterns();
  // Display screen(std::string const&);
  void matrix();
  void getMatrix();
  Pattern update(Pattern const&);
  double energy(Pattern const&);
};
}  // namespace nn

#endif