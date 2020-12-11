#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <map>

constexpr int64_t MAX_JOLTAGE_INCREASE = 3;

using entry = uint64_t;

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
	std::fstream _input("./resources/adapter_array.txt", std::fstream::in);
	std::vector<entry> _entries;

	_entries.reserve(1000);

    _entries.push_back(0);

	if(_input.is_open()) {
		entry _currentEntry;
		while(_input >> _currentEntry) {
			_entries.push_back(_currentEntry);
		}
	}

	_input.close();

    std::sort(_entries.begin(), _entries.end());

	_entries.push_back(_entries.back() + MAX_JOLTAGE_INCREASE);

	return _entries;
}

int64_t solve_part_one(const std::vector<entry> &_entries) {
	int64_t _solution = 0;
	int64_t _ones = 0;
	int64_t _threes = 0;

	for(size_t i = 1; i < _entries.size(); i++) {
		if((_entries[i] - _entries[i - 1]) == 1) _ones++;
		else if((_entries[i] - _entries[i - 1]) == 3) _threes++;
	}

	_solution = _ones * _threes;

	return _solution;
}

int64_t solve_part_two(const std::vector<entry> &_entries) {
	std::map<size_t, int64_t> _lookup;
	std::stack<size_t> _indexesToLook;
	int64_t _solution = 0;

	_lookup[_entries.size() - 1] = 1;

	_indexesToLook.push(0);

	while(!_indexesToLook.empty()) {
		size_t _index = _indexesToLook.top();
		bool _found = true;
		int64_t _acc = 0;
		for(size_t i = _index + 1; i < _entries.size(); i++) {
			if((_entries[i] - _entries[_index]) > MAX_JOLTAGE_INCREASE) break;
			if(_lookup.find(i) != _lookup.end()) {
				_acc += _lookup[i];
			}
			else {
				_indexesToLook.push(i);
				_found = false;
			}
		}
		if(_found) {
			_lookup[_index] = _acc;
			_indexesToLook.pop();
		}
	}

	_solution = _lookup[0];

	return _solution;
}