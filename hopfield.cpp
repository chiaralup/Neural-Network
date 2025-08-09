#include "hopfield.hpp"

#include <SFML/Graphics.hpp>
#include <random>

sf::Image Hopfield::loadImage(const std::string& filename) {
  sf::Image image;

  if (!image.loadFromFile(filename)) {
    throw std::runtime_error{"Error during image charging"};
  }

  return image;
}

Drawable Hopfield::loadSprite(const std::string& filename) {
  Drawable drawable{Hopfield::loadImage(filename)};

  if (!drawable.texture.loadFromFile(filename)) {
    throw std::runtime_error{"Error during texture charging"};
  }

  drawable.sprite.setTexture(drawable.texture);

  return drawable;
}

auto Hopfield::resizeimage(const sf::Image& image) {
  std::vector<Pixel> p;
  auto size{image.getSize()};

  for (unsigned int r{0}; r < height_; ++r) {
    double by = static_cast<double>(height_) / static_cast<double>(size.y);
    double y{static_cast<double>(r) / static_cast<double>(by)};
    unsigned int j{static_cast<unsigned int>(y)};
    double t{y - j};

    for (unsigned int c{0}; c < width_; ++c) {
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

  for (unsigned int r{0}; r < height_; ++r) {
    for (unsigned int c{0}; c < width_; ++c) {
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
  auto p{resizeimage(image)};
  for (unsigned int r{0}; r < height_; ++r) {
    for (unsigned int c{0}; c < width_; ++c) {
      unsigned int index = r * width_ + c;
      assert(index < p.size() && "Indice fuori dai limiti nel vettore p");

      Pixel& pix = p[index];
      double m{(pix.pr + pix.pg + pix.pb) / 3.0};

      pattern.push_back(m < 127 ? -1 : 1);
    }
  }
  return pattern;
}

Drawable Hopfield::blackandwhite(const sf::Image& image) {
  Drawable drawable;
  drawable.image.create(width_, height_, sf::Color::Black);

  std::vector<int> pattern{Hopfield::pattern(image)};

  for (unsigned int i{0}; i < pattern.size(); ++i) {
    unsigned int row{i / width_};
    unsigned int col{i % width_};

    if (pattern[i] == 1) {
      drawable.image.setPixel(col, row, sf::Color::White);
    }
  }

  drawable.texture.loadFromImage(drawable.image);
  drawable.sprite.setTexture(drawable.texture);

  return drawable;
}

std::vector<int> Hopfield::corruption(const sf::Image& image) {
  std::default_random_engine eng;
  std::uniform_int_distribution<unsigned int> random_pix(0, N_ - 1);

  std::vector<int> pattern{Hopfield::pattern(blackandwhite(image).image)};
  std::vector<int> corr_pattern{pattern};

  for (unsigned int i{0}; i < (N_ / 2); ++i) {
    auto a{random_pix(eng)};
    auto b{random_pix(eng)};

    corr_pattern[a] = corr_pattern[b];
    corr_pattern[b] = corr_pattern[a];
  }

  return corr_pattern;
}

auto Hopfield::loadPatterns() {
  std::vector<std::vector<int>> patterns;
  for (unsigned int i{0}; i < files_.size(); ++i) {
    sf::Image image{loadImage(files_[i])};
    patterns.push_back(pattern(image));
  }
  return patterns;
}

auto Hopfield::matrix() {
  std::vector<std::vector<double>> W;
  auto patterns{loadPatterns()};
  for (unsigned int i{0}; i < N_; ++i) {
    for (unsigned int j{0}; j < N_; ++j) {
      if (i == j) {
        W[i][j] = 0;
      } else {
        double sum =
            std::accumulate(patterns.begin(), patterns.end(), 0.0,
                            [i, j](double total, const std::vector<int>& p) {
                              return total + static_cast<double>(p[i]) *
                                                 static_cast<double>(p[j]);
                            });
        W[i][j] = sum / static_cast<double>(N_);
      }
    }
  }
}

//  bisogna definire un bool per l'operatore==, inoltre al posto degli assert
//  si potrebbe fare un bool che controlla che pattern1.size == pattern2.size

//  assert(pattern1_.size() == width * height); //li mettiamo nel main?
//  assert(pattern2_.size() == width * height); //li mettiamo nel main?
//  assert(pattern(image1).size() == pattern(image2).size());
// li mettiamo nel main?
