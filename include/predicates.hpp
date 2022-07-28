#ifndef PREDICATES_H
#define PREDICATES_H


#include "fixed_string.hpp"

#include <regex>
#include <string>

#define FMT_HEADER_ONLY
#include "vendor/fmt/include/fmt/compile.h"

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

			constexpr std::string get_error()
			{
				return fmt::format(FMT_COMPILE("Value must be greater than {}"), _pred_value);
			}
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

			constexpr std::string get_error()
			{
				return fmt::format(FMT_COMPILE("Value must be greater or equal to {}"), _pred_value);
			}

		};

		template<int _pred_value>
		struct less_than
		{
			constexpr  less_than() {}

			bool operator()(int compare_value) const
			{
				return _pred_value > compare_value;
			}

			constexpr std::string get_error()
			{
				return fmt::format(FMT_COMPILE("Value must be less than {}"), _pred_value);
			}

		};

		template<int _pred_value>
		struct less_eq_than
		{
			constexpr   less_eq_than() {}

			bool operator()(int compare_value) const
			{
				return  _pred_value >= compare_value;
			}

			constexpr std::string get_error()
			{
				return fmt::format(FMT_COMPILE("Value must be less or equal to {}"), _pred_value);
			}
		};

		template<int _pred_value_1, int _pred_value_2>
		struct in_between
		{
			constexpr   in_between() {}

			bool operator()(int compare_value) const
			{
				return _pred_value_1 < compare_value && compare_value < _pred_value_2;
			}

			static inline std::string error = fmt::format(FMT_COMPILE("Value must be between {} and {}"), _pred_value_1,_pred_value_2);
		};

		template<int _pred_value_1, int _pred_value_2>
		struct in_between_eq
		{
			constexpr   in_between_eq() {}

			bool operator()(int compare_value) const
			{
				return _pred_value_1 <= compare_value && compare_value <= _pred_value_2;
			}

			constexpr std::string get_error()
			{
				return fmt::format(FMT_COMPILE("Value must be between or equal to {} and {}"), _pred_value_1,_pred_value_2);
			}

		};


		template<detail::fixed_string _pred_value>
		struct is_regex
		{
			constexpr  is_regex() {}

			bool operator()(const std::string& compare_value) const
			{
				std::regex pattern(static_cast<const char*>( _pred_value)); 
				return std::regex_match(compare_value, pattern);
			}

			constexpr std::string get_error()
			{
				return fmt::format(FMT_COMPILE("Value must match the following regular expression: {}"), _pred_value);
			}
		};
	}
}

#endif
