#include "DisjunctiveComponent.h"

void disjunctive_component::make(const long long& bits, const size_t& n, const long long& mask) {
	_n = n;
	_variables.resize(_n);
	set_used(0ULL);

	for (size_t i = 0; i < _n; ++i) {
		if ((mask >> i) & 1) {
			if ((bits >> (_n - i - 1)) & 1) {
				_variables[i] = variable_value::True;
			} else {
				_variables[i] = variable_value::False;
			}
		} else {
			_variables[i] = variable_value::Any;
		}
	}
}
std::string disjunctive_component::get_string() {
	std::string val = "";
	for (size_t i = 0; i < _n; ++i) {
		if (_variables[i] == variable_value::True) {
			val += "1";
		} else if (_variables[i] == variable_value::False) {
			val += "0";
		}
	}

	return val;
}
std::string disjunctive_component::get_print_string() {
	if (get_used() == 1) return "-";
	if (get_used() == 2) return "X";
	if (get_used() == 3) return "#";

	std::string val = "";
	for (size_t i = 0; i < _n; ++i) {
		if (_variables[i] == variable_value::True) {
			val += "1";
		} else if (_variables[i] == variable_value::False) {
			val += "0";
		}
	}

	return val;
}
std::string disjunctive_component::get_disjunctive_string() {
	std::string val = "";
	for (size_t i = 0; i < _n; ++i) {
		if (_variables[i] == variable_value::Any) continue;
		if (_variables[i] == variable_value::False) val += "!";
		val += i + 'a';
	}

	return val;
}
std::vector<disjunctive_component::variable_value> disjunctive_component::get_variables() {
	return _variables;
}
bool disjunctive_component::contains(disjunctive_component* sec) {
	bool val = true;
	std::vector<disjunctive_component::variable_value> vars = sec->get_variables();
	if (vars.size() > _n) return false;

	for (size_t i = 0; i < _n; ++i) {
		if (vars[i] == disjunctive_component::variable_value::Any) continue;
		if (vars[i] != _variables[i]) {
			val = false;
			break;
		}
	}
	return val;
}
size_t disjunctive_component::get_component_size() {
	size_t sz = 0;
	for (size_t i = 0; i < _n; ++i) {
		sz += _variables[i] != variable_value::Any;
	}
	return sz;
}
void disjunctive_component::set_used(const size_t& val) {
	_used = val;
}
size_t disjunctive_component::get_used() {
	return _used;
}