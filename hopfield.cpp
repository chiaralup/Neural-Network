#include "hopfield.hpp"

namespace nn {

unsigned Hopfield::getWidth() const { return width_; }

unsigned Hopfield::getHeight() const { return height_; }

unsigned Hopfield::getN() const { return width_ * height_; }

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

sf::Uint8 Hopfield::interpolation(unsigned p1, unsigned p2,
                                  unsigned p3,  // TESTARLA
                                  unsigned p4, double s, double t) {
  const double p{std::round((1 - s) * (1 - t) * p1 + s * (1 - t) * p2 +
                            (1 - s) * t * p3 + s * t * p4)};

  assert(p < 256);

  return static_cast<sf::Uint8>(p);
}

std::vector<sf::Color> Hopfield::resize_image(sf::Image const& image) {
  std::vector<sf::Color> p;
  p.reserve(getN());
  const auto size{image.getSize()};
  const double bx{static_cast<double>(width_) / static_cast<double>(size.x)};
  const double by{static_cast<double>(height_) / static_cast<double>(size.y)};

  for (unsigned r{0}; r < height_; ++r) {
    const double y{static_cast<double>(r) / by};
    unsigned j{static_cast<unsigned>(y)};
    if (j + 1 >= size.y) {
      j = size.y - 2;
    }
    const double t{y - j};

    assert(j + 1 < size.y && t >= 0. && t < 1.);

    for (unsigned c{0}; c < width_; ++c) {
      const double x{static_cast<double>(c) / bx};
      unsigned i{static_cast<unsigned>(x)};
      if (i + 1 >= size.x) {
        i = size.x - 2;
      }
      const double s{x - i};

      assert(i + 1 < size.x && s >= 0. && s < 1.);

      const sf::Color p1{image.getPixel(i, j)};
      const sf::Color p2{image.getPixel(i + 1, j)};
      const sf::Color p3{image.getPixel(i, j + 1)};
      const sf::Color p4{image.getPixel(i + 1, j + 1)};

      const auto pr{interpolation(p1.r, p2.r, p3.r, p4.r, s, t)};
      const auto pg{interpolation(p1.g, p2.g, p3.g, p4.g, s, t)};
      const auto pb{interpolation(p1.b, p2.b, p3.b, p4.b, s, t)};

      p[r * width_ + c] = {pr, pg, pb};
    }
  }
  return p;
}

Pattern& Hopfield::pattern(sf::Image const& image) {
  Pattern pattern;
  pattern.reserve(getN());
  auto const& p{resize_image(image)};

  for (const auto& pix : p) {
    double m{(pix.r + pix.g + pix.b) / 3.0};
    pattern.push_back(m < threshold_ ? -1 : 1);
  }

  assert(pattern.size() == getN());

  return pattern;
}

std::vector<Pattern> Hopfield::loadPatterns() {
  std::vector<Pattern> patterns;

  std::vector<std::string> const files = {"Avogadro.png", "Curie.png",
                                 "Einstein.png", "Heisenberg.png",
                                 "Hopfield.png", "Schrodinger.png"};

  for (auto const& filename : files) {
    sf::Image const image{loadImage(filename)};
    patterns.push_back(pattern(image));
  }

  assert(patterns.size() == files.size());

  return patterns;
}

Drawable Hopfield::baw_image(Pattern const& pattern) {
  Drawable drawable;
  drawable.image.create(width_, height_, sf::Color::Black);

  for (unsigned i{0}; i < pattern.size(); ++i) {
    unsigned row{i / width_};
    unsigned col{i % width_};

    if (pattern[i] == 1) {
      drawable.image.setPixel(col, row, sf::Color::White);
    }
  }

  drawable.texture.loadFromImage(drawable.image);
  drawable.sprite.setTexture(drawable.texture);

  return drawable;
}

Pattern Hopfield::corruption(Pattern const& pattern, unsigned ratio) {
  std::default_random_engine eng;
  std::uniform_int_distribution<unsigned> random_pix(0, getN() - 1);

  Pattern corr_pattern{pattern};

  // modificare

  for (unsigned i{0}; i < (getN() / ratio); ++i) {
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

void Hopfield::matrix(std::vector<Pattern> const&
                          patterns) {  // L'ABBIAMO USATA SOLO PER SCRIVERE SU
                                       // FILE. CONTROLLARE IL TIPO DI RITORNO
  std::ofstream file("weights.txt");
  if (!file.is_open()) {
    throw std::runtime_error{"Impossibile aprire il file weight.txt!"};
  }

  Matrix W(getN(), std::vector<double>(getN(), 0.));  // lasciamo così o W_?

  for (unsigned i{0}; i < getN(); ++i) {
    for (unsigned j{i + 1}; j < getN(); ++j) {
      double sum = std::accumulate(patterns.begin(), patterns.end(), 0.,
                                   [i, j](double total, Pattern const& p) {
                                     return total + p[i] * p[j];
                                   });
      W[i][j] = static_cast<double>(sum) / static_cast<double>(getN());
      W[j][i] = W[i][j];
    }
  }

  for (unsigned i{0}; i < getN(); ++i) {
    for (unsigned j{0}; j < getN(); ++j) {
      file << W[i][j] << " ";
    }
    file << '\n';
  }
  file.close();
}

Matrix Hopfield::getMatrix() {  // VERIFICARE IL RETURN TYPE
  std::ifstream file{"./weights.txt"};
  if (!file) {
    throw std::runtime_error{"Impossible to open file!"};
  }
  for (unsigned i{0}; i < getN(); ++i) {
    for (unsigned j{0}; j < getN(); ++j) {
      double Wij;
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

std::vector<Pattern> Hopfield::update(Pattern const& corr_pattern,
                                      Matrix const& W) {
  Pattern new_pattern{corr_pattern};
  std::vector<Pattern> evolution{new_pattern};

  bool finished{false};

  while (!finished) {
    for (unsigned i{0}; i < getN(); ++i) {
      Pattern old_pattern{new_pattern};
      double sum{0.};

      for (unsigned j{0}; j < getN(); ++j) {
        sum += (W[i][j] * new_pattern[j]);
      }
      new_pattern[i] = (sum < 0) ? -1 : 1;
      evolution.push_back(new_pattern);
      if (new_pattern == old_pattern) {
        finished = true;
        break;
      }
    }
  }
  return evolution;
}

// std::vector<Pattern> Hopfield::updating(Pattern const& corr_pattern,
//                                         Matrix const& W) {
//   Pattern current_state{corr_pattern};
//   Pattern next_state{update(current_state, W)};
//
//   std::vector<Pattern> evolution{next_state};
//
//   while (next_state != current_state) {
//     current_state = next_state;
//     next_state = update(next_state, W);
//     evolution.push_back(next_state);
//   }
//
//   return evolution;
// }

double Hopfield::energy(Pattern const& state) {
  double energy;
  double sum{0.};
  for (unsigned i{0}; i < getN(); ++i) {
    for (unsigned j{0}; j < getN(); ++j) {
      sum += (W_[i][j] * state[i] * state[j]);
    }
  }
  energy = sum / (-2.);
  return energy;
}

}  // namespace nn
