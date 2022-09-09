#ifndef JSON_LITERAL_H
#define JSON_LITERAL_H
#include <algorithm>


constexpr std::string remove_whitespace(std::string&& literal)
{
    auto space_iterator=std::remove(literal.begin(), literal.end(), ' ');
    literal.erase(space_iterator, literal.end());
    return literal;
}

#define JSON_LITERAL(...) remove_whitespace(std::string( #__VA_ARGS__))
#endif 
