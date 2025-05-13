#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture::Texture(char const *filename) {
    data = stbi_load(filename, &width, &height, &channels, 0);
        
    if (data == nullptr) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        exit(1);
    }

    std::cout << "Loaded texture: " << filename << std::endl;
    std::cout << "Texture dimensions: " << width << "x" << height << std::endl;
    std::cout << "Channels: " << channels << std::endl;
}

glm::vec3 Texture::getColor(double u, double v) {
    // std::cout<<"u:"<<std::to_string(u)<<","<<std::to_string(v)<<std::endl;
    double di = (width - 1) * u;
    double dj = (height - 1) * v;
    
    int i = static_cast<int>(di);
    int j = static_cast<int>(dj);
    // prevent exceeding bound of image
    i = std::min(i, width - 2);  
    j = std::min(j, height - 2);

    float up = di - i;
    float vp = dj - j;
    glm::vec3 c00 = getTextureMapPixel(i,j);
    glm::vec3 c01 = getTextureMapPixel(i+1,j);
    glm::vec3 c10 = getTextureMapPixel(i,j+1);
    glm::vec3 c11 = getTextureMapPixel(i+1,j+1);
    // bilinear interpolation
    glm::vec3 c = c00 * (1-up) * (1-vp) + c01 * (1-up) * (vp) +
                  c10 * (up) * (1-vp) + c11 * (up) * (vp);
    // std::cout<<"final colour"<<c.x<<","<<c.y<<","<<c.z<<std::endl;
    c = c00;
    return c;
}

glm::vec3 Texture::getTextureMapPixel(int i, int j) {
    // i = x coord, j = y coord
    int idx = (j * width + i) * channels;
    float r = data[idx];
    float g = data[idx+1];
    float b = data[idx+2];
    // std::cout<<i<<","<<j<<std::endl;
    glm::vec3 c = glm::vec3(r,g,b)/ 255.0f;  // normalize
    // std::cout<<c.x<<","<<c.y<<","<<c.z<<std::endl;
    return c;
}

Texture::~Texture() {
    std::cout<<"free texture"<<std::endl;
    if (data) {
        stbi_image_free(data);
    }
}