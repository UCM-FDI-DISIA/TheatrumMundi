#pragma once
#include "../src/ecs/Component.h"
#include <list>
class Area2D;
class CircleArea2D;
class RectArea2D;
class PhysicsBodyComponent : public ecs::Component
{
private:

	std::list<Area2D*> colisionableObjects; //List with the objets that the object must collide

public:
	
	__CMPID_DECL__(ecs::cmp::PHYSICSBODYCOMPONENT);

	PhysicsBodyComponent();

	PhysicsBodyComponent(const std::list<Area2D*>& _colisionableObjects); //Assigns the list of objects

	~PhysicsBodyComponent();

	void AddObjectToList(RectArea2D* newArea); //Add a Rect Colision to the list

	void AddObjectToList(CircleArea2D* newArea); //Add a Circle Colision to the list

	bool CheckAreaColision(RectArea2D* entityArea); //Checks if there is a colision with one of the objects

	bool CheckAreaColision(CircleArea2D* entityArea); //Checks if there is a colision with one of the objects 
};

