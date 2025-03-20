#pragma once
#include <iostream>
#include <string>
#include <list>
#include <unordered_map>
#include <Vector2D.h>
class CSVdataRecolector
{private:
	struct ClickDataTimeLine
	{
		std::string id;
		std::string scene;
		float currentTime;
	};
	struct ClicksDataSummary
	{
		std::string id;
		std::string scene;
		float numclicks;
	};
	struct SceneData
	{
		std::string scene;
		float passtime;
		float totaltimeinscene;
		float totalclicks;
	};
	std::list<ClickDataTimeLine> clickdataTimeList;
	std::unordered_map<std::string, ClicksDataSummary> clicksSummaryMap;
	std::unordered_map<std::string, SceneData> SceneSummaryMap;
	float iniSceneTime;
	float totaltime;
	Vector2D lastmousepos;
	float totalmouseMovement;
public:
	CSVdataRecolector();
	~CSVdataRecolector();
	void AddEntry(const std::string& _id,const std::string _scene,Vector2D pos);
	void enterScene(const std::string scene);
	void exitScene(const std::string scene);
	void safeData();
};

