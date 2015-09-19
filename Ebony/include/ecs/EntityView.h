#ifndef EBONY_ECS_ENTITY_VIEW_H_
#define EBONY_ECS_ENTITY_VIEW_H_

#include <memory>

#include "constants.h"
#include "Entity.h"

class EntityManager;

namespace ebony { namespace ecs {

	class EntityView {
	public:
		EntityView(const EntityView &view) = default;

		EntityView &operator=(const EntityView &view) = default;

		class Iterator {
		public:
			Iterator(const Iterator &it);

			Iterator &operator=(const Iterator &it);

			bool operator==(const Iterator &it);
			bool operator!=(const Iterator &it);

			Entity operator*() const;
			Iterator &operator++();

			EntityId _current;

		private:
			friend EntityView;

			Iterator() = default;
			Iterator(std::weak_ptr<EntityManager> manager,
						const ComponentMask &mask,
						EntityId current = 0);

			void goToFirstValid();

			ComponentMask _mask;
			std::weak_ptr<EntityManager> _manager;

		};

		Iterator begin();
		Iterator end();

		const Iterator begin() const;
		const Iterator end() const;

	private:
		friend EntityManager;

		EntityView() = default;
		EntityView(std::shared_ptr<EntityManager> manager, const ComponentMask &mask);

		ComponentMask _mask;
		std::weak_ptr<EntityManager> _manager;
	};

}
}

#endif
