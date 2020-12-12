#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

using entry = std::string;

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
	std::fstream _input("./resources/binary_boarding.txt", std::fstream::in);
	std::vector<entry> _entries;

	_entries.reserve(1000);

	if(_input.is_open()) {
		std::string _line;
		while(!_input.eof()) {
			std::getline(_input, _line);
			if(_line != "") _entries.push_back(_line);

		}
	}

	_input.close();

	return _entries;
}

size_t solve_part_one(const std::vector<entry> &_entries) {
	uint32_t _maxId;

	for(auto i: _entries) {
		uint32_t _id = 0;
		for(size_t j = 0; j < 10; j++) {
			_id <<= 1;
			if((i[j] == 'B') || (i[j] == 'R')) _id |= 1;
		}

		_maxId = std::max(_maxId, _id);
	}
	
	return _maxId;
}

size_t solve_part_two(const std::vector<entry> &_entries) {
	std::vector<uint32_t> _ids;
	size_t _foundId = 0;

	for(auto i: _entries) {
		uint32_t _id = 0;
		for(size_t j = 0; j < 10; j++) {
			_id <<= 1;
			if((i[j] == 'B') || (i[j] == 'R')) _id |= 1;
		}
		_ids.push_back(_id);
	}

	std::sort(_ids.begin(), _ids.end());

	for(size_t i = _ids.front(); i < _ids.back(); i++) {
		bool _isMySeatId = true;
		_isMySeatId &= std::binary_search(_ids.begin(), _ids.end(), i - 1);
		_isMySeatId &= !std::binary_search(_ids.begin(), _ids.end(), i);
		_isMySeatId &= std::binary_search(_ids.begin(), _ids.end(), i + 1);
		if(_isMySeatId) {
			_foundId = i;
			break;

		}
	}

	return _foundId;
}