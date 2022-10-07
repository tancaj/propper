#include <catch2/catch.hpp>
#include <propper.hpp>

#include "json_literal.h"
#include "mocks.h"

TEST_CASE("Greater than", "[predicates]")
{
	auto predicate = pr::pred::greater<15>();
	REQUIRE(predicate.get_error()=="Value must be greater than 15");

	REQUIRE(!predicate(10));
	REQUIRE(!predicate(15));
	REQUIRE(!predicate(-5));
	REQUIRE(predicate(16));
}
TEST_CASE("Less than", "[predicates]")
{
	auto predicate = pr::pred::less<15>();
	REQUIRE(predicate.get_error()=="Value must be less than 15");

	REQUIRE(predicate(10));
	REQUIRE(!predicate(15));
	REQUIRE(predicate(-5));
	REQUIRE(!predicate(16));
}

TEST_CASE("Greater than or equal to", "[predicates]")
{
	auto predicate = pr::pred::greater_eq<15>();
	REQUIRE(predicate.get_error()=="Value must be greater than or equal to 15");

	REQUIRE(!predicate(10));
	REQUIRE(predicate(15));
	REQUIRE(!predicate(-5));
	REQUIRE(predicate(16));
}

TEST_CASE("Less than or equal to", "[predicates]")
{
	auto predicate = pr::pred::less_eq<15>();
	REQUIRE(predicate.get_error()=="Value must be less than or equal to 15");

	REQUIRE(predicate(10));
	REQUIRE(predicate(15));
	REQUIRE(predicate(-5));
	REQUIRE(!predicate(16));
}

TEST_CASE("In range", "[predicates]")
{
	auto predicate = pr::pred::range<10,15>();
	REQUIRE(predicate.get_error()=="Value must be between 10 and 15");

	REQUIRE(!predicate(10));
	REQUIRE(!predicate(15));
	REQUIRE(!predicate(-5));
	REQUIRE(predicate(12));
}

TEST_CASE("In range equal", "[predicates]")
{
	auto predicate = pr::pred::range_eq<10,15>();
	REQUIRE(predicate.get_error()=="Value must be between or equal to 10 and 15");

	REQUIRE(predicate(10));
	REQUIRE(predicate(15));
	REQUIRE(!predicate(-5));
	REQUIRE(predicate(12));
}

TEST_CASE("Regex", "[predicates]")
{
	auto predicate = pr::pred::regex<"^[0-9]+$">();
	REQUIRE(predicate.get_error()=="Value must match the following regular expression: ^[0-9]+$");

	REQUIRE(predicate("5"));
	REQUIRE(!predicate("five"));
	REQUIRE(predicate("1337"));
	REQUIRE(!predicate(""));
}

TEST_CASE("Serialization with predicates", "[predicates]")
{
	auto json = JSON_LITERAL({
			"age" : 37,
			"email": "ibb@obb.com",
			"height" : 178,
			"password" : "IbbObb1337"
		});

	auto [mock, model_state] = pr::from_json<mock_registration_input>(json);

	REQUIRE(model_state.is_valid);

	REQUIRE(mock.email=="ibb@obb.com");
	REQUIRE(mock.password=="IbbObb1337");
	REQUIRE(mock.age==37);
	REQUIRE(mock.height==178);

	REQUIRE(pr::to_json(mock)==R"({"age":37,"email":"ibb@obb.com","height":178,"password":"IbbObb1337"})");
}

TEST_CASE("Predicate error handling", "[predicates]")
{
	auto json = JSON_LITERAL({
			"age" : -10,
			"email": "ibb@obb.com",
			"height" : 260,
			"password" : "Ibb10"
		});

	auto [mock, model_state] = pr::from_json<mock_registration_input>(json);

	REQUIRE(!model_state.is_valid);
	REQUIRE(model_state.model_errors.size()==3);

	REQUIRE(model_state.model_errors.find("age") != model_state.model_errors.end());
	REQUIRE(model_state.model_errors["age"].size()==1);
	REQUIRE(model_state.model_errors["age"][0] == "Value must be between 0 and 100");

	REQUIRE(model_state.model_errors.find("height") != model_state.model_errors.end());
	REQUIRE(model_state.model_errors["height"].size()==1);
	REQUIRE(model_state.model_errors["height"][0] == "Value must be less than 250");

	REQUIRE(model_state.model_errors.find("password") != model_state.model_errors.end());
	REQUIRE(model_state.model_errors["password"].size()==1);
	REQUIRE(model_state.model_errors["password"][0] == "Value must match the following regular expression: ^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{8,}$");

	REQUIRE(mock.email=="ibb@obb.com");
	REQUIRE(mock.password=="Ibb10");
	REQUIRE(mock.age==-10);
	REQUIRE(mock.height==260);

	REQUIRE(pr::to_json(mock)==R"({"age":-10,"email":"ibb@obb.com","height":260,"password":"Ibb10"})");
}
