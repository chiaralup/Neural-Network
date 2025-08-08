#ifndef ACQUISITION_HPP
#define ACQUISITION_HPP

struct Pixel {
  unsigned int pr;
  unsigned int pg;
  unsigned int pb;
};

class Acquisition {
 private:  // o protected?
  sf::Image image1_;
  sf::Image image2_;
  sf::Texture texture1;
  sf::Texture texture2;
  sf::Vector2u pillars;
  sf::Vector2u deep;

  unsigned int width_{150};
  unsigned int height_{200};

 public:
  Acquisition(const auto& image1, const auto& image2);
  void resizeimage(const auto& image);
  auto pattern1(const auto& image);
  auto pattern(const auto& image);

  // unsigned int getWidth() const { return width; }
  // unsigned int getHeight() const { return height; }
};

#endif