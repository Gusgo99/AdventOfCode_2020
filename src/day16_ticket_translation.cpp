#include "headers.hpp"

constexpr size_t FIELD_COUNT = 20;

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

using field_range = std::array<int64_t, 4>;
using ticket_fields = std::vector<int64_t>;

using entry = std::variant<field_range, ticket_fields>;

std::vector<entry> read_entries_from_file();
int64_t solve_part_one(std::vector<entry> _entries);
int64_t solve_part_two(const std::vector<entry> &_entries);

#include <chrono>

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
	std::ifstream _input("./resources/ticket_translation.txt");
	std::vector<entry> _entries;

	if(_input.is_open()) {
		std::string _line;
		for(size_t i = 0; i < FIELD_COUNT; i++) {
			std::getline(_input, _line);
			_line = _line.substr(_line.find(": ") + 2, _line.size());
			field_range _limits;
			size_t _limitPosition = 0;
			for(auto j: split(_line, " or ")) {
				_limits[_limitPosition++]  = std::stoll(j.substr(0, j.find("-")));
				_limits[_limitPosition++]  = std::stoll(j.substr(j.find("-") + 1, j.size()));

			}
			_entries.emplace_back(_limits);
		}

		std::getline(_input, _line);
		std::getline(_input, _line);
		std::getline(_input, _line);

		ticket_fields _ticket;
		for(auto i: split(_line, ",")) _ticket.push_back(std::stoll(i));
		_entries.emplace_back(_ticket);

		std::getline(_input, _line);
		std::getline(_input, _line);

		while(std::getline(_input, _line)) {
			_ticket.clear();
			for(auto i: split(_line, ",")) _ticket.push_back(std::stoll(i));
			_entries.emplace_back(_ticket);
		}
	}

	_input.close();

	return _entries;
}

int64_t solve_part_one(std::vector<entry> _entries) {
	int64_t _solution = 0;

	auto _is_field_in_range = [&](const int64_t _field, const field_range _limits) {
		return (
			((_field >= _limits[0]) && (_field <= _limits[1])) ||
			((_field >= _limits[2]) && (_field <= _limits[3]))
		);
	};

	for(size_t i = FIELD_COUNT + 1; i < _entries.size(); i++) {
		const ticket_fields &_currentTicket = std::get<ticket_fields>(_entries[i]);
		for(auto &j: _currentTicket) {
			bool _fieldIsValid = false;
			for(size_t k = 0; k < FIELD_COUNT; k++) {
				const field_range &_limits = std::get<field_range>(_entries[k]);
				if(_is_field_in_range(j, _limits)) {
					_fieldIsValid = true;

					break;
				}
			}
			if(!_fieldIsValid) _solution += j;
		}
	}

	return _solution;
}

int64_t solve_part_two(const std::vector<entry> &_entries) {
	int64_t _solution = 1;
	std::array<std::set<size_t>, FIELD_COUNT> _combinationTable;

	const ticket_fields &_myTicket = std::get<ticket_fields>(_entries[20]);

	for(auto &i: _combinationTable) {
		for(size_t j = 0; j < FIELD_COUNT; j++) {
			i.insert(j);
		}
	}

	auto _is_field_in_range = [&](const int64_t _field, const field_range _limits) {
		return (
			((_field >= _limits[0]) && (_field <= _limits[1])) ||
			((_field >= _limits[2]) && (_field <= _limits[3]))
		);
	};

	for(size_t i = FIELD_COUNT + 1; i < _entries.size(); i++) {
		const ticket_fields &_currentTicket = std::get<ticket_fields>(_entries[i]);
		auto _temp = _combinationTable;
		for(size_t j = 0; j < _currentTicket.size(); j++) {
			for(size_t k = 0; k < FIELD_COUNT; k++) {
				const field_range &_limits = std::get<field_range>(_entries[k]);
				if(!_is_field_in_range(_currentTicket[j], _limits)) {
					_combinationTable[j].erase(k);

				}
			}
		}
		for(auto j: _combinationTable) {
			if(j.empty()) {
				_combinationTable = _temp;
				break;
			}
		}
	}

	std::vector<size_t> _relation(_combinationTable.size());

	for(size_t k = 0; k < 20; k++) {
		for(size_t i = 0; i < _combinationTable.size(); i++) {
			if(_combinationTable[i].size() == 1) {
				_relation[i] = *_combinationTable[i].begin();

				for(auto &j: _combinationTable) j.erase(_relation[i]);

				break;
			}
		}
	}

	for(size_t i = 0; i < 6; i++) {
		auto _position = std::find(_relation.begin(), _relation.end(), i);
		_solution *= _myTicket[_position - _relation.begin()];

	}

	return _solution;
}