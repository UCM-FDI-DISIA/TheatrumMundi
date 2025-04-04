#pragma once
#include "ScenePuzzleTemplate.h"

class array;
class vector;
class SceneRoomTemplate;


class MusicPuzzleScene : public ScenePuzzleTemplate
{
	friend class Room1Scene;
public:
	MusicPuzzleScene(/*Room1Scene* room1*/);
	~MusicPuzzleScene();
	void init(SceneRoomTemplate* sr) override;
	void unload() override;
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

	std::vector<Notes> _correctComb1 = { DO, RE, MI}; //correct phase combination
	std::vector<Notes> _correctComb2 = { DO, RE, MI, FA, SOL };; //correct phase combination
	std::vector<Notes> _correctComb3 = { DO, RE, MI, FA, SOL, LA, SI };; //correct phase combination

	std::vector<std::vector<Notes>> _correctCombinations; //includes all correctCombinations
	std::vector<Notes> _currentComb; //players combination
	int _phase; //current phase player is in

	bool checkPhaseCombination(); //checks if current combination is correct
	void cleanCombination(); //cleans combinations vectors
	void addNoteToComb(Notes a); //deals with all possibilities when a note is added to the comb
	void changePhase(); //deals with all changes when a phase is changed
	void playAnimation(bool correct); //plays correct or incorrect animation

	void createPianoButtons(); //creates all sceme notesButtons

};