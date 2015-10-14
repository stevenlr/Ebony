#include "graphics/StaticModel.h"

#include <fstream>
#include <iostream>
#include <zlib.h>

#include "utils/io.h"

#define CHUNK 16384

using namespace std;
using namespace ebony;

namespace {
	int inf(ifstream &source, uint8_t **dest)
	{
		int ret;
		unsigned have;
		z_stream strm;
		unsigned char in[CHUNK];
		unsigned char out[CHUNK];
		uint8_t *destptr;
		int nbFaces = -1;

		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		strm.avail_in = 0;
		strm.next_in = Z_NULL;
		ret = inflateInit(&strm);
		*dest = destptr = nullptr;

		if (ret != Z_OK)
			return -1;

		do {
			source.read(reinterpret_cast<char *>(in), CHUNK);
			strm.avail_in = source.gcount();

			if (source.bad()) {
				inflateEnd(&strm);
				delete destptr;
				return -2;
			}

			if (strm.avail_in == 0) {
				break;
			}

			strm.next_in = in;

			do {
				strm.avail_out = CHUNK;
				strm.next_out = out;

				ret = inflate(&strm, Z_NO_FLUSH);
				
				switch (ret) {
				case Z_NEED_DICT:
					ret = Z_DATA_ERROR;
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					inflateEnd(&strm);
					return -3;
				}

				if (!destptr) {
					nbFaces = *reinterpret_cast<int32_t *>(out);
					destptr = new uint8_t[nbFaces * 33 * sizeof(GLfloat) + 4];
					*dest = destptr;
				}

				have = CHUNK - strm.avail_out;

				copy_n(reinterpret_cast<uint8_t *>(out), have, destptr);
				destptr += have;
			} while (strm.avail_out == 0);
		} while (ret != Z_STREAM_END);

		inflateEnd(&strm);

		return nbFaces;
	}
}

StaticModel::StaticModel(const string &filename)
{
	_vao = make_shared<gl::VertexArray>();
	_vbo = make_shared<gl::Buffer>();

	ifstream source;
	uint8_t *data;

	source.open(filename, ios_base::in | ios_base::binary);

	if (!source.is_open()) {
		throw runtime_error("Couldn't open model file");
	}

	_nbFaces = inf(source, &data);

	if (_nbFaces < 0) {
		throw runtime_error("Error when reading model");
	}

	glBindVertexArray(*_vao);
	glBindBuffer(GL_ARRAY_BUFFER, *_vbo);

	glBufferData(GL_ARRAY_BUFFER, _nbFaces * 3 * 11 * sizeof(GLfloat), data + 4, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), reinterpret_cast<void *>(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), reinterpret_cast<void *>(9 * sizeof(GLfloat)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] data;
}

void StaticModel::draw(int n) const
{
	glBindVertexArray(*_vao);
	glDrawArrays(GL_TRIANGLES, 0, _nbFaces * 3);
}
