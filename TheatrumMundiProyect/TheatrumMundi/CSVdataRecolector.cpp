#include "CSVdataRecolector.h"
#include <fstream>
#include "Game.h"
#include "SDLUtils.h"
#include <iterator>
#include <SDL.h>
CSVdataRecolector::CSVdataRecolector()
{
	clickdataTimeList = std::list<ClickDataTimeLine>();
	clicksSummaryMap = std::unordered_map<std::string,ClicksDataSummary>();
	SceneSummaryMap = std::unordered_map<std::string,SceneData>();
	iniSceneTime = 0;
	totaltime = 0;
	lastmousepos = Vector2D(0, 0);
	totalmouseMovement = 0;
	enterScene("ROOM1");
	
	AddEntry("A", "ROOM1", Vector2D(0, 0));
	AddEntry("A", "ROOM1", Vector2D(0, 0));
	SDL_Delay(1000);
	AddEntry("A", "ROOM1", Vector2D(0, 0));
	SDL_Delay(1000);
	exitScene("ROOM1");
	enterScene("ROOM3");
	AddEntry("A", "ROOM3", Vector2D(0, 0));
	SDL_Delay(1000);
	AddEntry("B", "ROOM3", Vector2D(0, 0));
	
	exitScene("ROOM3");
}

CSVdataRecolector::~CSVdataRecolector()
{
}

void CSVdataRecolector::AddEntry(const std::string& _id, const std::string _scene, Vector2D pos)
{
	//clickDataTimeLine
	ClickDataTimeLine aux = { _id,_scene,sdlutils().currRealTime()};
	clickdataTimeList.push_back(aux);
	//ClickDataSummary
	 std::string const auxid = _id + _scene;
	 auto a = clicksSummaryMap.find(auxid);
	 
	 if (a != clicksSummaryMap.end()) {
		 a.operator*().second.numclicks++;
	}
	 else {
		 clicksSummaryMap.insert({ auxid,{_id,_scene,1}});
	 }
	 //add the click to the sceneSummary
	 SceneSummaryMap[_scene].totalclicks++;
	

}

void CSVdataRecolector::enterScene(const std::string scene)
{
	auto a = SceneSummaryMap.find(scene);
	if (a != SceneSummaryMap.end()) {
		a.operator*().second.totaltimeinscene++;
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
	std::fstream archive;
	archive.open("../PlanDePruebas/prueba.csv", std::ios::out | std::ios::app);
	archive << "Historial de click" << "\n";
	archive << "ID" << ";" << "Scene" << ";" << "Momento" << "\n";
	for (auto a : clickdataTimeList) {
		archive << a.id << ";" << a.scene << ";" << a.currentTime << "\n";
	}
	archive<< "\n"<<"\n";
	archive<< "Resumen de click en botones"<<"\n";
	archive << "ID" << ";" << "Scene" << ";" << "Numero de click" << "\n";
	for (auto a : clicksSummaryMap) {
		archive << a.second.id << ";" << a.second.scene << ";" << a.second.numclicks << "\n";
	}
	archive << "\n" << "\n";
	archive << "Tiempo en las escenas" << "\n";
	archive << "Scene" << ";" << "Tiempo" << ";" << "Veces en la escena"<<";" << "totalClicks" << "\n";
	for (auto a : SceneSummaryMap) {
		archive << a.second.scene << ";" << a.second.passtime << ";" << a.second.totaltimeinscene << ";" << a.second.totalclicks << "\n";
	}
}
