#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

using entry = std::string;

std::vector<entry> read_entries_from_file();
size_t solve_part_one(const std::vector<entry> &_entries);
std::array<size_t, 5> solve_part_two(const std::vector<entry> &_entries);

int main() {
	std::vector<entry> _entries = read_entries_from_file();

	auto _solutionPart1 = solve_part_one(_entries);

	std::cout << "Number of trees found on the way: " << _solutionPart1 << std::endl;

	auto _solutionPart2 = solve_part_two(_entries);

	std::cout << "Trees found in each path: ";

	size_t _productOfTrees = 1;

	for(auto i: _solutionPart2) {
		std::cout << i << ", ";
		_productOfTrees *= i;

	}

	std::cout << "\b\b.\nProduct of trees found: " << _productOfTrees;

	return 0;
}

std::vector<entry> read_entries_from_file() {
	std::fstream _input("./resources/toboggan_trajectory.txt", std::fstream::in);
	std::vector<entry> _entries;

	_entries.reserve(1000);

	if(_input.is_open()) {
		std::string _mapLine;
		while(_input >> _mapLine) {
			_entries.push_back(_mapLine);
		}
	}

	_input.close();

	return _entries;
}

size_t solve_part_one(const std::vector<entry> &_entries) {
	size_t _treeCount = 0;
	size_t _linePosition = 0;

	for(size_t i = 0; i < _entries.size(); i++) {
		if(_entries[i][_linePosition] == '#') {
			_treeCount++;

		}
		_linePosition += 3;
		if(_linePosition >= _entries[i].size()) _linePosition -= _entries[i].size();
	}

	return _treeCount;
}

std::array<size_t, 5> solve_part_two(const std::vector<entry> &_entries) {
	constexpr size_t DIRECTIONS[5][2] = {{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};
	std::array<size_t, 5> _treeCount = {0};
	size_t _linePosition = 0;

	for(size_t i = 0; i < _treeCount.size(); i++) {
		auto [_right, _down] = DIRECTIONS[i];
		_linePosition = 0;
		for(size_t j = 0; j < _entries.size(); j += _down) {
			if(_entries[j][_linePosition] == '#') {
				_treeCount[i]++;

			}
			_linePosition += _right;
			if(_linePosition >= _entries[j].size()) _linePosition -= _entries[j].size();
		}
	}

	return _treeCount;
}