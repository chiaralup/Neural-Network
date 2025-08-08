
#include "acquisition.hpp"

#include <SFML/Graphics.hpp>
#include <cassert>
#include <fstream>
#include <iostream>

Acquisition::Acquisition(const sf::Image& image1, const sf::Image& image2)
    : image1(image1), image2(image2) {
  pillars = image1.getSize();
  deep = image2.getSize();

  width = pillars.x;
  height = pillars.y;

  bx = static_cast<double>(width) / static_cast<double>(deep.x);
  by = static_cast<double>(height) / static_cast<double>(deep.y);
}

void Acquisition::resizedimage() {
  for (unsigned int r = 0; r < height; ++r) {
    double y{static_cast<double>(r) / static_cast<double>(by)};
    unsigned int j{static_cast<unsigned int>(y)};
    double t{y - j};

    for (unsigned int c = 0; c < width; ++c) {
      double x{static_cast<double>(c) / static_cast<double>(bx)};
      unsigned int i{static_cast<unsigned int>(x)};
      double s{x - i};

      // assert(i + 1 <= width && j + 1 <= height);

      sf::Color p1 = image2.getPixel(i, j);
      sf::Color p2 = image2.getPixel(i + 1, j);
      sf::Color p3 = image2.getPixel(i, j + 1);
      sf::Color p4 = image2.getPixel(i + 1, j + 1);

      unsigned int pr = static_cast<unsigned int>(
          (1 - s) * (1 - t) * p1.r + s * (1 - t) * p2.r + (1 - s) * t * p3.r +
          s * t * p4.r);
      unsigned int pg = static_cast<unsigned int>(
          (1 - s) * (1 - t) * p1.g + s * (1 - t) * p2.g + (1 - s) * t * p3.g +
          s * t * p4.g);
      unsigned int pb = static_cast<unsigned int>(
          (1 - s) * (1 - t) * p1.b + s * (1 - t) * p2.b + (1 - s) * t * p3.b +
          s * t * p4.b);

      p_.push_back({pr, pg, pb});
    }
  }
}

void Acquisition::pixel1() {
  for (unsigned int r = 0; r < height; ++r) {
    for (unsigned int c = 0; c < width; ++c) {
      sf::Color pix = image1.getPixel(c, r);
      double m{(pix.r + pix.g + pix.b) / 3.0};

      pattern1_.push_back(m < 127 ? -1 : 1);
      // l'ho cambiato in operatore ternario, invece che ciclo if
    }
  }
}

void Acquisition::pixel2() {
  for (unsigned int r = 0; r < height; ++r) {
    for (unsigned int c = 0; c < width; ++c) {
      unsigned int index = r * width + c;

      assert(index < p_.size() && "Indice fuori dai limiti nel vettore p");

      Pixel& pix = p_[index];
      double m{(pix.pr + pix.pg + pix.pb) / 3.0};

      pattern2_.push_back(m < 127 ? -1 : 1);
    }
  }

  // bisogna definire un bool per l'operatore==, inoltre al posto degli assert
  // si potrebbe fare un bool che controlla che pattern1.size == pattern2.size

  //  assert(pattern1_.size() == width * height);
  //  assert(pattern2_.size() == width * height);
  //  assert(pattern1_.size() == pattern2_.size());
};
