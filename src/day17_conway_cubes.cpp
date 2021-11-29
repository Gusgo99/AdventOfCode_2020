#include "headers.hpp"

using entry = std::string;

using point3d = std::array<int64_t, 3>;
using point4d = std::array<int64_t, 4>;

point3d operator+(const point3d &_lhs, const point3d &_rhs) {
	return {_lhs[0] + _rhs[0], _lhs[1] + _rhs[1], _lhs[2] + _rhs[2]};
}

point4d operator+(const point4d &_lhs, const point4d &_rhs) {
	return {_lhs[0] + _rhs[0], _lhs[1] + _rhs[1], _lhs[2] + _rhs[2], _lhs[3] + _rhs[3]};
}

std::vector<entry> read_entries_from_file();
int64_t solve_part_one(const std::vector<entry> &_entries);
int64_t solve_part_two(const std::vector<entry> &_entries);

int main() {
	std::vector<entry> _entries = read_entries_from_file();

	auto _startT = std::chrono::high_resolution_clock::now();
	auto _solutionPart1 = solve_part_one(_entries);
	auto _endT = std::chrono::high_resolution_clock::now();

	std::cout << "Part 1 solution: " << _solutionPart1 << std::endl;

	std::cout << "Part 1 time: " << std::chrono::duration<double>(_endT - _startT).count() << std::endl;

	_startT = std::chrono::high_resolution_clock::now();
	auto _solutionPart2 = solve_part_two(_entries);
	_endT = std::chrono::high_resolution_clock::now();

	std::cout << "Part 2 solution: " << _solutionPart2 << std::endl;

	std::cout << "Part 2 time: " << std::chrono::duration<double>(_endT - _startT).count() << std::endl;

	return 0;
}

std::vector<entry> read_entries_from_file() {
	std::ifstream _input("./resources/conway_cubes.txt");
	std::vector<entry> _entries;

	_entries.reserve(1000);

	if(_input.is_open()) {
		std::string _currentEntry;
		while(std::getline(_input, _currentEntry)) {
			_entries.push_back(_currentEntry);
		}
	}

	_input.close();

	return _entries;
}

int64_t solve_part_one(const std::vector<entry> &_entries) {
	int64_t _solution = 0;
	std::map<point3d, int> _current;
	std::map<point3d, int> _next = _current;

	point3d _start = {0, 0, 0};
	point3d _end = {int64_t(_entries.size()), int64_t(_entries[0].size()), 1};

	for(size_t i = 0; i < _entries.size(); i++) {
		for(size_t j = 0; j < _entries[i].size(); j++) {
			_current[point3d{int64_t(i), int64_t(j), 0}] = _entries[i][j] == '#';
		}
	}

	auto _get_sorrounding_occupied_count = [&_current](const point3d &_pos) -> size_t {
		size_t _occupied{0};

		for(auto i: {-1, 0, 1}) {
			for(auto j: {-1, 0, 1}) {
				for(auto k: {-1, 0, 1}) {
					if((i != 0) || (j != 0) || (k != 0)) {
						if(_current[_pos + point3d{i, j, k}]) _occupied++;

					}
				}
			}
		}

		return _occupied;
	};

	for(size_t i = 0; i < 6; i++) {
		_start = _start + point3d{-1, -1, -1};
		_end = _end + point3d{1, 1, 1};
		for(int64_t _x = _start[0]; _x < _end[0]; _x++) {
			for(int64_t _y = _start[1]; _y < _end[1]; _y++) {
				for(int64_t _z = _start[2]; _z < _end[2]; _z++) {
					size_t _count = _get_sorrounding_occupied_count({_x, _y, _z});
					bool _currentCellState = _current[{_x, _y, _z}];
					bool _nextCellState;
					if(_currentCellState) {
						if((_count != 2) && (_count != 3)) _nextCellState = false;
						else _nextCellState = true;
					}
					else {
						if(_count == 3) _nextCellState = true;
						else _nextCellState = false;

					}
					_next[{_x, _y, _z}] = _nextCellState;
				}
			}
		}
		std::swap(_current, _next);
	}

	for(auto i: _current) _solution += i.second ? 1 : 0;

	return _solution;
}

int64_t solve_part_two(const std::vector<entry> &_entries) {
	int64_t _solution = 0;
	std::map<point4d, int> _current;
	std::map<point4d, int> _next = _current;

	point4d _start = {0, 0, 0, 0};
	point4d _end = {int64_t(_entries.size()), int64_t(_entries[0].size()), 1, 1};

	for(size_t i = 0; i < _entries.size(); i++) {
		for(size_t j = 0; j < _entries[i].size(); j++) {
			_current[point4d{int64_t(i), int64_t(j), 0, 0}] = _entries[i][j] == '#';
		}
	}

	auto _get_sorrounding_occupied_count = [&_current](const point4d &_pos) -> size_t {
		size_t _occupied{0};

		for(auto i: {-1, 0, 1}) {
			for(auto j: {-1, 0, 1}) {
				for(auto k: {-1, 0, 1}) {
					for(auto l: {-1, 0, 1}) {
						if((i != 0) || (j != 0) || (k != 0) || (l != 0)) {
							if(_current[_pos + point4d{i, j, k, l}]) _occupied++;

						}
					}
				}
			}
		}

		return _occupied;
	};

	for(size_t i = 0; i < 6; i++) {
		_start = _start + point4d{-1, -1, -1, -1};
		_end = _end + point4d{1, 1, 1, 1};
		for(int64_t _x = _start[0]; _x < _end[0]; _x++) {
			for(int64_t _y = _start[1]; _y < _end[1]; _y++) {
				for(int64_t _z = _start[2]; _z < _end[2]; _z++) {
					for(int64_t _u = _start[2]; _u < _end[2]; _u++) {
						size_t _count = _get_sorrounding_occupied_count({_x, _y, _z, _u});
						bool _currentCellState = _current[{_x, _y, _z, _u}];
						bool _nextCellState;
						if(_currentCellState) {
							if((_count != 2) && (_count != 3)) _nextCellState = false;
							else _nextCellState = true;
						}
						else {
							if(_count == 3) _nextCellState = true;
							else _nextCellState = false;

						}
						_next[{_x, _y, _z, _u}] = _nextCellState;
					}
				}
			}
		}
		std::swap(_current, _next);
	}

	for(auto i: _current) _solution += i.second ? 1 : 0;

	return _solution;
}