#ifndef PREDICATES_H
#define PREDICATES_H

#include "fixed_string.hpp"
#include <regex>
#include <string>

namespace pr
{
	namespace pred
	{
		template<int _pred_value>
		struct greater_than
		{
			constexpr greater_than()
			{}

			bool operator()(int compare_value) const
			{
				return  _pred_value < compare_value;
			}

			static auto constexpr name = "greater_than";
			static auto constexpr value= _pred_value;
		};

		template<int _pred_value>
		struct greater_eq_than
		{
			constexpr  greater_eq_than()
			{}

			bool operator()(int compare_value) const
			{
				return  _pred_value <= compare_value;
			}

			static auto constexpr name = "greater_eq_than";
			static auto constexpr value= _pred_value;
		};

		template<int _pred_value>
		struct less_than
		{
			constexpr  less_than() {}

			bool operator()(int compare_value) const
			{
				return _pred_value > compare_value;
			}

			static auto constexpr name = "less_than";
			static auto constexpr value = _pred_value;
		};

		template<int _pred_value>
		struct less_eq_than
		{
			constexpr   less_eq_than() {}

			bool operator()(int compare_value) const
			{
				return  _pred_value >= compare_value;
			}

			static auto constexpr name = "less_eq_than";
			static auto constexpr value = _pred_value;
		};

		template<int _value1, int _value2>
		struct in_between
		{
			constexpr   in_between() {}

			bool operator()(int compare_value) const
			{
				return _value1 < compare_value && compare_value < _value2;
			}

			static auto constexpr name = "in_between";
			static auto constexpr value = std::vector<int>{ _value1,_value2 };
		};

		template<int _value1, int _value2>
		struct in_between_eq
		{
			constexpr   in_between_eq() {}

			bool operator()(int compare_value) const
			{
				return _value1 <= compare_value && compare_value <= _value2;
			}

			static auto constexpr name = "in_between_eq";
			static auto constexpr value = std::vector<int>{ _value1,_value2 };
		};


		template<detail::fixed_string _pred_value>
		struct is_regex
		{
			constexpr  is_regex() {}

			bool operator()(const std::string& compare_value) const
			{
				const std::regex pattern(static_cast<char*>( _pred_value)); 
				return std::regex_match(compare_value, pattern);
			}

			static auto constexpr name = "is_regex";
			static auto constexpr value = static_cast<char*>(_pred_value);
		};

		template<detail::fixed_string _pred_value>
		struct is_same
		{
			constexpr is_same(){}

			bool operator()(const std::string&  compare_value) const
			{
				std::string str((const char*)( _pred_value));
				return str == compare_value;
			}

			static auto constexpr name = "is_same";
			static auto constexpr value = static_cast<const char*>(_pred_value);
		};

	}
}

#endif
