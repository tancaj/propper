#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <propper.hpp>




class mock_model
{
public:

	int age;
	std::string name;

	static auto constexpr properties = std::make_tuple(
		pr::property< mock_model, int> {& mock_model::age, "age"},
		pr::property< mock_model, std::string> {& mock_model::name, "name"}
	);
};


TEST_CASE("Default mapping", "[mapping]")
{

	const char* json = "{\"age\":24,\"name\":\"Bob\"}";
	
	REQUIRE(pr::mapper::from_json<mock_model>(json).age == 24);
	REQUIRE(pr::mapper::from_json<mock_model>(json).name== "Bob");


}
