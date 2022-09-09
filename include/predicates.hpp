#ifndef PREDICATES_H
#define PREDICATES_H

#include "fixed_string.hpp"

#include <regex>
#include <string>

#define FMT_HEADER_ONLY
#include "vendor/fmt/include/fmt/compile.h"

namespace pr {
	namespace pred {
		template <int _pred_value>
		struct greater {
			constexpr greater()
			{
			}

			bool operator()(int compare_value) const
			{
				return _pred_value < compare_value;
			}

			constexpr std::string get_error()
			{
				return fmt::format(FMT_COMPILE("Value must be greater than {}"), _pred_value);
			}
		};

		template <int _pred_value>
		struct greater_eq {
			constexpr greater_eq()
			{
			}

			bool operator()(int compare_value) const
			{
				return _pred_value <= compare_value;
			}

			constexpr std::string get_error()
			{
				return fmt::format(FMT_COMPILE("Value must be greater than or equal to {}"), _pred_value);
			}
		};

		template <int _pred_value>
		struct less {
			constexpr less() { }

			bool operator()(int compare_value) const
			{
				return _pred_value > compare_value;
			}

			constexpr std::string get_error()
			{
				return fmt::format(FMT_COMPILE("Value must be less than {}"), _pred_value);
			}
		};

		template <int _pred_value>
		struct less_eq {
			constexpr less_eq() { }

			bool operator()(int compare_value) const
			{
				return _pred_value >= compare_value;
			}

			constexpr std::string get_error()
			{
				return fmt::format(FMT_COMPILE("Value must be less than or equal to {}"), _pred_value);
			}
		};

		template <int _pred_value_1, int _pred_value_2>
		struct range {
			constexpr range() { }

			bool operator()(int compare_value) const
			{
				return _pred_value_1 < compare_value&& compare_value < _pred_value_2;
			}

			constexpr std::string get_error()
			{
				return fmt::format(FMT_COMPILE("Value must be between {} and {}"), _pred_value_1, _pred_value_2);
			}
		};

		template <int _pred_value_1, int _pred_value_2>
		struct range_eq {
			constexpr range_eq() { }

			bool operator()(int compare_value) const
			{
				return _pred_value_1 <= compare_value && compare_value <= _pred_value_2;
			}

			constexpr std::string get_error()
			{
				return fmt::format(FMT_COMPILE("Value must be between or equal to {} and {}"), _pred_value_1, _pred_value_2);
			}
		};

		template <detail::fixed_string _pred_value>
		struct regex {
			constexpr regex() { }

			bool operator()(const std::string& compare_value) const
			{
				std::regex pattern(static_cast<const char*>(_pred_value));
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
