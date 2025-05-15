#include "ReadInvdesc.h"
#include "../src/json/json.hpp"  // Incluye json.hpp solo aquí
using json = nlohmann::json;  // Alias para json

using namespace std;
ReadInvdesc::ReadInvdesc()
{
	ReadJson();
}

void ReadInvdesc::ReadJson() {
	//Open de json path

	ifstream mJson("../resources/config/InventoryItems.json");
	assert(mJson);
	json items;
	mJson >> items;

	//_descriptions.reserve(items->AsArray().size()); // reserve enough space to avoid resizing
	for (auto& v : items["Inventory"]) {

		std::string key = v["id"];
		std::string txt = v["text"];

		_descriptions[key] = txt;

		
	}
	items.clear();  // Vacía su contenido
	items = json();    // Libera la memoria
	mJson.close();
}

ReadInvdesc::~ReadInvdesc()
{
	_descriptions.clear();
}

string ReadInvdesc::at(string id)
{
	return _descriptions[id];
}
