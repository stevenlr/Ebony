#include "utils/perlin.h"

#include <cstdint>
#include <algorithm>

using namespace std;

namespace {

	float noise(int x, int y, int o, int seed)
	{
		uint32_t hash = 0;
		uint32_t key[4] = {
			static_cast<uint32_t>(seed),
			static_cast<uint32_t>(x),
			static_cast<uint32_t>(y),
			static_cast<uint32_t>(o)
		};
		int i = 0;

		for (i = 0; i < 4; i++) {
			hash += key[i];
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return static_cast<float>(hash) / UINT32_MAX - 0.5f;
	}

}

namespace ebony {

	void generatePerlinNoise(float *data, int width, int xOffset, int yOffset, int firstOctave, int lastOctave, int size, int seed, float height)
	{
		float mult = height;

		size >>= firstOctave;
		fill_n(data, width * width, 0.0f);

		for (int o = firstOctave; o <= lastOctave && size >= 1; ++o) {
			for (int yr = 0; yr < width; yr += size) {
				for (int xr = 0; xr < width; xr += size) {
					float h00 = 0, h10 = 0, h01 = 0, h11 = 0;

					h00 = noise(xr + xOffset, yr + yOffset, o, seed);
					h10 = noise(xr + xOffset + size, yr + yOffset, o, seed);
					h01 = noise(xr + xOffset, yr + yOffset + size, o, seed);
					h11 = noise(xr + xOffset + size, yr + yOffset + size, o, seed);

					float h0 = h00;
					float h1 = h10;
					float dh0 = (h01 - h00) / size;
					float dh1 = (h11 - h10) / size;

					for (int yl = 0; yl < size && yr + yl < width ; ++yl) {
						float h = h0;
						float dh = (h1 - h0) / size;

						for (int xl = 0; xl < size && xr + xl < width; ++xl) {
							data[(yr + yl) * width + xr + xl] += h * mult;
							h += dh;
						}

						h0 += dh0;
						h1 += dh1;
					}
				}
			}

			size >>= 1;
			mult *= 0.5;
		}
	}

}
