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

sf::Uint8 Hopfield::interpolation(unsigned p1, unsigned p2, unsigned p3,
                                  unsigned p4, double s, double t) {
  double const p{std::round((1 - s) * (1 - t) * p1 + s * (1 - t) * p2 +
                            (1 - s) * t * p3 + s * t * p4)};

  assert(p < 256);

  return static_cast<sf::Uint8>(p);
}

std::vector<sf::Color> Hopfield::resizeImage(sf::Image const& image) {
  auto const size{image.getSize()};
  if (size.x < 2 || size.y < 2) {
    throw std::runtime_error(
        "resize_image: too small image, both dimensions must be >= 3");
  }
  std::vector<sf::Color> p;

  double const bx{static_cast<double>(width_) / static_cast<double>(size.x)};
  double const by{static_cast<double>(height_) / static_cast<double>(size.y)};

  for (unsigned r{0}; r < height_; ++r) {
    double y{static_cast<double>(r) / by};
    unsigned j{static_cast<unsigned>(y)};

    const double t{j + 1 < size.y ? y - j : 0.};

    for (unsigned c{0}; c < width_; ++c) {
      double x{static_cast<double>(c) / bx};
      unsigned i{static_cast<unsigned>(x)};

      double const s{i + 1 < size.x ? x - i : 0.};

      sf::Color p1{image.getPixel(i, j)};
      sf::Color p2{(i + 1 < size.x) ? image.getPixel(i + 1, j) : p1};
      sf::Color p3{(j + 1 < size.y) ? image.getPixel(i, j + 1) : p1};
      sf::Color p4{(i + 1 < size.x && j + 1 < size.y)
                       ? image.getPixel(i + 1, j + 1)
                       : p1};

      auto const pr{interpolation(p1.r, p2.r, p3.r, p4.r, s, t)};
      auto const pg{interpolation(p1.g, p2.g, p3.g, p4.g, s, t)};
      auto const pb{interpolation(p1.b, p2.b, p3.b, p4.b, s, t)};

      p.push_back({pr, pg, pb});
    }
  }
  return p;
}

Pattern Hopfield::pattern(sf::Image const& image) {
  Pattern pattern;
  auto const& p{resizeImage(image)};

  for (const auto& pix : p) {
    double m{(pix.r + pix.g + pix.b) / 3.0};
    pattern.push_back(m < 127 ? -1 : 1);
  }

  assert(pattern.size() == getN());

  return pattern;
}

Drawable Hopfield::bawImage(Pattern const& pattern) {
  Drawable drawable;
  drawable.image.create(width_, height_, sf::Color::Black);

  for (unsigned i{0}; i < pattern.size(); ++i) {
    unsigned const row{i / width_};
    unsigned const col{i % width_};

    assert(row < height_ && col < width_);

    if (pattern[i] == 1) {
      drawable.image.setPixel(col, row, sf::Color::White);
    }
  }

  drawable.texture.loadFromImage(drawable.image);
  drawable.sprite.setTexture(drawable.texture);

  return drawable;
}

Pattern Hopfield::corruption(Pattern const& pattern, unsigned ratio) {
  if (ratio == 0) {
    throw std::runtime_error("ratio must be > 0");
  }

  std::random_device r;
  std::default_random_engine eng(r());
  std::uniform_int_distribution<unsigned> random_pix(0, getN() - 1);

  Pattern corr_pattern{pattern};

  auto const n_swaps{getN() / ratio};
  for (unsigned i{0}; i < n_swaps; ++i) {
    auto const n{random_pix(eng)};

    corr_pattern[n] *= -1;
  }

  return corr_pattern;
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

void Hopfield::matrix(std::vector<Pattern> const& patterns) {
  std::ofstream file("weights.txt");
  if (!file.is_open()) {
    throw std::runtime_error{"Unable to open the file weight.txt"};
  }

  auto n{getN()};

  for (unsigned i{0}; i < n; ++i) {
    for (unsigned j{i + 1}; j < n; ++j) {
      int sum{0};
      for (auto const& p : patterns) {
        sum += p[i] * p[j];
      }
      W_[i][j] = static_cast<double>(sum) / static_cast<double>(n);
      W_[j][i] = W_[i][j];
    }
  }

  for (unsigned i{0}; i < getN(); ++i) {
    for (unsigned j{0}; j < getN(); ++j) {
      file << W_[i][j] << " ";
    }
    file << '\n';
  }
}

Matrix Hopfield::getMatrix() {
  std::ifstream file{"./weights.txt"};
  if (!file) {
    throw std::runtime_error{"Unable to open the file weight.txt"};
  }

  auto n{getN()};
  for (unsigned i{0}; i < n; ++i) {
    for (unsigned j{0}; j < n; ++j) {
      double Wij;
      file >> Wij;
      W_[i][j] = Wij;
      if ((i == j) && (W_[i][j] != 0)) {
        throw std::runtime_error{"W_[i][j] should be 0 in the diagonal"};
      }
    }
  }
  return W_;
}

Pattern Hopfield::update(const Pattern& corr_pattern) {
  Pattern new_pattern{corr_pattern};

  auto n{getN()};

  for (unsigned i{0}; i < n; ++i) {
    double sum{0.};
    for (unsigned j{0}; j < n; ++j) {
      sum += (W_[i][j] * new_pattern[j]);
    }
    new_pattern[i] = (sum < 0) ? -1 : 1;
  }

  return new_pattern;
}

double Hopfield::energy(Pattern const& state) {
  double energy{0.};
  auto n{getN()};

  for (unsigned i{0}; i < n; ++i) {
    for (unsigned j{i + 1}; j < n; ++j) {
      energy += (W_[i][j] * state[i] * state[j]);
    }
  }
  energy = -energy;
  return energy;
}

}  // namespace nn
