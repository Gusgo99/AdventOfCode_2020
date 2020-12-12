#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using entry = std::string;

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
	std::ifstream _input("./resources/seating_system.txt");
	std::vector<entry> _entries;

	_entries.reserve(1000);

	_entries.push_back("");

	if(_input.is_open()) {
		std::string _currentEntry;
		while(std::getline(_input, _currentEntry)) {
			_entries.push_back("," + _currentEntry + ",");
		}
	}

	_input.close();
	
	_entries.push_back(_entries.back());

	std::fill(_entries.back().begin(), _entries.back().end(), ',');

	_entries.front() = _entries.back();

	return _entries;
}

int64_t solve_part_one(const std::vector<entry> &_entries) {
	int64_t _solution = 0;
	std::vector<entry> _current = _entries;
	std::vector<entry> _next = _entries;

	auto _get_sorrounding_occupied_count = [&_current](const size_t _i, const size_t _j) -> size_t {
		size_t _occupied{0};

		for(auto i: {-1, 0, 1}) {
			for(auto j: {-1, 0, 1}) {
				if((i != 0) || (j != 0)) {
					if(_current[i + _i][j + _j] == '#') _occupied++;
				}
			}
		}

		return _occupied;
	};

	bool _changed = true;

	while(_changed) {
		_changed = false;
		for(size_t i = 1; i < _entries.size() - 1; i++) {
			for(size_t j = 1; j < _entries[i].size() - 1; j++) {
				size_t _occupiedCount = _get_sorrounding_occupied_count(i, j);
				if((_current[i][j] == 'L') && (_occupiedCount == 0)) {
					_next[i][j] = '#';
					_changed = true;

				}
				else if((_current[i][j] == '#') && (_occupiedCount >= 4)) {
					_next[i][j] = 'L';
					_changed = true;

				}
			}
		}
		_current = _next;
	}

	for(auto i: _current) _solution += std::count(i.begin(), i.end(), '#');

	return _solution;
}

int64_t solve_part_two(const std::vector<entry> &_entries) {
	int64_t _solution = 0;
	std::vector<entry> _current = _entries;
	std::vector<entry> _next = _entries;

	auto _get_sorrounding_occupied_count = [&_current](const size_t _i, const size_t _j) -> size_t {
		size_t _occupied{0};

		for(auto i: {-1, 0, 1}) {
			for(auto j: {-1, 0, 1}) {
				if((i != 0) || (j != 0)) {
					for(size_t k = 1; _current[(i * k) + _i][(j * k) + _j] != ','; k++) {
						if(_current[(i * k) + _i][(j * k) + _j] == 'L') break;
						if(_current[(i * k) + _i][(j * k) + _j] == '#') {
							_occupied++;
							break;
						}
					}
				}
			}
		}

		return _occupied;
	};

	bool _changed = true;

	while(_changed) {
		_changed = false;
		for(size_t i = 1; i < _entries.size() - 1; i++) {
			for(size_t j = 1; j < _entries[i].size() - 1; j++) {
				size_t _occupiedCount = _get_sorrounding_occupied_count(i, j);
				if((_current[i][j] == 'L') && (_occupiedCount == 0)) {
					_next[i][j] = '#';
					_changed = true;

				}
				else if((_current[i][j] == '#') && (_occupiedCount >= 5)) {
					_next[i][j] = 'L';
					_changed = true;

				}
			}
		}
		_current = _next;
	}

	for(auto i: _current) _solution += std::count(i.begin(), i.end(), '#');

	return _solution;
}