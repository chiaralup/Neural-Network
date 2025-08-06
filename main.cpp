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

  unsigned int width{pillars.x};
  unsigned int height{pillars.y};

  double bx{static_cast<double>(width) / static_cast<double>(deep.x)};
  double by{static_cast<double>(height) / static_cast<double>(deep.y)};

  std::cout << bx << " " << by << '\n';

  std::vector<Pixel> p;

  for (unsigned int c = 0; c < width; c++) {
    double x{static_cast<double>(c) / static_cast<double>(bx)};
    unsigned int i{static_cast<unsigned int>(x)};
    double s{x - i};

    for (unsigned int r = 0; r < height; r++) {
      double y{static_cast<double>(r) / static_cast<double>(by)};
      unsigned int j{static_cast<unsigned int>(y)};
      double t{y - j};

      sf::Color p1 = image2.getPixel(i, j);
      sf::Color p2 = image2.getPixel(i + 1, j);
      sf::Color p3 = image2.getPixel(i, j + 1);
      sf::Color p4 = image2.getPixel(i + 1, j + 1);

      //assert(i + 1 <= width && j + 1 <= height);

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

  std::vector<int> pattern1;
  std::vector<int> pattern2;

  for (unsigned int r = 0; r < width; r++) {
    for (unsigned int c = 0; c < height; c++) {
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

  for (unsigned int r = 0; r < width; r++) {     // riga
    for (unsigned int c = 0; c < height; c++) {  // colonna
      unsigned int index = c * width + r;        // vettore ad una dimensione

      assert(index < p.size() && "Indice fuori dai limiti nel vettore p");

      Pixel& pix = p[index];  // riferimento alla struct, vettore che ci serve
      double m{(pix.pr + pix.pg + pix.pb) / 3.0};

      pattern2.push_back(m < 127 ? -1 : 1);
    }
  }

  assert(pattern1.size() == width * height);
  assert(pattern2.size() == width * height);
  assert(pattern1.size() == pattern2.size());

  std::cout << "Valori bianchi in pattern2: "
            << std::count(pattern2.begin(), pattern2.end(), 1) << '\n';

  sf::RenderWindow window(sf::VideoMode(800, 600), "Neural Network");

  // std::cout << image1.getSize().x << " " << image1.getSize().y << '\n';
  // std::cout << image2.getSize().x << " " << image2.getSize().y << '\n';

  sf::Sprite sprite1;
  sprite1.setTexture(texture1);
  sf::Sprite sprite2;
  sprite2.setTexture(texture2);

  sf::Image resizedimage2;
  resizedimage2.create(width, height, sf::Color::Black);

  for (unsigned int i = 0; i < pattern2.size(); i++) {
    unsigned int row{(i / width)};
    unsigned int col{i % width};

    if (pattern2[i] == 1) {
      resizedimage2.setPixel(col, row, sf::Color::White);
    }
  }

  sf::Texture resizedtexture2;
  resizedtexture2.loadFromImage(resizedimage2);
  sf::Sprite resizedsprite2;
  resizedsprite2.setTexture(resizedtexture2);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    window.clear();
    window.draw(resizedsprite2);
    window.display();
  }

  return 0;
}
