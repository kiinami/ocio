//
// Created by kinami on 5/28/26.
//

#ifndef OCIO_OPENGL_SHADER_H
#define OCIO_OPENGL_SHADER_H

#include "glad/gl.h"

#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

// Declarations ///////////////////////////////////////////////////////////////

unsigned int load_shader_from_path(const char* path, GLenum type);
unsigned int load_shaders(const char* vertex_shader_path, const char* fragment_shader_path);

// Implementations ////////////////////////////////////////////////////////////

inline unsigned int load_shader_from_path(const char* path, const GLenum type) {
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

inline unsigned int load_shaders(
  const char* vertex_shader_path, const char* fragment_shader_path
) {
    const unsigned int vertex_shader =
      load_shader_from_path(vertex_shader_path, GL_VERTEX_SHADER);
    const unsigned int fragment_shader =
      load_shader_from_path(fragment_shader_path, GL_FRAGMENT_SHADER);

    const unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    int success;
    char log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, log);
        printf("Shader link error\nError:\n %s", log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

#endif //OCIO_OPENGL_SHADER_H
