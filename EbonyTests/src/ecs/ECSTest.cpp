#include "ecs/ECSTest.h"

using namespace std;

namespace ebony { namespace tests {

using namespace ecs;

ECSEntityTest::ECSEntityTest()
{
	manager1 = EntityManager::makeInstance();
	manager2 = EntityManager::makeInstance();
}

TEST_F(ECSEntityTest, Equality)
{
	ecs::Entity e1 = manager1->create();

	ASSERT_EQ(e1, e1);
}

TEST_F(ECSEntityTest, Copy)
{
	ecs::Entity e1 = manager1->create();
	ecs::Entity e2 = e1;

	ASSERT_EQ(e1, e2);
}

TEST_F(ECSEntityTest, InequalitySameManager)
{
	ecs::Entity e1 = manager1->create();
	ecs::Entity e2 = manager1->create();

	ASSERT_NE(e1, e2);
}

TEST_F(ECSEntityTest, InequalityDifferentManager)
{
	ecs::Entity e1 = manager1->create();
	ecs::Entity e2 = manager2->create();

	ASSERT_NE(e1, e2);
}

TEST_F(ECSEntityTest, Valid)
{
	ecs::Entity e = manager1->create();

	ASSERT_TRUE(e.isValid());
	ASSERT_TRUE(e);
}

TEST_F(ECSEntityTest, InvalidAfterDestroy)
{
	ecs::Entity e = manager1->create();
	ecs::Entity eCopy = e;
	e.destroy();
	ASSERT_FALSE(e.isValid());
	ASSERT_FALSE(e);
	ASSERT_FALSE(eCopy.isValid());
	ASSERT_FALSE(eCopy);
}

TEST_F(ECSEntityTest, InvalidAfterManagerDestruction)
{
	shared_ptr<ecs::EntityManager> manager = ecs::EntityManager::makeInstance();
	ecs::Entity e = manager->create();

	manager.reset();
	ASSERT_FALSE(e.isValid());
}

ECSComponentTest::ECSComponentTest()
{
	manager = EntityManager::makeInstance();
	e1 = manager->create();
	e2 = manager->create();
}

TEST_F(ECSComponentTest, AddValid)
{
	ASSERT_TRUE(e1.addComponent<ComponentA>());
}

TEST_F(ECSComponentTest, Equality)
{
	ecs::Component<ComponentA> c = e1.addComponent<ComponentA>();

	ASSERT_EQ(c, c);
}

TEST_F(ECSComponentTest, Copy)
{
	ecs::Component<ComponentA> c = e1.addComponent<ComponentA>();
	ecs::Component<ComponentA> cCopy = c;

	ASSERT_EQ(c, cCopy);
}

TEST_F(ECSComponentTest, InequalityDifferentType)
{
	ecs::Component<ComponentA> ca = e1.addComponent<ComponentA>();
	ecs::Component<ComponentB> cb = e1.addComponent<ComponentB>();

	ASSERT_NE(ca, cb);
}

TEST_F(ECSComponentTest, InequalityDifferentEntity)
{
	ecs::Component<ComponentA> c1 = e1.addComponent<ComponentA>();
	ecs::Component<ComponentA> c2 = e2.addComponent<ComponentA>();

	ASSERT_NE(c1, c2);
}

TEST_F(ECSComponentTest, AddExistingReturnsCurrent)
{
	ecs::Component<ComponentA> c = e1.addComponent<ComponentA>();

	ASSERT_EQ(e1.addComponent<ComponentA>(), c);
}

TEST_F(ECSComponentTest, GetExisting)
{
	ecs::Component<ComponentA> a = e1.addComponent<ComponentA>();
	ecs::Component<ComponentB> b = e1.addComponent<ComponentB>();

	b->data = 2;

	ASSERT_TRUE(e1.getComponent<ComponentA>());
	ASSERT_EQ(e1.getComponent<ComponentB>()->data, 2);
}

TEST_F(ECSComponentTest, GetNonExisting)
{
	e1.addComponent<ComponentB>();

	ASSERT_FALSE(e1.getComponent<ComponentA>());
}

TEST_F(ECSComponentTest, HasExisting)
{
	e1.addComponent<ComponentB>();

	ASSERT_TRUE(e1.hasComponent<ComponentB>());
}

TEST_F(ECSComponentTest, HasNotNonExisting)
{
	e1.addComponent<ComponentB>();

	ASSERT_FALSE(e1.hasComponent<ComponentA>());
}

TEST_F(ECSComponentTest, HasMultiple)
{
	e1.addComponent<ComponentA>();
	e1.addComponent<ComponentB>();

	ASSERT_TRUE((e1.hasComponents<ComponentB, ComponentA>()));
	ASSERT_TRUE((e1.hasComponents<ComponentA, ComponentB>()));
}

TEST_F(ECSComponentTest, HasNotMultiple)
{
	ecs::Component<ComponentB> b = e1.addComponent<ComponentB>();

	ASSERT_FALSE((e1.hasComponents<ComponentB, ComponentA>()));
	ASSERT_FALSE((e1.hasComponents<ComponentA, ComponentB>()));
}

TEST_F(ECSComponentTest, Removal)
{
	ecs::Component<ComponentA> a = e1.addComponent<ComponentA>();
	e1.removeComponent<ComponentA>();

	ASSERT_FALSE(e1.hasComponent<ComponentA>());
}

TEST_F(ECSComponentTest, InvalidAfterRemove)
{
	ecs::Component<ComponentA> a = e1.addComponent<ComponentA>();
	e1.removeComponent<ComponentA>();

	ASSERT_FALSE(a);
}

TEST_F(ECSComponentTest, InvalidAfterEntityRemove)
{
	ecs::Component<ComponentA> a = e1.addComponent<ComponentA>();
	e1.destroy();

	ASSERT_FALSE(a);
}

TEST_F(ECSComponentTest, CallDefaultConstructorOnAddComponent)
{
	ecs::Component<DestructableComponent> a = e1.addComponent<DestructableComponent>();

	ASSERT_FALSE(a->destroyed);
	ASSERT_TRUE(a->constructed);
}

TEST_F(ECSComponentTest, CallDestructorOnRemoveComponent)
{
	ecs::Component<DestructableComponent> a = e1.addComponent<DestructableComponent>();

	e1.removeComponent<DestructableComponent>();

	ASSERT_TRUE(a->destroyed);
	ASSERT_TRUE(a->constructed);
}

TEST_F(ECSComponentTest, CallDestructorOnDestroyEntity)
{
	ecs::Component<DestructableComponent> a = e1.addComponent<DestructableComponent>();

	e1.destroy();

	ASSERT_TRUE(a->destroyed);
	ASSERT_TRUE(a->constructed);
}

TEST_F(ECSComponentTest, RemoveAllComponents)
{
	e1.addComponent<ComponentA>();
	e1.addComponent<ComponentB>();

	e1.removeAllComponents();

	ASSERT_FALSE(e1.hasComponent<ComponentA>());
	ASSERT_FALSE(e1.hasComponent<ComponentB>());
}

TEST_F(ECSComponentTest, RemoveAllComponentsCallsDestructor)
{
	ecs::Component<DestructableComponent> a = e1.addComponent<DestructableComponent>();

	e1.removeAllComponents();

	ASSERT_TRUE(a->destroyed);
}

ECSRequestTest::ECSRequestTest()
{
	manager = ecs::EntityManager::makeInstance();

	e0 = manager->create();

	eab = manager->create();
	eab.addComponent<ComponentA>();
	eab.addComponent<ComponentB>();

	ea = manager->create();
	ea.addComponent<ComponentA>();

	eb = manager->create();
	eb.addComponent<ComponentB>();
}

TEST_F(ECSRequestTest, OneComponent)
{
	bool hasB = false, hasAB = false, hasOther = false;
	ecs::EntityView view = manager->getEntitiesWith<ComponentB>();

	for (ecs::Entity e : view) {
		if (e == eb) {
			hasB = true;
		} else if (e == eab) {
			hasAB = true;
		} else {
			hasOther = true;
		}
	}

	ASSERT_TRUE(hasB);
	ASSERT_TRUE(hasAB);
	ASSERT_FALSE(hasOther);
}

TEST_F(ECSRequestTest, MultipleComponent)
{
	bool hasAB = false, hasOther = false;
	ecs::EntityView view = manager->getEntitiesWith<ComponentB, ComponentA>();

	for (ecs::Entity e : view) {
		if (e == eab) {
			hasAB = true;
		} else {
			hasOther = true;
		}
	}

	ASSERT_TRUE(hasAB);
	ASSERT_FALSE(hasOther);
}

TEST_F(ECSRequestTest, NoResult)
{
	bool hasFound = false;
	ecs::EntityView view = manager->getEntitiesWith<ComponentC>();

	for (ecs::Entity e : view) {
		hasFound = true;
	}

	ASSERT_FALSE(hasFound);
}

}}
