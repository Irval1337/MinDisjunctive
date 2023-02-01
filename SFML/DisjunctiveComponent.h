#pragma once

#include <vector>
#include <iostream>
#include <string>

class disjunctive_component {
private:
	enum variable_value {
		True, False, Any
	};
private:
	std::vector<variable_value> _variables;
	size_t _used;
	size_t _n;
public:
	void make(const long long&, const size_t&, const long long&);
	std::string get_string();
	std::string get_print_string();
	std::string get_disjunctive_string();
	std::vector<variable_value> get_variables();
	bool contains(disjunctive_component*);
	size_t get_component_size();
	void set_used(const size_t&);
	size_t get_used();
};
