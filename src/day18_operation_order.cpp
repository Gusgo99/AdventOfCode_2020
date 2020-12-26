#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>

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

using entry = std::string;

std::vector<entry> read_entries_from_file();
int64_t solve_part_one(const std::vector<entry> &_entries);
int64_t solve_part_two(const std::vector<entry> &_entries);

int main() {
	auto _entries = read_entries_from_file();
	
	auto _begin = std::chrono::high_resolution_clock::now();
	auto _solutionPart1 = solve_part_one(_entries);
	auto _end = std::chrono::high_resolution_clock::now();

	std::cout << "Part 1 solution: " << _solutionPart1 << std::endl;

	std::cout << "Time: " << std::chrono::duration<double>(_end - _begin).count() << std::endl;

	_begin = std::chrono::high_resolution_clock::now();
	auto _solutionPart2 = solve_part_two(_entries);
	_end = std::chrono::high_resolution_clock::now();

	std::cout << "Part 2 solution: " << _solutionPart2 << std::endl;

	std::cout << "Time: " << std::chrono::duration<double>(_end - _begin).count() << std::endl;

	return 0;
}

std::vector<entry> read_entries_from_file() {
	std::ifstream _input("./resources/operation_order.txt");
	std::vector<entry> _entries;

	if(_input.is_open()) {
		std::string _line;
        
        while(std::getline(_input, _line)) {
            _entries.push_back(_line);
        }
	}

	_input.close();

	return _entries;
}

int64_t solve_part_one(const std::vector<entry> &_entries) {
	int64_t _solution = 0;

	std::stack<int64_t> _values;
	std::stack<char> _operations;
	
	auto _execute_operations = [&]() {
		while(!_operations.empty()) {
			if(_operations.top() == '(') break;
			int64_t _a = _values.top();
			_values.pop();
			int64_t _b = _values.top();
			_values.pop();
			if(_operations.top() == '+') _values.push(_a + _b);
			if(_operations.top() == '*') _values.push(_a * _b);
			_operations.pop();

		}
	};

	for(auto i: _entries) {
		for(auto j: split(i, " ")) {
			if((j[0] == '+') || (j[0] == '*')) {
				_execute_operations();
				_operations.push(j[0]);
			}
			else {
				while(j[0] == '(') {
					_operations.push(j[0]);
					j = j.substr(1);

				}

				_values.push(std::stoll(j));

				while(j.back() == ')') {
					j.pop_back();
					_execute_operations();
					_operations.pop();
				}
			}
		}

		_execute_operations();
		_solution += _values.top();
		_values.pop();

	}

	return _solution;
}

int64_t solve_part_two(const std::vector<entry> &_entries) {
	int64_t _solution = 0;

	std::stack<int64_t> _values;
	std::stack<char> _operations;
	
	auto _execute_operations = [&]() {
		while(!_operations.empty()) {
			if(_operations.top() == '(') break;
			int64_t _a = _values.top();
			_values.pop();
			int64_t _b = _values.top();
			_values.pop();
			if(_operations.top() == '+') _values.push(_a + _b);
			if(_operations.top() == '*') _values.push(_a * _b);
			_operations.pop();

		}
	};

	for(auto i: _entries) {
		for(auto j: split(i, " ")) {
			if((j[0] == '+') || (j[0] == '*')) {
				if(j[0] == '*') _execute_operations();
				_operations.push(j[0]);

			}
			else {
				while(j[0] == '(') {
					_operations.push(j[0]);
					j = j.substr(1);

				}

				_values.push(std::stoll(j));

				while(j.back() == ')') {
					j.pop_back();
					_execute_operations();
					_operations.pop();

				}
			}
		}

		_execute_operations();
		_solution += _values.top();
		_values.pop();

	}

	return _solution;
}