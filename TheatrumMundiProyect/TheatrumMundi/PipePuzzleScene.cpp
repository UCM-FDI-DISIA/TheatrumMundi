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


PipePuzzleScene::PipePuzzleScene()
	:ScenePuzzleTemplate()
{
	
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
	
	_modules.push_back(new Module({ nextId, RIGHT, {'P', 1}, {'N', 1}, {'N', 0}, {'P', 0}, true }));
	_modules.push_back(new Module({ nextId++,  RIGHT, {'P', 0}, {'N', 1}, {'N', 0}, {'P', 3}, true }));
	_modules.push_back(new Module({ nextId++,  DOWN,  {'P', 7}, {'P', 1}, {'N', 0}, {'P', 6}, true }));
	_modules.push_back(new Module({ nextId++,  RIGHT,  {'P', 6}, {'P', 2}, {'N', 0}, {'P', 4}, false }));
	_modules.push_back(new Module({ nextId++,  RIGHT, {'P', 5}, {'N', 0}, {'P', 3}, {'N', 1}, true }));
	_modules.push_back(new Module({ nextId++,  UP, {'P', 4}, {'N', 0}, {'P', 5}, {'N', 1}, true }));
}

void PipePuzzleScene::pathCreation()
{
	/*auto plantilla = entityManager->addEntity();

	// add transfomr
	auto plantillaT = entityManager->addComponent<Transform>(
		plantilla, Vector2D(0, 0), Vector2D(0, 0), 1350, 750, 0
	);

	// add image
	entityManager->addComponent<Image>(plantilla, &sdlutils().images().at("plantilla"));

	// add area of visualization of the image
	entityManager->addComponent<RectArea2D>(plantilla);*/
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
	if (_waterPipes[8]->getPipeInfo().result ==true) //the last pipe has the solution
	{
		solved = true;
		Win();
		return true;
	}
	else
	{
		solved= false;
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

void PipePuzzleScene::init()
{
	if (!isStarted) {
		solved = false;
		moduleCreation();
		pipeCreation();
		pathCreation();

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
			_pipesEnt.push_back(entityManager->addEntity());
			
			// add transfomr
			auto pipeTransform = entityManager->addComponent<Transform>(
				_pipesEnt[i], pipePositions[i], Vector2D(0, 0), 70, 70, 0
			);

			// add image
			entityManager->addComponent<Image>(_pipesEnt[i], &sdlutils().images().at("exit"));

			// add area of visualization of the image
			entityManager->addComponent<RectArea2D>(_pipesEnt[i]);

			Image* imageComponent = _pipesEnt[i]->getMngr()->getComponent<Image>(_pipesEnt[i]);

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
			_modulesEnt.push_back(entityManager->addEntity());

			// add transfomr
			auto moduleTransform = entityManager->addComponent<Transform>(
				_modulesEnt[i], modulePositions[i], Vector2D(0, 0), 70, 70, 0
			);

			// add image
			entityManager->addComponent<Image>(_modulesEnt[i], &sdlutils().images().at("module"));

			Transform* transformComponent = _modulesEnt[i]->getMngr()->getComponent<Transform>(_modulesEnt[i]);

			if (i == 2)
			{
				transformComponent->setRot(transformComponent->getRot() + 90.0f);
			}
			else if (i == 5)
			{
				transformComponent->setRot(transformComponent->getRot() + 270.0f);
			}
			
			// add area of visualization of the image
			entityManager->addComponent<RectArea2D>(_modulesEnt[i]);

			//add click component
			ClickComponent* clk = entityManager->addComponent<ClickComponent>(_modulesEnt[i]);
			clk->connect(ClickComponent::JUST_CLICKED, [this, i]() {
				changeDirection(i); 
				});
		}


/*		vector<Vector2D> pathPositions = {
	Vector2D(50, 50),
	Vector2D(150, 50),
	Vector2D(250, 50),
	Vector2D(350, 50),
	Vector2D(450, 50),
	Vector2D(550, 50),
	Vector2D(650, 50),
	Vector2D(750, 50),

	Vector2D(50, 150),
	Vector2D(150, 150),
	Vector2D(250, 150),
	Vector2D(350, 150),
	Vector2D(450, 150),
	Vector2D(550, 150),
	Vector2D(650, 150),
	Vector2D(750, 150),

	Vector2D(50, 250),
	Vector2D(150, 250),
	Vector2D(250, 250),
	Vector2D(350, 250),
	Vector2D(450, 250),
	Vector2D(550, 250),
	Vector2D(650, 250),
	Vector2D(750, 250),
	Vector2D(400, 350)
		};



		for (int i = 0; i < pathPositions.size(); i++) {

			// create path
			_pathEnt.push_back(entityManager->addEntity());
			

			// add transform
			auto pathTransform = entityManager->addComponent<Transform>(
				_pathEnt[i], pathPositions[i], Vector2D(0, 0), 60, 60, 0
			);

			cout << "CARGADO" << i << endl;
			// add image
			entityManager->addComponent<Image>(_pathEnt[i], &sdlutils().images().at("init"));
			// add area of visualization of the image
			entityManager->addComponent<RectArea2D>(_pathEnt[i]);
			Image* imageComponent = _pathEnt[i]->getMngr()->getComponent<Image>(_pathEnt[i]);

			if (_waterPath[i]._withWater)
			{
				//texture with water
				imageComponent->setTexture(&sdlutils().images().at("pathWith"));

			}
			else
			{
				imageComponent->setTexture(&sdlutils().images().at("pathWithout"));
			}
			
			

		}
	*/
		/*// create cube
		auto cubeEntity = entityManager->addEntity();

		// add transfomr
		auto cubeTransform = entityManager->addComponent<Transform>(
			cubeEntity, Vector2D(900,580), Vector2D(0, 0), 150, 150, 0
		);
		// add image
		entityManager->addComponent<Image>(cubeEntity, &sdlutils().images().at("cube"));

		// add area of visualization of the image
		entityManager->addComponent<RectArea2D>(cubeEntity);
		*/
		//create exit botton
		auto exitEntity = entityManager->addEntity();

		// add transfomr
		auto exitTransform = entityManager->addComponent<Transform>(
			exitEntity, Vector2D(50, 580), Vector2D(0, 0), 150, 150, 0
		);
		// add image
		entityManager->addComponent<Image>(exitEntity, &sdlutils().images().at("exit"));

		// add area of visualization of the image
		entityManager->addComponent<RectArea2D>(exitEntity);

		//add click component
		ClickComponent* clk = entityManager->addComponent<ClickComponent>(exitEntity);
		clk->connect(ClickComponent::JUST_CLICKED, []() {
			Game::Instance()->getSceneManager()->popScene();
			});


	}
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
			
			_waterPipes[pipe]->changePipeInfo().result = true;
			
			
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

	//checks if the four neightbourds the module has, carries water
	bool receivesWater = false;

	if (modInfo.right.first == 'P' && _waterPipes[modInfo.right.second]->getPipeInfo().result) {
		receivesWater = true;
	}
	else if (modInfo.right.first == 'M' && _modules[modInfo.right.second]->getModuleInfo().result) {
		receivesWater = true;
	}
	else if (modInfo.right.first == 'N' && modInfo.right.second == 1) {
		receivesWater = true;
	}

	if (modInfo.left.first == 'P' && _waterPipes[modInfo.left.second]->getPipeInfo().result) {
		receivesWater = true;
	}
	else if (modInfo.left.first == 'M' && _modules[modInfo.left.second]->getModuleInfo().result) {
		receivesWater = true;
	}
	else if (modInfo.left.first == 'N' && modInfo.left.second == 1) {
		receivesWater = true;
	}

	if (modInfo.up.first == 'P' && _waterPipes[modInfo.up.second]->getPipeInfo().result) {
		receivesWater = true;
	}
	else if (modInfo.up.first == 'M' && _modules[modInfo.up.second]->getModuleInfo().result) {
		receivesWater = true;
	}
	else if (modInfo.up.first == 'N' && modInfo.up.second == 1) {
		receivesWater = true;
	}

	if (modInfo.down.first == 'P' && _waterPipes[modInfo.down.second]->getPipeInfo().result) {
		receivesWater = true;
	}
	else if (modInfo.down.first == 'M' && _modules[modInfo.down.second]->getModuleInfo().result) {
		receivesWater = true;
	}
	else if (modInfo.down.first == 'N' && modInfo.down.second == 1) {
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

	//create queue for 3 types of elements that have to be updated 
	std::queue<int> pipesToUpdate, modulesToUpdate, pathsToUpdate; 

	// add modules 
	for (int i = 0; i < _modules.size(); ++i) {
		modulesToUpdate.push(i);
		//std::cout << "Module " << i << " has water." << std::endl;  
		
	}

	// add pipes
	for (int i = 0; i < _waterPipes.size(); ++i) {
		pipesToUpdate.push(i);
		//std::cout << "Pipe " << i << " has water." << std::endl;  
		
	}

	// add paths
	for (int i = 0; i < _waterPath.size(); ++i) {
		pathsToUpdate.push(i);
		//std::cout << "Path " << i << " has water." << std::endl;  
	}

	//while there is something to update we process
	while (!pipesToUpdate.empty() || !modulesToUpdate.empty() || !pathsToUpdate.empty()) {

		//Update modules
		while (!modulesToUpdate.empty()) {

			//get the index of the fist one
			int moduleIndex = modulesToUpdate.front();

			//pop to have the next one ready
			modulesToUpdate.pop();
			//std::cout << "Updating Module " << moduleIndex << std::endl;  

			// update x module
			waterPassModule(moduleIndex);

			// if module has water, neightbour pipes need to be checked
			if (_modules[moduleIndex]->getModuleInfo().result) {
				for (int i = 0; i < _waterPipes.size(); ++i) {
					if (_waterPipes[i]->isConnectedToModule(moduleIndex) && !_waterPipes[i]->getPipeInfo().result) {

						pipesToUpdate.push(i);  // x pipe needs to be checkeds
						//std::cout << "Pipe " << i << " is now receiving water." << std::endl;
					}
				}
			}
		}

		//Update pipes
		while (!pipesToUpdate.empty()) {

			//get the index of the fist one
			int pipeIndex = pipesToUpdate.front();

			//pop to have the next one ready
			pipesToUpdate.pop();
			//std::cout << "Updating Pipe " << pipeIndex << std::endl; 

			// update x pipe
			waterPassPipe(pipeIndex);

			// if pipe has water, we need to check the paths
			if (_waterPipes[pipeIndex]->getPipeInfo().result) {
				for (int i = 0; i < _waterPath.size(); ++i) {
					// check that pipe and path are conected anf path doesnt have water for it to be updated
					if (_waterPath[i]._whoTocheck.num == pipeIndex && !_waterPath[i]._withWater) {
						pathsToUpdate.push(i);  // x path needs to be checked
						//std::cout << "Path " << i << " is now receiving water." << std::endl;  
					}
				}
			}
		}

		//Update paths
		while (!pathsToUpdate.empty()) {

			//get the index of the fist one
			int pathIndex = pathsToUpdate.front();

			//pop to have the next one ready
			pathsToUpdate.pop();
			//std::cout << "Updating Path " << pathIndex << std::endl;

			// safe the previous water state
			bool previousWaterState = _waterPath[pathIndex]._withWater;

			// update x path
			waterPassPath(pathIndex); 

			// ckeck if path has change water state
			bool waterChanged = (_waterPath[pathIndex]._withWater != previousWaterState);

			// we only readd it if state of water has changed
			if (_waterPath[pathIndex]._withWater && waterChanged) {
				pathsToUpdate.push(pathIndex);  // if now the path has water we added again for it to be checked
				
			}
		}

	}

	// Verificar si el puzzle está resuelto
	Check();
}

void PipePuzzleScene::Win()
{
	
	//resolvedPuzzle(3);
	
	//puts the gloves in scene
	// create entity
	auto gloveEntity = entityManager->addEntity();

	// add transfomr
	auto cubeTransform = entityManager->addComponent<Transform>(
		gloveEntity, Vector2D(900, 580), Vector2D(0, 0), 150, 150, 0
	);
	// add image
	entityManager->addComponent<Image>(gloveEntity, &sdlutils().images().at("gloves"));

	// add area of visualization of the image
	entityManager->addComponent<RectArea2D>(gloveEntity);

	//add click component
	ClickComponent* clk = entityManager->addComponent<ClickComponent>(gloveEntity);
	clk->connect(ClickComponent::JUST_CLICKED, [this]() {
		//add object to inventory and deeactivates visibility of the image;
		});

	//gloveEntity->getMngr()->setActive(gloveEntity, false);

}

void PipePuzzleScene::addToInventory()
{

}

PipePuzzleScene::~PipePuzzleScene()
{
	unload();
}
