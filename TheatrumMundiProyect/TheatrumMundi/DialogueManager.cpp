#include "DialogueManager.h"
#include <fstream>
#include <cassert>
using namespace std;
void DialogueManager::ReadJson(){
	ifstream mJson("../resources/config/dialogues.json");
	assert(mJson);

}

DialogueManager::DialogueManager()
{
}

void DialogueManager::ReadDialogue()
{
}
