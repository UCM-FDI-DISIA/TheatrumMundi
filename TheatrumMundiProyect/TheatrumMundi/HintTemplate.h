#pragma once
#include <string>

using namespace std;

class Image;
class HintTemplate
{

	//Hint class
	//This class is used as a template for the hints that the player can collect
	//It is used to change the description of the hint and the image
protected:
	string description;
	Image* image;

public:
	HintTemplate();
	//void init();
	virtual void render() = 0;
	HintTemplate(const string& _desc, Image* _im);
	~HintTemplate();
	
private:

	
};
