#include "headers.hpp"

constexpr int64_t MAGIC_NUMBER = 20201227;

struct input {
    int64_t doorPublic;
	int64_t cardPublic;
};

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

input read_entries_from_file();
int64_t solve_part_one(input &_entries);

int main() {
	input _input = read_entries_from_file();

	auto _start = std::chrono::high_resolution_clock::now();
	auto _solutionPart1 = solve_part_one(_input);
	auto _end = std::chrono::high_resolution_clock::now();

	std::cout << "Part 1 solution: " << _solutionPart1 << std::endl;

	std::cout << "Part 1 time: " << std::chrono::duration<double>(_end - _start).count() << std::endl;

	return 0;
}

input read_entries_from_file() {
	std::fstream _file("./resources/combo_breaker.txt", std::fstream::in);
	input _input;

	if(_file.is_open()) {
		std::string _line;
		std::getline(_file, _line);
		_input.cardPublic = std::stoll(_line);
		std::getline(_file, _line);
		_input.doorPublic = std::stoll(_line);
	}

	_file.close();

	return _input;
}

int64_t solve_part_one(input &_input) {
	int64_t _solution = 1;
	size_t _loopSize = 0;

	int64_t _product = 1;
	int64_t _subjectNumber = 0;
	while(true) {
		_product *= 7;
		_product %= MAGIC_NUMBER;
		_loopSize++;
		if(_product == _input.cardPublic) {
			_subjectNumber = _input.doorPublic;
			break;

		}
		else if(_product == _input.doorPublic) {
			_subjectNumber = _input.cardPublic;
			break;

		}
	}
	
	for(size_t i = 0; i < _loopSize; i++) {
		_solution *= _subjectNumber;
		_solution %= MAGIC_NUMBER;
	}

	return _solution;
}