#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
  struct Pixel {
    double pr;
    double pg;
    double pb;
  };

  sf::Image image1;
  sf::Image image2;
  sf::Texture texture1;
  sf::Texture texture2;

  auto pillars{image1.getSize()};
  auto deep{image2.getSize()};

  auto bx{pillars.x / deep.x};  // forse il tipo va specificato
  auto by{pillars.y / deep.y};

  std::vector<Pixel> p;

  for (unsigned int r = 0; r < pillars.x; r++) {
    double x{static_cast<double>(r) / static_cast<double>(bx)};
    unsigned int i{static_cast<unsigned int>(x)};
    double s{x - i};

    for (unsigned int c = 0; c < pillars.y; c++) {
      double y{static_cast<double>(c) / static_cast<double>(by)};
      unsigned int j{static_cast<unsigned int>(y)};
      double t{y - j};

      sf::Color p1 = image2.getPixel(i, j);
      sf::Color p2 = image2.getPixel(i + 1, j);
      sf::Color p3 = image2.getPixel(i, j + 1);
      sf::Color p4 = image2.getPixel(i + 1, j + 1);

      double pr = (1 - s) * (1 - t) * p1.r + s * (1 - t) * p2.r +
                  (1 - s) * t * p3.r + s * t * p4.r;
      double pg = (1 - s) * (1 - t) * p1.g + s * (1 - t) * p2.g +
                  (1 - s) * t * p3.g + s * t * p4.g;
      double pb = (1 - s) * (1 - t) * p1.b + s * (1 - t) * p2.b +
                  (1 - s) * t * p3.b + s * t * p4.b;

      p.push_back({pr, pg, pb});
    }
  }

  std::vector<int> pattern1;
  std::vector<int> pattern2;

  for (unsigned int r = 0; r < pillars.x; r++) {
    for (unsigned int c = 0; c < pillars.y; c++) {
      sf::Color pix = image1.getPixel(r, c);
      double m{(pix.r + pix.g + pix.b) / 3.};
      if (m < 127) {
        pattern1.push_back(-1);
      } else {
        pattern1.push_back(+1);
      }
    }
  }

  for (unsigned int r = 0; r < deep.x; r++) {
    for (unsigned int c = 0; c < deep.y; c++) {
      unsigned int index = r * deep.y + c;  // vettore ad un dimensione
      Pixel& pix = p[index];  // riferimento alla struct, vettore che ci serve
      double m{(pix.pr + pix.pg + pix.pb) / 3.0};

      if (m < 127) {
        pattern2.push_back(-1);
      } else {
        pattern2.push_back(+1);
      }
    }
  }

  // auto [width, height] = texture2.getSize();
  // std::vector<uint8_t> pixels(width * height * 3);  // preso da sfml
  // texture2.update(pixels.data());

  sf::RenderWindow window(sf::VideoMode(800, 600), "Neural Network");

  if (!image1.loadFromFile("pillars.jpg")) {
    std::cerr << "Errore nel caricamento dell'immagine" << '\n';
    return -1;
  }

  if (!image2.loadFromFile("deepsky.jpg")) {
    std::cerr << "Errore nel caricamento dell'immagine" << '\n';
    return -1;
  }

  if (!texture1.loadFromFile("pillars.jpg")) {
    return -1;
  }

  if (!texture2.loadFromFile("deepsky.jpg")) {
    return -1;
  }

  std::cout << image1.getSize().x << " " << image1.getSize().y << '\n';
  std::cout << image2.getSize().x << " " << image2.getSize().y << '\n';

  // sf::Sprite sprite1;
  // sprite1.setTexture(texture1);
  // sf::Sprite sprite2;
  // sprite2.setTexture(texture2);
  //
  // while (window.isOpen()) {
  //   sf::Event event;
  //   while (window.pollEvent(event)) {
  //     if (event.type == sf::Event::Closed) window.close();
  //   }
  //
  //   window.clear();
  //   window.display();
  // }
  //
  // return 0;
}
