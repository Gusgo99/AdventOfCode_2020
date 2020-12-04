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

	std::cout << "Valid passports found: " << _solutionPart1 << std::endl;

	auto _solutionPart2 = solve_part_two(_entries);

	std::cout << "Valid passports found: " << _solutionPart2 << std::endl;

	return 0;
}

std::vector<entry> read_entries_from_file() {
	std::fstream _input("./resources/passport_processing.txt", std::fstream::in);
	std::vector<entry> _entries;

	_entries.reserve(1000);

	if(_input.is_open()) {
		std::string _passportInfo;
		std::string _passportLine;
		while(!_input.eof()) {
			std::getline(_input, _passportLine);
			if(_passportLine == "") {
				_entries.push_back(_passportInfo);
				_passportInfo = "";

			}
			else {
				_passportInfo += " " + _passportLine;
			}
		}
	}

	_input.close();

	return _entries;
}

size_t solve_part_one(const std::vector<entry> &_entries) {
	size_t _validCount = 0;

	for(auto i: _entries) {
		bool _valid = true;
		for(auto j: {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"}) {
			_valid &= i.find(j) != std::string::npos;

		}
		if(_valid) _validCount++;
	}

	return _validCount;
}

size_t solve_part_two(const std::vector<entry> &_entries) {
	const std::array<std::string, 7> REGEXES = {
		"byr:(19[2-9][0-9]|200[0-2])",
		"iyr:20(1[0-9]|20)",
		"eyr:20(2[0-9]|30)",
		"hgt:(1([5-8][0-9]|9[0-3])cm|(59|6[0-9]|7[0-6])in)",
		"hcl:#[0-9a-f]{6}",
		"ecl:(amb|blu|brn|gry|grn|hzl|oth)",
		"pid:[0-9]{9}"
	};
	size_t _validCount = 0;
	std::regex _match;

	for(auto &i: _entries) {
		bool _valid = true;
		for(auto j: REGEXES) {
			_match = std::regex(" " + j + " ");
			_valid &= std::regex_search(" " + i + " ", _match);
		}
		if(_valid) _validCount++;
	}

	return _validCount;
}