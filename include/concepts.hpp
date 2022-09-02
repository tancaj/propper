#ifndef CONCEPTS_H
#define CONCEPTS_H

#include <functional>

namespace pr {
namespace detail {

    namespace constraints {
        template <typename _type>
        struct is_vector : public std::false_type {
        };

        template <typename _type>
        struct is_vector<std::vector<_type>> : public std::true_type {
        };

        template <typename _type>
        concept propped_object = std::is_class_v<std::decay_t<_type>> && requires(_type prop_type)
        {
            { prop_type.properties };
        };

        template <typename _type>
        concept json_trivials = std::is_same_v < std::decay_t<_type>,
        int >
            || std::is_same_v<std::decay_t<_type>, double> || std::is_same_v<std::decay_t<_type>, float> || std::is_same_v<std::decay_t<_type>, bool> || std::is_same_v<std::decay_t<_type>, std::string>;

        template <typename _type>
        concept string = std::is_same_v<_type, std::string>;

        template <typename _type>
        concept vector = is_vector<std::decay_t<_type>>::value;

        template <typename _type>
        concept property_type = propped_object<_type> || json_trivials<_type> || vector<_type>;

        template <typename _type>
        concept predicate_invocable_types = std::predicate<_type, int> || std::predicate<_type, double> || std::predicate<_type, float> || std::predicate<_type, bool> || std::predicate<_type, const char*> || std::predicate<_type, std::string>;

        template <typename _type>
        concept predicate = requires(_type pred)
        {
            {
                pred.get_error()
                } -> std::same_as<std::string>;
        }
        &&predicate_invocable_types<_type>;
    }
}
}

#endif
