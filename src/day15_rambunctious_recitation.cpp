#include "headers.hpp"

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

using entry = int64_t;

std::vector<entry> read_entries_from_file();
int64_t solve_part_one(const std::vector<entry> &_entries);
int64_t solve_part_two(const std::vector<entry> &_entries);

int main() {
	auto _entries = read_entries_from_file();
	
	auto _solutionPart1 = solve_part_one(_entries);

	std::cout << "Part 1 solution: " << _solutionPart1 << std::endl;

	auto _solutionPart2 = solve_part_two(_entries);

	std::cout << "Part 2 solution: " << _solutionPart2 << std::endl;

	return 0;
}

std::vector<entry> read_entries_from_file() {
	std::ifstream _input("./resources/rambunctious_recitation.txt");
	std::vector<entry> _entries;

	if(_input.is_open()) {
		std::string _line;
		std::getline(_input, _line);
		for(auto i: split(_line, ",")) _entries.push_back(std::stoll(i));
	}

	_input.close();

	return _entries;
}

int64_t solve_part_one(const std::vector<entry> &_entries) {
	constexpr size_t END_POSITION = 2020;
	int64_t _solution = 0;
	std::map<int64_t, size_t> _lastPositions;


	for(size_t i = 0; i < _entries.size() - 1; i++) {
		_lastPositions[_entries[i]] = i;

	}

	int64_t _last = _entries.back();
	for(size_t i = _entries.size() - 1; i < END_POSITION - 1; i++) {
		int64_t _next = 0;
		if(_lastPositions.find(_last) == _lastPositions.end()) _next = 0;
		else _next = i - _lastPositions[_last];
		_lastPositions[_last] = i;
		_last = _next;

	}
	
	_solution = _last;

	return _solution;
}

int64_t solve_part_two(const std::vector<entry> &_entries) {
	constexpr size_t END_POSITION = 30000000;
	int64_t _solution = 0;
	std::unordered_map<int64_t, size_t> _lastPositions;

	_lastPositions.reserve(END_POSITION);

	for(size_t i = 0; i < _entries.size() - 1; i++) {
		_lastPositions[_entries[i]] = i;

	}

	int64_t _last = _entries.back();
	for(size_t i = _entries.size() - 1; i < END_POSITION - 1; i++) {
		int64_t _next = 0;
		if(_lastPositions.find(_last) == _lastPositions.end()) _next = 0;
		else _next = i - _lastPositions[_last];
		_lastPositions[_last] = i;
		_last = _next;

	}
	
	_solution = _last;

	return _solution;
}