#pragma once
#include <fstream>
#include <cassert>
#include <unordered_map>
#include <deque>

class ReadInvdesc
{
private:
	std::unordered_map<std::string, std::string> _descriptions;
public:
	ReadInvdesc();
	void ReadJson();
	~ReadInvdesc();
	std::string at(std::string id);
	
};

