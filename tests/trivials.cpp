#include <catch2/catch.hpp>

#include <propper.hpp>
#include <vector>

#include "json_literal.h"

#include "nested_mocks.h"

TEST_CASE("Trivial serialization", "[trivial properties]")
{

    std::string json = JSON_LITERAL(
        {
            "level" : 28,
            "material" : "steel",
            "need_repair" : true,
            "price" : 160.5
        });

    auto [mock, model_state]
        = pr::from_json<mock_weapon>(json);

    REQUIRE(model_state.is_valid == true);

    REQUIRE(mock.level == 28);
    REQUIRE(mock.material == "steel");
    REQUIRE(mock.need_repair == true);
    REQUIRE(mock.price == Approx(160.5));

    REQUIRE(pr::to_json(mock) == R"({"level":28,"material":"steel","need_repair":true,"price":160.5})");
}

TEST_CASE("Missing property handling", "[trivial properties]")
{
    std::string json = JSON_LITERAL(
        {
            "level" : 28,
            "material" : "steel",
            "need_repair" : true
        });

    auto [mock, model_state] = pr::from_json<mock_weapon>(json);

    REQUIRE(model_state.is_valid == false);
    REQUIRE(model_state.model_errors.size() == 1);
    REQUIRE(model_state.model_errors.find("price") != model_state.model_errors.end());
    REQUIRE(model_state.model_errors["price"].size() == 1);
    REQUIRE(model_state.model_errors["price"][0] == "missing property");

    REQUIRE(mock.level == 28);
    REQUIRE(mock.material == "steel");
    REQUIRE(mock.need_repair == true);

    REQUIRE(pr::to_json(mock) == R"({"level":28,"material":"steel","need_repair":true,"price":0.0})");
}

TEST_CASE("Type mismatch handling", "[trivial properties]")
{
    std::string json = JSON_LITERAL(
        {
            "level" : "high",
            "material" : "steel",
            "need_repair" : true,
            "price" : 160.5
        });

    auto [mock, model_state] = pr::from_json<mock_weapon>(json);

    REQUIRE(model_state.is_valid == false);
    REQUIRE(model_state.model_errors.size() == 1);
    REQUIRE(model_state.model_errors.find("level") != model_state.model_errors.end());
    REQUIRE(model_state.model_errors["level"].size() == 1);
    REQUIRE(model_state.model_errors["level"][0] == "type mismatch");

    REQUIRE(mock.material == "steel");
    REQUIRE(mock.need_repair == true);
    REQUIRE(mock.price == Approx(160.5));

    REQUIRE(pr::to_json(mock) == R"({"level":0,"material":"steel","need_repair":true,"price":160.5})");
}
