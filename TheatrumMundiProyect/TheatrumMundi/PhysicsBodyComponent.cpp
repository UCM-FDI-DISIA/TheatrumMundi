#include "PhysicsBodyComponent.h"
#include "../src/Components/Area2D.h"
#include "../src/Components/CircleArea2D.h"
#include "../src/Components/RectArea2D.h"

using namespace std;

PhysicsBodyComponent::PhysicsBodyComponent()
{

}

/// <summary>
/// Assigns the list of objects
/// </summary>
/// <param name="_colisionableObjects"></param>
PhysicsBodyComponent::PhysicsBodyComponent(const list<Area2D*>& _colisionableObjects)
{
	colisionableObjects = _colisionableObjects;
}

/// <summary>
/// Clears the memory from the list
/// </summary>
PhysicsBodyComponent::~PhysicsBodyComponent()
{
	for (auto& elem : colisionableObjects) delete elem;
}

/// <summary>
/// Add RectArea to list
/// </summary>
/// <param name="newArea"></param>
void PhysicsBodyComponent::AddObjectToList(RectArea2D* newArea)
{
	colisionableObjects.push_back(newArea);
}

/// <summary>
/// Add CircleArea to list
/// </summary>
/// <param name="newArea"></param>
void PhysicsBodyComponent::AddObjectToList(CircleArea2D* newArea)
{
	colisionableObjects.push_back(newArea);
}

/// <summary>
/// Compares if the CircleArea of the entity collides with some objet of the list
/// </summary>
/// <param name="entityArea"></param>
/// <returns></returns>
bool PhysicsBodyComponent::CheckAreaColision(Area2D* entityArea)
{
	for (auto& elem : colisionableObjects) {
		if (elem->overlapsWithArea(entityArea)) return false;
	}
	return true;
}


