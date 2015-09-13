#ifndef EBONY_ECS_ENTITY_H_
#define EBONY_ECS_ENTITY_H_

#include <memory>

#include "constants.h"

namespace ebony { namespace ecs {

	class EntityManager;

	class Entity {
	public:
		Entity() = default;
		Entity(std::shared_ptr<EntityManager> manager, EntityId id, EntityVersion version);
		Entity(const Entity &entity) = default;

		Entity &operator=(const Entity &entity) = default;
		bool operator==(const Entity &entity) const;
		bool operator!=(const Entity &entity) const;
		operator bool() const;

		bool isValid() const;
		void destroy();

	private:
		friend EntityManager;

		std::weak_ptr<EntityManager> _manager;
		EntityId _id;
		EntityVersion _version;
	};

}
}

#endif
