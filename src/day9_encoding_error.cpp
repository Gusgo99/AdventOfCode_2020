#include "headers.hpp"

constexpr size_t PREAMBLE = 25;

using entry = int64_t;

std::vector<entry> read_entries_from_file();
int64_t solve_part_one(const std::vector<entry> &_entries);
int64_t solve_part_two(const std::vector<entry> &_entries);

int main() {
	std::vector<entry> _entries = read_entries_from_file();

	auto _solutionPart1 = solve_part_one(_entries);

	std::cout << "Part 1 solution: " << _solutionPart1 << std::endl;

	auto _solutionPart2 = solve_part_two(_entries);

	std::cout << "Part 2 solution: " << _solutionPart2 << std::endl;

	return 0;
}

std::vector<entry> read_entries_from_file() {
	std::fstream _input("./resources/encoding_error.txt", std::fstream::in);
	std::vector<entry> _entries;

	_entries.reserve(1000);

	if(_input.is_open()) {
		entry _currentEntry;
		while(_input >> _currentEntry) {
			_entries.push_back(_currentEntry);
		}
	}

	_input.close();

	return _entries;
}

int64_t solve_part_one(const std::vector<entry> &_entries) {
	int64_t _solution = 0;

	for(size_t i = PREAMBLE; i < _entries.size(); i++) {
		bool _found = false;
		for(size_t j = i - PREAMBLE; j < i; j++) {
			if(std::find(_entries.begin() + j, _entries.begin() + i, _entries[i] - _entries[j]) != (_entries.begin() + i)) {
				_found = true;
				break;
			}
		}
		if(!_found) {
			_solution = _entries[i];
			break;
		}
	}

	return _solution;
}

int64_t solve_part_two(const std::vector<entry> &_entries) {
	const int64_t FOUND_NUMBER = solve_part_one(_entries);
	int64_t _solution = 0;

	std::vector<entry> _sums = _entries;

	for(size_t i = 1; i < _entries.size(); i++) _sums[i] += _sums[i - 1];

	for(size_t i = 0; i < _entries.size(); i++) {
		if(std::binary_search(_sums.begin(), _sums.end(), FOUND_NUMBER + _sums[i])) {
			auto _beginRange = _entries.begin() + i;
			auto _endRange = std::find(_sums.begin(), _sums.end(), FOUND_NUMBER + _sums[i]);
			auto _actualEndRange = _entries.begin() + (_endRange - _sums.begin());
			_solution = *std::min_element(_beginRange, _actualEndRange) + *std::max_element(_beginRange, _actualEndRange);
			break;

		}
	}

	return _solution;
}