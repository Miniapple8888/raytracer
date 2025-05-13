#pragma once
#include <iostream>
#include <vector>
#include "glm/glm.hpp"


class Texture {
public:
    int width;
    int height;
    int channels;
    unsigned char* data;
    std::vector<std::vector<glm::vec3>> text_map;

    Texture(char const *filename);

    // map uv coord to texture coord
    glm::vec3 getColor(double u, double v);

    glm::vec3 getTextureMapPixel(int i, int j);

    ~Texture();
};