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

	DEFINE_PROPS(
		PROP(mock_model, age, pr::pred::greater_than<25>),
		PROP(mock_model, height),
		PROP(mock_model, name),
		PROP(mock_model, subscribed)
	)


	 /* BEGIN_PROPS
        PROP(mock_model,age,pr::pred::greater_than<25>,pr::pred::greater_eq_than<15>),
        PROP(mock_model,height)
        //pr::property<mock_model, decltype(mock_model::age), pr::pred::greater_than<25>> { &mock_model::age, "age" },
		//pr::property<mock_model, double> {&mock_model::height, "height"},
		//pr::property<mock_model, bool> {&mock_model::subscribed, "subscribed"},
		//pr::property<mock_model, std::string, pr::pred::is_regex<"asd">> {&mock_model::name, "name"},
		//pr::property<mock_model, std::vector<std::vector<int>>> {&mock_model::stuff, "stuff"},
		//pr::property<mock_model, std::vector<mock_model_2>> {&mock_model::geges, "geges"},
		//pr::property<mock_model, std::vector<std::vector<mock_model_2>>> {&mock_model::gagus, "gagus"}
	END_PROPS*/
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


	auto string = pr::to_json(mock);

}
