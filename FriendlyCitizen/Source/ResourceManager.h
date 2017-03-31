#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <vector>
#include <deque>
#include <algorithm>


// Remember not to use "Broodwar" in any global class constructor!

class ResourceManager
{
public:
	struct mineralPatch{
			BWAPI::Unit unit;
			std::string name;
			std::deque<BWAPI::Unit> workers;
			int center;
		};	
	
	struct workerUnit{
		BWAPI::Unit unit;
		mineralPatch* mineral;
		std::string status;		// Idle, Mining, Returning Cargo, Waiting,
	};


	static void onStart();
	static void onFrame();
	static void buildPylonsNProbes();
	static void buildPylonsNProbes2();
	static void drawMinCircles();
	static void findMinPatches();

	static void stdGather();
	static void stdGather2();
	static void queueGather();

	static void queueManager();
	static void queueManager2();

	static int workTime(mineralPatch m);
	static int workTime(BWAPI::Unit unit, mineralPatch m, int n);
	static int workTime2(BWAPI::Unit unit, mineralPatch m, int n);
	static int roundTrip(BWAPI::Unit u , mineralPatch m);
	static mineralPatch* roundTrip_min(BWAPI::Unit u, std::vector<ResourceManager::mineralPatch>* patches);

	

	static std::vector<ResourceManager::mineralPatch> minPatches;
	static std::vector<ResourceManager::workerUnit> wrkUnits;
	static std::vector<BWAPI::Unit> centers;
	static BWTA::BaseLocation* mainBase;
	static std::string log;
};

