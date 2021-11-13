#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

namespace pr
{
	namespace detail
	{
		class propper_exception : public std::exception
		{
		public:
			std::string details;

			propper_exception(const std::string& det) :details(det){}

			const char* what() const noexcept override
			{
				return "Propper exception";
			}

		};
	}

	using propper_error = detail::propper_exception;
}

#endif 
