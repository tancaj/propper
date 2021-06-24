#ifndef MODELSTATE_H
#define MODELSTATE_H

#include <unordered_map>
#include <vector>
#include <string>


namespace pr
{
	namespace detail
	{
		using error_map = std::unordered_map<std::string, std::vector<std::string>>;

		struct model_state
		{
			bool is_valid=true;
			error_map model_errors{};
		};
	}
}


#endif