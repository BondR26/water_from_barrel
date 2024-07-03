#include <iostream>
#include <vector>

#include "barrel.h"
#include "participant.h"

#include "thread_pool.h"

constexpr int participants = 10;
double gWaterBarrel = 500;

//Бочка заповнена водою на 500л.
//10 людей(незалежно одна від одної) 
//черпаками дістають воду об'ємом 1 літр за раз якщо бочка заповнена більше як на половину або 0.5 літри за раз якщо рівень менше. 
//
//Одночасно кілька людей можуть вимірювати рівень в бочці якщо ніхто у цей час не черпає.
//Одночасно тільки одна людина може черпати.
//Кожна людина може випорожнювати наповнений черпак незалежно від інших.
//
//Вимірювання рівня води складає 0.2 сек за раз;
//Зачерпування з бочки в черпак складає 0.4 - 1.1 (рандом)сек за раз;
//Спорожнення одного черпака складає 0.2 - 0.8 (рандом)сек за раз.
//
//Симулювати спорожнення бочки якщо кожна людина це окремий тред у вигляді таблиці :
//час | людина N | вимірювання або зачерпування або спорожнення | рівень води відомий людині.

int main()
{
	// Seed the random number generator
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	Barrel barrel(gWaterBarrel);
	std::vector<std::unique_ptr<WaterDrawer>> actors;

	for (int i = 0; i < participants; i++)
	{
		actors.emplace_back(new WaterDrawer(i, barrel));
	}

	thread_pool actions(participants);

	while (!barrel.isEmpty())
	{
		system("CLS");

		// get the random actor number
		int id = std::rand() % 10;
		// get the random action number
		WaterDrawer::ActionType type = static_cast<WaterDrawer::ActionType>((std::rand() % 3));
		
		switch (type)
		{
		case WaterDrawer::ActionType::eDraw :
		{
			actions.submit([&actors, id]() {actors[id]->drawFrom(); });

		}break;
		case WaterDrawer::ActionType::ePoorOut:
		{
			actions.submit([&actors, id]() {actors[id]->poorOut(); });
		}break;
		case WaterDrawer::ActionType::eCheckLevel:
		{
			actions.submit([&actors, id]() {actors[id]->checkWaterLever(); });
		}break;
		};

		std::cout << " Time | Person ID | Action Type | WaterLevel \n";
		for (auto& person : actors)
			std::cout << *person << std::endl;
	}

	return EXIT_SUCCESS;
}