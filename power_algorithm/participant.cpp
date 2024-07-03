#include "participant.h"

#include <iostream>
#include <map>
#include <string>
#include <atomic>


WaterDrawer::WaterDrawer(int id_, Barrel& shared_barrel)
	: _id(id_)
	, _barrel(shared_barrel)
{
	if (shared_barrel.getTonnage() > 250)
	{
		quantity_to_draw = 1;
	}
	else
	{
		quantity_to_draw = 0.5;
	}
}

WaterDrawer::~WaterDrawer() {}

void WaterDrawer::drawFrom()
{
	//simulate waiting
	auto mls = rand() % (1100 - 400) + 400;
	std::this_thread::sleep_for(std::chrono::milliseconds(mls));

	_barrel.takeOutWater(quantity_to_draw);

	std::lock_guard<std::mutex> lk(_mtx);
	_action = ActionType::eDraw;
}

void WaterDrawer::poorOut()
{
	//simulate waiting
	auto mls = rand() % (800 - 200) + 200;
	std::this_thread::sleep_for(std::chrono::milliseconds(mls));

	std::lock_guard<std::mutex> lk(_mtx);
	_action = ActionType::ePoorOut;
}

void WaterDrawer::checkWaterLever()
{
	//simulate waiting
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	_wat_lvl.store(_barrel.getTonnage(),std::memory_order_relaxed);
	//std::cout << "Participant " << _id << " cheked water level to be at " << _wat_lvl << std::endl;
	std::lock_guard<std::mutex> lk(_mtx);
	_action = ActionType::eCheckLevel;
}

const std::map<WaterDrawer::ActionType, std::string> gActionTypeNames
{
	{WaterDrawer::ActionType::eDraw , "drawing water" },
	{WaterDrawer::ActionType::ePoorOut, "pooring out" },
	{WaterDrawer::ActionType::eCheckLevel, "checking level" },
	{WaterDrawer::ActionType::eUnknown, "unknown" }
};

std::ostream& operator << (std::ostream& os, const WaterDrawer& wd)
{
	double lvl = wd._wat_lvl.load();
	auto strlvl = lvl == -1 ? "unknown" : std::to_string(lvl);
	os << "\t" << wd._id << "\t" << gActionTypeNames.find(wd._action)->second << "\t\t" << strlvl << std::endl;
	return os;
}