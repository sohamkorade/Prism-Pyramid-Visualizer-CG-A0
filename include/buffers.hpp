#pragma once

// standard c++ headers
#include <iostream>
#include <vector>

// glfw and glad
#include <glad/glad.h>

// stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class VBO {
 public:
  GLuint ID;
  VBO(std::vector<GLfloat> &vertices) {
    glGenBuffers(1, &ID);
    bind();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);
  }
  ~VBO() { glDeleteBuffers(1, &ID); }
  void bind() { glBindBuffer(GL_ARRAY_BUFFER, ID); }
  void unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
};

class VAO {
 public:
  GLuint ID;
  VAO() {
    glGenVertexArrays(1, &ID);
    bind();
  }
  ~VAO() { glDeleteVertexArrays(1, &ID); }
  void add_attributes(VBO &vbo, GLuint index, GLint size, GLenum type,
                      GLboolean normalized, GLsizei stride,
                      const void *pointer) {
    vbo.bind();
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);
    vbo.unbind();
  }
  void bind() { glBindVertexArray(ID); }
  void unbind() { glBindVertexArray(0); }
};

class EBO {
 public:
  GLuint ID;
  EBO(std::vector<GLuint> &indices) {
    glGenBuffers(1, &ID);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);
  }
  ~EBO() { glDeleteBuffers(1, &ID); }
  void bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }
  void unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
};

class Texture {
 public:
  GLuint ID;
  Texture(std::string path) {
    glGenTextures(1, &ID);
    bind();
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data =
        stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      GLenum format;
      if (nrChannels == 1)
        format = GL_RED;
      else if (nrChannels == 3)
        format = GL_RGB;
      else if (nrChannels == 4)
        format = GL_RGBA;
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    unbind();
  }
  ~Texture() { glDeleteTextures(1, &ID); }
  void bind() { glBindTexture(GL_TEXTURE_2D, ID); }
  void unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
};