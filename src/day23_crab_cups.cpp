#include "headers.hpp"

using input = std::string;

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
	std::fstream _file("./resources/crab_cups.txt", std::fstream::in);
	input _input;

	if(_file.is_open()) {
		std::string _line;
		std::getline(_file, _line);
		_input = _line;
	}

	_file.close();

	return _input;
}

int64_t solve_part_one(input &_input) {
	int64_t _solution = 0;
	std::string _current = _input;
	std::string _next;

	auto _one_minus = [&](char _v, const std::string _excluded) {
		while(true) {
			_v--;
			if(_v < '1') _v = '9';
			if(_excluded.find(_v) == std::string::npos) break;
		}
		size_t _position = _current.find(_v);

		return _position;
	};

	for(size_t i = 0; i < 100; i++) {
		_next = _current.substr(0, 1);
		std::string _temp = _current.substr(1, 3);
		_current = _current.substr(4);
		size_t _nextPosition = _one_minus(_next[0], _temp);
		_next += _current.substr(0, _nextPosition + 1);
		_next += _temp;
		_next += _current.substr(_nextPosition + 1);
		_next += _next[0];
		_next = _next.substr(1);
		_current = _next;

	}

	while(_current[0] != '1') {
		_current += _current[0];
		_current = _current.substr(1);
	}

	_current = _current.substr(1);
	
	_solution = std::stoll(_current);

	return _solution;
}

int64_t solve_part_two(input &_input) {
	uint64_t _solution = 1;
	std::vector<size_t> _state(1000000 + 1);
	size_t _current = _input.front() - '0';

	auto _one_minus = [&](size_t _v, const std::array<size_t, 3> &_excluded) {
		while(true) {
			_v--;
			if(_v < 1) _v = _state.size() - 1;
			if(std::find(_excluded.begin(), _excluded.end(), _v) == _excluded.end()) break;
		}

		return _v;
	};
	
	for(size_t i = 0; i < _state.size(); i++) {
		_state[i] = i + 1;
	}

	for(size_t i = 0; i < _input.size(); i++) {
		if((i + 1) < _input.size()) {
			_state[_input[i] - '0'] = _input[i + 1] - '0';
		}
		else {
			_state[_input[i] - '0'] = _input.size() + 1;
		}
	}

	_state.back() = _current;
	
	for(size_t i = 0; i < 10000000; i++) {
		std::array<size_t, 3> _temp;
		_temp[0] = _state[_current];
		_temp[1] = _state[_temp[0]];
		_temp[2] = _state[_temp[1]];

		_state[_current] = _state[_temp[2]];
		size_t _insertPosition = _one_minus(_current, _temp);
		_state[_temp[2]] = _state[_insertPosition];
		_state[_insertPosition] = _temp[0];
		_current = _state[_current];

	}

	_current = _state[1];

	for(size_t i = 0; i < 2; i++) {
		_solution *= _current;
		_current = _state[_current];
	}

	return _solution;
}