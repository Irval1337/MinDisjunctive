#include "MinimalDisjunctive.h"

long long min_disjunctive::generate_mask(const size_t& permutation, const size_t& n) {
	size_t i = 0, index = 0;
	std::vector<int> v(n, 0);
	std::vector<int> inverse(n);

	for (size_t i = 0; i < n; ++i) {
		inverse[i] = i;
	}

	while (i < n) {
		v[i]++;
		for (int j = i; j--;)
			v[j] = v[j + 1] + 1;

		if (v[0] > n) ++i;
		else {
			i = 0;
			++index;
			if (permutation > index) continue;
			long long val = 0;

			for (int j = n; j--;) {
				if (v[j] > 0) {
					val = (val | (1 << inverse[v[j] - 1])) ^ (0 << inverse[v[j] - 1]);
				}
			}

			return val;
		}
	}

	return -1;
}
bool min_disjunctive::check_in_dnf(std::vector<std::string> dnf, std::string element) {
	bool val = false;
	for (size_t i = 0; i < dnf.size(); ++i) {
		val |= dnf[i] == element;
	}
	return val;
}
int min_disjunctive::get_size(std::vector<std::string> dnf) {
	int val = 0;
	for (size_t i = 0; i < dnf.size(); ++i) {
		for (size_t j = 0; j < dnf[i].size(); ++j) {
			val += dnf[i][j] != '!';
		}
	}
	return val;
}
void min_disjunctive::generate_answers(int p, std::vector<std::string> dnf) {
	bool is_in = false;
	if (_md < get_size(dnf)) {
		return;
	}
	if (p >= (1 << _n)) {
		if (_md > get_size(dnf)) {
			_md = get_size(dnf);
			_answers.clear();
		}
		_answers.push_back(dnf);
		return;
	}

	for (size_t j = 0; j < _not_used[p].size(); ++j) {
		if (check_in_dnf(dnf, _not_used[p][j]->get_disjunctive_string())) {
			generate_answers(p + 1, dnf);
		} else {
			dnf.push_back(_not_used[p][j]->get_disjunctive_string());
			generate_answers(p + 1, dnf);
			dnf.pop_back();
			is_in = true;
		}
	}
	if (!is_in)
		generate_answers(p + 1, dnf);
}

min_disjunctive::min_disjunctive(const long long& num, const size_t& n) {
	_n = n;
	_function_number = num;
	_md = 100;
	_components = std::vector<std::vector<disjunctive_component*>>(1ULL << n, std::vector<disjunctive_component*>((1ULL << n) - 1, nullptr));
	_not_used = std::vector<std::vector<disjunctive_component*>>(1ULL << n);
}
min_disjunctive::min_disjunctive(const std::string& str, const size_t& n) {
	_n = n;
	_function_number = 0;
	_md = 100;
	_components = std::vector<std::vector<disjunctive_component*>>(1ULL << n, std::vector<disjunctive_component*>((1ULL << n) - 1, nullptr));
	_not_used = std::vector<std::vector<disjunctive_component*>>(1ULL << n);
	for (size_t i = 0; i < str.size(); ++i) {
		set_bit(i, str[i] - '0');
	}
}

int min_disjunctive::get_bit(const int& pos) {
	return (_function_number >> ((1ULL << _n) - pos - 1ULL)) & 1ULL;
}
void min_disjunctive::set_bit(const int& pos, const int& val) {
	_function_number = (_function_number | (1ULL << ((1ULL << _n) - pos - 1ULL))) ^ ((val ^ 1ULL) << ((1ULL << _n) - pos - 1ULL));
}
unsigned long long min_disjunctive::get_function_number() {
	return _function_number;
}
std::string min_disjunctive::get_string_function_number() {
	std::string val = "";

	for (size_t i = 0; i < (1 << _n); ++i) {
		val += '0' + get_bit(i);
	}

	return val;
}
std::string min_disjunctive::get_mask_name(const long long& mask, const size_t& n) {
	std::string val = "";
	for (size_t i = 0; i < n; ++i) {
		if ((mask >> i) & 1) {
			val += 'a' + i;
		}
	}

	return val;
}
std::string min_disjunctive::get_permutation_name(const size_t& permutation, const size_t& n) {
	return get_mask_name(generate_mask(permutation, n), n);
}
void min_disjunctive::generate_table() {
	for (long long current_value = 0; current_value < (1 << _n); ++current_value) {
		for (size_t i = 1; i < (1 << _n); ++i) {
			disjunctive_component* comp = new disjunctive_component();
			comp->make(current_value, _n, generate_mask(i, _n));
			_components[current_value][i - 1] = comp;
		}
	}
}
void min_disjunctive::remove_zeros() {
	for (long long current_value = 0; current_value < (1 << _n); ++current_value) {
		if (get_bit(current_value)) continue;
		for (size_t i = 1; i < (1 << _n); ++i) {
			_components[current_value][i - 1]->set_used(1);
		}
	}
}
void min_disjunctive::remove_same() {
	for (long long current_value = 0; current_value < (1 << _n); ++current_value) {
		if (get_bit(current_value)) continue;
		for (size_t i = 1; i < (1 << _n); ++i) {
			for (long long curr_tmp_value = 0; curr_tmp_value < (1 << _n); ++curr_tmp_value) {
				if (get_bit(curr_tmp_value) == 0) continue;
				if (_components[current_value][i - 1]->get_variables() == _components[curr_tmp_value][i - 1]->get_variables()) {
					_components[curr_tmp_value][i - 1]->set_used(2);
				}
			}
		}
	}
}
void min_disjunctive::remove_contains() {
	for (long long current_value = 0; current_value < (1 << _n); ++current_value) {
		if (get_bit(current_value) == 0) continue;
		for (size_t i = 1; i < (1 << _n); ++i) {
			if (_components[current_value][i - 1]->get_used() != 0) continue;

			for (size_t j = i + 1; j < (1 << _n); ++j) {
				if (_components[current_value][j - 1]->get_used() != 0) continue;
				if (_components[current_value][j - 1]->contains(_components[current_value][i - 1]))
					_components[current_value][j - 1]->set_used(3);
			}
		}
	}
}
void min_disjunctive::find_answers() {
	_answers.clear();

	if (_function_number == 0) {
		_answers.push_back({ "0" });
		return;
	} else if (_function_number == (1ULL << (1ULL << _n)) - 1) {
		_answers.push_back({ "1" });
		return;
	}

	for (size_t i = 0; i < (1 << _n); ++i) {
		_not_used[i].clear();
		for (size_t j = 1; j < (1 << _n); ++j) {
			if (_components[i][j - 1]->get_used() == 0)
				_not_used[i].push_back(_components[i][j - 1]);
		}
	}

	std::vector<std::string> tmp;
	std::vector<std::vector<std::string>> tmp_ans;
	generate_answers(0, tmp);
	for (size_t i = 0; i < _answers.size(); ++i) {
		std::sort(_answers[i].begin(), _answers[i].end());
	}
	std::sort(_answers.begin(), _answers.end());

	if (_answers.size() != 0) {
		tmp_ans.push_back(_answers[0]);
		for (size_t i = 1; i < _answers.size(); ++i) {
			if (_answers[i - 1] != _answers[i]) {
				tmp_ans.push_back(_answers[i]);
			}
		}
	}
	_answers = tmp_ans;
}
std::vector<std::vector<std::string>> min_disjunctive::get_answers() {
	return _answers;
}
void min_disjunctive::print_answers() {
	for (size_t i = 0; i < _answers.size(); ++i) {
		for (size_t j = 0; j < _answers[i].size(); ++j) {
			std::cout << _answers[i][j];
			if (j != _answers[i].size() - 1) std::cout << " v ";
		}
		std::cout << std::endl;
	}
}
void min_disjunctive::print_table() {
	for (size_t i = 1; i < (1 << _n); ++i) {
		std::cout << std::setw(5) << std::left << get_mask_name(generate_mask(i, _n), _n) << " ";
	}
	std::cout << std::endl;

	for (long long current_value = 0; current_value < (1 << _n); ++current_value) {
		for (size_t i = 1; i < (1 << _n); ++i) {
			if (_components[current_value][i - 1]->get_used() != 0)
				std::cout << std::setw(5) << std::left << _components[current_value][i - 1]->get_print_string() << " ";
			else std::cout << std::setw(5) << std::left << _components[current_value][i - 1]->get_disjunctive_string() << " ";
		}
		std::cout << std::endl;
	}
}
std::vector<std::vector<disjunctive_component*>> min_disjunctive::get_components() {
	return _components;
}