#ifndef ITERATION_H
#define ITERATION_H

#include <cstddef>
#include <model_state.hpp>
#include "vendor/nlohmann/single_include/nlohmann/json.hpp"

namespace pr
{
	namespace detail
	{
		template<std::size_t _iteration>
		struct is_zero_iteration : public std::integral_constant<bool, _iteration==0>{};

		template<std::size_t _iteration, typename _tag_type, typename _obj_type>
		typename std::enable_if_t<is_zero_iteration<_iteration>::value>
			iterate_properties(_obj_type&& object, nlohmann::json&& data, error_map& errors)
		{
			handle_property<_iteration>(std::forward<_obj_type>(object), std::move(data), errors, _tag_type{});
		}

		template<std::size_t _iteration, typename _tag_type, typename _obj_type>
		typename std::enable_if_t<!is_zero_iteration<_iteration>::value>
			iterate_properties(_obj_type&& object, nlohmann::json&& data, error_map& errors)
		{
			handle_property<_iteration>(std::forward<_obj_type>(object), std::move(data), errors, _tag_type{});
			iterate_properties<_iteration - 1, _tag_type>(std::forward<_obj_type>(object), std::move(data), errors);
		}
	}

}

#endif 
