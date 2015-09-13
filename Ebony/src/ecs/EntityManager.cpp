#include "ecs/ecs.h"

#include <limits>

using namespace std;

namespace ebony { namespace ecs {

	Entity EntityManager::create()
	{
		EntityId id;
		EntityVersion version;

		if (_freeList.empty()) {
			if (_nextEntity > numeric_limits<EntityId>::max()) {
				throw runtime_error("Entity id out of bounds");
			}

			growCapacity();
			id = _nextEntity++;
			version = _entityVersion[id] = 1;
		} else {
			id = _freeList.back();
			_freeList.pop_back();
			version = _entityVersion[id];
		}

		return Entity(this, id, version);
	}

	void EntityManager::destroy(Entity &entity)
	{
		if (!entity) {
			return;
		}

		++_entityVersion[entity._id];
		_freeList.push_back(entity._id);
		entity._manager = nullptr;
	}

	bool EntityManager::isEntityValid(const Entity &entity) const
	{
		return entity._id < _nextEntity && entity._version == _entityVersion[entity._id];
	}

	void EntityManager::growCapacity()
	{
		size_t newSize;

		if (_entityVersion.size() > 0) {
			newSize = _entityVersion.size();

			while (newSize <= _nextEntity) {
				newSize *= 2;
			}
		} else {
			newSize = 256;
		}

		_entityVersion.resize(newSize);
	}

}
}
