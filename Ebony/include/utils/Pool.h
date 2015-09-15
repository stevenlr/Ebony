#ifndef EBONY_UTILS_POOL_H_
#define EBONY_UTILS_POOL_H_

#include <vector>

#include <iostream>
using namespace std;

namespace ebony {

	template<typename T>
	class Pool {
	public:
		Pool(size_t incrementSize = 256) :
			_incrementSize(incrementSize)
		{}

		~Pool()
		{
			for (T *block : _blocks) {
				delete[] block;
			}
		}

		T *allocate()
		{
			if (_freeList.size() == 0) {
				grow();
			}

			T *ptr = _freeList.back();
			_freeList.pop_back();

			return ptr;
		}

		void free(const T *ptr)
		{
			_freeList.push_back(ptr);
		}

	private:
		void grow()
		{
			T *block = new T[_incrementSize];

			_blocks.push_back(block);

			for (size_t i = 0; i < _incrementSize; ++i) {
				_freeList.push_back(block + i);
			}
		}

		size_t _incrementSize;
		std::vector<T *> _blocks;
		std::vector<T *> _freeList;
	};

}

#endif
