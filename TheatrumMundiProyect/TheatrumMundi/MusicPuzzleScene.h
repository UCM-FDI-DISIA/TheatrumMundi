#pragma once
#include "ScenePuzzleTemplate.h"
#include "../src/sdlutils/VirtualTimer.h"

class array;
class vector;
class SceneRoomTemplate;
class Sound;

class MusicPuzzleScene : public ScenePuzzleTemplate
{
	friend class Room1Scene;
public:
	MusicPuzzleScene(/*Room1Scene* room1*/);
	~MusicPuzzleScene();
	void init(SceneRoomTemplate* sr) override;
	void unload() override;
	void refresh() override;
	bool Check() override;
	void Win() override;

private:

	//private enum for musical notes
	enum Notes {
		DO,
		RE,
		MI,
		FA,
		SOL,
		LA,
		SI
	};

	std::vector<Notes> _correctComb1 = { SOL, LA, FA}; //correct phase combination
	std::vector<Notes> _correctComb2 = { LA, SI, DO, SI, LA };; //correct phase combination
	std::vector<Notes> _correctComb3 = { LA, FA, LA, FA, LA, FA, SOL };; //correct phase combination

	std::vector<std::vector<Notes>> _correctCombinations; //includes all correctCombinations
	std::vector<Notes> _currentComb; //players combination
	int _phase; //current phase player is in

	entity_t musicalScore; //visual entity that changes with each phase
	entity_t mirrorScore; //visual entity that changes with each phase
	entity_t background; //background

	std::vector<entity_t> displayedNotes; //entities's vector of displayed musical notes
	ecs::entity_t hook;
	std::vector<std::shared_ptr<Sound>> musicalSounds; //vector that saves all musical notes' sounds

	//animation
	bool _animationType; //incorrect/correct combination

	bool _isAnimating = false;
	bool _isStartDelay = false;
	bool _isPlayingSequence = false;

	int _currentNoteIndex = 0;
	int _noteStartTime = 0;
	const int NOTE_DURATION = 900; //ms


	void initializeMusicalSounds(); //initializes musicalSounds vector
	void startSoundSequence(); //starts playing the sound sequence
	bool checkPhaseCombination(); //checks if current combination is correct
	void cleanCombination(); //cleans combinations vectors
	void addNoteToComb(Notes a); //deals with all possibilities when a note is added to the comb
	void changePhase(); //deals with all changes when a phase is changed
	bool playAnimation(bool correct); //plays correct or incorrect animation

	void createPianoButtons(); //creates all sceme notesButtons
	void updateMusicImages(); //updates musical score and mirror

	void updateDisplayedNotes(); //updates displayed musical notes based on buttons clicked
	void cleanDisplayedNotes(); //hides all displayed musical notes
	void initializeDisplayedNotes(); //initialize all displayed musical notes entities
};