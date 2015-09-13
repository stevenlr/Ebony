#ifndef EBONY_ECS_ENTITY_H_
#define EBONY_ECS_ENTITY_H_

#include "constants.h"

namespace ebony { namespace ecs {

	class EntityManager;

	class Entity {
	public:
		Entity();
		Entity(EntityManager *manager, EntityId id, EntityVersion version);
		Entity(const Entity &entity);

		Entity &operator=(const Entity &entity);
		bool operator==(const Entity &entity) const;
		bool operator!=(const Entity &entity) const;
		operator bool() const;

		bool isValid() const;
		void destroy();

	private:
		friend EntityManager;

		EntityManager *_manager;
		EntityId _id;
		EntityVersion _version;
	};

}
}

#endif
