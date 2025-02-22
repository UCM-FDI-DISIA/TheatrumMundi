#include "PhysicsBodyComponent.h"
#include "../src/Components/Area2D.h"
#include "../src/Components/CircleArea2D.h"
#include "../src/Components/RectArea2D.h"

using namespace std;

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
	colisionableObjects.clear(); //idk if this is necessary
}

/// <summary>
/// Adds new RectArea to the list
/// </summary>
/// <param name="newRectArea"></param>
void PhysicsBodyComponent::AddRectObjectToList(RectArea2D* newRectArea)
{
	colisionableObjects.push_back(newRectArea);
}

/// <summary>
/// Adds new CircleArea to the list
/// </summary>
/// <param name="newCircleArea"></param>
void PhysicsBodyComponent::AddCircleObjectToList(CircleArea2D* newCircleArea)
{
	colisionableObjects.push_back(newCircleArea);
}

/// <summary>
/// Compares if the RectArea of the entity collides with some objet of the list
/// </summary>
/// <param name="entityArea"></param>
/// <returns></returns>
bool PhysicsBodyComponent::CheckAreaColision(RectArea2D* entityArea)
{
	for (auto& elem : colisionableObjects) {
		if (elem->overlapsWith(entityArea)) return false;
	}
	return true;
}

/// <summary>
/// Compares if the CircleArea of the entity collides with some objet of the list
/// </summary>
/// <param name="entityArea"></param>
/// <returns></returns>
bool PhysicsBodyComponent::CheckAreaColision(CircleArea2D* entityArea)
{
	for (auto& elem : colisionableObjects) {
		if (elem->overlapsWith(entityArea)) return false;
	}
	return true;
}


