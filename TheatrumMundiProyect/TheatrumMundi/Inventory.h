#pragma once

#include <string>
#include <vector>

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
	Inventory();
	virtual ~Inventory();
	void addItem(Hint* item);
	bool hasItem(const std::string& _id) const; //Check if the hint is in the inventory
	void render(); //Render the the inventory

	void setActive(bool _active) { active = _active; }
	bool getActive() const { return active; }
	std::vector<Hint*>& getItems() { return items; };

private:
	bool active;
	
};