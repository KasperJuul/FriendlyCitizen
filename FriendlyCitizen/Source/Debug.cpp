#pragma once
#include "MilitaryManager.h"
#include "Debug.h"
#include "InformationManager.h"
#include "BuildingPlanner.h"
#include "BuildingPlacer.h"
#include <fstream>
#include <direct.h>
#include <time.h>

using namespace BWAPI;

std::vector<std::string> Debug::errorLog;
void Debug::writeLog(std::string message, std::string fileName, std::string folderName){
	_mkdir(folderName.c_str());
	std::ofstream writeTo;
	writeTo.open(folderName + "\\" + fileName + ".txt");
	writeTo << message+"\n";
	writeTo.close();
}

void Debug::writeLog(std::vector<std::string> messages, std::string fileName, std::string folderName){//Overload for vectors
	std::string temp = "";
	for (auto sentence : messages){
		temp += sentence + "\n";
	}
	writeLog(temp, fileName, folderName);
}

void Debug::writeLog(std::set<std::string> messages, std::string fileName, std::string folderName){//Overload for sets
	std::string temp = "";
	for (auto sentence : messages){
		temp += sentence + "\n";
	}
	writeLog(temp, fileName, folderName);
}//Improvement: Find a way to iterate over any type of collection.

void Debug::writeTimedLog(std::string message, std::string fileName, std::string folderName){//BUGGY - crashes starcraft.
	_mkdir(folderName.c_str());
	time_t rawTime;
	struct tm * timeInfo;
	time(&rawTime);
	localtime_s(timeInfo, &rawTime);
	std::ofstream writeTo;
	char buffer[26];
	asctime_s(buffer, sizeof buffer, timeInfo);
	writeTo.open(folderName + "\\" + buffer + "_" + fileName + ".txt");
	writeTo << message + "\n";
	writeTo.close();
}

void Debug::screenInfo(){
	for (auto w : InformationManager::workerUnits){
		//Broodwar->drawTextMap(w->unit->getPosition(), "%c%d", Text::Yellow, w->unit->getID());
		Broodwar->drawTextMap(w->unit->getPosition() + Position(0, -10), "%c%d", Text::Cyan, w->state);
		std::string attacking = "";
		if (w->unit->isConstructing()){
			attacking = "True";
		}
		else {
			attacking = "False";
		}
		Broodwar->drawTextMap(w->unit->getPosition() + Position(-5, -20), "%c%s", Text::Yellow, attacking.c_str());
	}

	for (auto bub : BuildingPlacer::builders){
		//Broodwar->drawTextMap(w->unit->getPosition(), "%c%d", Text::Yellow, w->unit->getID());
		Broodwar->drawTextMap(bub->unit->getPosition() + Position(0, -20), "%c%d", Text::Purple, bub->state);
	}

	for (auto &u : Broodwar->self()->getUnits()){
		if (u->getType().isWorker()){
			Broodwar->drawTextMap(u->getPosition(), "%c%d", Text::Yellow, u->getID());
		}
		else if (u->getType().isResourceDepot()){
			Broodwar->drawTextMap(u->getPosition() + Position(0,-15), "%c%d", Text::Orange, u->getID());
		}
		else{
			Broodwar->drawTextMap(u->getPosition(), "%c%d", Text::BrightRed, u->getID());
		}

	}
	for (auto &u : Broodwar->enemy()->getUnits()){
		if (u->getType().isResourceDepot()){
			Broodwar->drawTextMap(u->getPosition(), "%c%d", Text::Orange, u->getID());
		}
	}

	std::string workers = "Workers: " + std::to_string(InformationManager::workerUnits.size());
	
	std::string builders = "";

	BWAPI::Broodwar->drawTextScreen(100, 60, std::to_string(MilitaryManager::mainState).c_str());

	int length = 0;
	for (auto b : BuildingPlacer::builders){
		if (length < 5){
			builders += "[" + std::to_string(b->unit->getID()) + ", " + b->buildingProject.toString() + "] ";
		}
		else{
			break;
		}
		length++;
	}
	
	std::string priorities = "";
	length = 0;
	for (auto b : BuildingPlanner::findOrder()){
		if (length < 5){
			priorities += "[" + b.unitType.toString() + "] ";
		}
		else{
			break;
		}
		length++;
	}

	Broodwar->drawTextScreen(20, 10, workers.c_str());

	Broodwar->drawTextScreen(20, 20, builders.c_str());

	Broodwar->drawTextScreen(20, 30, priorities.c_str());

	Broodwar->drawTextScreen(20, 40, "Econ = %d", (int) BuildingPlanner::econ);
	Broodwar->drawTextScreen(20, 50, "Mili = %d", (int) BuildingPlanner::mili);
	Broodwar->drawTextScreen(20, 60, "Tech = %d", (int) BuildingPlanner::tech);

//	Broodwar->drawTextScreen(20, 30, std::to_string(MilitaryManager::mainState).c_str());
//	int i = 4;
//	for (auto u : InformationManager::enemyUnits){
//		Broodwar->drawTextScreen(20, 10*i, u.selfType.getName().c_str());
//		i++;
//		if (i > 19){
//			break;
//		}
//	}


	//Broodwar->drawTextScreen(20, 200, std::to_string(InformationManager::enemyUnits.size()).c_str());
	int some = 0;
	for (auto m :ResourceManager::minPatches){
		std::string minstring = m.name + ": ";
		std::string gathered = "";
		if (!m.workers.empty()){
			for (unsigned int j = 0; j < m.workers.size(); j++){
				minstring += "[" + std::to_string(m.workers.at(j)->getID()) + "] ";
			}
		}
		if (m.unit->isBeingGathered()){
			gathered = "True";
		}
		else {
			gathered = "False";
		}
		Broodwar->drawTextScreen(20, 80 + (some * 10), minstring.c_str());
		Broodwar->drawTextMap(m.unit->getPosition() + Position(-5, -10), "%c%s", Text::Yellow, m.name.c_str());
		Broodwar->drawTextMap(m.unit->getPosition() + Position(-5, -20), "%c%s", Text::Yellow, gathered.c_str());

		some++;

		if (some > 16){
			break;
		}
	}



	Broodwar->drawTextScreen(588, 15, "%c%d/%d", Text::Yellow, Broodwar->self()->supplyUsed() / 2, BWAPI::Broodwar->self()->supplyTotal() / 2);
	Broodwar->drawTextScreen(452, 15, "%c%d", Text::Yellow, InformationManager::reservedMinerals);
	Broodwar->drawTextScreen(520, 15, "%c%d", Text::Yellow, InformationManager::reservedGas);

}

void Debug::errorLogMessages(std::string input){
	std::string temp = "Frame count: " + std::to_string(Broodwar->getFrameCount());
	temp += " Msg: " + input;
	Debug::errorLog.push_back(temp);
}

void Debug::endWriteLog(){
	_mkdir("tests");
	std::ofstream writeTo;
	writeTo.open("tests\\ErrorLog.txt");
	for (auto message : Debug::errorLog){
		writeTo << message + "\n";
	}
	writeTo.close();
}