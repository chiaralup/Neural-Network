#ifndef HOPFIELD_HPP
#define HOPFIELD_HPP

#include <SFML/Graphics.hpp>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <random>

namespace nn {

using Pattern = std::vector<int>;
using Matrix = std::vector<std::vector<double>>;

struct Drawable {
  sf::Image image;
  sf::Texture texture;
  sf::Sprite sprite;
};

class Hopfield {
  unsigned width_;
  unsigned height_;
  Matrix W_;

 public:
  Hopfield(unsigned width, unsigned height)
      : width_{width},
        height_{height},
        W_{width * height, std::vector<double>(width * height, 0.)} {}

  unsigned getWidth() const;
  unsigned getHeight() const;
  unsigned getN() const;

  sf::Image loadImage(std::string const&);
  Drawable loadSprite(std::string const&);

  sf::Uint8 interpolation(unsigned p1, unsigned p2, unsigned p3, unsigned p4,
                          double s, double t);
  std::vector<sf::Color> resizeImage(sf::Image const&);
  Pattern pattern(sf::Image const&);
  Drawable bawImage(Pattern const&);
  Pattern corruption(Pattern const&, unsigned);
  std::vector<Pattern> loadPatterns();
  void matrix(std::vector<Pattern> const&);
  Matrix getMatrix();
  Pattern update(const Pattern& corr_pattern);
  double energy(Pattern const&);
};
}  // namespace nn

#endif