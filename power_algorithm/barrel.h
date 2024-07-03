#pragma once
#include <shared_mutex>


class Barrel
{
public:
	Barrel(size_t tonnage);
	~Barrel();

	void takeOutWater(double liters_);

	bool isEmpty() const;

	size_t getTonnage() const;

private:
	mutable std::shared_mutex _mtx;
	int _tonnage;
};