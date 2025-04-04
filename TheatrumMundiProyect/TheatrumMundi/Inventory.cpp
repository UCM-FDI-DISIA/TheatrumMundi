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

inline Vector2D Inventory::GetPosition(int i)
{
	if (i >= 0 && i < positions.size()) {
		return positions[i];
	}
	return Vector2D(0, 0);
}

/// <summary>
/// Changes text description displayed on screen
/// </summary>
/// <param name="_id">Item id</param>
/// <param name="invEntityList">Iventory item entities</param>
/// <param name="backgroundTextTransform">Visual item description background's transform</param>
void Inventory::setTextDescription(std::string _id, std::vector<Entity*>& invEntityList, Transform* backgroundTextTransform)
{
	int index = 0;

	//Searches item with desired id
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		if (it[0]->getID() == _id)
		{
			//set text description
			_textDescription->Description = it[0]->getDescription();

			//set background position
			backgroundTextTransform->setPosY(GetPosition(index).getY() + 40);
			backgroundTextTransform->setPosX(535);
			backgroundTextTransform->setWidth(100);

			//set text description position
			_textDescription->posY = GetPosition(index).getY();
		}
		index++;
	}
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
void Inventory::removeItem(const std::string& idToRemove, std::vector<Entity*>& invEntityList, std::list<std::string>& invIdList)
{
	auto hintIt = hints.begin();
	auto entityIt = invEntityList.begin();
	auto IdIt = invIdList.begin();
	auto itemIt = items.begin();

	while (itemIt != items.end()) {
		if ((*itemIt)->getID() == idToRemove) {
			// move the rest of the items up only if the items that are shown are between 0 and 2
			
		    
			//saves the position of the entity to be removed
			float removedY = 0;
			if (entityIt != invEntityList.end()) {
				auto transform = (*entityIt)->getMngr()->getComponent<Transform>(*entityIt);
				removedY = transform->getPos().getY();
			}

			// desactivate the item from the lists
			if (hintIt != hints.end()) {
				(*hintIt)->getMngr()->setActive(*hintIt, false);
				hintIt = hints.erase(hintIt);
			}

			if (entityIt != invEntityList.end()) {
				(*entityIt)->getMngr()->setActive(*entityIt, false);
				entityIt = invEntityList.erase(entityIt);
			}

			if (IdIt != invIdList.end()) {
				IdIt = invIdList.erase(IdIt);
			}

			itemIt = items.erase(itemIt);
			
			
			if (firstItem == 0) {
				int i = 0;
				int posE = std::distance(invEntityList.begin(), entityIt);
				// move the rest of the items up
				for (auto it = entityIt; it != invEntityList.end(); ++it, i++) {
					auto transform = (*it)->getMngr()->getComponent<Transform>(*it);
					transform->setPosY(transform->getPos().getY() - 150);

					if (i == 2 - posE) {
						(*it)->getMngr()->setActive(*it, true);
					}
				}

				int j = 0;
				int posH = std::distance(hints.begin(), hintIt);
				for (auto it = hintIt; it != hints.end(); ++it) {
					auto transform = (*it)->getMngr()->getComponent<Transform>(*it);
					transform->setPosY(transform->getPos().getY() - 150);

					if (j == 2 - posH) {
						(*it)->getMngr()->setActive(*it, true);
					}
				}
			}
			else //if the first item is not 0, we have to move the rest of the items up
			{
				int i = 0;
				//int posE = std::distance(invEntityList.begin(), entityIt);
				// move the rest of the items up
				for (auto it = invEntityList.begin(); it != entityIt; ++it, i++) {
					auto transform = (*it)->getMngr()->getComponent<Transform>(*it);
					transform->setPosY(transform->getPos().getY() + 150);

					if (i == firstItem - 1) {
						(*it)->getMngr()->setActive(*it, true);
					}
				}
				int j = 0;
				for (auto it = hints.begin(); it != hintIt; ++it, j++) {
					auto transform = (*it)->getMngr()->getComponent<Transform>(*it);
					transform->setPosY(transform->getPos().getY() + 150);
					if (j == firstItem - 1) {
						(*it)->getMngr()->setActive(*it, true);
					}
				}
			firstItem = firstItem - 1;
			}
			

			return;
		}

		++hintIt;
		++entityIt;
		++IdIt;
		++itemIt;
	}
}

void Inventory::eraseNotRoomItems()
{
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

