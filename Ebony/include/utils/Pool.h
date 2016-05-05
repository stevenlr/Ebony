#ifndef EBONY_UTILS_POOL_H_
#define EBONY_UTILS_POOL_H_

#include <vector>

#include "assert.h"

namespace ebony {

class IPool {
public:
	virtual ~IPool()
	{}

	virtual void *allocate() = 0;
	virtual void free(void *ptr) = 0;
};

template<typename T>
class Pool : public IPool {
public:
	Pool(size_t incrementSize = 256) :
		_incrementSize(incrementSize)
	{}

	virtual ~Pool()
	{
		for (void *block : _blocks) {
			::operator delete(block);
		}
	}

	inline void *allocate() override
	{
		if (_freeList.size() == 0) {
			grow();
		}

		T *ptr = _freeList.back();
		_freeList.pop_back();

		return ptr;
	}

	inline void free(void *ptr) override
	{
		T *ptrT = reinterpret_cast<T *>(ptr);

#ifndef NDEBUG
		bool found = false;
		uintptr_t target = reinterpret_cast<uintptr_t>(ptr);

		for (const void *block: _blocks) {
			uintptr_t start	= reinterpret_cast<uintptr_t>(block);
			uintptr_t end	= start + sizeof(T) * _incrementSize;

			if (target >= start && target <= end - sizeof(T)) {
				found = true;
				break;
			}
		}

		DEBUG_ASSERT(found, "Freeing object using wrong pool");
#endif

		ptrT->~T();
		_freeList.push_back(ptrT);
	}

private:
	void grow()
	{
		T *block = reinterpret_cast<T *>(::operator new(sizeof(T) * _incrementSize));

		_blocks.push_back(block);

		for (size_t i = 0; i < _incrementSize; ++i) {
			_freeList.push_back(block++);
		}
	}

	size_t				_incrementSize;
	std::vector<void *>	_blocks;
	std::vector<T *>	_freeList;
};

}

#endif
