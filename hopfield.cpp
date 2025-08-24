#include "hopfield.hpp"

namespace nn {

unsigned int Hopfield::getWidth() const { return width_; }

unsigned int Hopfield::getHeight() const { return height_; }

unsigned int Hopfield::getN() const { return width_ * height_; }

sf::Image Hopfield::loadImage(std::string const& filename) {
  sf::Image image;

  auto basePath{std::filesystem::path(__FILE__).parent_path()};
  auto fullPath{basePath / "images" / filename};

  if (!image.loadFromFile(fullPath.string())) {
    throw std::runtime_error{"Error during image charging"};
  }

  return image;
}

Drawable Hopfield::loadSprite(std::string const& filename) {
  Drawable drawable;

  auto basePath{std::filesystem::path(__FILE__).parent_path()};
  auto fullPath{basePath / "images" / filename};

  drawable.image = Hopfield::loadImage(filename);

  if (!drawable.texture.loadFromFile(fullPath.string())) {
    throw std::runtime_error{"Error during texture charging"};
  }

  drawable.sprite.setTexture(drawable.texture);

  return drawable;
}

std::vector<Pixel> Hopfield::resize_image(sf::Image const& image) {
  std::vector<Pixel> p;
  const auto size{image.getSize()};
  const double bx{static_cast<double>(width_) / static_cast<double>(size.x)};
  const double by{static_cast<double>(height_) / static_cast<double>(size.y)};

  for (unsigned int r{0}; r < height_; ++r) {
    const double y{static_cast<double>(r) / by};
    const unsigned int j{std::min(static_cast<unsigned int>(y), size.y - 2)};
    const double t{y - j};

    for (unsigned int c{0}; c < width_; ++c) {
      const double x{static_cast<double>(c) / bx};
      const unsigned int i{std::min(static_cast<unsigned int>(x), size.x - 2)};
      const double s{x - i};

      assert(i + 1 < size.x && j + 1 < size.y);

      const sf::Color p1{image.getPixel(i, j)};
      const sf::Color p2{image.getPixel(i + 1, j)};
      const sf::Color p3{image.getPixel(i, j + 1)};
      const sf::Color p4{image.getPixel(i + 1, j + 1)};

      const unsigned int pr{static_cast<unsigned int>(
          (1 - s) * (1 - t) * p1.r + s * (1 - t) * p2.r + (1 - s) * t * p3.r +
          s * t * p4.r)};
      const unsigned int pg{static_cast<unsigned int>(
          (1 - s) * (1 - t) * p1.g + s * (1 - t) * p2.g + (1 - s) * t * p3.g +
          s * t * p4.g)};
      const unsigned int pb{static_cast<unsigned int>(
          (1 - s) * (1 - t) * p1.b + s * (1 - t) * p2.b + (1 - s) * t * p3.b +
          s * t * p4.b)};

      assert(pr < 256 && pg < 256 && pb < 256);

      p.push_back({pr, pg, pb});
    }
  }
  return p;
}

Pattern Hopfield::pattern(sf::Image const& image) {
  Pattern pattern;
  auto p{resize_image(image)};
  for (unsigned int r{0}; r < height_; ++r) {
    for (unsigned int c{0}; c < width_; ++c) {
      unsigned int index = r * width_ + c;
      assert(index < p.size());

      Pixel& pix = p[index];
      double m{(pix.pr + pix.pg + pix.pb) / 3.0};

      pattern.push_back(m < 127 ? -1 : 1);
    }
  }
  return pattern;
}

std::vector<Pattern> Hopfield::loadPatterns() {
  std::vector<Pattern> patterns;
  for (unsigned int i{0}; i < files_.size(); ++i) {
    sf::Image image{loadImage(files_[i])};
    patterns.push_back(pattern(image));
  }
  return patterns;
}

Drawable Hopfield::baw_image(Pattern const& pattern) {
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

Pattern Hopfield::corruption(Pattern const& pattern, int ratio) {
  std::default_random_engine eng;
  std::uniform_int_distribution<unsigned int> random_pix(0, getN() - 1);

  Pattern corr_pattern{pattern};

  // modificare

  for (unsigned int i{0}; i < (getN() / ratio); ++i) {
    auto a{random_pix(eng)};
    auto b{random_pix(eng)};

    std::swap(corr_pattern[a], corr_pattern[b]);
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

void Hopfield::matrix(std::vector<Pattern> const& patterns) {  // L'ABBIAMO USATA SOLO PER SCRIVERE SU FILE. CONTROLLARE IL TIPO DI RITORNO
  std::ofstream file("weights.txt");
  if (!file.is_open()) {
    throw std::runtime_error{"Impossibile aprire il file weight.txt!"};
  }

  Matrix W(getN(), std::vector<double>(getN(), 0.));  // lasciamo così o W_?
  
  for (unsigned int i{0}; i < getN(); ++i) {
    for (unsigned int j{0}; j < getN(); ++j) {
      if (i == j) {
        W[i][j] = 0.;
      } else {
        double sum = std::accumulate(patterns.begin(), patterns.end(), 0.,
                                     [i, j](double total, const Pattern& p) {
                                       return total + p[i] * p[j];
                                     });
        W[i][j] = static_cast<double>(sum) / static_cast<double>(getN());
      }
      file << W[i][j] << " ";
    }
    file << '\n';
  }
  file.close();
}

Matrix Hopfield::getMatrix() { //VERIFICARE IL RETURN TYPE
  std::ifstream file{"./weights.txt"};
  if (!file) {
    throw std::runtime_error{"Impossible to open file!"};
  }
  for (unsigned int i{0}; i < getN(); ++i) {
    for (unsigned int j{0}; j < getN(); ++j) {
      double Wij{0.};
      file >> Wij;
      W_[i][j] = Wij;
      if ((i == j) && (W_[i][j] != 0)) {
        throw std::runtime_error{"W_[i][j] should be 0 when i == j"};
      }
      // if (i == j) {
      //   std::cout << "Diag[" << i << "] = " << W_[i][j] << "\n";
      // }
    }
  }
  file.close();
  return W_;
}

// void Hopfield::update(const std::vector<int>& corr_pattern) {
//   std::vector<int> old_pattern{corr_pattern};
//   std::vector<int> new_pattern{corr_pattern};
//
//   while (true) {
//     for (unsigned int i{0}; i < getN(); ++i) {
//       double sum{0.};
//       for (unsigned int j{0}; j < getN(); ++j) {
//         sum += (W_[i][j] * old_pattern[j]);
//       }
//       new_pattern[i] = (sum < 0) ? -1 : 1;
//     }
//
//     Drawable drawable{blackandwhite(new_pattern)};
//     drawable.sprite.setScale(3.f, 3.f);
//     window().draw(drawable.sprite);
//     window().display();
//
//     if (new_pattern == old_pattern) {
//       break;
//     }
//     sf::sleep(sf::milliseconds(100));
//     old_pattern = new_pattern;
//   }
// }
//

Pattern Hopfield::update(Pattern const& corr_pattern) {
  Pattern new_pattern{corr_pattern};

  for (unsigned int i{0}; i < getN(); ++i) {
    double sum{0.};
    for (unsigned int j{0}; j < getN(); ++j) {
      sum += (W_[i][j] * corr_pattern[j]);
    }
    new_pattern[i] = (sum < 0) ? -1 : 1;
  }
  return new_pattern;
}

// Pattern Hopfield::update(const Pattern& initial_pattern) {
//   Pattern current_state = initial_pattern;
//   Pattern next_state;
//   bool finished = false;
//
//   while (!finished) {
//     next_state = current_state; // Inizializza next_state con il pattern
//     corrente bool changed = false; for (unsigned int i = 0; i < getN(); ++i) {
//       double sum = 0.;
//       for (unsigned int j = 0; j < getN(); ++j) {
//         sum += (W_[i][j] * current_state[j]);
//       }
//       int new_value = (sum < 0) ? -1 : 1;
//       if (new_value != next_state[i]) {
//         next_state[i] = new_value;
//         changed = true;
//       }
//     }
//
//     if (!changed) {
//       finished = true;
//     }
//
//     // Aggiorna lo stato per l'iterazione successiva
//     current_state = next_state;
//   }
//
//   return current_state;
// }

double Hopfield::energy(Pattern const& state) {
  double energy;
  double sum{0.};
  for (unsigned int i{0}; i < getN(); ++i) {
    for (unsigned int j{0}; j < getN(); ++j) {
      sum += (W_[i][j] * state[i] * state[j]);
    }
  }
  energy = sum / (-2.);
  return energy;
}

}  // namespace nn
