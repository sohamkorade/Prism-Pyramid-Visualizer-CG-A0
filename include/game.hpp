#pragma once

// glfw and glad
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION

// helpers
#include "buffers.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "shape.hpp"
#include "text.hpp"
#include "utils.hpp"

// mouse
bool mouse_first = true;
float mouse_yaw = -90.0f;  // yaw is initialized to -90.0 degrees since a yaw of
                           // 0.0 results in a direction vector pointing to the
                           // right so we initially rotate a bit to the left.
float mouse_pitch = 0.0f;
float mouse_lastX = 800.0f / 2.0;
float mouse_lastY = 600.0 / 2.0;
float mouse_fov = 45.0f;
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (mouse_first) {
    mouse_lastX = xpos;
    mouse_lastY = ypos;
    mouse_first = false;
  }

  float xoffset = xpos - mouse_lastX;
  float yoffset =
      mouse_lastY - ypos;  // reversed since y-coordinates go from bottom to top
  mouse_lastX = xpos;
  mouse_lastY = ypos;

  float sensitivity = 0.01f;  // change this value to your liking
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  mouse_yaw += xoffset;
  mouse_pitch += yoffset;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (mouse_pitch > 89.0f) mouse_pitch = 89.0f;
  if (mouse_pitch < -89.0f) mouse_pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(mouse_yaw)) * cos(glm::radians(mouse_pitch));
  front.y = sin(glm::radians(mouse_pitch));
  front.z = sin(glm::radians(mouse_yaw)) * cos(glm::radians(mouse_pitch));
  cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  mouse_fov -= (float)yoffset;
  if (mouse_fov < 1.0f) mouse_fov = 1.0f;
  if (mouse_fov > 45.0f) mouse_fov = 45.0f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, true);

    // toggle wireframe mode (useful for debugging)
    if (key == GLFW_KEY_COMMA)
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else if (key == GLFW_KEY_PERIOD)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
}

// glfw: initialize and configure
void glfw_ready() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

// glad: load all OpenGL function pointers
void glad_ready() {
  // gladLoadGL();
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    die("Failed to initialize GLAD");

  glEnable(GL_DEPTH_TEST);
}

GLFWwindow *make_window(int width, int height, std::string title) {
  glfw_ready();

  // glfw window creation
  GLFWwindow *window =
      glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!window) {
    glfwTerminate();
    die("Failed to create GLFW window");
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

  glad_ready();

  return window;
}

void clear_screen(glm::vec3 color) {
  glClearColor(color.r, color.b, color.g, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

class Game {
 public:
  std::string title;
  int width, height;
  GLFWwindow *window;
  glm::vec3 bg_color = rgb(0.1f, 0.1f, 0.1f);
  std::map<std::string, Font> fonts;
  std::vector<Mesh *> shapes;

  // camera
  Camera camera;

  Game(std::string title, int width, int height)
      : title(title), width(width), height(height) {
    window = make_window(width, height, title);
    load_font("fonts/Antonio-Bold.ttf", "antonio");  // default font
    camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
    camera.aspect_ratio = (float)width / (float)height;
    // enable_mouse();
    // basic_lighting();
  }

  ~Game() {
    delete_fonts();
    delete_shapes();
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  void delete_fonts() {
    for (auto &pair : fonts) {
      Font &f = pair.second;
      glDeleteTextures(1, &f.glyphs['A'].TextureID);
      glDeleteVertexArrays(1, &f.VAO);
      glDeleteBuffers(1, &f.VBO);
    }
  }

  void delete_shapes() {
    for (auto &shape : shapes) delete shape;
    shapes.clear();
  }

  void load_font(std::string font_name, std::string alias) {
    fonts[alias] = compile_font(font_name, width, height);
  }

  void text(std::string text, float x, float y, float scale = 1.0f,
            glm::vec3 color = Colors::white,
            std::string font_alias = "antonio") {
    if (fonts.count(font_alias) == 0) die("Font alias not found: ", font_alias);
    RenderText(text, x, y, scale, color, fonts[font_alias]);
  }

  void text(std::vector<std::string> lines, float x, float y,
            float scale = 1.0f, glm::vec3 color = Colors::white,
            std::string font_alias = "antonio") {
    if (fonts.count(font_alias) == 0) die("Font alias not found: ", font_alias);
    for (auto &line : lines) {
      RenderText(line, x, y, scale, color, fonts[font_alias]);
      y -= 50.0f * scale + 10.0f;
    }
  }

  void loop(void processInput(Game &), void update(Game &),
            void render(Game &)) {
    while (!glfwWindowShouldClose(window)) {
      camera.new_frame();

      processInput(*this);
      kbd_move_camera();

      for (auto &shape : shapes) basic_shape_move(shape);

      update(*this);

      clear_screen(bg_color);

      for (auto &shape : shapes) shape->render(camera);

      render(*this);

      // glfw: swap buffers and poll IO events
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  bool on_keypress(int key) { return glfwGetKey(window, key) == GLFW_PRESS; }
  bool on_keyrelease(int key) {
    return glfwGetKey(window, key) == GLFW_RELEASE;
  }
  bool on_keyup(int key) {
    if (on_keypress(key)) {
      while (!on_keyrelease(key)) glfwWaitEvents();
      return true;
    }
    return false;
  }
  void close() { glfwSetWindowShouldClose(window, true); }

  void add_shape(Mesh *shape) { shapes.push_back(shape); }
  void add_shapes(std::vector<Mesh *> &shapes) {
    for (auto &shape : shapes) add_shape(shape);
  }

  void kbd_move_camera() {
    if (on_keypress(GLFW_KEY_W)) camera.move(FORWARD);
    if (on_keypress(GLFW_KEY_S)) camera.move(BACKWARD);
    if (on_keypress(GLFW_KEY_A)) camera.move(LEFT);
    if (on_keypress(GLFW_KEY_D)) camera.move(RIGHT);
    if (on_keypress(GLFW_KEY_Q)) camera.move(UP);
    if (on_keypress(GLFW_KEY_E)) camera.move(DOWN);

    // arcball camera
    if (on_keypress(GLFW_KEY_KP_4)) camera.rotate(LEFT);
    if (on_keypress(GLFW_KEY_KP_6)) camera.rotate(RIGHT);
    if (on_keypress(GLFW_KEY_KP_8)) camera.rotate(UP);
    if (on_keypress(GLFW_KEY_KP_2)) camera.rotate(DOWN);
  }

  void enable_mouse() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
  }

  void disable_mouse() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(window, NULL);
    glfwSetScrollCallback(window, NULL);
  }

  void basic_lighting() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  void basic_shape_move(Mesh *shape) {
    const bool shift =
        on_keypress(GLFW_KEY_LEFT_SHIFT) || on_keypress(GLFW_KEY_RIGHT_SHIFT);
    const int sign = shift ? -1 : +1;

    auto &s = shape->state;

    const float ANGLE = 1.0f;
    const float DISTANCE = 0.01f;

    // translation
    if (on_keypress(GLFW_KEY_I)) s.position.y += DISTANCE;
    if (on_keypress(GLFW_KEY_K)) s.position.y -= DISTANCE;
    if (on_keypress(GLFW_KEY_J)) s.position.x -= DISTANCE;
    if (on_keypress(GLFW_KEY_L)) s.position.x += DISTANCE;
    if (on_keypress(GLFW_KEY_O)) s.position.z -= DISTANCE;
    if (on_keypress(GLFW_KEY_U)) s.position.z += DISTANCE;

    // auto rotation
    if (on_keypress(GLFW_KEY_V)) s.rotation_axis = 1 * sign;
    if (on_keypress(GLFW_KEY_B)) s.rotation_axis = 2 * sign;
    if (on_keypress(GLFW_KEY_N)) s.rotation_axis = 3 * sign;
    if (on_keypress(GLFW_KEY_M)) s.rotation_axis = 0 * sign;

    // manual rotation
    if (on_keypress(GLFW_KEY_Z)) shape->rotate(BasisVectors::X, ANGLE * sign);
    if (on_keypress(GLFW_KEY_X)) shape->rotate(BasisVectors::Y, ANGLE * sign);
    if (on_keypress(GLFW_KEY_C)) shape->rotate(BasisVectors::Z, ANGLE * sign);
  }
};
