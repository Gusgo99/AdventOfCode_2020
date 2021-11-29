#include "headers.hpp"

using entry = std::vector<std::set<char>>;

std::vector<entry> read_entries_from_file();
size_t solve_part_one(const std::vector<entry> &_entries);
size_t solve_part_two(const std::vector<entry> &_entries);

int main() {
	std::vector<entry> _entries = read_entries_from_file();

	auto _solutionPart1 = solve_part_one(_entries);

	std::cout << "Highest seat ID: " << _solutionPart1 << std::endl;

	auto _solutionPart2 = solve_part_two(_entries);

	std::cout << "My seat: " << _solutionPart2 << std::endl;

	return 0;
}

std::vector<entry> read_entries_from_file() {
	std::fstream _input("./resources/custom_customs.txt", std::fstream::in);
	std::vector<entry> _entries;

	_entries.reserve(1000);

	if(_input.is_open()) {
		entry _group;
		std::string _line;
		while(!_input.eof()) {
			std::getline(_input, _line);
			if(_line == "") {
				_entries.push_back(_group);
				_group.clear();

			}
			else {
				std::set<char> _characters;
				for(auto i: _line) _characters.insert(i);
				_group.push_back(_characters);
			}
		}
	}

	_input.close();

	return _entries;
}

size_t solve_part_one(const std::vector<entry> &_entries) {
	uint32_t _maxId = 0;

	for(auto i: _entries) {
		std::set<char> _groupCharacters;
		for(auto j: i) _groupCharacters.merge(j);
		_maxId += _groupCharacters.size();

	}

	return _maxId;
}

size_t solve_part_two(const std::vector<entry> &_entries) {
	uint32_t _maxId = 0;

	for(auto i: _entries) {
		std::set<char> _groupCharacters = i.front();
		for(auto j: i) {
			std::set<char> _intersection;
			std::set_intersection(j.begin(), j.end(), _groupCharacters.begin(), _groupCharacters.end(), std::inserter(_intersection, _intersection.end()));
			_groupCharacters = _intersection;

		}
		_maxId += _groupCharacters.size();

	}

	return _maxId;
}