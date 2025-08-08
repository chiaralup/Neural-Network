#include <SFML/Graphics.hpp>
#include <cassert>
#include <fstream>
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

  if (!image2.loadFromFile("orecchino.png")) {
    std::cerr << "Errore nel caricamento dell'immagine" << '\n';
    return -1;
  }

  if (!texture1.loadFromFile("pillars.jpg")) {
    return -1;
  }

  if (!texture2.loadFromFile("orecchino.png")) {
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

  for (unsigned int r = 0; r < height; ++r) {
    double y{static_cast<double>(r) / static_cast<double>(by)};
    unsigned int j{static_cast<unsigned int>(y)};
    double t{y - j};

    for (unsigned int c = 0; c < width; ++c) {
      double x{static_cast<double>(c) / static_cast<double>(bx)};
      unsigned int i{static_cast<unsigned int>(x)};
      double s{x - i};

      // assert(i + 1 <= width && j + 1 <= height);

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
    }
  }

  std::vector<int> pattern1;
  std::vector<int> pattern2;

  for (unsigned int r = 0; r < height; ++r) {
    for (unsigned int c = 0; c < width; ++c) {
      sf::Color pix = image1.getPixel(c, r);
      double m{(pix.r + pix.g + pix.b) / 3.0};

      pattern1.push_back(m < 127 ? -1 : 1);
      // l'ho cambiato in operatore ternario, invece che ciclo if
    }
  }

  for (unsigned int r = 0; r < height; ++r) {
    for (unsigned int c = 0; c < width; ++c) {
      unsigned int index = r * width + c;

      assert(index < p.size() && "Indice fuori dai limiti nel vettore p");

      Pixel& pix = p[index];
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

  //  sf::Image resizedimage1;
  //  resizedimage1.create(width, height, sf::Color::Black);
  //
  //  for (unsigned int i = 0; i < pattern1.size(); ++i) {
  //    unsigned int row{i / width};
  //    unsigned int col{i % width};
  //
  //    if (pattern1[i] == 1) {
  //      resizedimage1.setPixel(col, row, sf::Color::White);
  //    }
  //  }
  //
  //  sf::Texture resizedtexture1;
  //  resizedtexture1.loadFromImage(resizedimage1);
  //  sf::Sprite resizedsprite1;
  //  resizedsprite1.setTexture(resizedtexture1);

  sf::Image resizedimage2;
  resizedimage2.create(width, height, sf::Color::Black);

  for (unsigned int i = 0; i < pattern2.size(); ++i) {
    unsigned int row{i / width};
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
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();
    window.draw(resizedsprite2);
    window.display();
  }
// primo tentativo 
  //  unsigned int N{width * height};
  //  std::vector<std::vector<int>> W(N, std::vector<int>(N));
  //
  //  for (unsigned int i{0}; i < N; ++i) {
  //    for (unsigned int j{0}; j < N; ++j) {
  //      if (i == j) {
  //        W[i][j] = 0;
  //      } else {
  //        double sum{(static_cast<double>(pattern1[i] * pattern1[j] +
  //                                        pattern2[i] * pattern2[j])) /
  //                   static_cast<double>(N)};
  //        W[i][j] = static_cast<int>(sum);
  //      }
  //    }
  //  }
  //  std::ofstream file("/home/aalessia/programmazione/finale/weight.txt");
  //  if (!file.is_open()) {
  //    throw std::runtime_error{"Impossible to open file!"};
  //  }
  //  for (const auto& r : W) {
  //    for (const auto& val : r) {
  //      file << val << " ";
  //    }
  //    file << '\n';
  //  }
  //
  //  file.close();
  //
// secondo tentativo  
  //  unsigned int N{width * height};
  //  std::vector<std::vector<double>> W(N, std::vector<double>(N, 0.));
  //  for (unsigned int i{0}; i < N; ++i) {
  //    for (unsigned int j{0}; j < N; ++j) {
  //      if (i != j) {
  //        W[i][j] = (1.0 / N) * (static_cast<double>(pattern1[i] * pattern1[j]
  //        +
  //                                                   pattern2[i] *
  //                                                   pattern2[j]));
  //      } else {
  //        W[i][j] = 0.;
  //      }
  //    }
  //  }
  //
  //  std::ofstream file("weight.txt");
  //  if (!file.is_open()) {
  //    throw std::runtime_error{"Impossibile aprire il file weight.txt!"};
  //  }
  //
  //  for (unsigned int i = 0; i < N; ++i) {
  //    for (unsigned int j = 0; j < N; ++j) {
  //      file << W[i][j] << " ";
  //    }
  //    file << '\n';
  //  }
  //
  //  file.close();

  
  //terzo tentativo
  //  unsigned int N = width * height;
  //
  //  std::ofstream file("weight.txt");
  //  if (!file.is_open()) {
  //    throw std::runtime_error{"Impossibile aprire il file weight.txt!"};
  //  }
  //
  //  for (unsigned int i = 0; i < N; ++i) {
  //    for (unsigned int j = 0; j < N; ++j) {
  //      if (i == j) {
  //        file << 0 << " ";
  //      } else {
  //        int val = static_cast<int>(
  //            (pattern1[i] * pattern1[j] + pattern2[i] * pattern2[j]) /
  //            static_cast<double>(N));
  //        file << val << " ";
  //      }
  //    }
  //    file << '\n';
  //  }
  //
  //  file.close();
  //
  
};