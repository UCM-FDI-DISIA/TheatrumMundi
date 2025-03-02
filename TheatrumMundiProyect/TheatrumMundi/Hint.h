#pragma once
#include <string>

class Texture;
class Hint
{

	//Hint class
	//This class is used as a template for the hints that the player can collect
	//It is used to change the description of the hint and the image
protected:
	std::string id; //ID of the hint
	std::string description; //Description of the hint
	Texture* img; //Image of the hint

public:
	Hint(const std::string& _id, const std::string& _desc, Texture* _img);
	~Hint();

	std::string getDescription() { return description; }
	std::string getID() { return id; }

	void setActive(bool _active) { active = _active; }
	bool getActive() const { return active; }
	
private:
	bool active;
	
};
