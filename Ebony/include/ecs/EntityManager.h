#ifndef EBONY_ECS_ENTITY_MANAGER_H_
#define EBONY_ECS_ENTITY_MANAGER_H_

#include <vector>
#include <memory>

#include "constants.h"
#include "Entity.h"
#include "utils/Pool.h"

namespace ebony { namespace ecs {

	class EntityManager : public std::enable_shared_from_this<EntityManager> {
	public:
		EntityManager(const EntityManager &manager) = delete;
		EntityManager &operator=(const EntityManager &manager) = delete;

		static std::shared_ptr<EntityManager> makeInstance();

		Entity create();
		bool isEntityValid(const Entity &entity) const;

	private:
		friend Entity;

		EntityManager() = default;

		void destroy(Entity &entity);
		void growCapacity();

		EntityId _nextEntity = 0;
		std::vector<EntityId> _freeList;
		std::vector<EntityVersion> _entityVersion;
		std::vector<ComponentMask> _componentMasks;
		std::vector<IPool *> _componentPools;
	};

}
}

#endif
