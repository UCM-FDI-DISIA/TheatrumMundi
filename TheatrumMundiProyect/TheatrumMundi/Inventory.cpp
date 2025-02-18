#include "Inventory.h"
#include "iostream"

Inventory::Inventory()
{

}

void Inventory::addItem(HintTemplate* item)
{
	inventory.push_back(item);
}

void Inventory::init()
{
	for (HintTemplate* item : inventory)
	{
		item->render();
	}
}
