#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <variant>

using mask_assignment = std::pair<uint64_t, uint64_t>;
using memory_assignment = std::pair<uint64_t, int64_t>;

using entry = std::variant<mask_assignment, memory_assignment>;

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
	std::ifstream _input("./resources/docking_data.txt");
	std::vector<entry> _entries;

	if(_input.is_open()) {
		std::string _line;
		while(std::getline(_input, _line)) {
			size_t _position = _line.find("mask = ");
			if(_position != std::string::npos) {
				uint64_t _andMask = 0;
				uint64_t _orMask = 0;
				for(auto i: _line.substr(7, _line.size())) {
					_andMask <<= 1;
					_orMask <<= 1;
					if(i == '0') {
						_andMask |= 1;
					}
					else if(i == '1') {
						_orMask |= 1;
					}
				}
				_entries.push_back(mask_assignment{_andMask, _orMask});
			}
			else {
				uint64_t _address = std::stoull(_line.substr(4, _line.size()));
				int64_t _value = std::stoll(_line.substr(_line.find("] = ") + 4, _line.size()));
				_entries.push_back(memory_assignment{_address, _value});
			}
		}
	}

	_input.close();

	return _entries;
}

int64_t solve_part_one(const std::vector<entry> &_entries) {
	int64_t _solution = 0;

	std::map<uint64_t, int64_t> _memory;

	uint64_t _orMask = 0;
	uint64_t _andMask = ~_orMask;

	for(auto &i: _entries) {
		if(std::holds_alternative<mask_assignment>(i)) {
			auto [_newAndMask, _newOrMask] = std::get<mask_assignment>(i);
			_andMask = _newAndMask;
			_orMask = _newOrMask;
		}
		else {
			auto [_address, _value] = std::get<memory_assignment>(i);
			_memory[_address] = _value;
			_memory[_address] |= _orMask;
			_memory[_address] &= ~_andMask;
		}
	}

	for(auto i: _memory) _solution += i.second;

	return _solution;
}

int64_t solve_part_two(const std::vector<entry> &_entries) {
	int64_t _solution = 0;
	std::vector<uint64_t> _addressesMask;

	std::map<uint64_t, int64_t> _memory;

	uint64_t _orMask = 0;
	uint64_t _andMask = ~_orMask;

	auto _update_addresses_mask = [&_addressesMask](uint64_t _range) {
		size_t _bitCount = 0;

		for(uint64_t i = _range; i != 0; _bitCount++) i &= (i - 1);

		_addressesMask.resize(1 << _bitCount);
		
		_addressesMask[0] = 0;

		for(size_t i = 1; i < (size_t{1} << _bitCount); i++) {
			_addressesMask[i] = _addressesMask[i - 1];
			uint64_t _toSum = 1;
			while(_toSum != 0) {
				while((_range & _toSum) == 0) _toSum <<= 1;
				
				uint64_t _carry = _toSum & _addressesMask[i];
				_addressesMask[i] ^= _toSum;
				_toSum = _carry << 1;
			}
		}
	};

	for(auto &i: _entries) {
		if(std::holds_alternative<mask_assignment>(i)) {
			auto [_newAndMask, _newOrMask] = std::get<mask_assignment>(i);
			_andMask = _newAndMask;
			_orMask = _newOrMask;
			_update_addresses_mask((~(_andMask | _orMask)) & ((uint64_t{1} << 36) - 1));
		}
		else {
			auto [_address, _value] = std::get<memory_assignment>(i);
			_address |= _orMask;
			_address &= _andMask | _orMask;
			for(auto j: _addressesMask) _memory[_address | j] = _value;
		}
	}

	for(auto i: _memory) _solution += i.second;

	return _solution;
}