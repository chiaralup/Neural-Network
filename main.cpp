#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>

int main() {
  struct Pixel {
    unsigned int pr;
    unsigned int pg;
    unsigned int pb;
  };

  sf::Image image1;
  sf::Image image2;
  sf::Texture texture1;
  sf::Texture texture2;

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

  auto pillars{image1.getSize()};
  auto deep{image2.getSize()};

  auto bx{pillars.x / deep.x};  // forse il tipo va specificato
  auto by{pillars.y / deep.y};

  std::vector<Pixel> p;

  std::vector<uint8_t> pixels(pillars.x * pillars.y * 4);  // preso da sfml

  for (unsigned int c = 0; c < pillars.y; c++) {
    double y{static_cast<double>(c) / static_cast<double>(by)};
    unsigned int j{static_cast<unsigned int>(y)};
    double t{y - j};

    for (unsigned int r = 0; r < pillars.x; r++) {
      double x{static_cast<double>(r) / static_cast<double>(bx)};
      unsigned int i{static_cast<unsigned int>(x)};
      double s{x - i};

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

      p.push_back({pr, pg, pb});

      auto f = (c * pillars.x + r) * (+4);

      pixels[f] = static_cast<unsigned char>(pr);
      pixels[f + 1] = static_cast<unsigned char>(pg);
      pixels[f + 2] = static_cast<unsigned char>(pb);
      pixels[f + 3] = 255;  // completamente opaco
    }
  }

  std::vector<int> pattern1;
  std::vector<int> pattern2;

  for (unsigned int r = 0; r < pillars.x; r++) {
    for (unsigned int c = 0; c < pillars.y; c++) {
      sf::Color pix = image1.getPixel(r, c);
      double m{(pix.r + pix.g + pix.b) / 3.};

      pattern1.push_back(m < 127 ? -1 : 1);
      // l'ho cambiato in operatore ternario, invece che ciclo if
    }
  }

  // credo che il problema sia in questo ciclo, perchè stiamo lavorando su deep,
  // ma usando le grandezze di pillars, l'assert fallisce sempre
  // dovremmo provare a scrivere un parte di programma che controlla il massimo
  // numero di pixel sia 255

  for (unsigned int r = 0; r < deep.x; r++) {
    for (unsigned int c = 0; c < deep.y; c++) {
      unsigned int index = c * pillars.x + r;  // vettore ad una dimensione

      assert(index < p.size() && "Indice fuori dai limiti nel vettore p");

      Pixel& pix = p[index];  // riferimento alla struct, vettore che ci serve
      double m{(pix.pr + pix.pg + pix.pb) / 3.0};

      pattern2.push_back(m < 127 ? -1 : 1);
    }
  }

  sf::RenderWindow window(sf::VideoMode(800, 600), "Neural Network");

  // std::cout << image1.getSize().x << " " << image1.getSize().y << '\n';
  // std::cout << image2.getSize().x << " " << image2.getSize().y << '\n';

  sf::Sprite sprite1;
  sprite1.setTexture(texture1);
  sf::Sprite sprite2;
  sprite2.setTexture(texture2);

  sf::Image resizedimage;
  resizedimage.create(pillars.x, pillars.y, pixels.data());

  sf::Texture resizedtexture;
  resizedtexture.loadFromImage(resizedimage);

  sf::Sprite resizedSprite;
  resizedSprite.setTexture(resizedtexture);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    window.clear();
    window.draw(resizedSprite);
    window.display();
  }

  return 0;
}
