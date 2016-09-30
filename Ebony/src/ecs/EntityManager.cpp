#include "stdafx.h"
#include "ecs/EntityManager.h"

#include "ecs/Entity.h"
#include "ecs/Component.h"

#include "utils/assert.h"

namespace ebony { namespace ecs {

unsigned int EntityManager::_nbComponentTypes = 0;

std::shared_ptr<EntityManager> EntityManager::makeInstance()
{
	return std::shared_ptr<EntityManager>(new EntityManager());
}

Entity EntityManager::create()
{
	EntityId id;
	EntityVersion version;

	if (_freeList.empty()) {
		ASSERT(_nextEntity <= std::numeric_limits<EntityId>::max(), "Entity id out of bounds");

		growCapacity();
		id = _nextEntity++;
		version = _entityVersion[id] = 1;
	} else {
		id = _freeList.back();
		_freeList.pop_back();
		version = _entityVersion[id];
	}

	return Entity(shared_from_this(), id, version);
}

void EntityManager::destroy(Entity &entity)
{
	if (!isEntityValid(entity)) {
		return;
	}

	removeAllComponents(entity);

	if (++_entityVersion[entity._id] != 0) {
		_freeList.push_back(entity._id);
	}

	entity._manager.reset();
}

void EntityManager::removeAllComponents(const Entity &entity)
{
	if (!isEntityValid(entity)) {
		return;
	}

	ComponentMask mask = _componentMasks[entity._id];

	for (size_t i = 0; i < MAX_COMPONENTS; ++i) {
		if (mask.test(i)) {
			_componentPools[i]->free(_components[i][entity._id]);
			_components[i][entity._id] = nullptr;
		}
	}

	_componentMasks[entity._id].reset();
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

	_entityVersion.resize(newSize, 0);
	_componentMasks.resize(newSize, ComponentMask());

	for (std::vector<void *> &components : _components) {
		components.resize(newSize);
	}
}

void EntityManager::accomodateComponents()
{
	size_t oldSize = _componentPools.size();

	_componentPools.resize(_nbComponentTypes);
	_components.resize(_nbComponentTypes);

	for (unsigned int i = oldSize; i < _nbComponentTypes; ++i) {
		_components[i].resize(_componentMasks.size());
	}
}

}}
