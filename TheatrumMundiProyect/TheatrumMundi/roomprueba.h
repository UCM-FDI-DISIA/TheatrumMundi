#pragma once
#pragma once
#include "SceneRoomTemplate.h"
class roomprueba : public SceneRoomTemplate
{
private:
	std::string _eventToRead;
	bool isOpen; //Boolean which function is to permite player to enter the mausoleum or not (if the key is on the door or not)

	//Reference to entities which appear and dissapear
	ecs::entity_t _quitButton; //Reference to the zoomed Quit Button
	ecs::entity_t balance; // Image of the rope
	
protected:
	enum Room2Event
	{
		InitialDialogue = 0,
		CorpseDialogue,
		BalancePuzzleScene,
		BalanceSceneRsv,
		event_size,
	};
public:
	roomprueba();
	~roomprueba();
	void init() override;
	void resolvedPuzzle(int i) override;
	void unload() override;
};

