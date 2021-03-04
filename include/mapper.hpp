#ifndef MAPPER_H
#define MAPPER_H

#include "vendor/nlohmann/single_include/nlohmann/json.hpp"
#include "property.hpp"
#include "concepts.hpp"
#include "exception.hpp"

namespace pr
{
	class mapper
	{
	private:

		template<std::size_t _iteration>
		struct is_zero_iteration : public std::integral_constant<bool, _iteration==0>{};


	private:

		template <typename _prop_type, detail::property_pod _value_type>
		_value_type get_value(const _prop_type& prop, const nlohmann::json& data)
		{
			if (data.find(prop.name) == data.end())
			{
				nlohmann::json error_obj;
				error_obj["property"] = prop.name;
				error_obj["error"] = "missing property";
				mapping_err.push_back(error_obj);
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
				nlohmann::json error_obj;
				error_obj["property"] = prop.name;
				error_obj["error"] = "type mismatch";
				mapping_err.push_back(error_obj);
				return _value_type{};
			}
			catch (const nlohmann::json::exception& e)
			{
				nlohmann::json error_obj;
				error_obj["property"] = prop.name;
				error_obj["error"] = e.what();
				mapping_err.push_back(error_obj);
				return _value_type{};
			}

		}

		template <typename _prop_type, detail::property_vector _value_type>
		_value_type get_value(const _prop_type& prop, const nlohmann::json& data)
		{
			_value_type arr{};
			using arr_type = typename _value_type::value_type;

			if (data.find(prop.name) == data.end())
			{
				nlohmann::json error_obj;
				error_obj["property"] = prop.name;
				error_obj["error"] = "missing property";
				return arr;
			}

			if constexpr (std::is_class_v<arr_type> && !std::is_same_v<arr_type, std::string>)
			{
				const auto values = data[prop.name];
				for (const auto& value : arr)
					arr.emplace_back(from_json_object(value));
			}
			else
			{
				try
				{
					arr = data[prop.name].get<_value_type>();
					for(const auto& value : arr)
						verify_predicates(prop, value);
				}
				catch (const nlohmann::json::type_error)
				{
					nlohmann::json error_obj;
					error_obj["property"] = prop.name;
					error_obj["error"] = "type mismatch";
					mapping_err.push_back(error_obj);
					return _value_type{};
				}
				catch (const nlohmann::json::exception& e)
				{
					nlohmann::json error_obj;
					error_obj["property"] = prop.name;
					error_obj["error"] = e.what();
					mapping_err.push_back(error_obj);
					return _value_type{};
				}

			}

			return arr;
		}

		template<typename _prop_type, detail::property_object _value_type>
		_value_type get_value(const _prop_type& prop, const nlohmann::json& data)
		{
			return from_json_object<_value_type>(data[prop.name]);
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
			{
				nlohmann::json error_obj;
				error_obj["property"] = prop_name;
				error_obj["property_value"] = value;
				error_obj["predicate"] = pred.name;
				error_obj["predicate_value"] = pred.value;
				pred_err.push_back(error_obj);
			}
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
		nlohmann::json pred_err;
		nlohmann::json mapping_err;

	public:
		template<detail::property_object _obj_type>
		static _obj_type from_json(const char* json)
		{
			mapper model_mapper = mapper();
			_obj_type object{};

			auto data = nlohmann::json::parse(json);

			model_mapper.iterate_data<std::tuple_size<decltype(_obj_type::properties)>::value - 1>(object, data);

			if(!model_mapper.mapping_err.empty())
				throw detail::mapping_exception(model_mapper.mapping_err.dump());

			if (!model_mapper.pred_err.empty())
				throw detail::predicate_exception(model_mapper.pred_err.dump());

			return object;
		}
	};




}





#endif
