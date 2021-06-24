#ifndef MAPPER_H
#define MAPPER_H

#include "vendor/nlohmann/single_include/nlohmann/json.hpp"
#include "property.hpp"
#include "concepts.hpp"
#include "exception.hpp"
#include "model.hpp"

#include <string>

namespace pr
{
	class serializer
	{
	private:

		template<std::size_t _iteration>
		struct is_zero_iteration : public std::integral_constant<bool, _iteration==0>{};


	private:

		void add_error(const std::string& name, const std::string& error)
		{
			auto [it, inserted]=errors.try_emplace(name, std::vector<std::string>{error});
			if (!inserted)
				it->second.emplace_back(error);
		}

		template <typename _prop_type, detail::property_pod _value_type>
		_value_type get_value(const _prop_type& prop, const nlohmann::json& data)
		{
			if (data.find(prop.name) == data.end())
			{
				add_error(prop.name, "missing property");
				return _value_type{};
			}

			try
			{
				auto value=data[prop.name].get< _value_type>();
				verify_predicates(prop, value);

				return value;
			}
			catch (const nlohmann::json::type_error)
			{
				add_error(prop.name, "type mismatch");
				return _value_type{};
			}
			catch (const nlohmann::json::exception& e)
			{
				add_error(prop.name, e.what());
				return _value_type{};
			}

		}


		template <typename _prop_type, detail::property_vector _value_type>
		_value_type get_value(const _prop_type& prop, const nlohmann::json& data)
		{
			if (data.find(prop.name) == data.end())
			{
				add_error(prop.name, "missing property");
				return _value_type{};
			}

			using arr_type = typename _value_type::value_type;

			return get_array_value<_prop_type,arr_type>(prop,data[prop.name]);
		}


		template<typename _prop_type, detail::property_object _value_type>
		_value_type get_value(const _prop_type& prop, const nlohmann::json& data)
		{
			return from_json_object<_value_type>(data[prop.name]);
		}

		template<typename _prop_type, detail::property_pod _arr_type>
		std::vector<_arr_type> get_array_value(const _prop_type& prop, const nlohmann::json& data)
		{
			try
			{
				auto arr = data.get<std::vector<_arr_type>>();
				for(const auto& value : arr)
					verify_predicates(prop, value);

				return arr;
			}
			catch (const nlohmann::json::type_error)
			{
				add_error(prop.name, "type mismatch");
				return std::vector<_arr_type>{};
			}
			catch (const nlohmann::json::exception& e)
			{
				add_error(prop.name, e.what());
				return std::vector<_arr_type>{};
			}
		}

		template<typename _prop_type, detail::property_object _arr_type>
		std::vector<_arr_type> get_array_value(const _prop_type& prop, const nlohmann::json& data)
		{
			std::vector<_arr_type> arr;
			for (const auto& value : data)
				arr.emplace_back(from_json_object(value));

			return arr;
		}

		template<typename _prop_type, detail::property_vector _arr_type>
		std::vector<_arr_type> get_array_value(const _prop_type& prop, const nlohmann::json& data)
		{
			std::vector<_arr_type> arr;

			using arr_type = typename _arr_type::value_type;
			for (const auto& value : data)
				arr.emplace_back(get_array_value<_prop_type,arr_type>(prop,value));

			return arr;
		}

		template<std::size_t _iteration, typename _obj_type>
		void set_property(_obj_type&& object, const nlohmann::json& data)
		{
			constexpr auto property = std::get<_iteration>(std::decay_t<_obj_type>::properties);
			using prop_value_type = typename decltype(property)::type;

			object.*(property.member) = get_value<decltype(property),prop_value_type>(property,data);
		}

		template<typename _prop_type, typename _val_type>
		void verify_predicates(_prop_type prop, _val_type value)
		{
			std::apply([&value, &prop, this](auto&&... pred)
			{((verify_predicate(pred,value,prop.name)), ...); }, prop.predicates);
		}

		template< typename _prop_type, detail::property_string _val_type>
		void verify_predicates(_prop_type prop, _val_type value)
		{
			std::apply([&value,&prop,this](auto&&... pred)
			{((verify_predicate(pred,value.data(),prop.name)), ...); }, prop.predicates);
		}

		template<typename _pred_type, typename _val_type>
		void verify_predicate(_pred_type pred, _val_type value, const char* prop_name )
		{
			if (!pred(value))
				add_error(prop_name, pred.error);
		}

		template<std::size_t _iteration, typename _obj_type>
		typename std::enable_if_t<is_zero_iteration<_iteration>::value>
		iterate_data(_obj_type&& object, const nlohmann::json& data)
		{
			set_property<_iteration>(object, data);
		}

		template<std::size_t _iteration, typename _obj_type>
		typename std::enable_if_t<!is_zero_iteration<_iteration>::value>
		iterate_data(_obj_type&& object, const nlohmann::json& data)
		{
			set_property<_iteration>(object, data);
			iterate_data<_iteration - 1>(object, data);
		}


		template<typename _obj_type>
		_obj_type from_json_object(const nlohmann::json& data)
		{
			_obj_type object{};
			iterate_data<std::tuple_size<decltype(_obj_type::properties)>::value - 1>(object, data);
			return object;
		}

	private:
		detail::error_map errors;

	public:
		template<detail::property_object _obj_type>
		static _obj_type from_json(const char* json)
		{
			serializer ser = serializer();
			_obj_type object{};

			auto data = nlohmann::json::parse(json);

			ser.iterate_data<std::tuple_size<decltype(_obj_type::properties)>::value - 1>(object, data);

			if (!ser.errors.empty())
			{
				object.model_state.is_valid = false;
				object.model_state.model_errors= ser.errors;
			}

			return object;
		}
	};




}





#endif
