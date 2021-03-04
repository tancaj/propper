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
				return "Propper error";
			}

		};

		class predicate_exception : public propper_exception
		{

		public:
			predicate_exception(const std::string& det) :propper_exception(det){}


			const char* what() const noexcept override
			{
				return "One of the values did not statisfy its predicate";
			}
		};

		class mapping_exception : public propper_exception
		{

		public:
			mapping_exception(const std::string det) :propper_exception(det){}

			const char* what() const noexcept override
			{
				return "Mapping error";
			}
		};
	}

	using predicate_error = detail::predicate_exception;
	using mapping_error = detail::mapping_exception;
	using propper_error = detail::propper_exception;
}

#endif 
