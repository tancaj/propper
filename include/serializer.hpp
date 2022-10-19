#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>

#include "constraints.hpp"
#include "iteration.hpp"
#include "model_state.hpp"
#include "property.hpp"
#include "vendor/nlohmann/single_include/nlohmann/json.hpp"

namespace pr {
	namespace detail {

		struct from_json_tag { };
		struct to_json_tag { };

		template <typename _type>
		using inner_type = typename std::decay_t<_type>::value_type;

		template <constraints::propped_object _object>
		_object from_json_object(nlohmann::json&& data, detail::error_map& errors)
		{
			_object object{};
			iterate_properties<std::tuple_size<decltype(_object::properties)>::value - 1,
				from_json_tag>(object, std::move(data), errors);
			return object;
		}

		template <constraints::propped_object _object>
		nlohmann::json to_json_object(_object&& object)
		{
			auto json = nlohmann::json::object();
			auto error = error_map{};
			iterate_properties<
				std::tuple_size<decltype(std::decay_t<_object>::properties)>::value - 1, to_json_tag>(
					std::forward<_object>(object), std::move(json), error);
			return json;
		}

		inline void add_error(detail::error_map& errors, std::string_view name,
			std::string_view error)
		{
			auto [it, inserted] = errors.try_emplace(
				std::string(name), std::vector<std::string> { std::string(error) });
			if (!inserted)
				it->second.emplace_back(error);
		}

		template <typename _predicate, typename _value>
		void verify_predicate(_predicate pred, _value value, const char* prop_name,
			detail::error_map& errors)
		{
			if (!pred(value))
				add_error(errors, prop_name, pred.get_error());
		}

		template <typename _prop, typename _value>
		void verify_predicates(_prop prop, _value value,
			detail::error_map& errors)
		{
			std::apply(
				[&value, &prop, &errors](auto&&... pred) {
				((verify_predicate(pred, value, prop.name, errors)), ...);
			},
				prop.predicates);
		}

		template <typename _prop, constraints::string _string>
		void verify_predicates(_prop prop, _string&& value,
			detail::error_map& errors)
		{
			std::apply(
				[&value, &prop, &errors](auto&&... pred) {
				((verify_predicate(pred, value, prop.name, errors)), ...);
			},
				prop.predicates);
		}


		template <typename _prop, constraints::json_trivials _trivial>
		_trivial read_value(const _prop& prop, nlohmann::json&& data,
			detail::error_map& errors)
		{
			auto json = data.find(prop.name);

			if (json == data.end()) {
				add_error(errors, prop.name, "missing property");
				return _trivial{};
			}

			try {
				auto value = json->template get<_trivial>();
				verify_predicates(prop, value, errors);
				return value;
			}
			catch (const nlohmann::json::type_error) {
				add_error(errors, prop.name, "type mismatch");
				return _trivial{};
			}


		}

		template <typename _prop, constraints::optional _optional>
		_optional read_value(const _prop& prop, nlohmann::json&& data,
			detail::error_map& errors)
		{
			auto json = data.find(prop.name);

			if (json == data.end())
				return std::nullopt;

			if (json->is_null())
				return std::nullopt;

			try
			{
				auto value = json->template get<inner_type<_optional>>();
				verify_predicates(prop, value, errors);
				return value;
			}
			catch (const nlohmann::json::type_error)
			{
				add_error(errors, prop.name, "type mismatch");
				return std::nullopt;
			}
		}

		template <typename _prop, constraints::vector _vector>
		_vector read_value(const _prop& prop, nlohmann::json&& data,
			detail::error_map& errors)
		{
			auto json = data.find(prop.name);
			if (json == data.end()) {
				add_error(errors, prop.name, "missing property");
				return _vector{};
			}

			return read_array_value<_prop, inner_type<_vector>>(
				prop, std::move(*json), errors);
		}

		template <typename _prop, constraints::propped_object _object>
		_object read_value(const _prop& prop, nlohmann::json&& data,
			detail::error_map& errors)
		{
			return from_json_object<_object>(data[prop.name], errors);
		}

		template <typename _prop, constraints::json_trivials _trivials>
		std::vector<_trivials> read_array_value(const _prop& prop,
			nlohmann::json&& data,
			detail::error_map& errors)
		{
			try {
				auto arr = data.get<std::vector<_trivials>>();
				for (const auto& value : arr)
					verify_predicates(prop, value, errors);

				return arr;
			}
			catch (const nlohmann::json::type_error) {
				add_error(errors, prop.name, "type mismatch");
				return std::vector<_trivials> {};
			}
		}

		template <typename _prop, constraints::propped_object _object>
		std::vector<_object> read_array_value(const _prop& prop,
			nlohmann::json&& data,
			detail::error_map& errors)
		{
			std::vector<_object> arr;
			for (auto&& value : data)
				arr.emplace_back(from_json_object<_object>(std::move(value), errors));

			return arr;
		}

		template <typename _prop, constraints::vector _vector>
		std::vector<_vector> read_array_value(const _prop& prop,
			nlohmann::json&& data,
			detail::error_map& errors)
		{
			std::vector<_vector> arr;

			for (auto&& value : data)
				arr.emplace_back(
					read_array_value<_prop, inner_type<_vector>>(prop, std::move(value), errors));

			return arr;
		}

		template <constraints::json_trivials _inner_trivial,
			constraints::vector _vector>
		nlohmann::json write_array_value(_vector&& values)
		{
			auto json = nlohmann::json::array();
			for (auto&& value : values)
				json.emplace_back(std::forward<_inner_trivial>(value));

			return json;
		}

		template <constraints::propped_object _inner_object,
			constraints::vector _vector>
		nlohmann::json write_array_value(_vector&& values)
		{
			auto json = nlohmann::json::array();
			for (auto&& value : values)
				json.emplace_back(to_json_object(std::forward<_inner_object>(value)));

			return json;
		}

		template <constraints::vector _inner_vector, constraints::vector _vector>
		nlohmann::json write_array_value(_vector&& values)
		{
			auto json = nlohmann::json::array();
			for (auto&& value : values)
				json.emplace_back(write_array_value<inner_type<_inner_vector>>(
					std::forward<inner_type<_vector>>(value)));

			return json;
		}

		template <constraints::propped_object _object>
		nlohmann::json write_value(_object&& value)
		{
			return to_json_object(std::forward<_object>(value));
		}

		template <constraints::vector _vector>
		nlohmann::json write_value(_vector&& value)
		{
			return write_array_value<inner_type<_vector>>(
				std::forward<_vector>(value));
		}

		template <constraints::optional _optional>
		nlohmann::json write_value(_optional&& value)
		{
			if (value) return *value;
			else return nullptr;
		}

		template <constraints::json_trivials _trivial>
		_trivial write_value(_trivial&& value)
		{
			return value;
		}

		template <std::size_t _iteration, typename _object>
		void handle_property(_object&& object, nlohmann::json&& data,
			detail::error_map& errors, from_json_tag)
		{
			constexpr auto property = std::get<_iteration>(std::decay_t<_object>::properties);
			using prop_value_type = typename decltype(property)::type;

			object.*(property.member) = read_value<decltype(property), prop_value_type>(
				property, std::move(data), errors);
		}

		template <std::size_t _iteration, typename _object>
		void handle_property(_object&& object, nlohmann::json&& data,
			detail::error_map& errors, to_json_tag)
		{
			constexpr auto property = std::get<_iteration>(std::decay_t<_object>::properties);
			using prop_value_type = typename decltype(property)::type;
			data[property.name] = write_value(object.*(property.member));
		}

	} // namespace detail

	template <detail::constraints::propped_object _object>
	std::pair<_object, detail::model_state> from_json(const std::string& json)
	{
		detail::error_map errors;
		auto object = detail::from_json_object<_object>(nlohmann::json::parse(json), errors);
		return std::make_pair(object, detail::model_state(errors));
	}

	template <detail::constraints::propped_object _object>
	std::string to_json(_object&& object)
	{
		auto json = detail::to_json_object(std::forward<_object>(object));
		return json.dump();
	}
} // namespace pr

#endif
