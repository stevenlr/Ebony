#include "stdafx.h"
#include "utils/io.h"

#include "utils/assert.h"

using namespace std;

namespace ebony {

bool readFile(const string &filename, string *output)
{
	ifstream stream;

	ASSERT(output, "");
	stream.open(filename, ios_base::binary | ios_base::in);

	if (!stream.good()) {
		stream.close();
		return false;
	}

	size_t size;

	stream.seekg(0, ios_base::end);
	size = static_cast<size_t>(stream.tellg());
	stream.seekg(0, ios_base::beg);
	output->resize(size, ' ');
	stream.read(&*output->begin(), size);

	stream.close();

	return true;
}

}
