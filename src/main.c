#include "glad/gl.h"

#include <GLFW/glfw3.h>
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

unsigned int VBO_, VAO_;

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

void init_opengl(void) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

char* read_file(const char* path, size_t* size) {
  FILE* file = fopen(path, "rb");
  if (!file) return NULL;

  fseek(file, 0, SEEK_END);
  const size_t length = (size_t) ftell(file);
  rewind(file);

  char* buffer = malloc(length + 1);
  if (!buffer) {
    fclose(file);
    return NULL;
  }

  const size_t read = fread(buffer, 1, length, file);
  fclose(file);

  if (read != length) {
    free(buffer);
    return NULL;
  }

  if (size) *size = length;

  return buffer;
}

unsigned int load_shader(const char* path, const GLenum type) {
  const unsigned int shader = glCreateShader(type);

  size_t source_size;
  char* source = read_file(path, &source_size);
  if (!source) {
    free(source);
    glDeleteShader(shader);
    return 0;
  }

  const char* src = source;
  glShaderSource(shader, 1, &src, (GLint*) &source_size);
  free(source);

  glCompileShader(shader);

  int success;
  char log[512];

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, log);
    printf("Shader compile error\nPath: %s\nError:\n %s", path, log);
  }

  return shader;
}

void set_shaders(
  const char* vertex_shader_path, const char* fragment_shader_path
) {
  const unsigned int vertex_shader =
    load_shader(vertex_shader_path, GL_VERTEX_SHADER);
  const unsigned int fragment_shader =
    load_shader(fragment_shader_path, GL_FRAGMENT_SHADER);

  shader_program_ = glCreateProgram();
  glAttachShader(shader_program_, vertex_shader);
  glAttachShader(shader_program_, fragment_shader);
  glLinkProgram(shader_program_);

  int success;
  char log[512];
  glGetProgramiv(shader_program_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program_, 512, NULL, log);
    printf("Shader link error\nError:\n %s", log);
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

void load_geometry() {
  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);

  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void render_loop(void) {
  while (!glfwWindowShouldClose(window_)) {
    process_input(window_);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program_);
    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
}

void destroy_geometry() {
  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &VBO_);
}

void destroy_shaders() {
  glDeleteProgram(shader_program_);
}

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
  set_shaders("../src/shaders/shader.vert", "../src/shaders/shader.frag");
  load_geometry();

  render_loop();

  destroy_geometry();
  destroy_shaders();
  destroy_glfw();
  return 0;
}
