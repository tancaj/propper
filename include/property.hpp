#ifndef PROPERTY_H
#define PROPERTY_H

#include "concepts.hpp"

namespace pr {
namespace detail {

    template <typename _class, detail::constraints::property_type _prop, detail::constraints::predicate... _pred>
    struct property {
        constexpr property(_prop _class::*member, const char* name)
            : member(member)
            , name(name)
            , predicates(_pred()...)
        {
        }

        using type = _prop;
        _prop _class::*member;
        const char* name;
        std::tuple<_pred...> predicates;
    };

    template <typename... _type>
    constexpr std::tuple<_type...> make_properties(_type&&... args)
    {
        return std::make_tuple(std::forward<_type>(args)...);
    }
}
}

#ifndef PROP
#define PROP(C, member, ...) \
    pr::detail::property<C, decltype(##C::##member), ##__VA_ARGS__> { &##C::##member, #member }
#endif

#ifndef DEFINE_PROPS
#define DEFINE_PROPS(...) static auto constexpr properties = pr::detail::make_properties( \
                              ##__VA_ARGS__)
#endif

#endif