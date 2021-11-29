#include "headers.hpp"

using entry = std::tuple<uint32_t, uint32_t, char, std::string>;

std::vector<entry> read_entries_from_file();
size_t solve_part_one(const std::vector<entry> &_entries);
size_t solve_part_two(const std::vector<entry> &_entries);

int main() {
	std::vector<entry> _entries = read_entries_from_file();

	size_t _solutionPart1 = solve_part_one(_entries);

	std::cout << "Valid passwords for shopkeeper's old job: " << _solutionPart1 << std::endl;

	size_t _solutionPart2 = solve_part_two(_entries);

	std::cout << "Actual valid passwords: " << _solutionPart2 << std::endl;

	return 0;
}

std::vector<entry> read_entries_from_file() {
	std::fstream _passwords("./resources/password_philosophy.txt", std::fstream::in);
	std::vector<entry> _entries;

	_entries.reserve(1000);

	if(_passwords.is_open()) {
		uint32_t _num1, _num2;
		char _character;
		std::string _password;
		// n-n c: cccccccccccc
		while(_passwords >> _num1) {
			_passwords.get();
			_passwords >> _num2;
			_passwords >> _character;
			_passwords.get();
			_passwords >> _password;
			_entries.emplace_back(_num1, _num2, _character, _password);

		}
	}

	_passwords.close();

	return _entries;
}

size_t solve_part_one(const std::vector<entry> &_entries) {
	size_t _validCount = 0;
	for(const auto &[_min, _max, _c, _str]: _entries) {
		size_t _found = std::count(_str.begin(), _str.end(), _c);
		if((_found >= _min) && (_found <= _max)) {
			_validCount++;

		}
	}

	return _validCount;
}

size_t solve_part_two(const std::vector<entry> &_entries) {
	size_t _validCount = 0;
	for(const auto &[_position1, _position2, _c, _str]: _entries) {
		if((_str[_position1 - 1] == _c) + (_str[_position2 - 1] == _c) == 1) {
			_validCount++;

		}
	}

	return _validCount;
}