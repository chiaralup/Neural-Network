#include "hopfield.hpp"

#include <SFML/Graphics.hpp>

Hopfield::Hopfield(const sf::Image& image1, const sf::Image& image2)
    : image1_(image1), image2_(image2) {}

auto Hopfield::resizeimage(const sf::Image& image) {
  std::vector<Pixel> p;
  auto size{image.getSize()};

  for (unsigned int r = 0; r < height_; ++r) {
    double by = static_cast<double>(height_) / static_cast<double>(size.y);
    double y{static_cast<double>(r) / static_cast<double>(by)};
    unsigned int j{static_cast<unsigned int>(y)};
    double t{y - j};

    for (unsigned int c = 0; c < width_; ++c) {
      double bx = static_cast<double>(width_) / static_cast<double>(size.x);
      double x{static_cast<double>(c) / static_cast<double>(bx)};
      unsigned int i{static_cast<unsigned int>(x)};
      double s{x - i};

      // assert(i + 1 <= width && j + 1 <= height);

      sf::Color p1 = image.getPixel(i, j);
      sf::Color p2 = image.getPixel(i + 1, j);
      sf::Color p3 = image.getPixel(i, j + 1);
      sf::Color p4 = image.getPixel(i + 1, j + 1);

      unsigned int pr = static_cast<unsigned int>(
          (1 - s) * (1 - t) * p1.r + s * (1 - t) * p2.r + (1 - s) * t * p3.r +
          s * t * p4.r);
      unsigned int pg = static_cast<unsigned int>(
          (1 - s) * (1 - t) * p1.g + s * (1 - t) * p2.g + (1 - s) * t * p3.g +
          s * t * p4.g);
      unsigned int pb = static_cast<unsigned int>(
          (1 - s) * (1 - t) * p1.b + s * (1 - t) * p2.b + (1 - s) * t * p3.b +
          s * t * p4.b);

      p.push_back({pr, pg, pb});
    }
  }
  return p;
}

auto Hopfield::pattern1(const sf::Image& image) {  // NON SERVE FORSE
  std::vector<int> pattern1;

  for (unsigned int r = 0; r < height_; ++r) {
    for (unsigned int c = 0; c < width_; ++c) {
      sf::Color pix = image.getPixel(c, r);
      double m{(pix.r + pix.g + pix.b) / 3.0};

      pattern1.push_back(m < 127 ? -1 : 1);
      // l'ho cambiato in operatore ternario, invece che ciclo if
    }
  }

  return pattern1;
}

std::vector<int> Hopfield::pattern(const sf::Image& image) {
  std::vector<int> pattern;
  for (unsigned int r = 0; r < height_; ++r) {
    for (unsigned int c = 0; c < width_; ++c) {
      auto p{resizeimage(image)};

      unsigned int index = r * width_ + c;
      assert(index < p.size() && "Indice fuori dai limiti nel vettore p");

      Pixel& pix = p[index];
      double m{(pix.pr + pix.pg + pix.pb) / 3.0};

      pattern.push_back(m < 127 ? -1 : 1);
    }
  }
  return pattern;
}

// auto Hopfield::blackandwhite(const auto& pattern){return }

// bisogna definire un bool per l'operatore==, inoltre al posto degli assert
// si potrebbe fare un bool che controlla che pattern1.size == pattern2.size

//  assert(pattern1_.size() == width * height); //li mettiamo nel main?
//  assert(pattern2_.size() == width * height); //li mettiamo nel main?
//  assert(pattern(image1).size() == pattern(image2).size());
// li mettiamo nel main?
