#ifndef EBONY_UTILS_PERLIN_H_
#define EBONY_UTILS_PERLIN_H_

namespace ebony {

void generatePerlinNoise(float *data, int width, int xOffset, int yOffset, int firstOctave, int lastOctave, int size, int seed, float height);

}

#endif
