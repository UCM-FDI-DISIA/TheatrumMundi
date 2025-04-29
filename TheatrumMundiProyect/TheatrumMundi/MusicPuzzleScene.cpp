#include "MusicPuzzleScene.h"

#include "DialogueManager.h"

#include "../src/utils/Vector2D.h";

#include "EntityFactory.h"

#include <array>

#include "../src/Components/ClickComponent.h"

#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "../src/ecs/Manager.h"

#include "../src/components/Image.h"

#include <vector>

#include "iostream"

#include "../src/components/RectArea2D.h"

#include "../src/components/TriggerComponent.h"

#include "../src/Components/Transform.h"

#include "../src/Components/LogComponent.h"

#include "Log.h"

#include "Room1.h"

#include "SceneRoomTemplate.h"

using namespace std;

MusicPuzzleScene::MusicPuzzleScene(): _phase(0)
{
    _correctCombinations.push_back(_correctComb1);
    _correctCombinations.push_back(_correctComb2);
    _correctCombinations.push_back(_correctComb3);

    //debug correct comb
    cout << "CORRECT COMB:" << endl;
    for (auto a : _correctCombinations[_phase])
    {
        cout << a << " ";
    }
    cout << endl;

    //debug currentComb
    cout << "CURRENT COMB:";
    for (auto a : _currentComb)
    {
        cout << a << " ";
    }
    cout << endl;
}

MusicPuzzleScene::~MusicPuzzleScene()
{
}

void MusicPuzzleScene::init(SceneRoomTemplate* sr)
{
    if (!isStarted)
    {
        isStarted = true;
        //Register scene in dialogue manager
        //dialogueManager->setScene(this);

        //Puzzle Scene
        room = sr;

        //background + musical notes helpful guide (visual)

        //musicalScore + mirror (changes in each phase)
        musicalScore = entityFactory->CreateImageEntity(entityManager, "fase1Partitura", Vector2D(800, 0), Vector2D(0, 0), 1349 / 3, 748 / 3, 0, ecs::grp::DEFAULT);
        mirror = entityFactory->CreateImageEntity(entityManager, "fase1Espejo", Vector2D(0, 300), Vector2D(0, 0), 1349 / 3, 748 / 3, 0, ecs::grp::DEFAULT);

        //displayed notes
        initializeDisplayedNotes();
        //cleanDisplayedNotes();

        //piano buttons
        createPianoButtons();


#pragma region UI


#pragma region Inventory

        //INVENTORY
        //Invntory Background
        auto InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1150, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::DEFAULT);
        entityManager->setActive(InventoryBackground, false);

        auto upButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
        entityManager->setActive(upButton, false);

        auto downButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 748 - 268 / 3 - 20), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
        entityManager->setActive(downButton, false);

        //InventoryButton
        auto inventoryButton = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(60 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
        ClickComponent* invOpen = entityManager->addComponent<ClickComponent>(inventoryButton);
        invOpen->connect(ClickComponent::JUST_CLICKED, [this, sr, InventoryBackground, upButton, downButton, inventoryButton]() //Lamda function
            {
                //AudioManager::Instance().playSound(buttonSound);
                sr->GetInventory()->setActive(!sr->GetInventory()->getActive());  // Toggle the inventory

                // If the inventory is active, activate the items
                if (sr->GetInventory()->getActive()) {
                    entityManager->setActive(InventoryBackground, true);

                    inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(925);
                    entityManager->setActive(downButton, true);
                    entityManager->setActive(upButton, true);

                    for (int i = 0; i < sr->GetInventory()->getItemNumber(); ++i) {
                        invObjects[i]->getMngr()->setActive(invObjects[i], true);
                    }
                }
                else {
                    entityManager->setActive(InventoryBackground, false);
                    entityManager->setActive(InventoryBackground, false);
                    entityManager->setActive(downButton, false);
                    entityManager->setActive(upButton, false);
                    inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);

                    for (int i = 0; i < sr->GetInventory()->getItemNumber(); ++i) {
                        invObjects[i]->getMngr()->setActive(invObjects[i], false);
                    }
                }
            });

        ClickComponent* UPbuttonInventoryClick = entityManager->getComponent<ClickComponent>(upButton);
        UPbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, /*buttonSound,*/ upButton, sr]() {

            //AudioManager::Instance().playSound(buttonSound);
            sr->scrollInventory(-1);
            });

        ClickComponent* DOWNbuttonInventoryClick = entityManager->getComponent<ClickComponent>(downButton);
        DOWNbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, /*buttonSound,*/ downButton, sr]() {

            //AudioManager::Instance().playSound(buttonSound);
            sr->scrollInventory(1);
            });
        //Log


#pragma endregion

        //BackButton
        auto _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);

        //Click component Open log button
        ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
        clkOpen->connect(ClickComponent::JUST_CLICKED, [this, inventoryButton, InventoryBackground, downButton, upButton, _backButton]()
            {
                inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);
                HideInventoryItems(InventoryBackground, downButton, upButton, room);
                room->GetInventory()->setFirstItem(0);
                auto _backButtonImage = _backButton->getMngr()->getComponent<Image>(_backButton);
                _backButtonImage->setW(_backButton->getMngr()->getComponent<Transform>(_backButton)->getWidth());
                _backButtonImage->setH(_backButton->getMngr()->getComponent<Transform>(_backButton)->getHeight());
                _backButtonImage->setPosOffset(0, 0);
                Game::Instance()->getSceneManager()->popScene();
            });

        //Log
        //dialogueManager->Init(0, entityFactory, entityManager, true, areaLayerManager, "SalaIntermedia1");
       // Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);
        //startDialogue("Puerta");

#pragma endregion

    }
    //createInvEntities(sr);
}


void MusicPuzzleScene::unload()
{
}

bool MusicPuzzleScene::Check()
{
    if (_phase == _correctCombinations.size() - 1)
    {
        //puzzle win
        Win();
        return true;
    }
    else
    {
        return false;
    }
}

void MusicPuzzleScene::Win()
{
    solved = true;
    room->resolvedPuzzle(4);
}

bool MusicPuzzleScene::checkPhaseCombination()
{
    auto _correctComb = _correctCombinations[_phase]; //correct combination based on current phase
    int index = 0;
    bool correctAns = true;

    while (index < _correctComb.size() && correctAns)
    {
        if (_currentComb[index] != _correctComb[index])
        {
            correctAns = false;
        }
        else
        {
            index++;
        }
    }
    
    return correctAns;
}

void MusicPuzzleScene::cleanCombination()
{
    _currentComb.clear();
}

void MusicPuzzleScene::addNoteToComb(Notes pressedNote)
{
    //debug button pressed
    cout << "BUTTON PRESSED: " << pressedNote << endl;

    //save button pressed on currentComb
    _currentComb.push_back(pressedNote);

    //debug currentComb
    cout << "CURRENT COMB:";
    for (auto a : _currentComb)
    {
        cout << a << " ";
    }
    cout << endl;
    
    //if currentComb is full check if its correct
    if (_currentComb.size() >= _correctCombinations[_phase].size())
    {
        //check
        if (checkPhaseCombination()) //if its correct
        {
            if (!Check())
            {
                changePhase();
            }
        }
        else //wrong combination
        {
            //play animation
            playAnimation(false);

            //clean current comb
            cleanCombination();

            //debug currentComb
            cout << "CURRENT COMB:";
            for (auto a : _currentComb)
            {
                cout << a << " ";
            }
            cout << endl;

            //reset image musical score if needed
        }
    }
}

void MusicPuzzleScene::changePhase()
{
    //play animation
    playAnimation(true);

    //clean currentComb
    cleanCombination();

    //increment phase
    _phase++;

    //change musical score & mirror
    updateMusicImages();

    //reset musical notes
    cleanDisplayedNotes();

    //debugs
    cout << "PHASE: " << _phase << endl;
    cout << "CORRECT COMB:";
    for (auto a : _correctCombinations[_phase])
    {
        cout << a << " ";
    }
    cout << endl;

    //debug currentComb
    cout << "CURRENT COMB:";
    for (auto a : _currentComb)
    {
        cout << a << " ";
    }
    cout << endl;
}

void MusicPuzzleScene::playAnimation(bool correct)
{
    //no buttons can be pressed, the current comb plays on audio and each note appears on screen
    //-->insert animation


    if (correct)
    {
        //if currentComb was correct a joyful sound is played after
        //joyful sound
    }
    else
    {
        //if currentComb was incorrect a bad sound is played after
        //bad sound
    }
}

void MusicPuzzleScene::updateMusicImages()
{
    auto musicalScoreIm = entityManager->getComponent<Image>(musicalScore);
    auto mirrorIm = entityManager->getComponent<Image>(mirror);

    switch (_phase)
    {
    case 0:
        musicalScoreIm->setTexture(&sdlutils().images().at("fase1Partitura"));
        mirrorIm->setTexture(&sdlutils().images().at("fase1Espejo"));
        break;
    case 1:
        musicalScoreIm->setTexture(&sdlutils().images().at("fase2Partitura"));
        mirrorIm->setTexture(&sdlutils().images().at("fase2Espejo"));
        break;
    case 2:
        musicalScoreIm->setTexture(&sdlutils().images().at("fase3Partitura"));
        mirrorIm->setTexture(&sdlutils().images().at("fase3Espejo"));
        break;
    default:
        break;
    }
}

void MusicPuzzleScene::updateDisplayedNotes()
{
    for (size_t i = 0; i < _currentComb.size(); ++i)
    {
        string texture;

        if (_currentComb[i] == Notes::DO)
        {
            texture = "Do";
        }
        else if (_currentComb[i] == Notes::SI)
        {
            texture = "Si";
            //cambiar posicion
        }
        else
        {
            texture = "NotaBasica";
        }
        
        auto noteIm = entityManager->getComponent<Image>(displayedNotes[i]);
        noteIm->setTexture(&sdlutils().images().at(texture));

        entityManager->setActive(displayedNotes[i], true);

        /*
        // Crea la imagen en esa posición
        auto musicalNotes = entityFactory->CreateImageEntity(
            entityManager,
            texture,
            pos,
            Vector2D(0, 0),     // velocidad inicial 0
            1349 / 3,           // ancho de la imagen
            748 / 3,            // alto de la imagen
            0,                  // rotación
            ecs::grp::DEFAULT   // grupo
        );*/

    }

    // Ocultar notas sobrantes si _currentComb ha disminuido
    for (size_t i = _currentComb.size(); i < displayedNotes.size(); ++i)
    {
        entityManager->setActive(displayedNotes[i], false);
    }
}

void MusicPuzzleScene::cleanDisplayedNotes()
{
    //hide displayed notes vector
    for (auto a : displayedNotes)
    {
        entityManager->setActive(a, false);
    }
}

void MusicPuzzleScene::initializeDisplayedNotes()
{
    // Suponemos que currentComb es el vector que contiene la combinación musical actual
    const int spacingX = 100; // espacio entre cada imagen en el eje X
    const int baseX = 100;    // posición inicial X
    const int posY = 300;     // posición fija en Y

    for (size_t i = 0; i < Notes::SI + 1; ++i)
    {
        // Calcula la posición horizontal
        Vector2D pos(baseX + i * spacingX, posY);
        
        // Crea la imagen en esa posición
        auto musicalNotes = entityFactory->CreateImageEntity(
            entityManager,
            "NotaBasica",
            pos,
            Vector2D(0, 0),     // velocidad inicial 0
            1349 / 3,           // ancho de la imagen
            748 / 3,            // alto de la imagen
            0,                  // rotación
            ecs::grp::DEFAULT   // grupo
        );

        // Puedes almacenar el entityID si luego necesitas manipularlos
        displayedNotes.push_back(musicalNotes);
    }
}

void MusicPuzzleScene::createPianoButtons()
{
    auto buttDo = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(518, 430), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
    auto buttRe = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(562, 430), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
    auto buttMi = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(606, 430), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
    auto buttFa = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(650, 430), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
    auto buttSol = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(694, 430), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
    auto buttLa = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(738, 430), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
    auto buttSi = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(782, 430), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
    

    ClickComponent* clickButtDo = entityManager->getComponent<ClickComponent>(buttDo);
    clickButtDo->connect(ClickComponent::JUST_CLICKED, [this]() {
        //play piano key sound
        if (!solved)
        {
            addNoteToComb(DO);
            updateDisplayedNotes();
        }        
        });

    ClickComponent* clickButtRe = entityManager->getComponent<ClickComponent>(buttRe);
    clickButtRe->connect(ClickComponent::JUST_CLICKED, [this]() {
        //play piano key sound
        if (!solved)
        {
            addNoteToComb(RE);
            updateDisplayedNotes();
        }
        });

    ClickComponent* clickButtMi = entityManager->getComponent<ClickComponent>(buttMi);
    clickButtMi->connect(ClickComponent::JUST_CLICKED, [this]() {
        //play piano key sound
        if (!solved)
        {
            addNoteToComb(MI);
            updateDisplayedNotes();
        }
        });

    ClickComponent* clickButtFa = entityManager->getComponent<ClickComponent>(buttFa);
    clickButtFa->connect(ClickComponent::JUST_CLICKED, [this]() {
        //play piano key sound
        if (!solved)
        {
            addNoteToComb(FA);
            updateDisplayedNotes();
        }
        });

    ClickComponent* clickButtSol = entityManager->getComponent<ClickComponent>(buttSol);
    clickButtSol->connect(ClickComponent::JUST_CLICKED, [this]() {
        //play piano key sound
        if (!solved)
        {
            addNoteToComb(SOL);
            updateDisplayedNotes();
        }
        });

    ClickComponent* clickButtLa = entityManager->getComponent<ClickComponent>(buttLa);
    clickButtLa->connect(ClickComponent::JUST_CLICKED, [this]() {
        //play piano key sound
        if (!solved)
        {
            addNoteToComb(LA);
            updateDisplayedNotes();
        }
        });

    ClickComponent* clickButtSi = entityManager->getComponent<ClickComponent>(buttSi);
    clickButtSi->connect(ClickComponent::JUST_CLICKED, [this]() {
        //play piano key sound
        if (!solved)
        {
            addNoteToComb(SI);
            updateDisplayedNotes();
        }
        });
}



