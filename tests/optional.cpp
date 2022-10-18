
#include <catch2/catch.hpp>

#include <propper.hpp>
#include <vector>

#include "json_literal.h"


struct mock_optional
{
    std::string email;
    std::optional<std::string> name;


    DEFINE_PROPS(
        PROP(mock_optional, email),
        PROP(mock_optional, name)
    );
};

TEST_CASE("With value", "[optional]")
{
    std::string json = JSON_LITERAL(
        {
            "email" : "ibb@obb.com",
            "name" : "Ibb"
        });

    auto [mock, model_state] = pr::from_json<mock_optional>(json);

    REQUIRE(model_state.is_valid == true);
    REQUIRE(mock.email == "ibb@obb.com");
    REQUIRE(mock.name == "Ibb");
    REQUIRE(pr::to_json(mock) == R"({"email":"ibb@obb.com","name":"Ibb"})");

    std::string asd = pr::to_json(mock);
}

TEST_CASE("With null", "[optional]")
{
    std::string json = JSON_LITERAL(
        {
            "email" : "ibb@obb.com",
            "name" : null
        });

    auto [mock, model_state] = pr::from_json<mock_optional>(json);

    REQUIRE(model_state.is_valid == true);
    REQUIRE(mock.email == "ibb@obb.com");
    REQUIRE(mock.name == std::nullopt);
    REQUIRE(pr::to_json(mock) == R"({"email":"ibb@obb.com","name":null})");

    std::string asd = pr::to_json(mock);
}

TEST_CASE("Missing property", "[optional]")
{
    std::string json = JSON_LITERAL(
        {
        });

    auto [mock, model_state] = pr::from_json<mock_optional>(json);

    REQUIRE(model_state.is_valid == false);
    REQUIRE(model_state.model_errors["email"].size() == 1);
    REQUIRE(model_state.model_errors["email"][0] == "missing property");

    REQUIRE(mock.email == "");
    REQUIRE(mock.name == std::nullopt);
    REQUIRE(pr::to_json(mock) == R"({"email":"","name":null})");

    std::string asd = pr::to_json(mock);
}

