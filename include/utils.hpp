#pragma once

// standard
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// show error message and exit
void die(std::string msg, std::string msg2 = "") {
  std::cerr << msg << " " << msg2 << std::endl;
  exit(-1);
}

glm::vec3 rgb(float r, float g, float b) { return glm::vec3(r, g, b); }

float randfloat(float min, float max) {
  return min + float(rand()) / float(RAND_MAX / (max - min));
}

glm::vec3 randcolor() {
  return rgb(randfloat(0, 1), randfloat(0, 1), randfloat(0, 1));
}

namespace Colors {
const glm::vec3 white = rgb(1, 1, 1);
const glm::vec3 black = rgb(0, 0, 0);
const glm::vec3 red = rgb(1, 0, 0);
const glm::vec3 green = rgb(0, 1, 0);
const glm::vec3 blue = rgb(0, 0, 1);
const glm::vec3 yellow = rgb(1, 1, 0);
}  // namespace Colors

namespace BasisVectors {
const glm::vec3 X = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 Y = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Z = glm::vec3(0.0f, 0.0f, 1.0f);
}  // namespace BasisVectors