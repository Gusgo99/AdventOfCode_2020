#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <stack>
#include <vector>

using point2d = std::pair<int64_t, int64_t>;

point2d operator+(const point2d &_lhs, const point2d &_rhs) {
	return {_lhs.first + _rhs.first, _lhs.second + _rhs.second};
}

enum direction {E, W, SE, SW, NE, NW};

constexpr std::array<point2d, 6> NEIGHBOURS{
	point2d{2, 0},
	point2d{-2, 0},
	point2d{1, -1},
	point2d{-1, -1},
	point2d{1, 1},
	point2d{-1, 1}
};

struct input {
	std::vector<std::vector<direction>> flips;
};

std::vector<std::string> split(std::string _input, const std::string &_delimiter) {
	std::vector<std::string> _output;
	size_t _position;

	_position = _input.find(_delimiter);

	while(_position != std::string::npos) {
		_output.push_back(_input.substr(0, _position));

		_input = _input.substr(_position + _delimiter.size(), _input.size());
		
		_position = _input.find(_delimiter);
	}

	_output.push_back(_input);

	return _output;
}

input read_entries_from_file();
int64_t solve_part_one(input &_entries);
int64_t solve_part_two(input &_entries);

int main() {
	input _input = read_entries_from_file();

	auto _start = std::chrono::high_resolution_clock::now();
	auto _solutionPart1 = solve_part_one(_input);
	auto _end = std::chrono::high_resolution_clock::now();

	std::cout << "Part 1 solution: " << _solutionPart1 << std::endl;

	std::cout << "Part 1 time: " << std::chrono::duration<double>(_end - _start).count() << std::endl;

	_start = std::chrono::high_resolution_clock::now();
	auto _solutionPart2 = solve_part_two(_input);
	_end = std::chrono::high_resolution_clock::now();

	std::cout << "Part 2 solution: " << _solutionPart2 << std::endl;

	std::cout << "Part 2 time: " << std::chrono::duration<double>(_end - _start).count() << std::endl;

	return 0;
}

input read_entries_from_file() {
	std::fstream _file("./resources/lobby_layout.txt", std::fstream::in);
	input _input;

	if(_file.is_open()) {
		std::string _line;
		while(std::getline(_file, _line)) {
			std::vector<direction> _lineFlips;
			while(!_line.empty()) {
				auto _front = _line.substr(0, 2);
				if(_front == "se") {
					_lineFlips.push_back(direction::SE);
					_line = _line.substr(2);

				}
				else if(_front == "sw") {
					_lineFlips.push_back(direction::SW);
					_line = _line.substr(2);

				}
				else if(_front == "nw") {
					_lineFlips.push_back(direction::NW);
					_line = _line.substr(2);

				}
				else if(_front == "ne") {
					_lineFlips.push_back(direction::NE);
					_line = _line.substr(2);

				}
				else if(_front[0] == 'e') {
					_lineFlips.push_back(direction::E);
					_line = _line.substr(1);

				}
				else if(_front[0] == 'w') {
					_lineFlips.push_back(direction::W);
					_line = _line.substr(1);

				}
			}
			_input.flips.emplace_back(std::move(_lineFlips));
		}
	}

	_file.close();

	return _input;
}

int64_t solve_part_one(input &_input) {
	int64_t _solution = 0;
	std::map<point2d, bool> _state;

	for(auto &i: _input.flips) {
		point2d _position{INT64_MAX / 2, INT64_MAX / 2};
		for(auto j: i) {
			_position = _position + NEIGHBOURS[j];

		}
		
		_state[_position] ^= true;
	}

	for(auto i: _state) {
		_solution += i.second ? 1 : 0;

	}

	return _solution;
}

int64_t solve_part_two(input &_input) {
	uint32_t _solution = 0;
	std::map<point2d, bool> _state;
	std::map<point2d, bool> _next;
	point2d _start{0, 0};
	point2d _end{0, 0};

	for(auto &i: _input.flips) {
		point2d _position{0, 0};
		for(auto j: i) {
			_position = _position + NEIGHBOURS[j];
			
		}

		_start.first = std::min(_start.first, _position.first);
		_start.second = std::min(_start.second, _position.second);
		_end.first = std::max(_end.first, _position.first);
		_end.second = std::max(_end.second, _position.second);

		_state[_position] ^= true;
	}
	
	for(size_t i = 0; i < 100; i++) {
		_start = _start + point2d{-2, -2};
		_end = _end + point2d{2, 2};
		for(int64_t _x = _start.first; _x < _end.first; _x++) {
			for(int64_t _y = _start.second; _y < _end.second; _y++) {
				if(((_x + _y) % 2) != 0) continue;
				const point2d _position{_x, _y};
				size_t _count = 0;
				for(auto k: NEIGHBOURS) if(_state[_position + k]) _count++;
				bool _nextValue = _state[_position];
				if(_nextValue) {
					if((_count == 0) || (_count > 2)) _nextValue = false;

				}
				else {
					if(_count == 2) _nextValue = true;

				}
				_next[_position] = _nextValue;
			}
		}
		std::swap(_state, _next);
	}

	for(auto i: _state) {
		_solution += i.second ? 1 : 0;
		if(((i.first.first + i.first.second) % 2) != 0) std::cout << "Oh, oh!\n";

	}

	return _solution;
}