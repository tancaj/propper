#include <catch2/catch.hpp>

#include <propper.hpp>
#include <string>
#include <vector>

#include "json_literal.h"

#include "mocks.h"

TEST_CASE("Nested serialization", "[nested properties]")
{
    std::string json = JSON_LITERAL(
        { "name" : "Clan2000",
            "rating" : 4.5,
            "users" : [
                {
                    "level" : 24,
                    "name" : "Bob34",
                    "weapons" :
                        [
                            {
                                "level" : 13,
                                "material" : "wood",
                                "need_repair" : true,
                                "price" : 100.5
                            }
                        ]
                },
                {
                    "level" : 32,
                    "name" : "bossman",
                    "weapons" :
                        [
                            {
                                "level" : 21,
                                "material" : "wood",
                                "need_repair" : false,
                                "price" : 120.5
                            },
                            {
                                "level" : 41,
                                "material" : "steel",
                                "need_repair" : false,
                                "price" : 500.0
                            }
                        ]
                },
                {
                    "level" : 2,
                    "name" : "n00b",
                    "weapons" : []
                }

            ] });

    auto [mock, model_state]
        = pr::from_json<mock_clan>(json);

    REQUIRE(model_state.is_valid == true);

    REQUIRE(mock.name == "Clan2000");
    REQUIRE(mock.rating == Approx(4.5));

    REQUIRE(mock.users.size() == 3);

    REQUIRE(mock.users[0].name == "Bob34");
    REQUIRE(mock.users[0].level == 24);

    REQUIRE(mock.users[0].weapons.size() == 1);
    REQUIRE(mock.users[0].weapons[0].material == "wood");
    REQUIRE(mock.users[0].weapons[0].price == Approx(100.5));
    REQUIRE(mock.users[0].weapons[0].need_repair == true);
    REQUIRE(mock.users[0].weapons[0].level == 13);

    REQUIRE(mock.users[1].name == "bossman");
    REQUIRE(mock.users[1].level == 32);

    REQUIRE(mock.users[1].weapons.size() == 2);
    REQUIRE(mock.users[1].weapons[0].material == "wood");
    REQUIRE(mock.users[1].weapons[0].price == Approx(120.5));
    REQUIRE(mock.users[1].weapons[0].need_repair == false);
    REQUIRE(mock.users[1].weapons[0].level == 21);

    REQUIRE(mock.users[1].weapons[1].material == "steel");
    REQUIRE(mock.users[1].weapons[1].price == Approx(500.0));
    REQUIRE(mock.users[1].weapons[1].need_repair == false);
    REQUIRE(mock.users[1].weapons[1].level == 41);

    REQUIRE(mock.users[2].name == "n00b");
    REQUIRE(mock.users[2].level == 2);

    REQUIRE(mock.users[2].weapons.empty());

    auto string = pr::to_json(mock);

    REQUIRE(string == json);
}
