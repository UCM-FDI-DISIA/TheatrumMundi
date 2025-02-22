#pragma once
#include "../src/ecs/Component.h"
#include <list>
class Area2D;
class CircleArea2D;
class RectArea2D;
class PhysicsBodyComponent : ecs::Component
{
private:

	std::list<Area2D*> colisionableObjects; //List with the objets that the object must collide

public:
	__CMPID_DECL__(ecs::cmp::PhysicsBodyComponent);

	PhysicsBodyComponent(const std::list<Area2D*>& _colisionableObjects); //Assigns the list of objects

	~PhysicsBodyComponent();

	void AddRectObjectToList(RectArea2D* newRectArea); //Add a Rect Colision to the list

	void AddCircleObjectToList(CircleArea2D* newCircleArea); //Add a Circle Colision to the list

	bool CheckAreaColision(RectArea2D* entityArea); //Checks if there is a colision with one of the objects

	bool CheckAreaColision(CircleArea2D* entityArea); //Checks if there is a colision with one of the objects 
};

