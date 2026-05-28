#include "glad/gl.h"
#include "shader.h"

#include <GLFW/glfw3.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Settings ///////////////////////////////////////////////////////////////////

const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "ocio_opengl";

// Variables //////////////////////////////////////////////////////////////////

GLFWwindow* window_;

unsigned int shader_program_;

unsigned int VBO_, VAO_, EBO_;

// Forward declarations ///////////////////////////////////////////////////////

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void process_input(GLFWwindow* window);

// Functions //////////////////////////////////////////////////////////////////

void init_glfw(void) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void create_window(void) {
  window_ = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);

  if (window_ == NULL) printf("Failed to create GLFW window\n");

  glfwMakeContextCurrent(window_);
  glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
}

void load_glad(void) {
  if (!gladLoadGL(glfwGetProcAddress)) printf("Failed to initialize GLAD\n");
}

void init_opengl(void) { glClearColor(0.2f, 0.3f, 0.3f, 1.0f); }

void load_geometry() {
  const float vertices[] = {
    // positions         // colors
    0.5f,
    -0.5f,
    0.0f,
    1.0f,
    0.0f,
    0.0f, // bottom right
    -0.5f,
    -0.5f,
    0.0f,
    0.0f,
    1.0f,
    0.0f, // bottom left
    0.0f,
    0.5f,
    0.0f,
    0.0f,
    0.0f,
    1.0f // top
  };
  const unsigned int indices[] = {0, 1, 2};

  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glGenBuffers(1, &EBO_);

  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW
  );

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
    1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float))
  );
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void render_loop(void) {
  while (!glfwWindowShouldClose(window_)) {
    process_input(window_);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program_);
    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
}

void destroy_geometry() {
  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &VBO_);
  glDeleteBuffers(1, &EBO_);
}

void destroy_shaders() { glDeleteProgram(shader_program_); }

void destroy_glfw(void) { glfwTerminate(); }

void framebuffer_size_callback(
  GLFWwindow* window, const int width, const int height
) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// Main ///////////////////////////////////////////////////////////////////////

int main(void) {
  init_glfw();
  create_window();
  load_glad();
  init_opengl();
  shader_program_ =
    load_shaders("../src/shaders/shader.vert", "../src/shaders/shader.frag");
  load_geometry();

  render_loop();

  destroy_geometry();
  destroy_shaders();
  destroy_glfw();
  return 0;
}
