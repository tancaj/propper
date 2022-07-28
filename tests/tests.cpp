#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <propper.hpp>
#include <vector>




class mock_model
{
public:

	int age;
	double height;
	bool subscribed;
	std::string name;
	std::vector<std::vector<int>> stuff;


	static auto constexpr properties = pr::make_properties(
		pr::property<mock_model, int, pr::pred::greater_than<25>> {&mock_model::age, "age"},
		pr::property<mock_model, double> {&mock_model::height, "height"},
		pr::property<mock_model, bool> {&mock_model::subscribed, "subscribed"},
		pr::property<mock_model, std::string, pr::pred::is_regex<"asd">> {&mock_model::name, "name"},
		pr::property<mock_model, std::vector<std::vector<int>>> {&mock_model::stuff, "stuff"}
	);
};


TEST_CASE("Default mapping", "[mapping]")
{

	const char* json = "{\"age\":24,\"name\":\"Doe\",\"height\":178.4,\"subscribed\": true, \"stuff\": [[1,2,3,4],[5,6,7,8]]}";
	
	auto [mock, model_state] = pr::from_json<mock_model>(json);

	REQUIRE(model_state.is_valid == false);

	REQUIRE(mock.age == 24);
	REQUIRE(mock.name== "Doe");
	REQUIRE(mock.height == Approx(178.4));
	REQUIRE(mock.subscribed==true);

}
