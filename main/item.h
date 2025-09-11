#pragma once
#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item {
private:
	std::string name;
	int cooldown;
public:
	Item(std::string, int);
};

//class Potion : Item {
//private:
//	const int SPEED_BUFF = 100;
//public:
//	Potion(int);
//	const int getSpeed() const { return SPEED_BUFF; }
//};

class Vest : Item {
private:
	const float DAMAGE_REDUCTION = 20.0f;
public:
	Vest(float);
	const int getDamageReduction() const { return DAMAGE_REDUCTION; }
};

class Food : Item {
private:
	const float HEALTH_BOOST = 10.0f;
public:
	Food(float);
	const int getDamageReduction() const { return HEALTH_BOOST; }
};

class Bandage : Item {
private:
	const float HEALING_REGEN = 5.0f;
public:
	Bandage(float);
	const int getDamageReduction() const { return HEALING_REGEN; }
};

#endif