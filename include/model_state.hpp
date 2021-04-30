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
		private:
			friend class serializer;

			bool is_prop_valid;
			bool is_pred_valid;

			error_map model_errors;

		public:
			bool valid() const
			{
				return is_prop_valid && is_pred_valid;
			}

			bool property_valid() const 
			{
				return is_prop_valid;
			}

			bool predicates_valid() const 
			{
				return is_pred_valid;
			}

			const error_map& errors() const
			{
				return model_errors;
			}

			
		};
	}
}


#endif