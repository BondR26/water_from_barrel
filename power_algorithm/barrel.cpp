#include "barrel.h"

Barrel::Barrel(size_t tonnage_) : _tonnage(tonnage_)
{

}

Barrel::~Barrel()
{

}

void Barrel::takeOutWater(double liters_)
{
	std::unique_lock<std::shared_mutex> lk(_mtx);

	_tonnage -= liters_;
}

bool Barrel::isEmpty() const
{
	std::shared_lock<std::shared_mutex> lk(_mtx);
	return _tonnage < 0;
}

size_t Barrel::getTonnage() const
{
	std::shared_lock<std::shared_mutex> lk(_mtx);
	return _tonnage;
}