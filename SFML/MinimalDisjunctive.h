#pragma once

#include <string>
#include <iostream>
#include "DisjunctiveComponent.h"
#include <vector>
#include <iomanip>
#include <algorithm>

class min_disjunctive {
private:
	unsigned long long _function_number;
	size_t _n, _md;
	std::vector<std::vector<disjunctive_component*>> _components, _not_used;
	std::vector<std::vector<std::string>> _answers;
private:
	long long generate_mask(const size_t&, const size_t&);
	bool check_in_dnf(std::vector<std::string>, std::string);
	int get_size(std::vector<std::string>);
	void generate_answers(int p, std::vector<std::string>);
public:
	min_disjunctive(const long long&, const size_t&);
	min_disjunctive(const std::string&, const size_t&);
public:
	int get_bit(const int&);
	void set_bit(const int&, const int&);
	unsigned long long get_function_number();
	std::string get_string_function_number();
	std::string get_mask_name(const long long&, const size_t&);
	std::string get_permutation_name(const size_t&, const size_t&);
	void generate_table();
	void remove_zeros();
	void remove_same();
	void remove_contains();
	void find_answers();
	std::vector<std::vector<std::string>> get_answers();
	void print_answers();
	void print_table();
	std::vector<std::vector<disjunctive_component*>> get_components();
};