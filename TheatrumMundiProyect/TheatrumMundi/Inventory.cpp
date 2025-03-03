#include "Inventory.h"
#include "Hint.h"
#include "iostream"
#include "Transform.h"


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

void Inventory::render()
{
	/*std::cout << "Rendering inventory" << std::endl;
	int posX = 50;  // Pos X
	int posY = 100; // Pos Y
	int offset = 50; // separation between items

	for (int i = 0; i < items.size(); ++i) {
		if (items[i]->getActive()) {
			items[i]->render(posX, posY);
			std::cout << "Rendering item at (" << posX << ", " << posY << ")" << std::endl;
			posY += offset; // increment the Y position
		}
	}*/

	
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

std::vector<Hint*>& Inventory::getItems(int firstItem)
{
	if (firstItem < 0) {
		std::vector<Hint*> renderItems;
		if (firstItem + 3 >= items.size()) {
			for (int i = 0; i < items.size();++i) {
				renderItems.push_back(items[i]);
			}
		}
		else {
			for (int i = 0; i < 3;++i) {
				renderItems.push_back(items[i]);
			}
		}
		return renderItems;
	}

	else if(firstItem + 2 >= items.size())
	{
		std::vector<Hint*> renderItems;
		for (int i = 0; i < items.size();++i) {
			renderItems.push_back(items[firstItem]);
			++firstItem;
		}
		return renderItems;
	}
	else
	{
		std::vector<Hint*> renderItems;
		if (firstItem + 2 >= items.size()) {
			for (int i = firstItem; i < items.size();++i) {
				renderItems.push_back(items[i]);
			}
		}
		else {
			for (int i = 0; i < 3;++i) {
				renderItems.push_back(items[i]);
			}
		}
	}

}

/*void Inventory::setItem(ecs::Entity* _ent, const std::string& _id, SceneTemplate* _myScene)
{
	if (hasItem(_id))
	{
		_ent = _myScene->entityFactory->CreateInteractableEntity(_ent, _id, _myScene->entityFactory->RECTAREA, Vector2D(0, 0), Vector2D(0, 0), 100, 100, 0, _myScene->areaLayerManager, _myScene->entityFactory->DRAG);
	}


}*/
		
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
