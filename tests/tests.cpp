#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <propper.hpp>
#include <vector>



class mock_user
{
public:

	int age;
	double height;
	bool subscribed;
	std::string name;

	DEFINE_PROPS(
		PROP(mock_user, age, pr::pred::greater_than<25>),
		PROP(mock_user, height),
		PROP(mock_user, name),
		PROP(mock_user, subscribed)
	)
};


TEST_CASE("Default mapping", "[mapping]")
{

	const char* json = "{\"age\":24,\"name\":\"Doe\",\"height\":178.4,\"subscribed\": true, \"stuff\": [[1,2,3,4],[5,6,7,8]]}";
	
	auto [mock, model_state] = pr::from_json<mock_user>(json);

	REQUIRE(model_state.is_valid == false);

	REQUIRE(mock.age == 24);
	REQUIRE(mock.name== "Doe");
	REQUIRE(mock.height == Approx(178.4));
	REQUIRE(mock.subscribed==true);


	auto string = pr::to_json(mock);
}
