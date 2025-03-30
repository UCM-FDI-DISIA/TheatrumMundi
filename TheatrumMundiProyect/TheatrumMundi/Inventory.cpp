#include "Inventory.h"
#include "Hint.h"
#include "iostream"
#include "Transform.h"
#include "Manager.h"
#include "DragComponent.h"


/// <summary>
/// Create the inventory, setting the array of positions, the fitst item, the active to false and the originalPos to 0,0
/// </summary>
Inventory::Inventory()
	: active(false), firstItem(0)
{
	for (int i = 0; i < TOTALITEMSTOSHOW; ++i) {
		positions.push_back(Vector2D(1135, 175 + i * 150));
	}
	originalPos = { 0,0 };

	_textDescription = new DescriptionInfo{" ",0};
}
/// <summary>
/// Destroy the reference to the hints
/// </summary>
Inventory::~Inventory()
{
	for (auto& item : items)
	{
		delete item;
	}
}
/// <summary>
/// Sets the first item to show in the inventory
/// </summary>
/// <returns></returns> --> returns the index of the first item to shw
int Inventory::getItemNumber()
{
	//when we click on the up or down arrow in the inventory, we have to change firstItem

	if (firstItem < 0)
	{
		firstItem = 0; //firstItem cannot be a invalid position
	}
	if (firstItem >= items.size())
	{
		return 0; //If there are no items, return 0
	}
	return std::min(3, static_cast<int>(items.size()) - firstItem); //Return the number of items to be rendered

}
/// <summary>
/// Add a new Hint to the inventory
/// </summary>
/// <param name="item"></param> --> New item to add
void Inventory::addItem(Hint* item)
{
	items.push_back(item);
}
/// <summary>
/// Remove the id item from the inventory
/// </summary>
/// <param name="idToRemove"></param> -->Id of the item to is going to been removed
void Inventory::removeItem(const std::string& idToRemove, std::vector<Entity*>& invEntityList)
{
	//The name don't have to be removed, if we have 2 entities of the same (imagine a BUG) then the inventory don't create the both of them
	auto hintIt = hints.begin();
	auto entityIt = invEntityList.begin();
	for (auto it = items.begin(); it != items.end(); ++it) {
		if (it[0]->getID() == idToRemove) {
			items.erase(it);
			hintIt[0]->getMngr()->setActive(hintIt[0], false);
			hints.erase(hintIt);
			entityIt[0]->getMngr()->setActive(entityIt[0], false);
			invEntityList.erase(entityIt);
			return;
		}
		++hintIt;
		++entityIt;
	}
}

/// <summary>
/// Checks if the id is define in the inventory or not
/// </summary>
/// <param name="_id"></param> --> id of the object to find
/// <returns></returns> --> true in case an item has id and false in the other case
bool Inventory::hasItem(const std::string& _id) const
{
	for (auto item : items)
	{
		if (item->getID() == _id)
		{
			return true;
		}
	}

	return false;
}

