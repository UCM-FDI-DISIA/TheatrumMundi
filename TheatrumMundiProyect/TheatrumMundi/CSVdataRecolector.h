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
		float totaltimesinscene;
		float totalclicks;
	};
	std::list<ClickDataTimeLine> clickdataTimeList;
	std::unordered_map<std::string, ClicksDataSummary> clicksSummaryMap;
	std::unordered_map<std::string, SceneData> SceneSummaryMap;
	float iniSceneTime;
	Vector2D lastmousepos;
	Vector2D totalmouseMovement;
	int numtester;
	std::string getUIBtn(const std::string& id);
public:
	CSVdataRecolector();
	~CSVdataRecolector();
	void AddEntry(const std::string& _id,const std::string _scene);
	void enterScene(const std::string scene);
	void exitScene(const std::string scene);
	void safeData();
	
};

