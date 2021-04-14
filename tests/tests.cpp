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


	static auto constexpr properties = std::make_tuple(
		pr::property<mock_model, int> {&mock_model::age, "age"},
		pr::property<mock_model, double> {&mock_model::height, "height"},
		pr::property<mock_model, bool> {&mock_model::subscribed, "subscribed"},
		pr::property<mock_model, std::string> {&mock_model::name, "name"},
		pr::property<mock_model, std::vector<std::vector<int>>> {&mock_model::stuff, "stuff"}
	);
};


TEST_CASE("Default mapping", "[mapping]")
{

	const char* json = "{\"age\":24,\"name\":\"Doe\",\"height\":178.4,\"subscribed\": true, \"stuff\": [[1,2,3,4],[5,6,7,8]]}";
	mock_model mock;
	
	try
	{
		mock = pr::map::from_json<mock_model>(json);
	}
	catch (pr::propper_error& e)
	{
		auto det = e.details;
	}

	REQUIRE(mock.age == 24);
	REQUIRE(mock.name== "Doe");
	REQUIRE(std::abs(mock.height-178.4)<0.01 );
	REQUIRE(mock.subscribed==true);

}
