#pragma once

#include <string>
#include <vector>
#include "Entity.h"
#include "Vector2D.h"
#include "Manager.h"
#include "../../src/Components/Transform.h"
#include <string>
#include "../../TheatrumMundi/DescriptionInfo.h"
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
	Vector2D originalPos;
	std::vector<Vector2D> positions; //Array where all the positions are
	DescriptionInfo* _textDescription;
public:

	std::vector<ecs::Entity*> hints; //Array of inventory entities

	Inventory(); // Const
	virtual ~Inventory(); //Dest

	void addItem(Hint* item); //Adds a new object to the inventory
	void removeItem(const std::string& idToRemove, std::vector<Entity*>& invEntityList); //remove the item from the inventory in all his aspects
	bool hasItem(const std::string& _id) const; //Check if the hint is in the inventory

	inline Vector2D setPosition() { return positions[hints.size()]; } //Sets the position of the item to the inventory using the array of Positions
	inline void setActive(bool _active) { active = _active; } //set the item to active or not
	inline bool getActive() const { return active; } //Returns if the item is active or not
	int getItemNumber(); //Get the hints that are going to be rendered

	void setFirstItem(int _firstItem) { firstItem = _firstItem; } //Set the first hint to be rendered
	int getFirstItem() const { return firstItem; } //Get the first hint

	inline std::vector<Hint*> getItems() { return items; } //Get all the hintss
	inline Vector2D GetPosition(int i) { return hints[i]->getMngr()->getComponent<Transform>(hints[i])->getPos(); } //Return the position of the item in the inventory USING IN THE CREATION OF THE INVENTITIES IN THE PUZZLESCENES
	
	void setTextDescription(std::string _id, std::vector<Entity*>& invEntityList, Transform* backgroundTextTransform);
	DescriptionInfo* getTextDescription() { return _textDescription; }

private:
	bool active; //Defines if the object is active or not
	int firstItem; //Index to set the first item of the inventory (if firstItem == 1) shows the 1,2 and 3 objects of the array of hints
	const int TOTALITEMSTOSHOW = 10; //Total of items in the inventory, is used to set the array of positions the limit of positions to get assigned
};