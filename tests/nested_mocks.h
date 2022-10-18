#ifndef MOCKS_H
#define MOCKS_H

#include <string>
#include <vector>
#include "property.hpp"


struct mock_weapon {
	std::string material;
	float price;
	bool need_repair;
	int level;

	DEFINE_PROPS(
		PROP(mock_weapon, material),
		PROP(mock_weapon, price),
		PROP(mock_weapon, need_repair),
		PROP(mock_weapon, level));
};

struct mock_user {
	std::string name;
	int level;
	std::vector<mock_weapon> weapons;

	DEFINE_PROPS(
		PROP(mock_user, name),
		PROP(mock_user, level),
		PROP(mock_user, weapons));
};

struct mock_clan {
	std::string name;
	float rating;
	std::vector<mock_user> users;

	DEFINE_PROPS(
		PROP(mock_clan, name),
		PROP(mock_clan, rating),
		PROP(mock_clan, users));
};

#endif 
