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