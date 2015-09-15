#ifndef EBONY_UTILS_POOL_H_
#define EBONY_UTILS_POOL_H_

#include <vector>

#include <iostream>
using namespace std;

namespace ebony {

	class IPool {
	public:
		virtual ~IPool()
		{}

		virtual void *allocate() = 0;
		virtual void free(void *ptr) = 0;
	};

	template<typename T>
	class Pool : IPool {
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

		void *allocate() override
		{
			if (_freeList.size() == 0) {
				grow();
			}

			T *ptr = _freeList.back();
			_freeList.pop_back();

			return ptr;
		}

		void free(void *ptr) override
		{
			_freeList.push_back(static_cast<T *>(ptr));
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
