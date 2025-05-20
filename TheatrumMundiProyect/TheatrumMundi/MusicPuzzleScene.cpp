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

#include "AudioManager.h"

using namespace std;

MusicPuzzleScene::MusicPuzzleScene(): _phase(0)
{
    dialogueManager = new DialogueManager(3);
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

        //Puzzle Scene
        room = sr;

        //initialize musical sounds
        initializeMusicalSounds();

        //background
        background = entityFactory->CreateImageEntity(entityManager, "fondoOrgano", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);

        //mirror
        auto mirror = entityFactory->CreateImageEntity(entityManager, "espejo", Vector2D(175, 0), Vector2D(0, 0), 345/1.5, 707/1.5, 0, ecs::grp::DEFAULT);
        entityManager->setActive(mirror, false);

        //instructions
        auto instructions = entityFactory->CreateImageEntity(entityManager, "instrucciones", Vector2D(940, 0), Vector2D(0, 0), 354 / 1.5, 707 / 1.5, 0, ecs::grp::DEFAULT);
        entityManager->setActive(instructions, false);

        //doorA
        auto doorA = entityFactory->CreateInteractableEntity(entityManager, "puertaAMusical", EntityFactory::RECTAREA, Vector2D(394, 9), Vector2D(0, 0), 328 / 1.5, 667 / 1.5, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);

        ClickComponent* clickDoorA = entityManager->getComponent<ClickComponent>(doorA);
        clickDoorA->connect(ClickComponent::JUST_CLICKED, [this, mirror, doorA]() {
            //show mirror
            entityManager->setActive(mirror, true);

            //hide doorA
            entityManager->setActive(doorA, false);
        });

        //doorB
        auto doorB = entityFactory->CreateInteractableEntity(entityManager, "puertaBMusical", EntityFactory::RECTAREA, Vector2D(746, 11), Vector2D(0, 0), 328 / 1.5, 667 / 1.5, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);

        ClickComponent* clickDoorB = entityManager->getComponent<ClickComponent>(doorB);
        clickDoorB->connect(ClickComponent::JUST_CLICKED, [this, instructions, doorB]() {
            //show mirror
            entityManager->setActive(instructions, true);

            //hide doorA
            entityManager->setActive(doorB, false);
            });

        //musicalScore + mirror (changes in each phase)
        musicalScore = entityFactory->CreateImageEntity(entityManager, "fase1Partitura", Vector2D(545, 330), Vector2D(0, 0), 403 / 1.5, 171 / 1.5, 0, ecs::grp::DEFAULT);
        mirrorScore = entityFactory->CreateImageEntity(entityManager, "fase1Espejo", Vector2D(230, 100), Vector2D(0, 0), 208/1.5, 395/1.5, 0, ecs::grp::DEFAULT);

        //displayed notes
        initializeDisplayedNotes();
        cleanDisplayedNotes();

        //piano buttons
        createPianoButtons();

        //Create Hook
        int variant = Game::Instance()->getDataManager()->GetRoomVariant(1);
        if (variant != 2) {
            hook = entityFactory->CreateInteractableEntity(entityManager, "Varilla", EntityFactory::RECTAREA, Vector2D(530, 680), Vector2D(0, 0), 864 / 3, 207 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
            entityManager->getComponent<ClickComponent>(hook)->connect(ClickComponent::JUST_CLICKED, [this, sr]() {
                entityManager->setActive(hook, false);
                AddInvItem("VarillaInventario", sdlutils().invDescriptions().at("Varilla"), sr->GetInventory()->setPosition(), sr);
                entityManager->removeEntity(hook);
                });
            entityManager->setActive(hook, false);
        }
        //black musical notes
        entityFactory->CreateImageEntity(entityManager, "teclasNegras", Vector2D(430, 473), Vector2D(0, 0), 739 / 1.5, 44 / 1.5, 0, ecs::grp::BACKGROUND);

#pragma region UI


#pragma region Inventory

        //INVENTORY
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
                Game::Instance()->getSceneManager()->popScene();
            });

        //Log
        dialogueManager->setScene(this);
        dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, "SalaIntermedia1");
        logbtn =Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);
        if(Game::Instance()->getDataManager()->GetCharacterState(KEISARA))  startDialogue("ORGANO_2P");
        else
        {
            startDialogue("ORGANO_1P");
        }

#pragma endregion

    }
    createInvEntities(sr);
}


void MusicPuzzleScene::unload()
{
}

void MusicPuzzleScene::refresh()
{
    if (_isStartDelay)
    {
        if (sdlutils().virtualTimer().currRealTime() - _noteStartTime >= NOTE_DURATION)
        {
            //when initial animation delay is finished sound sequence begins
            _isStartDelay = false;
            startSoundSequence();
            _noteStartTime = sdlutils().virtualTimer().currRealTime(); //restart noteStartTime for sound sequence
        }
        return;
    }

    if (_isPlayingSequence)
    {
        int currTime = sdlutils().virtualTimer().currRealTime();
        
        if (currTime - _noteStartTime >= NOTE_DURATION - 200)
        {
            _currentNoteIndex++;

            if (_currentNoteIndex < _currentComb.size()) //if there is more musical notes to play...
            {
                audioManager().playSound(musicalSounds[_currentComb[_currentNoteIndex]]);
                _noteStartTime = currTime;
            }
            else if (_currentNoteIndex == _currentComb.size()) //if all musical notes from comb have been played...
            {
                std::shared_ptr<Sound> correctSound;
                
                //play correct or incorrect sound
                if(_animationType) correctSound = sdlutils().soundEffects().at("correcto");
                else correctSound = sdlutils().soundEffects().at("incorrecto");
                
                audioManager().playSound(correctSound);
                _noteStartTime = currTime;
            }
            else
            {
                //end of sound animation
                _isPlayingSequence = false;
                _isAnimating = false;

                //reactive all scene buttons
                if (_animationType)
                {
                    if (!solved && Check())
                    {
                        int variant = Game::Instance()->getDataManager()->GetRoomVariant(1);
                        if(variant != 2) hook->getMngr()->setActive(hook, true);
                        //puzzle win
                        Win();
                        
                        Image* img = entityManager->getComponent<Image>(background);
                        img->setTexture(&sdlutils().images().at("fondoMusicalCajon"));

                        //recompensas
                    }
                }
                else
                {
                    cleanCombination();
                    cleanDisplayedNotes();
                }
            }
        }
        else
        {
            return;
        }
    }
}

bool MusicPuzzleScene::Check()
{
    if (_phase == _correctCombinations.size() - 1) return true;
    else
    {
        changePhase();
        return false;
    }
}

void MusicPuzzleScene::Win()
{
    room->resolvedPuzzle(4);
    setSolved(true);
}

void MusicPuzzleScene::initializeMusicalSounds()
{
    musicalSounds.push_back(sdlutils().soundEffects().at("DoSound"));
    musicalSounds.push_back(sdlutils().soundEffects().at("ReSound"));
    musicalSounds.push_back(sdlutils().soundEffects().at("MiSound"));
    musicalSounds.push_back(sdlutils().soundEffects().at("FaSound"));
    musicalSounds.push_back(sdlutils().soundEffects().at("SolSound"));
    musicalSounds.push_back(sdlutils().soundEffects().at("LaSound"));
    musicalSounds.push_back(sdlutils().soundEffects().at("SiSound"));
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
        _isAnimating = true;
        //check
        if (checkPhaseCombination()) //if its correct
        {
            playAnimation(true);
        }
        else //wrong combination
        {
            playAnimation(false);            
        }
    }
}

void MusicPuzzleScene::changePhase()
{
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

bool MusicPuzzleScene::playAnimation(bool correct)
{
    _animationType = correct;

    _isAnimating = true;
    _isStartDelay = true; // little delay to let last musical note play accordingly
    _noteStartTime = sdlutils().virtualTimer().currRealTime();

    return true;
}

void MusicPuzzleScene::startSoundSequence()
{
    _isPlayingSequence = true;
    _currentNoteIndex = 0;
    _noteStartTime = sdlutils().virtualTimer().currRealTime();

    //plays first note on combination
    audioManager().playSound(musicalSounds[_currentComb[_currentNoteIndex]]);
}

void MusicPuzzleScene::updateMusicImages()
{
    auto musicalScoreIm = entityManager->getComponent<Image>(musicalScore);
    auto mirrorIm = entityManager->getComponent<Image>(mirrorScore);

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

        auto noteIm = entityManager->getComponent<Image>(displayedNotes[i]);
        auto noteTr = entityManager->getComponent<Transform>(displayedNotes[i]);
        
        int yPos = 385;
        int xPos = 0;
        float width = 25/1.2;
        float height = 65/1.2;

        int spacingX = 32 - _phase * 7.5;
        int baseX = 682;

        switch (_currentComb[i])
        {
        case Notes::DO:
            texture = "Do";
            xPos = baseX + i * spacingX;
            break;
        case Notes::RE:
            texture = "NotaBasica";
            xPos = baseX + i * spacingX;
            yPos = 380.0f;
            break;
        case Notes::MI:
            texture = "NotaBasica";
            xPos = baseX + i * spacingX;
            yPos = 373.0f;
            break;
        case Notes::FA:
            texture = "NotaBasica";
            xPos = baseX + i * spacingX;
            yPos = 363.0f;
            break;
        case Notes::SOL:
            texture = "NotaBasica";
            xPos = baseX + i * spacingX;
            yPos = 355.0f;
            break;
        case Notes::LA:
            texture = "NotaBasica";
            xPos = baseX + i * spacingX;
            yPos = 345.0f;
            break;
        case Notes::SI:
            texture = "Si";
            xPos = baseX + i * spacingX;
            yPos = 370.0f;
            break;
        default:
            texture = "NotaBasica";
            yPos = 395.0f;
            break;
        }

        
        
        noteIm->setTexture(&sdlutils().images().at(texture));
        noteTr->setPos(Vector2D(xPos, yPos));
        noteTr->setWidth(width);
        noteTr->setHeight(height);

        noteIm->setW(width*Game::Instance()->wscreenScale);
        noteIm->setH(height*Game::Instance()->wscreenScale);

        entityManager->setActive(displayedNotes[i], true);


    }

    // Hide non used notes if needed
    for (size_t i = _currentComb.size(); i < displayedNotes.size(); ++i)
    {
        entityManager->setActive(displayedNotes[i], false);
    }
}

void MusicPuzzleScene::cleanDisplayedNotes()
{
    //hide displayed notes
    for (auto a : displayedNotes)
    {
        entityManager->setActive(a, false);
    }
}

void MusicPuzzleScene::initializeDisplayedNotes()
{
    const int spacingX = 17; //space between notes
    const int baseX = 680; //initial x
    const int posY = 395; //initial y

    for (size_t i = 0; i < Notes::SI + 1; ++i)
    {
        //note position
        Vector2D pos(baseX + i * spacingX, posY);
        
        // create image entity
        auto musicalNotes = entityFactory->CreateImageEntity(entityManager, "NotaBasica", pos, Vector2D(0, 0), 210/6.5, 304/6.5, 0, ecs::grp::DEFAULT);
        displayedNotes.push_back(musicalNotes);
    }
}

void MusicPuzzleScene::createPianoButtons()
{
    auto buttDo = entityFactory->CreateInteractableEntity(entityManager, "doTecla", EntityFactory::RECTAREA, Vector2D(523, 479), Vector2D(0, 0), 97/1.5, 134 / 1.5, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
    auto buttRe = entityFactory->CreateInteractableEntity(entityManager, "reTecla", EntityFactory::RECTAREA, Vector2D(560, 480), Vector2D(0, 0), 83 / 1.5, 132 / 1.5, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
    auto buttMi = entityFactory->CreateInteractableEntity(entityManager, "miTecla", EntityFactory::RECTAREA, Vector2D(598, 480), Vector2D(0, 0), 70 / 1.5, 132 / 1.5, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
    auto buttFa = entityFactory->CreateInteractableEntity(entityManager, "faTecla", EntityFactory::RECTAREA, Vector2D(633, 481), Vector2D(0, 0), 69 / 1.5, 131 / 1.5, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
    auto buttSol = entityFactory->CreateInteractableEntity(entityManager, "solTecla", EntityFactory::RECTAREA, Vector2D(668, 482), Vector2D(0, 0), 73 / 1.5, 130 / 1.5, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
    auto buttSi = entityFactory->CreateInteractableEntity(entityManager, "siTecla", EntityFactory::RECTAREA, Vector2D(730, 480), Vector2D(0, 0), 99 / 1.5, 132 / 1.5, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
    auto buttLa = entityFactory->CreateInteractableEntity(entityManager, "laTecla", EntityFactory::RECTAREA, Vector2D(703, 481), Vector2D(0, 0), 82 / 1.5, 131 / 1.5, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
   
    ClickComponent* clickButtDo = entityManager->getComponent<ClickComponent>(buttDo);
    clickButtDo->connect(ClickComponent::JUST_CLICKED, [this]() {
        if (!_isAnimating)
        {
            //play organ key sound
            audioManager().playSound(musicalSounds[DO]);

            if (!solved)
            {
                addNoteToComb(DO);
                updateDisplayedNotes();
            }
        }
              
    });

    ClickComponent* clickButtRe = entityManager->getComponent<ClickComponent>(buttRe);
    clickButtRe->connect(ClickComponent::JUST_CLICKED, [this]() {
        
        if (!_isAnimating)
        {
            //play organ key sound
            audioManager().playSound(musicalSounds[RE]);

            if (!solved)
            {
                addNoteToComb(RE);
                updateDisplayedNotes();
            }
        }
        
        });

    ClickComponent* clickButtMi = entityManager->getComponent<ClickComponent>(buttMi);
    clickButtMi->connect(ClickComponent::JUST_CLICKED, [this]() {
        
        if (!_isAnimating)
        {
            //play organ key sound
            audioManager().playSound(musicalSounds[MI]);

            if (!solved)
            {
                addNoteToComb(MI);
                updateDisplayedNotes();
            }
        }
        
        });

    ClickComponent* clickButtFa = entityManager->getComponent<ClickComponent>(buttFa);
    clickButtFa->connect(ClickComponent::JUST_CLICKED, [this]() {
        
        if (!_isAnimating)
        {
            //play organ key sound
            audioManager().playSound(musicalSounds[FA]);

            if (!solved)
            {
                addNoteToComb(FA);
                updateDisplayedNotes();
            }
        }
        
        });

    ClickComponent* clickButtSol = entityManager->getComponent<ClickComponent>(buttSol);
    clickButtSol->connect(ClickComponent::JUST_CLICKED, [this]() {
        
        if (!_isAnimating)
        {
            //play organ key sound
            audioManager().playSound(musicalSounds[SOL]);

            if (!solved)
            {
                addNoteToComb(SOL);
                updateDisplayedNotes();
            }
        }
        });

    ClickComponent* clickButtLa = entityManager->getComponent<ClickComponent>(buttLa);
    clickButtLa->connect(ClickComponent::JUST_CLICKED, [this]() {
        
        if (!_isAnimating)
        {
            //play organ key sound
            audioManager().playSound(musicalSounds[LA]);

            if (!solved)
            {
                addNoteToComb(LA);
                updateDisplayedNotes();
            }
        }
        });

    ClickComponent* clickButtSi = entityManager->getComponent<ClickComponent>(buttSi);
    clickButtSi->connect(ClickComponent::JUST_CLICKED, [this]() {
        
        if (!_isAnimating)
        {
            //play organ key sound
            audioManager().playSound(musicalSounds[SI]);

            if (!solved)
            {
                addNoteToComb(SI);
                updateDisplayedNotes();
            }
        }
        });
}



