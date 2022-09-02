#ifndef MODELSTATE_H
#define MODELSTATE_H

#include <string>
#include <unordered_map>
#include <vector>

namespace pr {
namespace detail {
    using error_map = std::unordered_map<std::string, std::vector<std::string>>;

    struct model_state {
        bool is_valid = true;
        error_map model_errors;

        model_state(const error_map& errors)
            : model_errors(errors)
            , is_valid(errors.empty())
        {
        }
    };
}
}

#endif