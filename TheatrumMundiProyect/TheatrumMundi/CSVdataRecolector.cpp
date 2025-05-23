#include "CSVdataRecolector.h"
#include <fstream>
#include "Game.h"
#include "SDLUtils.h"
#include <iterator>
#include <SDL.h>
#include "InputHandler.h"
CSVdataRecolector::CSVdataRecolector()
{
	clickdataTimeList = std::list<ClickDataTimeLine>();
	clicksSummaryMap = std::unordered_map<std::string,ClicksDataSummary>();
	SceneSummaryMap = std::unordered_map<std::string,SceneData>();
	iniSceneTime = 0;
	lastmousepos = Vector2D(0, 0);
	totalmouseMovement = Vector2D(0, 0);
	numtester = 1;
}

CSVdataRecolector::~CSVdataRecolector()
{
}

void CSVdataRecolector::AddEntry(const std::string& _id, const std::string _scene)
{
	std::string aux_id = _id;
	if (_id == "B1" || _id == "B2" || _id == "B3" || _id == "B4" || _id == "B5" || _id == "B6" || _id == "B7") {
		aux_id = getUIBtn(_id);
	}
	//clickDataTimeLine
	ClickDataTimeLine aux = { aux_id,_scene,sdlutils().currRealTime()};
	clickdataTimeList.push_back(aux);
	//ClickDataSummary
	 std::string const auxid = aux_id + _scene;
	 auto a = clicksSummaryMap.find(auxid);
	 
	 if (a != clicksSummaryMap.end()) {
		 a.operator*().second.numclicks++;
	}
	 else {
		 clicksSummaryMap.insert({ auxid,{aux_id,_scene,1}});
	 }
	 //add the click to the sceneSummary
	 SceneSummaryMap[_scene].totalclicks++;

	 //Add de movement of the mosue
	 Vector2D auxvec = Vector2D(ih().getMousePos().first*Game::Instance()->wscreenScale, ih().getMousePos().second*Game::Instance()->hscreenScale);
	 Vector2D auxvec2 = auxvec;
	 auxvec.setX(std::abs(auxvec.getX()-lastmousepos.getX()));
	 auxvec.setY(std::abs(auxvec.getY() - lastmousepos.getY()));
	 lastmousepos;
	 totalmouseMovement += auxvec;
	 
}

void CSVdataRecolector::enterScene(const std::string scene)
{
	auto a = SceneSummaryMap.find(scene);
	if (a != SceneSummaryMap.end()) {
		a.operator*().second.totaltimesinscene++;
	}
	else {
		SceneSummaryMap.insert({ scene,{scene,0,1,0} });
	}
	iniSceneTime = sdlutils().currRealTime();
}

void CSVdataRecolector::exitScene(const std::string scene)
{
	SceneSummaryMap[scene].passtime += sdlutils().currRealTime() - iniSceneTime;

}

void CSVdataRecolector::safeData()
{
	std::string filename = "../PlanDePruebas/Tester" + std::to_string(numtester);
	filename += ".csv";
	std::fstream archive;
	archive.open(filename, std::ios::out | std::ios::app);
	
	
	archive<< "Resumen de click en botones"<<"\n";
	archive << "ID" << ";" << "Scene" << ";" << "Numero de click" << "\n";
	for (auto a : clicksSummaryMap) {
		archive << a.second.id << ";" << a.second.scene << ";" << a.second.numclicks << "\n";
	}
	archive << "\n" << "\n";
	archive << "Tiempo en las escenas" << "\n";
	archive << "Scene" << ";" << "Tiempo" << ";" << "Veces en la escena"<<";" << "totalClicks" << "\n";
	float totaltimaingame = 0;
	for (auto a : SceneSummaryMap) {
		archive << a.second.scene << ";" << a.second.passtime / 1000.0 << ";" << a.second.totaltimesinscene<< ";" << a.second.totalclicks << "\n";
		totaltimaingame += a.second.passtime;
	}
	archive << "tiempo de juego total: "<<";" << ";" << totaltimaingame / 60000.0 << ";" << "min";
	archive << "\n" << "\n";
	//Movement of the mouse
	archive << "Mov en x" << ";" << totalmouseMovement.getX() << "\n" << "Mov en y" << ";" << totalmouseMovement.getY() << "\n";

	archive << "\n" << "\n";
	archive << "Historial de click" << "\n";
	archive << "ID" << ";" << "Scene" << ";" << "Segundo Concreto" << "\n";
	for (auto a : clickdataTimeList) {
		archive << a.id << ";" << a.scene << ";" << a.currentTime / 1000.0 << "\n";
	}
	numtester++;
}

std::string CSVdataRecolector::getUIBtn(const std::string& id)
{
	std::string aux;
	if (id == "B1") aux = "exitBtn";
	else if (id == "B2") aux = "inventoryBtn";
	else if (id == "B3") aux = "pauseBtn";
	else if (id == "B4") aux = "arrowbtn";
	else if (id == "B5") aux = "";
	else if (id == "B6") aux = "arrowBtn";
	else if (id == "B7") aux = "logBtn";
	return aux;
}
