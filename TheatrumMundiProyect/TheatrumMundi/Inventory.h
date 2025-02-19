#pragma once

#include "HintTemplate.h"
#include <vector>

using namespace std;

class Inventory
{
	//InventoryScene class
	//This class is used to store the hints that the player has collected
	//It is used to render the hints in the inventory screen
	//It is used to add hints to the inventory
public:
	Inventory();
	~Inventory();
	void addItem(HintTemplate* item);
	void init();
private:

	vector<HintTemplate*> inventory; //Vector of hints
};