#ifndef ACQUISITION_HPP
#define ACQUISITION_HPP

struct Pixel {
  unsigned int pr;
  unsigned int pg;
  unsigned int pb;
};

class Acquisition {
 private:  // o protected?
  sf::Image image1;
  sf::Image image2;
  sf::Texture texture1;
  sf::Texture texture2;
  sf::Vector2u pillars;
  sf::Vector2u deep;

  unsigned int width;
  unsigned int height;
  double bx;
  double by;
  std::vector<Pixel> p_{};
  std::vector<int> pattern1_{};
  std::vector<int> pattern2_{};

 public:
  Acquisition(const sf::Image& image1, const sf::Image& image2);
  void resizedimage();
  void pixel1();
  void pixel2();
};

#endif