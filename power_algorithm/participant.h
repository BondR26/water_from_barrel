#pragma once

#include <mutex>

#include "barrel.h"


class WaterDrawer
{
public:

	enum class ActionType
	{
		eDraw,
		ePoorOut,
		eCheckLevel,
		eUnknown
	};

	WaterDrawer( int id_, Barrel& shared_barrel);

	~WaterDrawer();

	void drawFrom();

	void poorOut();

	void checkWaterLever();

	friend std::ostream& operator << (std::ostream& os, const WaterDrawer& wd);

private:
	double quantity_to_draw = 0;
	int _id;
	ActionType _action = ActionType::eUnknown;
	Barrel& _barrel;
	std::atomic<double> _wat_lvl = -1;
	std::mutex _mtx;
};