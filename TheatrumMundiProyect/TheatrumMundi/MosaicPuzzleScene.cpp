#include "MosaicPuzzleScene.h"
#include "PhysicsBodyComponent.h"
#include "SceneRoomTemplate.h"
#include "ClickComponent.h"
#include "RectArea2D.h"
#include "ClickableSpriteComponent.h"
#include "SDLUtils.h"
#include "Image.h"
#include "DialogueManager.h"
#include "Log.h"
#include "TriggerComponent.h"
#include "DragComponent.h"


/// <summary>
/// Create the dragSquares
/// </summary>
void MosaicPuzzleScene::createSquares()
{
	
	for (int i = 0; i < TOTALSQUARES; ++i) {
		auto it = entityFactory->CreateInteractableEntity(entityManager, imgId[i], EntityFactory::RECTAREA, positions[indexPositions[i]], Vector2D(0, 0), SQUAREWIDTH, SQUAREHEIGHT, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		squares.push_back(it);
		it->getMngr()->getComponent<Transform>(it)->setPosPure(positions[indexPositions[i]]);
		it->getMngr()->getComponent<ClickComponent>(it)->connect(ClickComponent::JUST_CLICKED,[this,it]() {
			if (!solved && !squareMoving) {
				square = it->getMngr()->getComponent<Transform>(it);
				MoveSquare();
			}
		});
		
	}
}

MosaicPuzzleScene::MosaicPuzzleScene()
{
	//Assignation of the possitions 
	dialogueManager = new DialogueManager(3);

	int index = 0; //Aux index to set the Y position
	for (int i = 0; i < TOTALSQUARES; ++i) { //Initializate the list of positions

		//Resset the index if we change row
		if (i%4 == 0) {
			index = 0;
		}

		//Assign the positions of the different rows
		if(i < 4) positions.push_back(Vector2D((WIDTHCORRECTOR * Game::Instance()->wscreenScale) + (index * SQUAREWIDTH * Game::Instance()->wscreenScale),(HEIGHTCORRECTOR * Game::Instance()->hscreenScale))); //First row possitions (0-3)
		else if(i < 8) positions.push_back(Vector2D((WIDTHCORRECTOR * Game::Instance()->wscreenScale) + (index * SQUAREWIDTH * Game::Instance()->wscreenScale), (HEIGHTCORRECTOR * Game::Instance()->hscreenScale) + (SQUAREHEIGHT * Game::Instance()->hscreenScale))); //Second row possitions (4-7)
		else if(i < 12)positions.push_back(Vector2D((WIDTHCORRECTOR * Game::Instance()->wscreenScale) + (index * SQUAREWIDTH * Game::Instance()->wscreenScale), (HEIGHTCORRECTOR * Game::Instance()->hscreenScale) + (2 * SQUAREHEIGHT * Game::Instance()->hscreenScale))); //Third row possitions (8-11)
		else positions.push_back(Vector2D((WIDTHCORRECTOR * Game::Instance()->wscreenScale) + (index * SQUAREWIDTH * Game::Instance()->wscreenScale), (HEIGHTCORRECTOR * Game::Instance()->hscreenScale) + (3 * SQUAREHEIGHT * Game::Instance()->hscreenScale))); //Third row possitions (12-15)
		++index;

	}
	freePos = Vector2D((WIDTHCORRECTOR * Game::Instance()->wscreenScale) + (index * SQUAREWIDTH * Game::Instance()->wscreenScale), (HEIGHTCORRECTOR * Game::Instance()->hscreenScale + (3 * SQUAREHEIGHT * Game::Instance()->hscreenScale)));
}

MosaicPuzzleScene::~MosaicPuzzleScene()
{

}

void MosaicPuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted) {
		isStarted = true;
		room = sr;
		squareMoving = false;

#pragma region SpecificEntitiesOfTheScene
		//Background
		auto background = entityFactory->CreateImageEntity(entityManager,"FondoMosaico",Vector2D(0,0),Vector2D(0,0), 1359, 748,0,ecs::grp::BACKGROUND);
		//SquareEntities
		createSquares();
#pragma endregion



#pragma region Buttons
		//Creation of all the buttons
		auto reset = entityFactory->CreateInteractableEntity(entityManager, "resetButton", EntityFactory::RECTAREA, Vector2D(80, 320), Vector2D(0, 0), 183 / 1.5, 172 / 1.5, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		reset->getMngr()->getComponent<ClickComponent>(reset)->connect(ClickComponent::JUST_CLICKED, [this] {
			if(!solved)ResetPuzzle();
		});
#ifdef _DEBUG
		auto resolve = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(400, 0), Vector2D(0, 0), 32, 32, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		resolve->getMngr()->getComponent<ClickComponent>(resolve)->connect(ClickComponent::JUST_CLICKED, [this] {
			Resolve();
		});
#endif // _DEBUG

#pragma endregion

		
#pragma region UI
#pragma region Inventory

		//INVENTORY
		//Invntory Background
		createInventoryUI();
		//Log


#pragma endregion

		//BackButton
		auto _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);

		//Click component Open log button
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, [this, _backButton]()
			{
				inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);
				HideInventoryItems();
				room->GetInventory()->setFirstItem(0);
				auto _backButtonImage = _backButton->getMngr()->getComponent<Image>(_backButton);
				_backButtonImage->setW(_backButton->getMngr()->getComponent<Transform>(_backButton)->getWidth());
				_backButtonImage->setH(_backButton->getMngr()->getComponent<Transform>(_backButton)->getHeight());
				_backButtonImage->setPosOffset(0, 0);
				if (solved) audioManager().playSound(sdlutils().soundEffects().at("Mosaico"));
				Game::Instance()->getSceneManager()->popScene();
			});
		//Log
		dialogueManager->Init(0, entityFactory, entityManager, true, areaLayerManager, "SalaIntermedia1");
		dialogueManager->setScene(this);
		logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager,this);
		if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA)) {
			startDialogue("MOSAICO1_2P");
		}
		else startDialogue("MOSAICO1_1P");
		//startDialogue("PuzzleCuervo");
		dialogueManager->setScene(this);
#pragma endregion

	}
	createInvEntities(sr);
}

/// <summary>
/// Checks if the square which was clicked is next to the free position in case is true, move the square with an animation
/// </summary>
/// <param name="square"></param>
void MosaicPuzzleScene::MoveSquare()
{
	//Saves the originalPos of the square to change the freePos into this
	originalPos = square->getPos();
	int xa = round(square->getPos().getX());
	int xb = round(freePos.getX());
	int ya = round(square->getPos().getY());
	int yb = round(freePos.getY());
	int diffX = xa - xb;
	int diffY = ya - yb;

	//Compare if is in the same X
	if (abs(diffX) < SQUAREWIDTH/2) {
		//If free position is under the square position, move square down
		int diff1 = round(square->getPos().getY()) + round((SQUAREHEIGHT * Game::Instance()->hscreenScale)) - round(freePos.getY());
		int diff2 = round(square->getPos().getY()) - round((SQUAREHEIGHT * Game::Instance()->hscreenScale)) - round(freePos.getY());
		if (abs(diff1) < SQUAREHEIGHT/2) {

			//Evitate to clcik in another square during the animation
			squareMoving = true;

			//Moving Square Animation into Bot
			SDL_AddTimer(30, [](Uint32, void* param) -> Uint32 {
				auto* self = static_cast<decltype(this)>(param);

				//add position in each iteration
				Vector2D aux = Vector2D(self->square->getPos().getX(), self->square->getPos().getY() + round(self->square->getHeight() / 5));
				self->square->setPosPure(aux);

				// if position is bigger or the same ends animation
				if (self->square->getPos().getY() >= self->freePos.getY()) {
					self->squareMoving = false;
					self->square->setPosPure(self->freePos);
					self->freePos.set(self->originalPos);
					if (self->Check()) {
						self->Win();
					}
					return 0;  // stop timer
				}

				// call timer again
				return 30;
				}, this);
		}

		//If free position is over the square position, move square up
		else if (abs(diff2) < SQUAREHEIGHT / 2) {

			//Evitate to clcik in another square during the animation
			squareMoving = true;

			//Moving Square Animation into top
			SDL_AddTimer(30, [](Uint32, void* param) -> Uint32 {
				auto* self = static_cast<decltype(this)>(param);

				//add position in each iteration
				Vector2D aux = Vector2D(round(self->square->getPos().getX()), round(self->square->getPos().getY() - round(self->square->getHeight() / 5)));
				self->square->setPosPure(aux);

				// if position is smaller or the same ends animation
				if (self->square->getPos().getY() <= self->freePos.getY()) {
					self->squareMoving = false;
					self->square->setPosPure(self->freePos);
					self->freePos.set(self->originalPos);
					if (self->Check()) {
						self->Win();
					}
					return 0;  // stop timer
				}

				// call timer again
				return 30;
				}, this);
		}
	}

//END IF

	//Compare if is in the same Y
	else if (abs(diffY) < SQUAREHEIGHT / 2) {
		int diff1 = round(square->getPos().getX()) + round((SQUAREWIDTH * Game::Instance()->wscreenScale)) - round(freePos.getX());
		int diff2 = round(square->getPos().getX()) - round((SQUAREWIDTH * Game::Instance()->wscreenScale)) - round(freePos.getX());
		//If free position is on the right of the square position, move square right
		if (abs(diff1) < SQUAREWIDTH / 2) {

			//Evitate to clcik in another square during the animation
			squareMoving = true;

			//Moving Square Animation into the right
			SDL_AddTimer(30, [](Uint32, void* param) -> Uint32 {
				auto* self = static_cast<decltype(this)>(param);

				//add position in each iteration
				Vector2D aux = Vector2D(round(self->square->getPos().getX()) + round(self->square->getWidth() / 5), round(self->square->getPos().getY()));
				self->square->setPosPure(aux);

				// if position is bigger or the same
				if (self->square->getPos().getX() >= self->freePos.getX()) {
					self->squareMoving = false;
					self->square->setPosPure(self->freePos);
					self->freePos.set(self->originalPos);
					if (self->Check()) {
						self->Win();
					}
					return 0;  // stop timer
				}

				// call timer again
				return 30;
				}, this);
		}

		//If free position is on the left of the square position, move square left
		else if (abs(diff2) < SQUAREHEIGHT / 2) {

			//Evitate to clcik in another square during the animation
			squareMoving = true;

			//Moving Square Animation into the left
			SDL_AddTimer(30, [](Uint32, void* param) -> Uint32 {
				auto* self = static_cast<decltype(this)>(param);


				//add position in each iteration
				Vector2D aux = Vector2D(round(self->square->getPos().getX()) - round(self->square->getWidth() / 5), self->square->getPos().getY());
				self->square->setPosPure(aux);

				// if position is smaller or the same
				if (self->square->getPos().getX() <= self->freePos.getX()) {
					self->squareMoving = false;
					self->square->setPosPure(self->freePos);
					self->freePos.set(self->originalPos);
					if (self->Check()) {
						self->Win();
					}
					return 0;  // stop timer
				}

				// call timer again
				return 30;
				}, this);
		}
	}
}

void MosaicPuzzleScene::ResetPuzzle()
{
	for (int i = 0; i < TOTALSQUARES; ++i) {
		squares[i]->getMngr()->getComponent<Transform>(squares[i])->setPosPure(positions[indexPositions[i]]);
	}
	freePos = Vector2D((WIDTHCORRECTOR * Game::Instance()->wscreenScale) + (3 * SQUAREWIDTH * Game::Instance()->wscreenScale), (HEIGHTCORRECTOR * Game::Instance()->hscreenScale + (3 * SQUAREHEIGHT * Game::Instance()->hscreenScale)));
}

void MosaicPuzzleScene::Resolve()
{
	for (int i = 0; i < TOTALSQUARES; ++i) {
		squares[i]->getMngr()->getComponent<Transform>(squares[i])->setPosPure(positions[i]);
	}
	freePos = Vector2D(750 * Game::Instance()->wscreenScale, 475 * Game::Instance()->hscreenScale);
	if (Check()) Win();
}

/// <summary>
/// Checks if the squares are in the correct possitions
/// </summary>
/// <returns></returns> 
bool MosaicPuzzleScene::Check()
{
	int index = 0;
	bool win = true;
	while (win && index < TOTALSQUARES) {
		auto transform = squares[index]->getMngr()->getComponent<Transform>(squares[index])->getPos();
		if (positions[index].getX() != transform.getX() || positions[index].getY() != transform.getY())  { //if the positions aren't the same return false
			win = false;
		}
		++index;
	}
	return win;
}

/// <summary>
/// Sets the solved boolean to true
/// </summary>
void MosaicPuzzleScene::Win()
{
	//start Dialogue
	if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA)) startDialogue("MOSAICO2_2P");
	else startDialogue("MOSAICO2_1P");
	room->resolvedPuzzle(3);
	setSolved(true);
}
