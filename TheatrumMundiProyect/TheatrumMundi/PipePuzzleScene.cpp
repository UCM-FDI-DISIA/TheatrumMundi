#include "PipePuzzleScene.h"
#include "EntityFactory.h"

#include "../src/utils/Vector2D.h";

#include "../src/game/Game.h";
#include "../src/components/Transform.h"
#include "Direction.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"
#include "../src/ecs/Entity.h"
#include "../TheatrumMundi/SceneManager.h"

#include "../src/components/ClickComponent.h"
#include "../src/components/RectArea2D.h"
#include "SceneRoomTemplate.h"
#include "../src/components/TriggerComponent.h"
#include "Inventory.h"
#include "DialogueManager.h"
#include "../src/Components/ScrollComponent.h"

#include "../src/game/Game.h"
#include "Log.h"
#include "ClickableSpriteComponent.h"

PipePuzzleScene::PipePuzzleScene()
	:ScenePuzzleTemplate()
{
	dialogueManager = new DialogueManager(1);
}

void PipePuzzleScene::pipeCreation()
{
	int nextPipeId = 0;

	_waterPipes.push_back(new Pipe({ nextPipeId, Pipe::TWO, {'M', 0, DOWN}, {'M', 1, RIGHT}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, Pipe::ONE, {'M', 0,   RIGHT}, {'P', 0,  NONE}, true }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, Pipe::TWO, {'P', 5,  NONE}, {'P', 3,  NONE}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++,Pipe::ONE, {'M', 1,  DOWN}, {'M', 4,  UP}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++,Pipe::TWO, {'M', 3,  DOWN}, {'M', 5,  RIGHT}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++,Pipe::ONE, {'M', 4,   RIGHT}, {'M', 5,  UP}, true }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, Pipe::TWO, {'M', 3,  RIGHT}, {'M', 2,  DOWN}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, Pipe::ONE, {'P', 6,  NONE}, {'M', 2,  RIGHT}, false }));
	_waterPipes.push_back(new Pipe({ nextPipeId++, Pipe::TWO, {'P', 4,  NONE}, {'P', 7,  NONE}, false }));


}

void PipePuzzleScene::moduleCreation()
{
	// P is pipe, M is module and N is null, the number means which one it is
	//if N 1 means it is always going to have water
	//if N 0 never going to have water(null direction)
	 int nextId = 0;
	
	_modules.push_back(new Module({ nextId, RIGHT, {'P', 0}, true }));
	_modules.push_back(new Module({ nextId++,  RIGHT, {'P', 2}, true }));
	_modules.push_back(new Module({ nextId++,  DOWN,  {'P', 5}, true }));
	_modules.push_back(new Module({ nextId++,  RIGHT,  {'P', 10}, false }));
	_modules.push_back(new Module({ nextId++,  RIGHT, {'P', 18}, true }));
	_modules.push_back(new Module({ nextId++,  UP, {'P', 19}, true }));
}

void PipePuzzleScene::pathCreation()
{
	
    int nextId = 0;

	//PATH 0 
	_waterPath.push_back({ nextId, true, {'N',0,NONE}});//0

	auto path0 =entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(180, -50),
	Vector2D(0, 0), 90, 40, 0, ecs::grp::DEFAULT);
	_waterPath[0]._pathPieces.push_back({ path0, 2 });

	Transform* transformComponent = path0->getMngr()->getComponent<Transform>(path0);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);

	 path0 = entityFactory->CreateImageEntity(entityManager, "pathLWater", Vector2D(210, 20),
		 Vector2D(0, 0), 70, 60, 0, ecs::grp::DEFAULT);
	_waterPath[0]._pathPieces.push_back({ path0, 1 });

	 transformComponent = path0->getMngr()->getComponent<Transform>(path0);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);

	 path0 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(275, 50),
		 Vector2D(0, 0), 65, 40, 0, ecs::grp::DEFAULT);
	_waterPath[0]._pathPieces.push_back({ path0, 2 });
	

	//PATH 1
	_waterPath.push_back({ nextId++, true,{'M',0,RIGHT} });//1
	auto path1 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(410, 45),
		Vector2D(0, 0), 65, 40, 0, ecs::grp::DEFAULT);
	_waterPath[1]._pathPieces.push_back({ path1, 2 });

	 path1 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(475, 45),
		 Vector2D(0, 0), 75, 40, 0, ecs::grp::DEFAULT);
	_waterPath[1]._pathPieces.push_back({ path1, 2 });

	 path1 = entityFactory->CreateImageEntity(entityManager, "pathLWater", Vector2D(545, 52),
		 Vector2D(0, 0), 70, 60, 0, ecs::grp::DEFAULT);
	_waterPath[1]._pathPieces.push_back({ path1, 1 });

	transformComponent = path1->getMngr()->getComponent<Transform>(path1);
	transformComponent->setRot(transformComponent->getRot() + 270.0f);

	 path1 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(575, 118),
		 Vector2D(0, 0), 48, 40, 0, ecs::grp::DEFAULT);
	_waterPath[1]._pathPieces.push_back({ path1, 2 });

	transformComponent = path1->getMngr()->getComponent<Transform>(path1);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);



	//PATH 2 
	_waterPath.push_back({ nextId++, true,{'N',0,NONE} }); //2
	auto path2 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(26, -50),
		Vector2D(0, 0), 90, 40, 0, ecs::grp::DEFAULT);
	_waterPath[2]._pathPieces.push_back({ path2, 2 });

	transformComponent = path2->getMngr()->getComponent<Transform>(path2);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);

	 path2 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(40, 25),
		 Vector2D(0, 0), 63, 40, 0, ecs::grp::DEFAULT);
	_waterPath[2]._pathPieces.push_back({ path2, 2 });

	transformComponent = path2->getMngr()->getComponent<Transform>(path2);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);


	 path2 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(38, 90),
		 Vector2D(0, 0), 70, 40, 0, ecs::grp::DEFAULT);
	_waterPath[2]._pathPieces.push_back({ path2, 2 });

	transformComponent = path2->getMngr()->getComponent<Transform>(path2);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);

	 path2 = entityFactory->CreateImageEntity(entityManager, "pathLWater", Vector2D(55,150),
		 Vector2D(0, 0), 70, 60, 0, ecs::grp::DEFAULT);
	_waterPath[2]._pathPieces.push_back({ path2, 1 });
	transformComponent = path2->getMngr()->getComponent<Transform>(path2);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);
	

	//PATH 3 
	_waterPath.push_back({ nextId++, true ,{'M',1,RIGHT} });//3
	auto path3 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(189, 175),
		Vector2D(0, 0), 70, 40, 0, ecs::grp::DEFAULT);
	_waterPath[3]._pathPieces.push_back({ path3, 2 });

	 path3 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(258, 175),
		 Vector2D(0, 0), 77, 40, 0, ecs::grp::DEFAULT);
	_waterPath[3]._pathPieces.push_back({ path3, 2 });


	//PATH 4  
	_waterPath.push_back({ nextId++, false,{'P',0,NONE} });//4
	auto path4 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(405, 175),
		Vector2D(0, 0), 75, 40, 0, ecs::grp::DEFAULT);
	_waterPath[4]._pathPieces.push_back({ path4, 2 });

	path4 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(480, 175),
		Vector2D(0, 0), 80, 40, 0, ecs::grp::DEFAULT);
	_waterPath[4]._pathPieces.push_back({ path4, 2 });

	//PATH 5 
	_waterPath.push_back({ nextId++, true,{'P',1,NONE} });//5
	auto path5 = entityFactory->CreateImageEntity(entityManager, "pathLWater", Vector2D(630, 150),
		Vector2D(0, 0), 60, 60, 0, ecs::grp::DEFAULT);
	_waterPath[5]._pathPieces.push_back({ path5, 1 });

	 path5 = entityFactory->CreateImageEntity(entityManager, "pathLWater", Vector2D(660, 95),
		 Vector2D(0, 0), 60, 60, 0, ecs::grp::DEFAULT);
	_waterPath[5]._pathPieces.push_back({ path5, 1 });

	transformComponent = path5->getMngr()->getComponent<Transform>(path5);
	transformComponent->setRot(transformComponent->getRot() + 180.0f);


	//PATH 6 
	_waterPath.push_back({ nextId++, false,{'M',2,RIGHT} });//6
	auto path6 = entityFactory->CreateImageEntity(entityManager, "pathLNoWater", Vector2D(790, 60),
		Vector2D(0, 0), 60, 60, 0, ecs::grp::DEFAULT);
	_waterPath[6]._pathPieces.push_back({ path6, 1 });

	 path6 = entityFactory->CreateImageEntity(entityManager, "pathLNoWater", Vector2D(825, 10),
		 Vector2D(0, 0), 60, 50, 0, ecs::grp::DEFAULT);
	_waterPath[6]._pathPieces.push_back({ path6, 1 });

	transformComponent = path6->getMngr()->getComponent<Transform>(path6);
	transformComponent->setRot(transformComponent->getRot() + 180.0f);

	 path6 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(885, 0),
		 Vector2D(0, 0), 90, 40, 0, ecs::grp::DEFAULT);
	_waterPath[6]._pathPieces.push_back({ path6, 2 });
	
	 path6 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(975, 0),
		 Vector2D(0, 0), 90, 40, 0, ecs::grp::DEFAULT);
	_waterPath[6]._pathPieces.push_back({ path6, 2 });

	 path6 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(1065, 0),
		 Vector2D(0, 0), 90, 40, 0, ecs::grp::DEFAULT);
	_waterPath[6]._pathPieces.push_back({ path6, 2 });

	 path6 = entityFactory->CreateImageEntity(entityManager, "pathLNoWater", Vector2D(1165, -10),
		 Vector2D(0, 0), 80, 100, 0, ecs::grp::DEFAULT);
	_waterPath[6]._pathPieces.push_back({ path6, 1 });

	transformComponent = path6->getMngr()->getComponent<Transform>(path6);
	transformComponent->setRot(transformComponent->getRot() + 270.0f);

	//PATH 7
	_waterPath.push_back({ nextId++, false,{'M',1,DOWN} });//7
	auto path7 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(120, 245),
		Vector2D(0, 0), 72, 40, 0, ecs::grp::DEFAULT);
	_waterPath[7]._pathPieces.push_back({ path7, 2 });

	transformComponent = path7->getMngr()->getComponent<Transform>(path7);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);


	//PATH 8
	_waterPath.push_back({ nextId++, false ,{'M',4,UP} });//8
	auto path8 = entityFactory->CreateImageEntity(entityManager, "pathLNoWater", Vector2D(60, 320),
		Vector2D(0, 0), 60, 60, 0, ecs::grp::DEFAULT);
	_waterPath[8]._pathPieces.push_back({ path8, 1 });

	transformComponent = path8->getMngr()->getComponent<Transform>(path8);
	transformComponent->setRot(transformComponent->getRot() + 180.0f);

	 path8 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(50, 385),
		 Vector2D(0, 0), 52, 40, 0, ecs::grp::DEFAULT);
	_waterPath[8]._pathPieces.push_back({ path8, 2 });

	transformComponent = path8->getMngr()->getComponent<Transform>(path8);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);

	//PATH 9
	_waterPath.push_back({ nextId++, false,{'P',3,NONE} });//9
	auto path9 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(190, 310),
		Vector2D(0, 0), 80, 40, 0, ecs::grp::DEFAULT);
	_waterPath[9]._pathPieces.push_back({ path9, 2 });

	 path9 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(270, 310),
		 Vector2D(0, 0), 65, 40, 0, ecs::grp::DEFAULT);
	_waterPath[9]._pathPieces.push_back({ path9, 2 });

	//PATH 10
	_waterPath.push_back({ nextId++, false,{'P',2,NONE} });//10
	auto path10 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(405, 310),
		Vector2D(0, 0), 75, 40, 0, ecs::grp::DEFAULT);
	_waterPath[10]._pathPieces.push_back({ path10, 2 });

	 path10 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(480, 310),
		 Vector2D(0, 0), 80, 40, 0, ecs::grp::DEFAULT);
	_waterPath[10]._pathPieces.push_back({ path10, 2 });

	//PATH 11
	_waterPath.push_back({ nextId++, false ,{'P',5,NONE} });//11
	auto path11 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(338, 380),
		Vector2D(0, 0), 65, 40, 0, ecs::grp::DEFAULT);
	_waterPath[11]._pathPieces.push_back({ path11, 2 });
	transformComponent = path11->getMngr()->getComponent<Transform>(path11);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);

	//PATH 12
	_waterPath.push_back({ nextId++, false,{'M',3,RIGHT} });//12
	auto path12 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(625, 310),
		Vector2D(0, 0), 100, 40, 0, ecs::grp::DEFAULT);
	_waterPath[12]._pathPieces.push_back({ path12, 2 });

	//PATH 13
	_waterPath.push_back({ nextId++, false,{'P',6,NONE} });//13
	auto path13 = entityFactory->CreateImageEntity(entityManager, "pathLNoWater", Vector2D(790, 280),
		Vector2D(0, 0), 60, 60, 0, ecs::grp::DEFAULT);
	_waterPath[13]._pathPieces.push_back({ path13, 1 });

	 path13 = entityFactory->CreateImageEntity(entityManager, "pathLNoWater", Vector2D(825, 220),
		 Vector2D(0, 0), 60, 60, 0, ecs::grp::DEFAULT);
	_waterPath[13]._pathPieces.push_back({ path13, 1 });

	transformComponent = path13->getMngr()->getComponent<Transform>(path13);
	transformComponent->setRot(transformComponent->getRot() + 180.0f);


	 path13 = entityFactory->CreateImageEntity(entityManager, "pathLNoWater", Vector2D(885, 180),
		Vector2D(0, 0), 60, 60, 0, ecs::grp::DEFAULT);
	_waterPath[13]._pathPieces.push_back({ path13, 1 });

	
	path13 = entityFactory->CreateImageEntity(entityManager, "pathLNoWater", Vector2D(920, 100),
		Vector2D(0, 0), 60, 80, 0, ecs::grp::DEFAULT);
	_waterPath[13]._pathPieces.push_back({ path13, 1 });

	transformComponent = path13->getMngr()->getComponent<Transform>(path13);
	transformComponent->setRot(transformComponent->getRot() + 180.0f);

	 path13 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(980, 95),
		 Vector2D(0, 0), 90, 40, 0, ecs::grp::DEFAULT);
	_waterPath[13]._pathPieces.push_back({ path13, 2 });


	path13 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(1070, 95),
		Vector2D(0, 0), 80, 40, 0, ecs::grp::DEFAULT);
	_waterPath[13]._pathPieces.push_back({ path13, 2 });

	path13 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(1150, 95),
		Vector2D(0, 0), 60, 40, 0, ecs::grp::DEFAULT);
	_waterPath[13]._pathPieces.push_back({ path13, 2 });

	

	//PATH 14
	_waterPath.push_back({ nextId++, false,{'P',7,NONE} });//14
	auto path14 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(1205, 175),
		Vector2D(0, 0), 90, 40, 0, ecs::grp::DEFAULT);
	_waterPath[14]._pathPieces.push_back({ path14, 2 });


	transformComponent = path14->getMngr()->getComponent<Transform>(path14);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);

	 path14 = entityFactory->CreateImageEntity(entityManager, "pathLNoWater", Vector2D(1210, 240),
		 Vector2D(0, 0), 60, 70, 0, ecs::grp::DEFAULT);
	_waterPath[14]._pathPieces.push_back({ path14, 1 });

	//PATH 15
	_waterPath.push_back({ nextId++, false ,{'P',4,NONE} });//15
	auto path15 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(720, 585),
		Vector2D(0, 0), 70, 40, 0, ecs::grp::DEFAULT);
	_waterPath[15]._pathPieces.push_back({ path15, 2 });

	transformComponent = path15->getMngr()->getComponent<Transform>(path15);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);


	 path15 = entityFactory->CreateImageEntity(entityManager, "pathLNoWater", Vector2D(745, 635),
		 Vector2D(0, 0), 60, 70, 0, ecs::grp::DEFAULT);
	_waterPath[15]._pathPieces.push_back({ path15, 1 });

	transformComponent = path15->getMngr()->getComponent<Transform>(path15);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);


	path15 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(810, 665),
		Vector2D(0, 0), 90, 40, 0, ecs::grp::DEFAULT);
	_waterPath[15]._pathPieces.push_back({ path15, 2 });


	path15 = entityFactory->CreateImageEntity(entityManager, "pathLNoWater", Vector2D(900, 628),
		Vector2D(0, 0), 60, 70, 0, ecs::grp::DEFAULT);
	_waterPath[15]._pathPieces.push_back({ path15, 1 });


	 path15 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(900, 565),
		 Vector2D(0, 0), 90, 40, 0, ecs::grp::DEFAULT);
	_waterPath[15]._pathPieces.push_back({ path15, 2 });


	transformComponent = path15->getMngr()->getComponent<Transform>(path15);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);


	path15 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(900, 475),
		Vector2D(0, 0), 90, 40, 0, ecs::grp::DEFAULT);
	_waterPath[15]._pathPieces.push_back({ path15, 2 });

	transformComponent = path15->getMngr()->getComponent<Transform>(path15);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);

	path15 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(900, 385),
		Vector2D(0, 0), 90, 40, 0, ecs::grp::DEFAULT);
	_waterPath[15]._pathPieces.push_back({ path15, 2 });

	transformComponent = path15->getMngr()->getComponent<Transform>(path15);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);

	path15 = entityFactory->CreateImageEntity(entityManager, "pathLNoWater", Vector2D(932, 290),
		Vector2D(0, 0), 60, 70, 0, ecs::grp::DEFAULT);
	_waterPath[15]._pathPieces.push_back({ path15, 1 });

	transformComponent = path15->getMngr()->getComponent<Transform>(path15);
	transformComponent->setRot(transformComponent->getRot() + 180.0f);

	path15 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(992, 285),
		Vector2D(0, 0), 90, 40, 0, ecs::grp::DEFAULT);
	_waterPath[15]._pathPieces.push_back({ path15, 2 });

	path15 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(1080, 285),
		Vector2D(0, 0), 60, 40, 0, ecs::grp::DEFAULT);
	_waterPath[15]._pathPieces.push_back({ path15, 2 });






	//PATH 16
	_waterPath.push_back({ nextId++, false ,{'M',5,RIGHT} });//16
	auto path16 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(400, 635),
		Vector2D(0, 0), 70, 40, 0, ecs::grp::DEFAULT);
	_waterPath[16]._pathPieces.push_back({ path16, 2 });

	 path16 = entityFactory->CreateImageEntity(entityManager, "pathLNoWater", Vector2D(470, 600),
		 Vector2D(0, 0), 60, 70, 0, ecs::grp::DEFAULT);
	_waterPath[16]._pathPieces.push_back({ path16, 1 });

	 path16 = entityFactory->CreateImageEntity(entityManager, "pathLNoWater", Vector2D(500, 530),
		 Vector2D(0, 0), 60, 70, 0, ecs::grp::DEFAULT);
	_waterPath[16]._pathPieces.push_back({ path16, 1 });

	transformComponent = path16->getMngr()->getComponent<Transform>(path16);
	transformComponent->setRot(transformComponent->getRot() + 180.0f);

	 path16 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(560, 520),
		 Vector2D(0, 0), 80, 40, 0, ecs::grp::DEFAULT);
	_waterPath[16]._pathPieces.push_back({ path16, 2 });


	path16 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(640, 520),
		Vector2D(0, 0), 85, 40, 0, ecs::grp::DEFAULT);
	_waterPath[16]._pathPieces.push_back({ path16, 2 });

	//PATH 17
	_waterPath.push_back({ nextId++, true,{'M',4,RIGHT} });//17

	auto path17 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(110, 445),
		Vector2D(0, 0), 80, 40, 0, ecs::grp::DEFAULT);
	_waterPath[17]._pathPieces.push_back({ path17, 2 });

	 path17 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(190, 445),
		 Vector2D(0, 0), 70, 40, 0, ecs::grp::DEFAULT);
	_waterPath[17]._pathPieces.push_back({ path17, 2 });

	 path17 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(260, 445),
		 Vector2D(0, 0), 70, 40, 0, ecs::grp::DEFAULT);
	_waterPath[17]._pathPieces.push_back({ path17, 2 });


	//PATH 18
	_waterPath.push_back({ nextId++, true,{'N',0,NONE} });//18

	auto path18 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(45, 510),
		Vector2D(0, 0), 60, 40, 0, ecs::grp::DEFAULT);
	_waterPath[18]._pathPieces.push_back({ path18, 2 });

	transformComponent = path18->getMngr()->getComponent<Transform>(path18);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);

	 path18 = entityFactory->CreateImageEntity(entityManager, "pathLWater", Vector2D(30, 560),
		 Vector2D(0, 0), 60, 60, 0, ecs::grp::DEFAULT);
	_waterPath[18]._pathPieces.push_back({ path18, 1 });

	 path18 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(-60, 590),
		 Vector2D(0, 0), 90, 40, 0, ecs::grp::DEFAULT);
	_waterPath[18]._pathPieces.push_back({ path18, 2 });

	transformComponent = path18->getMngr()->getComponent<Transform>(path18);
	transformComponent->setRot(transformComponent->getRot() + 180.0f);


	//PATH 19
	_waterPath.push_back({ nextId++, true,{'N',0,NONE} });//19

	 auto path19 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(-45, 715),
		 Vector2D(0, 0), 90, 30, 0, ecs::grp::DEFAULT);
	_waterPath[19]._pathPieces.push_back({ path19, 2 });

	 path19 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(45, 715),
		 Vector2D(0, 0), 90, 30, 0, ecs::grp::DEFAULT);
	_waterPath[19]._pathPieces.push_back({ path19, 2 });
	
	 path19 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(135, 715),
		 Vector2D(0, 0), 90, 30, 0, ecs::grp::DEFAULT);
	_waterPath[19]._pathPieces.push_back({ path19, 2 });

	 path19 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(225, 715),
		 Vector2D(0, 0), 90, 30, 0, ecs::grp::DEFAULT);
	_waterPath[19]._pathPieces.push_back({ path19, 2 });

	 path19 = entityFactory->CreateImageEntity(entityManager, "pathLWater", Vector2D(315, 690),
		 Vector2D(0, 0), 60, 50, 0, ecs::grp::DEFAULT);
	_waterPath[19]._pathPieces.push_back({ path19, 1 });

	//PATH 20
	_waterPath.push_back({ nextId++, true,{'M',5,UP} });//20
	auto path20 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(335,510),
		Vector2D(0, 0), 65, 40, 0, ecs::grp::DEFAULT);
	_waterPath[20]._pathPieces.push_back({ path20, 2 });

	transformComponent = path20->getMngr()->getComponent<Transform>(path20);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);


	 path20 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(338, 570),
		 Vector2D(0, 0), 60, 40, 0, ecs::grp::DEFAULT);
	_waterPath[20]._pathPieces.push_back({ path20, 2 });

	transformComponent = path20->getMngr()->getComponent<Transform>(path20);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);

	//PATH 21
	_waterPath.push_back({ nextId++, false,{'P',8,NONE} });//21
	auto path21 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(1135, 345),
		Vector2D(0, 0), 70, 40, 0, ecs::grp::DEFAULT);
	_waterPath[21]._pathPieces.push_back({ path21, 2 });


	transformComponent = path21->getMngr()->getComponent<Transform>(path21);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);

	//PATH 22
	_waterPath.push_back({ nextId++, true,{'M',2,DOWN} });//22
	auto path22 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(715, 160),
		Vector2D(0, 0), 80, 40, 0, ecs::grp::DEFAULT);
	_waterPath[22]._pathPieces.push_back({ path22, 2 });


	transformComponent = path22->getMngr()->getComponent<Transform>(path22);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);
	
	 path22 = entityFactory->CreateImageEntity(entityManager, "pathWater", Vector2D(717, 235),
		 Vector2D(0, 0), 75, 40, 0, ecs::grp::DEFAULT);
	_waterPath[22]._pathPieces.push_back({ path22, 2 });

	transformComponent = path22->getMngr()->getComponent<Transform>(path22);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);

	//PATH 23
	_waterPath.push_back({ nextId++, false,{'M',0,DOWN} });//23
	auto path23 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(340, 112),
		Vector2D(0, 0), 68, 40, 0, ecs::grp::DEFAULT);
	_waterPath[23]._pathPieces.push_back({ path23, 2 });

	transformComponent = path23->getMngr()->getComponent<Transform>(path23);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);


	//PATH 24
	_waterPath.push_back({ nextId++, false ,{'M',3,DOWN},{} });//24
	auto path24 = entityFactory->CreateImageEntity(entityManager, "pathLNoWater", Vector2D(575, 370),
		Vector2D(0, 0), 70, 60, 0, ecs::grp::DEFAULT);
	_waterPath[24]._pathPieces.push_back({ path24, 1 });

	 transformComponent = path24->getMngr()->getComponent<Transform>(path24);
	transformComponent->setRot(transformComponent->getRot() + 90.0f);
	
	path24 = entityFactory->CreateImageEntity(entityManager, "pathNoWater", Vector2D(640, 400),
		Vector2D(0, 0), 60, 40, 0, ecs::grp::DEFAULT);
	_waterPath[24]._pathPieces.push_back({ path24, 2 });
	
	path24 = entityFactory->CreateImageEntity(entityManager, "pathLNoWater", Vector2D(680, 410),
		Vector2D(0, 0), 110, 70, 0, ecs::grp::DEFAULT);
	_waterPath[24]._pathPieces.push_back({ path24, 1 });

	transformComponent = path24->getMngr()->getComponent<Transform>(path24);
	transformComponent->setRot(transformComponent->getRot() + 270.0f);

}

bool PipePuzzleScene::Check()
{
	if (_waterPipes[8]->getPipeInfo().result ==true&&!solved) //the last pipe has the solution
	{

		std::shared_ptr<Sound> waterSound = sdlutils().soundEffects().at("agua");
		AudioManager::Instance().playSound(waterSound);
		solved = true;
		Win();
		return true;
	}
	else
	{

		return false;
	}
}

void PipePuzzleScene::changeDirection(int module)
{
	//changes the direction +90º

	if (_modules[module]->getModuleInfo().dir == Direction::RIGHT)
	{
		_modules[module]->changeModuleInfo().dir = Direction::DOWN;
	}
	else if (_modules[module]->getModuleInfo().dir == Direction::DOWN )
	{
		_modules[module]->changeModuleInfo().dir = Direction::LEFT;
	}
	else if (_modules[module]->getModuleInfo().dir == Direction::UP )
	{
		_modules[module]->changeModuleInfo().dir = Direction::RIGHT;
	}
	else if (_modules[module]->getModuleInfo().dir == Direction::LEFT )
	{
		_modules[module]->changeModuleInfo().dir = Direction::UP;
	}

	//cout<<"DIR"<<module<<" "<< _modules[module]->getModuleInfo().dir<<endl;
	//rotates the module
	Transform* transformComponent = _modulesEnt[module]->getMngr()->getComponent<Transform>(_modulesEnt[module]);
	
	float newRot = transformComponent->getRot() + 90.0f;
	if (newRot >= 360.0f) {
		newRot -= 360.0f;
	}
	transformComponent->setRot(newRot); 

	updatePuzzle();
	
}

void PipePuzzleScene::init(SceneRoomTemplate* sr)
{
	
	if (!isStarted) {
		_updatePuzzle = false;
		solved = false;
		isStarted = true;
		room = sr;

		AudioManager& a = AudioManager::Instance();

		std::shared_ptr<Sound> buttonSound = sdlutils().soundEffects().at("boton");
		a.setVolume(buttonSound, 0.2);

		

		std::shared_ptr<Sound> ropeSound = sdlutils().soundEffects().at("cuerda");
		
		//Create cube without water
		_cubeWithoutWater = entityFactory->CreateImageEntity(
			entityManager,"cubeWithoutWater",Vector2D(2, 0),Vector2D(0, 0),1346,748,0,ecs::grp::UNDER
		);
		

		//INVENTORY
				//Invntory Background
		auto InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1050, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::UI);
		entityManager->setActive(InventoryBackground, false);

		auto upButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(upButton, false);

		auto downButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 748 - 268 / 3 - 20), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(downButton, false);

		//InventoryButton
		auto inventoryButton = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		ClickComponent* invOpen = entityManager->addComponent<ClickComponent>(inventoryButton);
		invOpen->connect(ClickComponent::JUST_CLICKED, [this, sr, InventoryBackground, upButton, downButton, inventoryButton, buttonSound]() //Lamda function
			{
				AudioManager::Instance().playSound(buttonSound);
				sr->GetInventory()->setActive(!sr->GetInventory()->getActive());  // Toggle the inventory

				for (int i = 0; i < invObjects.size(); ++i) {
					std::cout << "objetos antes y despues: " << sr->GetInventory()->hints[i]->getMngr()->getComponent<Transform>(sr->GetInventory()->hints[i])->getPos().getY() << std::endl;
				}

				// If the inventory is active, activate the items
				if (sr->GetInventory()->getActive()) {
					entityManager->setActive(InventoryBackground, true);

					inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(925);
					entityManager->setActive(downButton, true);
					entityManager->setActive(upButton, true);
					entityManager->setActive(logbtn, false);
					for (int i = sr->GetInventory()->getFirstItem(); i < sr->GetInventory()->getFirstItem() + sr->GetInventory()->getItemNumber(); ++i) {
						invObjects[i]->getMngr()->setActive(invObjects[i], true);
					}
				}
				else {
					entityManager->setActive(InventoryBackground, false);
					entityManager->setActive(InventoryBackground, false);
					entityManager->setActive(downButton, false);
					entityManager->setActive(upButton, false);
					entityManager->setActive(logbtn, true);
					inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);

					for (int i = sr->GetInventory()->getFirstItem(); i < sr->GetInventory()->getFirstItem() + sr->GetInventory()->getItemNumber(); ++i) {
						invObjects[i]->getMngr()->setActive(invObjects[i], false);
					}
				}
			});

		ClickComponent* UPbuttonInventoryClick = entityManager->getComponent<ClickComponent>(upButton);
		UPbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, upButton, sr]() {

			AudioManager::Instance().playSound(buttonSound);
			scrollInventoryPuzzle(-1, sr);
			});

		ClickComponent* DOWNbuttonInventoryClick = entityManager->getComponent<ClickComponent>(downButton);
		DOWNbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, downButton, sr]() {

			AudioManager::Instance().playSound(buttonSound);
			scrollInventoryPuzzle(1, sr);
			});

		
		//Viriant logic
		int variant = Game::Instance()->getDataManager()->GetRoomVariant(0);
		entity_t gloveEntity;
		ClickComponent* clk;
		std::cout << "variant:" << variant << endl;
		
			// create entity
			gloveEntity = entityFactory->CreateInteractableEntity(entityManager, "guantes", EntityFactory::RECTAREA,
				Vector2D(1150, 840), Vector2D(0, 0), 150, 150, 0,
				areaLayerManager,
				EntityFactory::NODRAG,
				ecs::grp::DEFAULT);
			//add click component
			entityManager->getComponent<ClickComponent>(gloveEntity)->connect(ClickComponent::JUST_CLICKED, [this, gloveEntity, sr]() {
				gloveEntity->getMngr()->setActive(gloveEntity, false);
				Vector2D position = sr->GetInventory()->setPosition();
				AddInvItem("guantes", "Unos guantes. Ya está. Son unos guantes.", position, sr);
				});
		


		// create entity
		auto clock = entityFactory->CreateInteractableEntity(entityManager, "minutero", EntityFactory::RECTAREA,
			Vector2D(980, 820), Vector2D(0, 0), 150, 250, 90,
			areaLayerManager,
			EntityFactory::NODRAG,
			ecs::grp::DEFAULT);

		//add click component
		entityManager->getComponent<ClickComponent>(clock)->connect(ClickComponent::JUST_CLICKED, [this, clock,sr]() {
			clock->getMngr()->setActive(clock, false);
			Vector2D position = sr->GetInventory()->setPosition();
			AddInvItem("minutero", "La manecilla de los minutos de un reloj", position, sr);
			});
			

		/*entityManager->getComponent<ClickComponent>(spoon)->connect(ClickComponent::JUST_CLICKED, [this, spoon]() {
			spoon->getMngr()->setActive(spoon, false);
			roomEvent[Spoon]();
			});*/

		//Create background
		auto background = entityFactory->CreateImageEntity(
			entityManager, "Pared", Vector2D(2, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DEFAULT);
	

		//Create string segnment sprite
		
		/*
		* auto ChangeRoom1 = 
		sentityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(34, 160), Vector2D(0, 0), 136, 495, 0,
		areaLayerManager, 12, ((sdlutils().width()) / 12) , EntityFactory::SCROLLNORMAL, 1, 
		EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		*/
		_rope = entityFactory->CreateInteractableEntityScroll(entityManager, "rope", EntityFactory::RECTAREA,
			Vector2D(1200, 400), Vector2D(0, 0), 324 / 3, 893 / 3, 1, areaLayerManager,
			2, 150, EntityFactory::SCROLLNORMAL, 1,
			EntityFactory::NODRAG,
			ecs::grp::DEFAULT);
		//if clicked remove click comonent and start animation 
		ClickComponent* clComponent = _rope->getMngr()->getComponent<ClickComponent>(_rope);

		clComponent->connect(ClickComponent::JUST_CLICKED, [this,gloveEntity,clock,variant, ropeSound]() {
			//std::cout << "PULSADO CUERDA";
			AudioManager::Instance().playSound(ropeSound);
			if (solved)
			{
			//add gloves and clock
				if (!entityManager->getComponent<ScrollComponent>(_rope)->isScrolling() && entityManager->getComponent<ScrollComponent>(_rope)->finalPhaseCheck())
				{
					Image* img = entityManager->getComponent<Image>(_cubeWithoutWater);
					img->setTexture(&sdlutils().images().at("fullRope"));

					auto ScrollCube = entityManager->getComponent<ScrollComponent>(_rope);
					if(variant<=1)ScrollCube->addElementToScroll(entityManager->getComponent<Transform>(gloveEntity));
					ScrollCube->addElementToScroll(entityManager->getComponent<Transform>(_cubeWithoutWater));
					ScrollCube->addElementToScroll(entityManager->getComponent<Transform>(clock));
					entityManager->getComponent<ScrollComponent>(_rope)->Scroll(ScrollComponent::UP);
					//entityManager->setActive(gloveEntity, true);
					//entityManager->setActive(_clock, true);


				}

			}

			});
		//entityManager->setActive(_rope, false);


		moduleCreation();
		pipeCreation();
		pathCreation();

		//Register scene in dialogue manager
		dialogueManager->setScene(this);

		//startDialogue("Puzzle1");

		//vector with pipe positions
		vector<Vector2D> pipePositions = {
			Vector2D(335, 165), // pos pipe 0
			Vector2D(560, 160), // pos pipe 1
			Vector2D(335, 300), // pos pipe 2
			Vector2D(120, 300), // pos pipe 3
			Vector2D(720, 500), // pos pipe 4
			Vector2D(330, 430), // pos pipe 5
			Vector2D(720, 290), // pos pipe 6
			Vector2D(1210, 80), // pos pipe 7
			Vector2D(1140, 260)  // pos pipe 8
		};

		// creates the components of the pipe
		for (int i = 0; i < pipePositions.size(); i++) {

			// create entity
			//ENTIDADCONENTITYFACTORY
			auto pipeit = entityFactory->CreateInteractableEntityNotMoveSprite(entityManager, "exit", EntityFactory::RECTAREA, pipePositions[i], Vector2D(0, 0), 70, 70, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			_pipesEnt.push_back(pipeit);

			Image* imageComponent = pipeit->getMngr()->getComponent<Image>(pipeit);

			if (_waterPipes[i]->getPipeInfo().type==Pipe::ONE)
			{
				//one entry
				imageComponent->setTexture(&sdlutils().images().at("pipe1"));

			}
			else
			{
				//two entries
				imageComponent->setTexture(&sdlutils().images().at("pipe2"));
			}
		}


		//vector with positions of modules
		vector<Vector2D> modulePositions = {
			Vector2D(340,30), // pos module 0
			Vector2D(120, 160), // pos module 1
			Vector2D(720, 70), // pos module 2
			Vector2D(558,295), // pos module 3
			Vector2D(40,430), // pos module 4
			Vector2D(330,620), // pos module 5
		};


		// creates the components of the module
		for (int i = 0; i < modulePositions.size(); i++) {

			// create entity
			//ENTIDADCONENTITYFACTORY
			auto moduleit = entityFactory->CreateInteractableEntityNotMoveSprite(entityManager, "module", EntityFactory::RECTAREA, modulePositions[i], Vector2D(0, 0), 70, 70, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			_modulesEnt.push_back(moduleit);

			Transform* transformComponent = moduleit->getMngr()->getComponent<Transform>(moduleit);

			if (i == 2)
			{
				transformComponent->setRot(transformComponent->getRot() + 90.0f);
			}
			else if (i == 5)
			{
				transformComponent->setRot(transformComponent->getRot() + 270.0f);
			}
			
			//// add area of visualization of the image
			//entityManager->addComponent<RectArea2D>(_modulesEnt[i], areaLayerManager);

			//add click component
			ClickComponent* clk = entityManager->getComponent<ClickComponent>(moduleit);
			clk->connect(ClickComponent::JUST_CLICKED, [this, i]() {
				changeDirection(i); 
				});
		}

		//ENTIDADCONENTITYFACTORY
		auto _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);

		//Click component Open log button
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, [this, sr, InventoryBackground, downButton, upButton, inventoryButton,_backButton, buttonSound]()
		{
				AudioManager::Instance().playSound(buttonSound);

			inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);
			HideInventoryItems(InventoryBackground, downButton, upButton, sr);
			sr->GetInventory()->setFirstItem(0);
			auto _backButtonImage = _backButton->getMngr()->getComponent<Image>(_backButton);
			_backButtonImage->setW(_backButton->getMngr()->getComponent<Transform>(_backButton)->getWidth());
			_backButtonImage->setH(_backButton->getMngr()->getComponent<Transform>(_backButton)->getHeight());
			_backButtonImage->setPosOffset(0, 0);
			Game::Instance()->getSceneManager()->popScene();

		});

		
		

		dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, "SalaIntermedia1");
		logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager,this);

		startDialogue("PuzzleTuberias");
	}
	//IMPORTANT this need to be out of the isstarted!!!
	sr->GetInventory()->setFirstItem(0);
	createInvEntities(sr);
}

void PipePuzzleScene::waterPassPipe(int pipe) {

	
	Pipe::pipeInfo pipeData = _waterPipes[pipe]->getPipeInfo();  

	//checks if its type 1 pipe only 1 entry has to have water not 2
	if (pipeData.type == Pipe::ONE) {
		
		//the entry(neightbourd) can be a module or a pipe so if its a module it needs to have the direction the entry needs
		bool ent1 = (pipeData.entry1.name == 'P' && _waterPipes[pipeData.entry1.num]->getPipeInfo().result == true) ||
			(pipeData.entry1.name == 'M' && _modules[pipeData.entry1.num]->getModuleInfo().result == true &&
				_modules[pipeData.entry1.num]->getModuleInfo().dir == pipeData.entry1.direction);

		bool ent2 = (pipeData.entry2.name == 'P' && _waterPipes[pipeData.entry2.num]->getPipeInfo().result == true) ||
			(pipeData.entry2.name == 'M' && _modules[pipeData.entry2.num]->getModuleInfo().result == true &&
				_modules[pipeData.entry2.num]->getModuleInfo().dir == pipeData.entry2.direction);

		//if the condition is correct change the result(if it carries water) to true
		if ((ent1 && !ent2)|| (!ent1 &&ent2))
		
		{  
			_waterPipes[pipe]->changePipeInfo().result = true;
				
			
		}
		else {
			
			_waterPipes[pipe]->changePipeInfo().result = false;
			
			
		}
	}

	//this type of pipe needs to entries with water
	if (pipeData.type == Pipe::TWO) {
		//the entry(neightbourd) can be a module or a pipe so if its a module it needs to have the direction the entry needs
		bool entry1HasWater = (pipeData.entry1.name == 'P' && _waterPipes[pipeData.entry1.num]->getPipeInfo().result) ||
			(pipeData.entry1.name == 'M' && _modules[pipeData.entry1.num]->getModuleInfo().result &&
				_modules[pipeData.entry1.num]->getModuleInfo().dir == pipeData.entry1.direction);

		bool entry2HasWater = (pipeData.entry2.name == 'P' && _waterPipes[pipeData.entry2.num]->getPipeInfo().result) ||
			(pipeData.entry2.name == 'M' && _modules[pipeData.entry2.num]->getModuleInfo().result &&
				_modules[pipeData.entry2.num]->getModuleInfo().dir == pipeData.entry2.direction);

		//if the condition is correct change the result(if it carries water) to true
		if (entry1HasWater && entry2HasWater) {
			 
			_waterPipes[pipe]->changePipeInfo().result = true;
				
		}
		else {
			 
			_waterPipes[pipe]->changePipeInfo().result = false;
			
		}
	}

	
	//std::cout << "Pipe " << pipe << " tiene resultado: " << pipeData.result << std::endl;

}

void PipePuzzleScene::waterPassModule(int module) {
	Module::moduleInfo modInfo = _modules[module]->getModuleInfo();

	bool receivesWater = false;
	
	if (_waterPath[modInfo.whoToCheck.second]._withWater) {
		receivesWater = true;
	}
	
	_modules[module]->changeModuleInfo().result = receivesWater;
	//std::cout << "Module " << module << " checking if it receives water: " << receivesWater << std::endl;

}


void PipePuzzleScene::waterPassPath(int path) {

	//checks if the condition needed for a path to have water is correct
	//it checks that the element is conected to carries water and if its s module, is in the right direction
	if (_waterPath[path]._whoTocheck.name == 'M') {
		auto moduleInfo = _modules[_waterPath[path]._whoTocheck.num]->getModuleInfo();
		if (moduleInfo.result && moduleInfo.dir == _waterPath[path]._whoTocheck.dir) {
			_waterPath[path]._withWater = true; 
		}
		else {
			_waterPath[path]._withWater = false;
		}
	}
	else if (_waterPath[path]._whoTocheck.name == 'P') 
	{
		_waterPath[path]._withWater = _waterPipes[_waterPath[path]._whoTocheck.num]->getPipeInfo().result;
	}
	else
	{
		_waterPath[path]._withWater = true;
	}

	
	if (_waterPath[path]._withWater) {
		// texture with water

		for (int i=0;i<_waterPath[path]._pathPieces.size();i++)
		{
			Image* img = _waterPath[path]._pathPieces[i].first->getMngr()->getComponent<Image>(_waterPath[path]._pathPieces[i].first);
			if (_waterPath[path]._pathPieces[i].second == 1)//L pipe 
			{
				img->setTexture(&sdlutils().images().at("pathLWater"));
			}
			else //straight
			{
				img->setTexture(&sdlutils().images().at("pathWater"));
			}
		}
	
	}
	else {
		for (int i = 0; i < _waterPath[path]._pathPieces.size(); i++)
		{
			Image* img = _waterPath[path]._pathPieces[i].first->getMngr()->getComponent<Image>(_waterPath[path]._pathPieces[i].first);
			if (_waterPath[path]._pathPieces[i].second == 1)//L pipe 
			{
				img->setTexture(&sdlutils().images().at("pathLNoWater"));
			}
			else //straight
			{
				img->setTexture(&sdlutils().images().at("pathNoWater"));
			}
		}
		
	}

}


void PipePuzzleScene::unload()
{
	for (auto a : _waterPipes) delete a;
	for (auto a : _modules) delete a;
	
}
void PipePuzzleScene::updatePuzzle() {
    bool stateChanged = true;

    while (stateChanged) {
        stateChanged = false;

        
        for (int i = 0; i < _modules.size(); i++) {
            bool before = _modules[i]->getModuleInfo().result;
            waterPassModule(i);  
            bool after = _modules[i]->getModuleInfo().result;

           // std::cout << "Module " << i << " Before: " << before << " After: " << after << std::endl;

            if (before != after) {
                stateChanged = true;
            }
        }

      
        for (int i = 0; i < _waterPath.size(); i++) {
            bool before = _waterPath[i]._withWater;
            waterPassPath(i);  
            bool after = _waterPath[i]._withWater;

           // std::cout << "Path " << i << " Before: " << before << " After: " << after << std::endl;

            if (before != after) {
                stateChanged = true;
            }
        }

      
        for (int i = 0; i < _waterPipes.size(); i++) {
            bool before = _waterPipes[i]->getPipeInfo().result;
            waterPassPipe(i); 
            bool after = _waterPipes[i]->getPipeInfo().result;

           // std::cout << "Pipe " << i << " Before: " << before << " After: " << after << std::endl;

            if (before != after) {
                stateChanged = true;
            }
        }

       
        Check();
    }
}


void PipePuzzleScene::Win()
{
	room->resolvedPuzzle(0);

	//Change the texture of the cube sprite to one with water
	Image* img = entityManager->getComponent<Image>(_cubeWithoutWater);
	img->setTexture(&sdlutils().images().at("cubeWithWater"));
	if (!entityManager->getComponent<ScrollComponent>(_rope)->isScrolling()) {
		auto ScrollCube = entityManager->getComponent<ScrollComponent>(_rope);
		ScrollCube->setEndScrollCallback([]() {});
		ScrollCube->addElementToScroll(entityManager->getComponent<Transform>(_cubeWithoutWater));
		entityManager->getComponent<ScrollComponent>(_rope)->Scroll(ScrollComponent::DOWN);
	}
}

void PipePuzzleScene::ResolveScene()
{
	Win();
}

PipePuzzleScene::~PipePuzzleScene()
{
	unload();
}
