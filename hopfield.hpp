#ifndef HOPFIELD_HPP
#define HOPFIELD_HPP
#include <SFML/Graphics.hpp>
#include <cassert>
#include <fstream>
#include <iostream>

using Matrix = std::vector<std::vector<double>>;
using Pattern = std::vector<int>;

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

//enum class Image {
//  AVOGADRO,
//  CURIE,
//  EINSTEIN,
//  FEYNMAN,
//  GALILEO,
//  HEISEINBERG,
//  HOPFIELD,
//  SCHRODINGER
//};

class Hopfield {
  unsigned int width_{42};
  unsigned int height_{51};
  unsigned int N_{width_ * height_};
  Matrix W_{N_, std::vector<double>(N_, 0.)};

  // std::vector<std::string> files_{"pillars.jpg", "merginggalaxies.jpg",
  //                                 "cosmiccliffs.jpg",    "crabnebula.jpg",
  //                                 "globularcluster.jpg", "opencluster.jpg",
  //                                 "penguin.jpg",         "NGC628.jpg",
  //                                 "supernova.jpg", "facciasignora.jpg"};

  std::vector<std::string> files_{
      "avogadro.jpg", "curie.png",      "einstein.jpeg", "feynman.jpg",
      "galileo.png",  "heisenberg.jpg", "hopfield.jpg",  "schrodinger.jpeg"};

 public:
  auto getN() const { return N_; }

  auto getWidth() const { return width_; }
  auto getHeight() const { return height_; }
  sf::Image loadImage(const std::string& filename);
  Drawable loadSprite(const std::string& filename);
  std::vector<Pixel> resizeimage(const sf::Image& image);
  Pattern pattern(const sf::Image& image);
  Drawable blackandwhite(const Pattern& pattern);
  Pattern corruption(const Pattern& pattern);
  std::vector<Pattern> loadPatterns();
  Display screen(const std::string& filename);
  void matrix();
  void getMatrix();
  // void update(const Pattern& corr_pattern);
  Pattern update(const Pattern& corr_pattern);
  double energy(const Pattern& state);
};

#endif