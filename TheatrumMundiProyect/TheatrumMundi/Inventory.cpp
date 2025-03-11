#include "Inventory.h"
#include "Hint.h"
#include "iostream"
#include "Transform.h"
#include "Manager.h"
#include "DragComponent.h"


Inventory::Inventory()
	: active(false), firstItem(0)
{
	for (int i = 0; i < TOTALITEMSTOSHOW; ++i) {
		positions.push_back(Vector2D(120,150 + i * 150));
	}
}

Inventory::~Inventory()
{
	for (auto& item : items)
	{
		delete item;
	}
}

int Inventory::getItemNumber()
{
	//when we click on the up or down arrow in the inventory, we have to change firstItem

	if (firstItem < 0)
	{
		firstItem = 0; // firstItem cannot be a invalid position
	}
	if (firstItem >= items.size())
	{
		return 0; // If there are no items, return 0
	}
	return std::min(3, static_cast<int>(items.size()) - firstItem); // Return the number of items to be rendered

}

/// <summary>
/// When the puzzle scene is called this method change the NODRAG component of the Inventory Entities to true. If the puzzle scene is closed, it makes the opposite
/// </summary>
/// <param name="dragger"></param> --> If true Set the elems to Dragger
void Inventory::setDragger(bool dragger)
{
	if (dragger) {
		for (auto a : hints) {
			a->getMngr()->addComponent<DragComponent>(a);			
		}
	}
	else {
		for (auto a : hints) {
			//Caution, the last item added to the inventory may not have DragComponent
			if (a->getMngr()->hasComponent<DragComponent>(a)) {
				a->getMngr()->removeComponent<DragComponent>(a);
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

