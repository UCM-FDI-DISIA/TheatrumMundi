#include "Inventory.h"
#include "Hint.h"
#include "iostream"

Inventory::Inventory()
{

}

Inventory::~Inventory()
{
	for (auto& item : items)
	{
		delete item;
	}
}

void Inventory::addItem(Hint* item)
{
	items.push_back(item);
	//std::cout << "Added to inventory " << item->getDescription() << std::endl;
}

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
