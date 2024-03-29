#ifndef FIXEDSTRING_H
#define FIXEDSTRING_H

namespace pr {
namespace detail {
    template <unsigned _size>
    struct fixed_string {
        char buf[_size + 1] {};
        constexpr fixed_string(char const* str)
        {
            for (unsigned i = 0; i != _size; ++i)
                buf[i] = str[i];
        }
        constexpr operator char const*() const { return buf; }
    };

    template <unsigned _size>
    fixed_string(char const (&)[_size]) -> fixed_string<_size - 1>;

}
}

#endif
