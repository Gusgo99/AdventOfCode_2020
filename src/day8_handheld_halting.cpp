#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>

#include <chrono>

enum class instruction {ACC, JMP, NOP};

using entry = std::pair<instruction, int64_t>;

std::vector<entry> read_entries_from_file();
int64_t solve_part_one(const std::vector<entry> &_entries);
int64_t solve_part_two(std::vector<entry> &_entries);

int main() {
	std::vector<entry> _entries = read_entries_from_file();

	auto _solutionPart1 = solve_part_one(_entries);

	std::cout << "Part 1 solution: " << _solutionPart1 << std::endl;

	auto _solutionPart2 = solve_part_two(_entries);

	std::cout << "Part 2 solution: " << _solutionPart2 << std::endl;

	return 0;
}

std::vector<entry> read_entries_from_file() {
	std::fstream _input("./resources/handheld_halting.txt", std::fstream::in);
	std::vector<entry> _entries;

	_entries.reserve(1000);

	if(_input.is_open()) {
		entry _currentEntry;
		std::string _line;
		while(std::getline(_input, _line)) {
			if(_line.find("acc") != std::string::npos) {
				_currentEntry.first = instruction::ACC;
			}
			if(_line.find("jmp") != std::string::npos) {
				_currentEntry.first = instruction::JMP;
			}
			if(_line.find("nop") != std::string::npos) {
				_currentEntry.first = instruction::NOP;
			}
			_currentEntry.second = std::stoi(_line.substr(4, _line.size()));
			_entries.push_back(_currentEntry);
		}
	}

	_input.close();

	return _entries;
}

int64_t solve_part_one(const std::vector<entry> &_entries) {
	int64_t _solution = 0;

	std::vector<size_t> _executionCount(_entries.size());

	int64_t _accumulator = 0;

	for(size_t i = 0; i < _entries.size(); i++) {
		if(++_executionCount[i] == 2) {
			_solution = _accumulator;
			break;
		}
		auto [_instruction, _number] = _entries[i];
		switch(_instruction) {
			case instruction::ACC:
				_accumulator += _number;
				break;

			case instruction::JMP:
				i += _number - 1;
				break;

			default:
				break;
		}
	}

	return _solution;
}

int64_t solve_part_two(std::vector<entry> &_entries) {
	int64_t _solution = 0;

	std::vector<size_t> _executionCount(_entries.size());
	
	int64_t _accumulator = 0;

	for(size_t j = 0; j < _entries.size(); j++) {
		_accumulator = 0;
		size_t _counter = 0;
		if(_entries[j].first == instruction::NOP) _entries[j].first = instruction::JMP;
		else if(_entries[j].first == instruction::JMP) _entries[j].first = instruction::NOP;
		else continue;
		std::fill(_executionCount.begin(), _executionCount.end(), 0);
		for(size_t i = 0; i < _entries.size(); i++) {
			if(++_counter == _entries.size()) break;
			auto [_instruction, _number] = _entries[i];
			switch(_instruction) {
				case instruction::ACC:
					_accumulator += _number;
					break;

				case instruction::JMP:
					i += _number - 1;
					break;

				default:
					break;
			}
		}
		if(_counter != _entries.size()) break;
		else if(_entries[j].first == instruction::NOP) _entries[j].first = instruction::JMP;
		else if(_entries[j].first == instruction::JMP) _entries[j].first = instruction::NOP;
	}

	_solution = _accumulator;

	return _solution;
}