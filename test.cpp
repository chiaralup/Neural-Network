#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "hopfield.hpp"

//TEST_CASE("Testing resizeimage with 2x2 iamge") {
//  Hopfield hop;
//  hop.height_ = 2;
//  hop.width_ = 2;
//
//  sf::Image image;
//  image.create(2, 2);
//  image.setPixel(0, 0, sf::Color::Black);
//  image.setPixel(0, 0, sf::Color::Red);
//  image.setPixel(0, 0, sf::Color::Green);
//  image.setPixel(0, 0, sf::Color::Blue);
//
//  auto resized = hop.resizeimage(image);
//
//  CHECK(resized.size() == hop.width_ * hop.height_);
//  if (resized.size() == hop.width_ * hop.height_) {
//    CHECK(resized[0].r == 0);
//    CHECK(resized[0].g == 0);
//    CHECK(resized[0].b == 0);
//
//    CHECK(resized[1].r == 255);
//    CHECK(resized[1].g == 0);
//    CHECK(resized[1].b == 0);
//
//    CHECK(resized[2].r == 0);
//    CHECK(resized[2].g == 255);
//    CHECK(resized[2].b == 0);
//
//    CHECK(resized[3].r == 0);
//    CHECK(resized[3].g == 0);
//    CHECK(resized[3].b == 255);
//  }
//}