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
  Drawable drawable;
  drawable.image = Hopfield::loadImage(filename);

  if (!drawable.texture.loadFromFile(filename)) {
    throw std::runtime_error{"Error during texture charging"};
  }

  drawable.sprite.setTexture(drawable.texture);

  return drawable;
}

std::vector<Pixel> Hopfield::resizeimage(const sf::Image& image) {
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

auto Hopfield::loadPatterns() {
  std::vector<std::vector<int>> patterns;
  for (unsigned int i{0}; i < files_.size(); ++i) {
    sf::Image image{loadImage(files_[i])};
    patterns.push_back(pattern(image));
  }
  return patterns;
}

Drawable Hopfield::blackandwhite(const std::vector<int>& pattern) {
  Drawable drawable;
  drawable.image.create(width_, height_, sf::Color::Black);

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

std::vector<int> Hopfield::corruption(const std::vector<int>& pattern) {
  std::default_random_engine eng;
  std::uniform_int_distribution<unsigned int> random_pix(0, N_ - 1);

  std::vector<int> corr_pattern{pattern};

  // modificare

  for (unsigned int i{0}; i < (N_ / 10); ++i) {
    auto a{random_pix(eng)};
    auto b{random_pix(eng)};

    int val{corr_pattern[a]};
    corr_pattern[a] = corr_pattern[b];
    corr_pattern[b] = val;
  }

  return corr_pattern;
}

// Display Hopfield::screen(const std::string& filename) {
//   //  Display display;
//   Drawable initial{loadSprite(filename)};
//   initial.sprite.setPosition(25., 250.);
//
//   std::vector<int> baw_pattern{pattern(initial.image)};
//   Drawable baw{blackandwhite(baw_pattern)};
//   baw.sprite.setPosition(650., 250.);
//
//   // std::vector<int> corr_pattern{corruption(baw_pattern)};
//   Drawable corrupted{blackandwhite(corruption(baw_pattern))};
//   corrupted.sprite.setPosition(1150., 250.);
//
//   Display display = {initial, baw, corrupted};
//
//   return display;

// Drawable initial{Hopfield::loadSprite(filename)};
// initial.sprite.setPosition(25., 250.);
//
// Drawable blackandwhite;
// std::vector<int> baw_pattern{Hopfield::pattern(initial.image)};
// blackandwhite = Hopfield::blackandwhite(baw_pattern);
// blackandwhite.sprite.setPosition(650., 250.);
//
// Drawable corrupted;
// // std::vector<int> corr_pattern{corruption(baw_pattern)};
// corrupted = Hopfield::blackandwhite(Hopfield::corruption(baw_pattern));
// corrupted.sprite.setPosition(1150., 250.);
//
// Display display = {initial, blackandwhite, corrupted};
//
// return display;}

Matrix Hopfield::matrix() {
  // std::ofstream file("weights.txt");
  // if (!file.is_open()) {
  //   throw std::runtime_error{"Impossibile aprire il file weight.txt!"};
  // }

  Matrix W(N_, std::vector<double>(N_, 0.));
  auto patterns{loadPatterns()};
  for (unsigned int i{0}; i < N_; ++i) {
    for (unsigned int j{0}; j < N_; ++j) {
      if (i == j) {
        W[i][j] = 0.;
      } else {
        double sum =
            std::accumulate(patterns.begin(), patterns.end(), 0.,
                            [i, j](double total, const std::vector<int>& p) {
                              return total + static_cast<double>(p[i] * p[j]);
                            });
        W[i][j] = sum / static_cast<double>(N_);
      }
      // file << W[i][j] << " ";
    }
    // file << '\n';
  }
  // file.close();
  return W;
}

// void Hopfield::update(const std::vector<int>& corr_pattern) {
//   std::vector<int> new_pattern{corr_pattern};
//   std::vector<int> empty_vector;
//   std::vector<std::vector<int>> updating{empty_vector, corr_pattern};
//
//   std::ifstream file{"../weights.txt"};
//
//   if (!file) {
//     throw std::runtime_error{"Impossible to open file!"};
//   }
//
//   size_t t{0};
//   while (updating[t] != updating[t + 1]) {
//     if (updating.size() != t + 2) {
//       throw std::runtime_error{"Incorrect size of the vector updating"};
//     }  // chat dice che non va qui:RICONTROLLARE BENE
//     for (unsigned int i{0}; i < N_; ++i) {
//       double sum{0.};
//       for (unsigned int j{0}; j < N_; ++j) {
//         double Wij{0.};
//         file >> Wij;
//         sum += (Wij * new_pattern[j]);
//       }
//       new_pattern[i] = (sum < 0) ? -1 : 1;
//       updating.push_back(new_pattern);
//       ++t;
//     }
//   }
//   file.close();
// }

bool Hopfield::update(const std::vector<int>& corr_pattern,
                      std::vector<std::vector<int>>& updating) {
  std::vector<int> new_pattern{corr_pattern};
  Matrix W{matrix()};

  std::ifstream file{"./weights.txt"};
  if (!file) {
    throw std::runtime_error{"Impossible to open file!"};
  }

  for (unsigned int i{0}; i < N_; ++i) {
    double sum{0.};
    for (unsigned int j{0}; j < N_; ++j) {
      sum += (W[i][j] * corr_pattern[j]);
    }
    new_pattern[i] = (sum < 0) ? -1 : 1;
  }
  file.close();

  updating.push_back(new_pattern);

  if (new_pattern != corr_pattern) {
    return true;
  } else {
    return false;
  }
}

void Hopfield::convergence(const std::vector<int>& corr_pattern,
                           std::vector<std::vector<int>>& updating) {
  while (update(corr_pattern, updating) == true) {
    continue; 
  }
}

//  bisogna definire un bool per l'operatore==, inoltre al posto degli assert
//  si potrebbe fare un bool che controlla che pattern1.size == pattern2.size

//  assert(pattern1_.size() == width * height); //li mettiamo nel main?
//  assert(pattern2_.size() == width * height); //li mettiamo nel main?
//  assert(pattern(image1).size() == pattern(image2).size());
// li mettiamo nel main?
