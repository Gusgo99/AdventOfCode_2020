#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <map>
#include <stack>
#include <variant>
#include <regex>

struct input {
	std::map<int64_t, std::string> actualRules;
	std::map<int64_t, std::vector<std::vector<int64_t>>> linkedRules;
	std::vector<std::string> message;
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
size_t solve_part_one(input &_entries);
size_t solve_part_two(input &_entries);

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
	std::fstream _file("./resources/monster_message.txt", std::fstream::in);
	input _input;


	if(_file.is_open()) {
		std::string _line;
		while(std::getline(_file, _line)) {
			if(_line.empty()) continue;
			if(std::isalpha(_line[0])) {
				_input.message.push_back(_line);
			}
			else {
				auto _parts = split(_line, ":");
				int64_t _ruleNumber = std::stoll(_parts[0]);
				if(_line.find("\"") == std::string::npos) {
					for(auto i: split(_parts[1], "|")) {
						std::vector<int64_t> _partials;
						for(auto j: split(i, " ")) {
							if(!j.empty()) {
								_partials.push_back(std::stoll(j));
							}
						}
						_input.linkedRules[_ruleNumber].push_back(_partials);
					}
				}
				else {
					auto _info = split(_line, "\"");
					_input.actualRules[_ruleNumber] = _info[1];
				}
			}
		}
	}

	_file.close();

	return _input;
}

size_t solve_part_one(input &_input) {
	uint32_t _solution = 0;
	std::map<int64_t, std::string> _rules = _input.actualRules;
	std::stack<int64_t> _toCheck;

	_toCheck.push(0);

	while(!_toCheck.empty()) {
		int64_t _currentId = _toCheck.top();
		bool _completed = true;
		std::string _result;
		for(auto &i: _input.linkedRules[_currentId]) {
			if(!_result.empty()) _result += ")|(";
			for(auto j: i) {
				if(_rules[j].empty()) {
					_completed = false;
					_toCheck.push(j);
					break;

				}
				else if(_completed) {
					_result += "(" + _rules[j] + ")";

				}
			}
		}
		if(_completed) {
			_toCheck.pop();
			_rules[_currentId] = "(" + _result + ")";

		}
	}

	const std::regex _reg(_rules[0]);

	for(auto i: _input.message) {
		if(std::regex_match(i, _reg)) _solution++;

	}

	return _solution;
}

size_t solve_part_two(input &_input) {
	uint32_t _solution = 0;
	std::map<int64_t, std::string> _rules = _input.actualRules;
	std::stack<int64_t> _toCheck;

	_toCheck.push(0);

	while(!_toCheck.empty()) {
		int64_t _currentId = _toCheck.top();
		bool _completed = true;
		std::string _result;
		for(auto &i: _input.linkedRules[_currentId]) {
			if(!_result.empty()) _result += ")|(";
			for(auto j: i) {
				if(_rules[j].empty()) {
					_completed = false;
					_toCheck.push(j);
					break;

				}
				else if(_completed) {
					_result += "(" + _rules[j] + ")";

				}
			}
		}
		if(_completed) {
			_toCheck.pop();
			_rules[_currentId] = "(" + _result + ")";

			// Special cases
			if(_currentId == 8) _rules[8] = "(" + _rules[42] + "){1,}";
			if(_currentId == 11) {
					_rules[11] =
						"(((" + _rules[42] + "){1}(" + _rules[31] + "){1})"
						"|((" + _rules[42] + "){2}(" + _rules[31] + "){2})"
						"|((" + _rules[42] + "){3}(" + _rules[31] + "){3})"
						"|((" + _rules[42] + "){4}(" + _rules[31] + "){4}))";

			}

		}
	}

	const std::regex _reg(_rules[0]);

	for(auto i: _input.message) {
		if(std::regex_match(i, _reg)) _solution++;

	}

	return _solution;
}