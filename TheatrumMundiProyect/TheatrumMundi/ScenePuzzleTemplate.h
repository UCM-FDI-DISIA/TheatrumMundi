#pragma once
#include "SceneTemplate.h"
#include <list>
class ScenePuzzleTemplate: public SceneTemplate
{
	private:

	bool placeHand; //if an object is on the trigger

	bool firstEntry; //To create Descriptions components only at first time
	
	protected:
		 
		bool solved = false; //If the item is created or not

		SceneRoomTemplate* room; //Reference to the room scene

		bool logActive = false; //if log is activated

		Vector2D originalPos; //Saves the origianl position of an object

		ecs::entity_t textDescriptionEnt; //Description entities
		ecs::entity_t inventoryButton;
		Transform* _testTextTranform; //Description position
		std::list<ecs::entity_t> invElems; // all the btns and bacground of the inventory
		void createInventoryUI();

	public:
	
	inline void setOriginalPos(Vector2D pos) { originalPos = pos; }; //Assigns the origianl position to the object

	inline Vector2D getOriginalPos() const { return originalPos; }; //return the original Position of the object

	inline bool getSolved() const { return solved; }; //Returns if the puzzle is solved or not

	inline void setSolved(bool isSolved) { solved = isSolved; }; //Sets the boolean solved

	inline void SetplacedHand(bool isPlaced) { placeHand = isPlaced; }; //SetsPlaced to true or false

	std::vector<ecs::Entity*> invObjects;//Array of invEntities

	std::list<std::string> invID; //Array of inventory object names

	void createInvEntities(SceneRoomTemplate* sr); //Creates all the inventory items in this scene

	void AddInvItem(const std::string& id, const std::string& description, const Vector2D& position, SceneRoomTemplate* sr); //Add a new item to the inventory

	virtual bool isItemHand(const std::string& itemId) { return false; }; //NEED TO BE IN ALL THE PUZZLE WHICH HAD INVENTORY LOGIC, change the entities that depends of the inventory to visible true if the spcefifc item was handle in the right position (Example in ClockPuzzleScene)

	virtual void Exit(); //what the puzzle must do when the player exits the puzzle

	virtual bool Check() { return true; }; //Compares if the puzzle was correctly resolved or not

	virtual void Win() {}; //Logic of the puzzle when the player resolved correctily

	bool ItemAlreadyCreated(const std::string& id); //Search if the item is in the list, this is to evitate duplications of an item

	void compareInv(SceneRoomTemplate* sr); //Compare the inventory of the room with the inventory of the puzzle

	void reposInv(SceneRoomTemplate* sr); //Reposition the inventory items

	void CreateDescriptions(SceneRoomTemplate* sr);

	void scrollInventoryPuzzle(int dir, SceneRoomTemplate* sr); //scroll puzzle

	void HideInventoryItems();

	ScenePuzzleTemplate();

	~ScenePuzzleTemplate();
	



};