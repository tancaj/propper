#ifndef PROPERTY_H
#define PROPERTY_H

#include "concepts.hpp"

namespace pr {
namespace detail {

    template <typename _class, detail::constraints::property_type _type, detail::constraints::predicate... _pred>
    struct property {
        constexpr property(_type _class::*member, const char* name)
            : member(member)
            , name(name)
            , predicates(_pred()...)
        {
        }

        using type = _type;
        _type _class::*member;
        const char* name;
        std::tuple<_pred...> predicates;
    };

    template <class... _type>
    constexpr std::tuple<_type...> make_properties(_type&&... args)
    {
        return std::make_tuple(std::forward<_type>(args)...);
    }
}
}

#define PROP(C, member, ...) \
    pr::detail::property<C, decltype(##C::##member), ##__VA_ARGS__> { &##C::##member, #member }

#define DEFINE_PROPS(...) static auto constexpr properties = pr::detail::make_properties( \
                              ##__VA_ARGS__);

#endif