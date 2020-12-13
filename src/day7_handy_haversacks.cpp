#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <map>
#include <stack>

using entry = std::pair<size_t, std::vector<std::pair<size_t, size_t>>>;

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

std::vector<entry> read_entries_from_file();
size_t solve_part_one(const std::vector<entry> &_entries);
size_t solve_part_two(const std::vector<entry> &_entries);

int main() {
	std::vector<entry> _entries = read_entries_from_file();

	auto _solutionPart1 = solve_part_one(_entries);

	std::cout << "Part 1 solution: " << _solutionPart1 << std::endl;

	auto _solutionPart2 = solve_part_two(_entries);

	std::cout << "Part 2 solution: " << _solutionPart2 << std::endl;

	return 0;
}

std::vector<entry> read_entries_from_file() {
	std::fstream _input("./resources/handy_haversacks.txt", std::fstream::in);
	std::vector<entry> _entries;
	std::map<std::string, size_t> _bagColours;
	size_t _bagCount = 0;

	_bagColours["shiny gold"] = _bagCount++;

	auto _find_bag = [&](std::string _bagName) -> size_t {
		if(_bagColours.find(_bagName) == _bagColours.end()) {
			_bagColours[_bagName] = _bagCount++;
		}
		
		return _bagColours[_bagName];
	};

	_entries.reserve(1000);

	if(_input.is_open()) {
		entry _currentEntry;
		std::string _line;
		while(std::getline(_input, _line)) {
			_currentEntry.second.clear();
			size_t _position = _line.find(" bags");
			_currentEntry.first = _find_bag(_line.substr(0, _position));
			_position = _line.find(" bags contain ");
			_line = _line.substr(_position + 14, _line.size());
			if(_line.find("no other bags") != std::string::npos) continue;
			for(auto &i: split(_line, ", ")) {
				int _count = std::stoi(i);
				i = i.substr(2, i.find(" bag") - 2);
				_currentEntry.second.emplace_back(_count, _find_bag(i));

			}
			_entries.push_back(_currentEntry);
		}
	}

	_input.close();

	return _entries;
}

size_t solve_part_one(const std::vector<entry> &_entries) {
	uint32_t _solution = 0;
	std::vector<size_t> _found;

	std::stack<size_t> _toCheck;

	_toCheck.push(0);

	while(!_toCheck.empty()) {
		size_t _current = _toCheck.top();
		_toCheck.pop();
		for(const auto &i: _entries) {
			for(const auto &j: i.second) {
				if(j.second == _current) {
					_found.push_back(i.first);
					_toCheck.push(i.first);
					break;
				}
			}
		}
	}

	std::sort(_found.begin(), _found.end());

	_found.erase(std::unique(_found.begin(), _found.end()), _found.end());

	_solution = _found.size();

	return _solution;
}

size_t solve_part_two(const std::vector<entry> &_entries) {
	size_t _solution = 0;

	std::stack<std::pair<size_t, size_t>> _toCheck;

	_toCheck.push({0, 1});

	while(!_toCheck.empty()) {
		auto [_bag, _ammount] = _toCheck.top();
		_toCheck.pop();
		for(const auto &i: _entries) {
			if(i.first == _bag) {
				for(auto j: i.second) {
					_toCheck.push({j.second, j.first * _ammount});
					_solution += _ammount * j.first;

				}
				break;
			}
		}
	}

	return _solution;
}