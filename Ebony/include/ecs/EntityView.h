#ifndef EBONY_ECS_ENTITY_VIEW_H_
#define EBONY_ECS_ENTITY_VIEW_H_

#include <memory>

#include "constants.h"

namespace ebony { namespace ecs {
	
class EntityView;
class Entity;
class EntityManager;
	
class EntityView_Iterator {
public:
	EntityView_Iterator(const EntityView_Iterator &it);

	EntityView_Iterator &operator=(const EntityView_Iterator &it);

	bool operator==(const EntityView_Iterator &it);
	bool operator!=(const EntityView_Iterator &it);

	Entity operator*() const;
	EntityView_Iterator &operator++();

private:
	friend class EntityView;

	EntityView_Iterator() = default;
	EntityView_Iterator(std::weak_ptr<EntityManager> manager,
						const ComponentMask &mask,
						EntityId current = 0);

	void goToFirstValid();

	EntityId						_current;
	ComponentMask					_mask;
	std::weak_ptr<EntityManager>	_manager;
};

class EntityView {
public:
	typedef EntityView_Iterator Iterator;
		
	EntityView(const EntityView &view) = default;

	EntityView &operator=(const EntityView &view) = default;

	Iterator begin();
	Iterator end();

	const Iterator begin() const;
	const Iterator end() const;

private:
	friend class EntityManager;
	friend class EntityView_Iterator;

	EntityView() = default;
	EntityView(std::shared_ptr<EntityManager> manager, const ComponentMask &mask);

	ComponentMask					_mask;
	std::weak_ptr<EntityManager>	_manager;
};

}}

#endif
