#include "engine.hpp"
#include "prism.hpp"

Game game("Assignment 0", 800, 600);
int sides = 3;
std::vector<Mesh *> prism;
float transition = 0.0f;
int transition_direction = 0;  // +1 for prism, -1 for pyramid
bool help = false;
int name_x = 0;

void update(Game &game) {
  name_x++;
  if (name_x > game.width) name_x = -100;

  if (transition_direction) {
    if (transition_direction == +1) {
      transition += 0.01;
      if (transition > 1.0) transition = 1.0, transition_direction = 0;
    } else if (transition_direction == -1) {
      transition -= 0.01;
      if (transition < 0.0) transition = 0.0, transition_direction = 0;
    }
    for (auto &s : prism) {
      s->shader->use();
      s->shader->setFloat("transition", transition);
    }
  }
  // game.camera.Front = cameraFront;
  // game.camera.Zoom = mouse_fov;
}

std::string prefix(int sides) {
  switch (sides) {
    case 3:
      return "triangular";
    case 4:
      return "square";
    case 5:
      return "pentagonal";
    case 6:
      return "hexagonal";
    case 7:
      return "heptagonal";
    case 8:
      return "octagonal";
    case 9:
      return "nonagonal";
    case 10:
      return "decagonal";
    default:
      return std::to_string(sides) + "-gonal";
  }
}

void render(Game &game) {
  std::string polyhedron = transition < 0.5 ? "prism" : "pyramid";
  std::string name = prefix(sides) + " " + polyhedron;

  if (name == "triangular pyramid") name = "tetrahedron";
  if (name == "square prism") name = "cube";
  game.text(name, name_x, 15.0, 0.8);

  if (help) {
    game.text(
        {
            "H = Close Help",
            "IJKLUO = Move Polyhedron",
            "WASDQE = Move Camera",
            "+- = Change sides",
            "T = Toggle Prism / Pyramid",
            "VBNM = Auto Rotation",
            "ZXC = Manual Rotation",
            "ESC = Exit",
        },
        0, game.height - 50, 0.8);
  } else {
    game.text("H = Help", 0, game.height - 50, 0.8);
  }
}

void create_shapes() {
  if (game.shapes.size() > 0) {
    // save state
    auto state = game.shapes.front()->state;
    game.delete_shapes();
    game.add_shapes(prism = generate_prism(sides, 0.7));
    // restore state
    for (auto &s : prism) s->state = state;
  } else {
    game.add_shapes(prism = generate_prism(sides, 0.7));
  }
}

void processInput(Game &game) {
  if (game.on_keyup(GLFW_KEY_T)) {
    // for (auto &s : prism) s->visible = !s->visible;
    if (transition_direction == 0)
      if (transition == 0.0)
        transition_direction = +1;
      else if (transition == 1.0)
        transition_direction = -1;
  }

  if (game.on_keyup(GLFW_KEY_H)) help = !help;
  if (game.on_keyup(GLFW_KEY_SPACE)) {
    // reset state
    for (auto &s : prism) s->state.reset();
    game.camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
  }

  if (game.on_keyup(GLFW_KEY_KP_ADD)) {
    sides++;
    create_shapes();
  }
  if (game.on_keyup(GLFW_KEY_KP_SUBTRACT) && sides > 3) {
    sides--;
    create_shapes();
  }
}

int main(int argc, char *argv[]) {
  // parse number of sides of the polygon in the prism
  if (argc > 1) sides = std::stoi(argv[1]);

  // Shader *shader = new Shader("shaders/shader.frag", "shaders/shader.vert");
  // Texture *texture = new Texture("textures/cement_wall.jpeg");

  create_shapes();

  game.loop(processInput, update, render);
}
