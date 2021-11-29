#include "headers.hpp"

std::vector<uint32_t> read_entries_from_file();
std::array<uint32_t, 2> solve_part_one(const std::vector<uint32_t> &_entries);
std::array<uint32_t, 3> solve_part_two(const std::vector<uint32_t> &_entries);

int main() {
	std::vector<uint32_t> _entries = read_entries_from_file();

	std::sort(_entries.begin(), _entries.end());

	auto [_a, _b] = solve_part_one(_entries);

	std::cout << "Part one solution: " << _a << " * " << _b << " = " << (_a * _b) << std::endl;

	auto [_u, _v, _w] = solve_part_two(_entries);

	std::cout << "Part two solution: " << _u << " * " << _v << " * " << _w << " = " << (_u * _v * _w) << std::endl;

	return 0;
}

std::vector<uint32_t> read_entries_from_file() {
	std::fstream _expenseReport("./resources/expense_report.txt", std::fstream::in);
	std::vector<uint32_t> _entries;

	_entries.reserve(10000);

	if(_expenseReport.is_open()) {
		while(!_expenseReport.eof()) {
			uint32_t _temp;
			_expenseReport >> _temp;
			_entries.push_back(_temp);

		}
	}

	_expenseReport.close();

	return _entries;
}

std::array<uint32_t, 2> solve_part_one(const std::vector<uint32_t> &_entries) {
	std::array<uint32_t, 2> _found;

	for(auto i: _entries) {
		if(std::binary_search(_entries.begin(), _entries.end(), 2020 - i)) {
			_found[0] = i;
			_found[1] = 2020 - i;

		}
	}

	return _found;
}

std::array<uint32_t, 3> solve_part_two(const std::vector<uint32_t> &_entries) {
	std::array<uint32_t, 3> _found;

	for(auto i = _entries.begin(); i != _entries.end(); i++) {
		for(auto j = i + 1; j != _entries.end(); j++) {
			if(std::binary_search(_entries.begin(), _entries.end(), 2020 - *i - *j)) {
				_found[0] = *i;
				_found[1] = *j;
				_found[2] = 2020 - *i - *j;
			}
		}
	}

	return _found;
}