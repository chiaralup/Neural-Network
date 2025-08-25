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

//struct Pixel {
//  unsigned pr;
//  unsigned pg;
//  unsigned pb;
//};

struct Drawable {
  sf::Image image;
  sf::Texture texture;
  sf::Sprite sprite;
};

//struct Display {
//  Drawable initial;
//  Drawable blackandwhite;
//  Drawable corrupted;
//};

class Hopfield {
  unsigned width_;
  unsigned height_;
  unsigned threshold_{127};
  Matrix W_;

 public:
  Hopfield(unsigned width, unsigned height)
      : width_{width},
        height_{height},
        W_{width * height, std::vector<double>(width * height, 0.)} {}

  Hopfield(unsigned width, unsigned height, unsigned threshold)
      : width_{width},
        height_{height},
        threshold_{threshold},
        W_{width * height, std::vector<double>(width * height, 0.)} {}

  unsigned getWidth() const;
  unsigned getHeight() const;
  unsigned getN() const;
  sf::Image loadImage(
      std::string const&);  // è necessario che stiano dentro la classe?
  Drawable loadSprite(
      std::string const&);  // è necessario che stiano dentro la classe?
  sf::Uint8 interpolation(unsigned p1, unsigned p2, unsigned p3, unsigned p4,
                          double s, double t);
  std::vector<sf::Color> resize_image(sf::Image const&);
  Pattern pattern(sf::Image const&);
  Drawable baw_image(Pattern const&);
  Pattern corruption(Pattern const&, unsigned);
  std::vector<Pattern> loadPatterns();
  // Display screen(std::string const&);
  void matrix(std::vector<Pattern> const&);
  Matrix getMatrix();
  std::vector<Pattern> update(Pattern const&, Matrix const&);
  std::vector<Pattern> updating(Pattern const&, Matrix const&);
  double energy(Pattern const&);
};
}  // namespace nn

#endif