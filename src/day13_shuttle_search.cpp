#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <optional>

using entry = std::pair<int64_t, std::vector<std::optional<int64_t>>>;

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

entry read_entries_from_file();
int64_t solve_part_one(const entry &_entries);
int64_t solve_part_two(const entry &_entries);

int main() {
	auto _entries = read_entries_from_file();

	auto _solutionPart1 = solve_part_one(_entries);

	std::cout << "Part 1 solution: " << _solutionPart1 << std::endl;

	auto _solutionPart2 = solve_part_two(_entries);

	std::cout << "Part 2 solution: " << _solutionPart2 << std::endl;

	return 0;
}

entry read_entries_from_file() {
	std::ifstream _input("./resources/shuttle_search.txt");
	entry _entries;

	if(_input.is_open()) {
		std::string _line;
		std::getline(_input, _line);
		_entries.first = std::stoi(_line);
		std::getline(_input, _line);
		int64_t _pos = 0;
		for(auto i: split(_line, ",")) {
			if(i != "x") {
				_entries.second.emplace_back(std::stoi(i));
			}
			else {
				_entries.second.emplace_back();
			}
			_pos++;
		}
		
	}

	_input.close();

	return _entries;
}

int64_t solve_part_one(const entry &_entries) {
	for(int64_t i = _entries.first; true; i++) {
		for(size_t j = 0; j < _entries.second.size(); j++) {
			if(_entries.second[j].has_value()) {
				if((i % _entries.second[j].value()) == 0) {
					return (i - _entries.first) * _entries.second[j].value();
					break;
				}
			}
		}
	}

	return -1;
}

int64_t solve_part_two(const entry &_entries) {
	int64_t _solution = 0;

	std::vector<std::array<int64_t, 2>> _equations;

	for(size_t i = 0; i < _entries.second.size(); i++) {
		if(_entries.second[i].has_value()) {
			const int64_t _busId = _entries.second[i].value();
			
			_equations.push_back({
				_busId,
				(_busId - int64_t(i)) % _busId
			});
		}
	}

	while(_equations.size() > 1) {
		auto _firstEq{_equations.back()};
		_equations.pop_back();
		auto _secondEq{_equations.back()};
		_equations.pop_back();

		if(_firstEq[0] > _secondEq[0]) std::swap(_firstEq, _secondEq);

		std::array<int64_t, 2> _resEq{_firstEq[0] * _secondEq[0], 0};

		int64_t _addedValue = _secondEq[1] - _firstEq[1];

		for(int64_t i = 0; i < _firstEq[0]; i++) {
			if(((_secondEq[0] * i + _addedValue) % _firstEq[0]) == 0) {
				_resEq[1] = (i * _secondEq[0] + _secondEq[1]) % _resEq[0];
				break;
			}
		}
		_equations.push_back(_resEq);
	}
	
	_solution = _equations[0][1];

	return _solution;
}