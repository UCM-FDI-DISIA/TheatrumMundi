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
        dialogueManager->setScene(this);

        //Puzzle Scene
        room = sr;

        //background + musical notes helpful guide (visual)
        //auto StudyBackground = entityFactory->CreateImageEntity(entityManager, "ShelfBackground1", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);


        //piano buttons
        createPianoButtons();

        //musical score (changes in each round)

        //mirror (changes in each round)

        //UI (back button + inventory + log...)
        //BackButton
        auto _backButton = entityManager->addEntity(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
        entityManager->addComponent<Transform>(_backButton, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0);
        entityManager->addComponent<Image>(_backButton, &sdlutils().images().at("B1"));

        entityManager->addComponent<RectArea2D>(_backButton);
    }
}

void MusicPuzzleScene::refresh()
{
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
    // WIP active de respective part in room2 scene
    cout << "PUZZLE WIN" << endl;
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
        }        
        });

    ClickComponent* clickButtRe = entityManager->getComponent<ClickComponent>(buttRe);
    clickButtRe->connect(ClickComponent::JUST_CLICKED, [this]() {
        //play piano key sound
        if (!solved)
        {
            addNoteToComb(RE);
        }
        });

    ClickComponent* clickButtMi = entityManager->getComponent<ClickComponent>(buttMi);
    clickButtMi->connect(ClickComponent::JUST_CLICKED, [this]() {
        //play piano key sound
        if (!solved)
        {
            addNoteToComb(MI);
        }
        });

    ClickComponent* clickButtFa = entityManager->getComponent<ClickComponent>(buttFa);
    clickButtFa->connect(ClickComponent::JUST_CLICKED, [this]() {
        //play piano key sound
        if (!solved)
        {
            addNoteToComb(FA);
        }
        });

    ClickComponent* clickButtSol = entityManager->getComponent<ClickComponent>(buttSol);
    clickButtSol->connect(ClickComponent::JUST_CLICKED, [this]() {
        //play piano key sound
        if (!solved)
        {
            addNoteToComb(SOL);
        }
        });

    ClickComponent* clickButtLa = entityManager->getComponent<ClickComponent>(buttLa);
    clickButtLa->connect(ClickComponent::JUST_CLICKED, [this]() {
        //play piano key sound
        if (!solved)
        {
            addNoteToComb(LA);
        }
        });

    ClickComponent* clickButtSi = entityManager->getComponent<ClickComponent>(buttSi);
    clickButtSi->connect(ClickComponent::JUST_CLICKED, [this]() {
        //play piano key sound
        if (!solved)
        {
            addNoteToComb(SI);
        }
        });
}



