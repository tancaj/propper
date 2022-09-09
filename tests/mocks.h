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

struct mock_registration_input
{
	int age;
	int height;
	std::string email;
	std::string password;

	DEFINE_PROPS(
		PROP(mock_registration_input, age, pr::pred::range<0, 100>),
		PROP(mock_registration_input, height, pr::pred::greater<0>, pr::pred::less<250>),
		PROP(mock_registration_input, email),
		PROP(mock_registration_input, password, pr::pred::regex<"^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{8,}$">)
		);
};
#endif 
