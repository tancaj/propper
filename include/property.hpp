#ifndef PROPERTY_H
#define PROPERTY_H

#include "concepts.hpp"


namespace pr
{
	template<typename _class, detail::property_type _type, detail::predicate... _pred>
	struct property
	{
		constexpr property(_type _class::* member, const char* name)
			:member(member), name(name), predicates(std::forward<_pred>(_pred())...)
		{}

		using type = _type;
		_type _class::* member;
		const char* name;
		std::tuple<_pred...> predicates;
	};
}

#endif