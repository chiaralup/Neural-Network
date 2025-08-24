# Neural-Network
cmake -S . -B build -G"Ninja Multi-Config"
cmake --build build --config Debug
cmake --build build --config Debug --target test
cmake --build build --config Release
cmake --build build --config Release --target test

./build/Debug/neuralnet
./build/Release/neuralnet

#include <numeric>
//... altre inclusioni

std::vector<Pixel> Hopfield::resize_image(sf::Image const& image) {
  // L'assert iniziale verifica che le dimensioni dell'immagine non siano nulle.
  assert(image.getSize().x > 0 && image.getSize().y > 0 && "L'immagine di origine non puo' essere vuota.");

  const auto original_size = image.getSize();
  const unsigned int N_ = width_ * height_;

  std::vector<Pixel> resized_pixels;
  resized_pixels.reserve(N_); // Pre-alloca la memoria per evitare riallocazioni.

  // Crea una sequenza lineare di indici da 0 a N_-1.
  std::vector<unsigned int> indices(N_);
  std::iota(indices.begin(), indices.end(), 0);

  // Usa std::transform per calcolare ogni pixel dell'immagine ridimensionata.
  std::transform(indices.begin(), indices.end(), std::back_inserter(resized_pixels),
                 [&](unsigned int linear_index) -> Pixel {
    unsigned int r{linear_index / width_};
    unsigned int c{linear_index % width_};

    const double by{static_cast<double>(height_) / static_cast<double>(original_size.y)};
    const double y{static_cast<double>(r) / by};
    const unsigned int j{static_cast<unsigned int>(y)};
    const double t{y - j};

    const double bx{static_cast<double>(width_) / static_cast<double>(original_size.x)};
    const double x{static_cast<double>(c) / bx};
    const unsigned int i{static_cast<unsigned int>(x)};
    const double s{x - i};

    // Asserzione per i limiti dell'immagine originale.
    assert(i + 1 < original_size.x && j + 1 < original_size.y &&
           "Indice fuori dai limiti nel campionamento bilineare");

    const sf::Color p1 = image.getPixel(i, j);
    const sf::Color p2 = image.getPixel(i + 1, j);
    const sf::Color p3 = image.getPixel(i, j + 1);
    const sf::Color p4 = image.getPixel(i + 1, j + 1);

    unsigned int pr{static_cast<unsigned int>(
        (1 - s) * (1 - t) * p1.r + s * (1 - t) * p2.r + (1 - s) * t * p3.r +
        s * t * p4.r)};
    unsigned int pg{static_cast<unsigned int>(
        (1 - s) * (1 - t) * p1.g + s * (1 - t) * p2.g + (1 - s) * t * p3.g +
        s * t * p4.g)};
    unsigned int pb{static_cast<unsigned int>(
        (1 - s) * (1 - t) * p1.b + s * (1 - t) * p2.b + (1 - s) * t * p3.b +
        s * t * p4.b)};

    // Asserzione per i valori dei pixel.
    assert(pr < 256 && pg < 256 && pb < 256 && "Valore di colore fuori range [0-255]");

    return {pr, pg, pb};
  });

  return resized_pixels;
}




std::vector<Pixel> Hopfield::resize_image(sf::Image const& image) {
  std::vector<Pixel> p;
  const auto size{image.getSize()};
  const double bx{static_cast<double>(size.x) / static_cast<double>(width_)};
  const double by{static_cast<double>(size.y) / static_cast<double>(height_)};

  for (unsigned int r{0}; r < height_; ++r) {
    // coordinate "pixel-centered"
    const double y{(r + 0.5) * by - 0.5};
    const unsigned int j{static_cast<unsigned int>(std::floor(y))};
    const double t{y - j};

    for (unsigned int c{0}; c < width_; ++c) {
      const double x{(c + 0.5) * bx - 0.5};
      const unsigned int i{static_cast<unsigned int>(std::floor(x))};
      const double s{x - i};

      // clamp (non schiacciare sempre a size-2)
      const unsigned int i0 = std::min(i,     size.x - 1);
      const unsigned int i1 = std::min(i + 1, size.x - 1);
      const unsigned int j0 = std::min(j,     size.y - 1);
      const unsigned int j1 = std::min(j + 1, size.y - 1);

      const sf::Color p1{image.getPixel(i0, j0)};
      const sf::Color p2{image.getPixel(i1, j0)};
      const sf::Color p3{image.getPixel(i0, j1)};
      const sf::Color p4{image.getPixel(i1, j1)};

      const auto interp = [&](unsigned char c1, unsigned char c2, unsigned char c3, unsigned char c4) {
        return static_cast<unsigned int>(std::round(
          (1 - s) * (1 - t) * c1 +
          s * (1 - t) * c2 +
          (1 - s) * t * c3 +
          s * t * c4
        ));
      };

      const unsigned int pr = interp(p1.r, p2.r, p3.r, p4.r);
      const unsigned int pg = interp(p1.g, p2.g, p3.g, p4.g);
      const unsigned int pb = interp(p1.b, p2.b, p3.b, p4.b);

      p.push_back({pr, pg, pb});
    }
  }
  return p;
}
