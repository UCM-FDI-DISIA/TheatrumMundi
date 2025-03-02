#include "Inventory.h"
#include "Hint.h"
#include "iostream"

Inventory::Inventory()
	: active(false)
{

}

Inventory::~Inventory()
{
	for (auto& item : items)
	{
		delete item;
	}
}

void Inventory::render() const
{
	//std::cout << "Rendering inventory" << std::endl;
	if (active) {
		for (auto& item : items)
		{
			//std::cout << "Rendering item " << item->getDescription() << std::endl;
			if (item->getActive())
			{
				//std::cout << "Rendering item " << item->getDescription() << std::endl;
			
			}
		}
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
