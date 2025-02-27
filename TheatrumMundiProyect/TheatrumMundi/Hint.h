#pragma once
#include <string>

class Image;
class Hint
{

	//Hint class
	//This class is used as a template for the hints that the player can collect
	//It is used to change the description of the hint and the image
protected:
	std::string id; //ID of the hint
	std::string description; //Description of the hint
	Image* img; //Image of the hint

public:
	Hint(const std::string& _id, const std::string& _desc, Image* _img);
	~Hint();

	std::string getDescription() { return description; }
	std::string getID() { return id; }
	
private:

	
};
