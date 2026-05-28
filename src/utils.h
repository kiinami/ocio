//
// Created by kinami on 5/28/26.
//

#ifndef OCIO_OPENGL_UTILS_H
#define OCIO_OPENGL_UTILS_H

#include <stdio.h>
#include <stdlib.h>

// Declarations ///////////////////////////////////////////////////////////////

char* read_file(const char* path, size_t* size);

// Implementations ////////////////////////////////////////////////////////////

inline char* read_file(const char* path, size_t* size) {
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

#endif //OCIO_OPENGL_UTILS_H
