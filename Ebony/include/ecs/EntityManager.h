#ifndef EBONY_ECS_ENTITY_MANAGER_H_
#define EBONY_ECS_ENTITY_MANAGER_H_

#include <bitset>
#include <vector>

#include "constants.h"
#include "Entity.h"

namespace ebony { namespace ecs {

	class EntityManager {
	public:
		typedef std::bitset<MAX_COMPONENTS> ComponentMask;

		EntityManager(const EntityManager &manager) = delete;
		EntityManager &operator=(const EntityManager &manager) = delete;

		Entity create();
		void destroy(Entity &entity);
		bool isEntityValid(const Entity &entity) const;

	private:
		void growCapacity();

		EntityId _nextEntity = 0;
		std::vector<EntityId> _freeList;
		std::vector<EntityVersion> _entityVersion;
	};

}
}

#endif
