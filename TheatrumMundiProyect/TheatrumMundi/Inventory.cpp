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
	std::cout << "Rendering inventory" << std::endl;
	int posX = 50;  // Posición inicial X
	int posY = 100; // Posición inicial Y
	int offset = 50; // Espaciado entre los objetos del inventario

	for (size_t i = 0; i < items.size(); ++i) {
		if (!items[i]->getActive()) {
			items[i]->render(posX, posY);  // Renderiza en la posición (posX, posY)
			std::cout << "Rendering item at (" << posX << ", " << posY << ")" << std::endl;
			posY += offset; // Aumenta Y para el siguiente objeto
		}
	}

	
	/*for (auto& item : items)
	{
		//std::cout << "Rendering item " << item->getDescription() << std::endl;
		if (item->getActive())
		{
			item->render();
			std::cout << "Rendering item " << item->getDescription() << std::endl;
			
		}
	}*/
	
}


void Inventory::addItem(Hint* item)
{
	items.push_back(item);
	std::cout << "Added to inventory " << item->getDescription() << std::endl;
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
