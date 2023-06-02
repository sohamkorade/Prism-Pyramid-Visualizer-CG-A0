#include <engine.hpp>

int x = 0;
void update(Game &game) {
  x++;
  if (x > game.width) x = 0;
}

void render(Game &game) { game.text("hello", x, 0, 1, {1, 1, 1}, "antonio"); }

void processInput(Game &game) {
  if (game.on_keypress(GLFW_KEY_ESCAPE)) game.close();
}

int main() {
  Game game("Assignment 0", 800, 600);
  game.load_font("fonts/Antonio-Bold.ttf", "antonio");

  Mesh shape(
      "src/shader.vs", "src/shader.fs",
      {
          {3, GL_FLOAT},  // position
          {3, GL_FLOAT},  // color
          {2, GL_FLOAT}   // texture
      },
      {
          // positions          // colors           // texture coords
          0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
          0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
          -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f   // bottom left
      },
      {
          0, 1, 2  // first triangle
      },
      "textures/cement_wall.jpeg", GL_TRIANGLES);

  game.add_shape(shape);

  game.loop(processInput, update, render);
}
