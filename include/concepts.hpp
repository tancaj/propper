#ifndef CONCEPTS_H
#define CONCEPTS_H

#include <functional>

namespace pr
{
	namespace detail
	{
		template<typename T> struct is_vector : public std::false_type {};

		template<typename T, typename A>
		struct is_vector<std::vector<T, A>> : public std::true_type {};

		template<typename _type>
		concept property_object = std::is_class_v<_type> 
		&& requires(_type prop_type)
		{
			{prop_type.properties};
		};

		template<typename _type>
		concept property_pod = std::is_same_v<std::decay_t<_type>, int>
			|| std::is_same_v<std::decay_t<_type>, double>
			|| std::is_same_v<std::decay_t<_type>, float>
			|| std::is_same_v<std::decay_t<_type>, bool>
			|| std::is_same_v<std::decay_t<_type>, std::string>;

		template<typename _type>
		concept property_type = property_object<_type> || property_pod<_type> || is_vector<_type>::value;


		template<typename _type>
		concept property_string = std::is_same_v<_type, std::string>;

		template<typename _type>
		concept property_vector = is_vector<_type>::value;

		template<typename _type>
		concept predicate_invocable_types = std::predicate<_type, int> 
			|| std::predicate<_type, double>
			|| std::predicate<_type, float>
			|| std::predicate<_type, const char*>;

		template<typename _type>
		concept predicate_error_string = std::is_same_v<std::decay<_type>, std::string>;

		template<typename _type>
		concept predicate = requires(_type pred)
		{
			{pred.error};// -> predicate_error_string;
		}
		&& predicate_invocable_types<_type>;


		template<typename _type>
		concept floating = std::is_floating_point_v<_type>;
	}
}

#endif
