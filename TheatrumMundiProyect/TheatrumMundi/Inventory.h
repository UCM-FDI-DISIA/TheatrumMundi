#pragma once

#include <string>
#include <vector>
#include "Entity.h"
#include "Vector2D.h"
#include "Transform.h"
#include "Manager.h"
class SceneTemplate;
class Hint;
class Inventory
{
	//InventoryScene class
	//This class is used to store the hints that the player has collected
	//It is used to render the hints in the inventory screen
	//It is used to add hints to the inventory
protected:
	std::vector<Hint*> items; //Vector of hints
public:
	std::vector<ecs::Entity*> hints;
	std::vector<Vector2D> positions;
	Inventory();
	virtual ~Inventory();
	void addItem(Hint* item);
	bool hasItem(const std::string& _id) const; //Check if the hint is in the inventory
	//void render(); //Render the the inventory

	inline Vector2D setPosition() { return positions[hints.size()]; }
	void setActive(bool _active) { active = _active; }
	bool getActive() const { return active; }
	int getItemNumber(); //Get the hints that are going to be rendered
	inline std::vector<Hint*> getItems() { return items; } //Get all the hintss
	inline Vector2D GetPosition(int i) { return hints[i]->getMngr()->getComponent<Transform>(hints[i])->getPos(); }
private:
	bool active;
	int firstItem;
	const int TOTALITEMSTOSHOW = 10;
};