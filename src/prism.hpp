#include <engine.hpp>

std::vector<float> flatten(std::vector<glm::vec3> vertices,
                           glm::vec3 color = randcolor()) {
  std::vector<float> points;
  for (auto &v : vertices) {
    // position
    points.push_back(v.x);
    points.push_back(v.y);
    points.push_back(v.z);
    // color
    points.push_back(color.r);
    points.push_back(color.g);
    points.push_back(color.b);
    // texture
    points.push_back(0);
    points.push_back(0);
  }
  return points;
}

std::vector<Mesh *> generate_prism(int sides, float length,
                                    glm::vec3 basecolor = randcolor()) {
  // Shader *shader = new Shader("shaders/shader.frag", "shaders/shader.vert");
  // Texture *texture = new Texture("textures/cement_wall.jpeg");

  std::vector<Mesh *> shapes;

  float angle = 2 * M_PI / sides;
  auto ray = glm::vec4(length / 1.5, 0, 0, 1);
  auto axis = glm::vec3(0, 0, 1);
  auto rot = glm::rotate(glm::mat4(1), angle, axis);

  std::vector<glm::vec3> vertices;
  std::vector<GLuint> indices;

  // for base
  for (int i = 0; i < sides; i++) {
    indices.push_back(i);
    vertices.push_back(ray);
    ray = rot * ray;
  }

  shapes.push_back(
      new Mesh(flatten(vertices, basecolor), indices, GL_TRIANGLE_FAN));

  // for sides
  std::vector<glm::vec3> vertices2;
  for (auto &v : vertices) {
    vertices2.push_back(v);
    vertices2.push_back(glm::vec3(v.x, v.y, v.z + length));
  }
  // close the sides
  auto v = vertices.front();
  vertices2.push_back(v);
  vertices2.push_back(glm::vec3(v.x, v.y, v.z + length));

  std::vector<GLuint> indices2(sides * 2 + 2);
  std::iota(indices2.begin(), indices2.end(), 0);

  std::vector<float> points;
  int i = 0;
  glm::vec3 color = randcolor();
  auto top = glm::vec3(0.0f, 0.0f, length);
  for (auto &v : vertices2) {
    // change color after every quad
    if (i % 4 == 0) color = randcolor();
    // position
    points.push_back(v.x);
    points.push_back(v.y);
    points.push_back(v.z);
    // color
    points.push_back(color.r);
    points.push_back(color.g);
    points.push_back(color.b);
    // texture
    points.push_back(0);
    points.push_back(0);
    // position 2
    bool topvertex = (i % 2 == 1);
    points.push_back(topvertex ? top.x : v.x);
    points.push_back(topvertex ? top.y : v.y);
    points.push_back(topvertex ? top.z : v.z);
    i++;
  }

  shapes.push_back(new Mesh(points, indices2, GL_TRIANGLE_STRIP,
                             "shaders/sides.vert", "shaders/sides.frag",
                             "textures/cement_wall.jpeg",
                             {
                                 {3, GL_FLOAT},  // position
                                 {3, GL_FLOAT},  // color
                                 {2, GL_FLOAT},  // texture
                                 {3, GL_FLOAT},  // position 2
                             }));

  // for top
  // use the same vertices as the base but move them up
  for (auto &v : vertices) {
    v.z += length;
  }

  std::vector<float> points2;
  glm::vec3 color2 = randcolor();
  i = 0;
  for (auto &v : vertices) {
    // position
    points2.push_back(v.x);
    points2.push_back(v.y);
    points2.push_back(v.z);
    // color
    points2.push_back(color2.r);
    points2.push_back(color2.g);
    points2.push_back(color2.b);
    // texture
    points2.push_back(0);
    points2.push_back(0);
    // position 2
    points2.push_back(top.x);
    points2.push_back(top.y);
    points2.push_back(top.z);
    i++;
  }
  shapes.push_back(new Mesh(points2, indices, GL_TRIANGLE_FAN,
                             "shaders/sides.vert", "shaders/sides.frag",
                             "textures/cement_wall.jpeg",
                             {
                                 {3, GL_FLOAT},  // position
                                 {3, GL_FLOAT},  // color
                                 {2, GL_FLOAT},  // texture
                                 {3, GL_FLOAT},  // position 2
                             }));

  return shapes;
}