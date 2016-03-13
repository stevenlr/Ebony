#include "ecs/ECSTest.h"

namespace ebony { namespace tests {

using namespace ecs;

ECSTest::ECSTest()
{
	manager1 = EntityManager::makeInstance();
	manager2 = EntityManager::makeInstance();
}

TEST_F(ECSTest, EntityEquality)
{
	ecs::Entity e1 = manager1->create();

	ASSERT_EQ(e1, e1);
}

TEST_F(ECSTest, EntityInequalitySameManager)
{
	ecs::Entity e1 = manager1->create();
	ecs::Entity e2 = manager1->create();

	ASSERT_NE(e1, e2);
}

TEST_F(ECSTest, EntityInequalityDifferentManager)
{
	ecs::Entity e1 = manager1->create();
	ecs::Entity e2 = manager2->create();

	ASSERT_NE(e1, e2);
}

}}
