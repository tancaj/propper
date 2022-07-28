#ifndef ITERATION_H
#define ITERATION_H

#include <cstddef>

namespace pr
{
	namespace detail
	{
		template<std::size_t _iteration>
		struct is_zero_iteration : public std::integral_constant<bool, _iteration==0>{};
	}

}

#endif 
