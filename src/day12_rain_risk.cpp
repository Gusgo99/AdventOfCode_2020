#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <map>

using entry = std::pair<char, int64_t>;

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
	std::fstream _input("./resources/rain_risk.txt", std::fstream::in);
	std::vector<entry> _entries;

	_entries.reserve(1000);

	if(_input.is_open()) {
		entry _currentEntry;
		while(_input >> _currentEntry.first) {
			_input >> _currentEntry.second;
			_entries.push_back(_currentEntry);
		}
	}

	_input.close();

	return _entries;
}

int64_t solve_part_one(const std::vector<entry> &_entries) {
	int64_t _solution = 0;

	int64_t _x = 0;
	int64_t _y = 0;

	int64_t _currentAngle = 0;

	auto _move_by_angle = [&](const int64_t _distance) {
	int64_t _consideredAngle = _currentAngle % 360;
	if(_consideredAngle < 0) _consideredAngle += 360;

	switch(_consideredAngle) {
		case 0:
			_x += _distance;
			break;
		case 90:
			_y += _distance;
			break;
		case 180:
			_x -= _distance;
			break;
		case 270:
			_y -= _distance;
			break;
		}
	};

	for(auto [_instruction, _parameter]: _entries) {
		switch(_instruction) {
			case 'N':
				_y += _parameter;
				break;
			case 'S':
				_y -= _parameter;
				break;
			case 'E':
				_x += _parameter;
				break;
			case 'W':
				_x -= _parameter;
				break;
			case 'L':
				_currentAngle += _parameter;
				break;
			case 'R':
				_currentAngle -= _parameter;
				break;
			case 'F':
				_move_by_angle(_parameter);
		}
	}

	_solution += std::abs(_x) + std::abs(_y);

	return _solution;
}

constexpr int64_t INITIAL_WAYPOINT_X = 10;
constexpr int64_t INITIAL_WAYPOINT_Y = 1;

int64_t solve_part_two(const std::vector<entry> &_entries) {
	int64_t _solution = 0;

	int64_t _x = 0;
	int64_t _y = 0;
	int64_t _wx = INITIAL_WAYPOINT_X;
	int64_t _wy = INITIAL_WAYPOINT_Y;

	for(auto [_instruction, _parameter]: _entries) {
		size_t _rotations = size_t(_parameter / 90);
		switch(_instruction) {
			case 'N':
				_wy += _parameter;
				break;
			case 'S':
				_wy -= _parameter;
				break;
			case 'E':
				_wx += _parameter;
				break;
			case 'W':
				_wx -= _parameter;
				break;
			case 'L':
				for(size_t j = 0; j < _rotations; j++) {
					std::swap(_wx, _wy);
					_wx = -_wx;
				}
				break;
			case 'R':
				for(size_t j = 0; j < _rotations; j++) {
					std::swap(_wx, _wy);
					_wy = -_wy;
				}
				break;
			case 'F':
				_x += _parameter * _wx;
				_y += _parameter * _wy;
				break;
		}
	}

	_solution += std::abs(_x) + std::abs(_y);

	return _solution;
}