#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <stack>
#include <vector>
#include <set>

struct input {
	std::vector<std::set<size_t>> ingredients;
    std::vector<std::set<size_t>> allergens;
	std::vector<std::string> ingredientNames;
	std::vector<std::string> allergenNames;

	size_t find_ingredient(const std::string &_ingredient) {
		const auto _ingredientBegin = ingredientNames.begin();
		const auto _ingredientEnd = ingredientNames.end();
		const auto _foundIngredient = std::find(_ingredientBegin, _ingredientEnd, _ingredient);

		size_t _index = _foundIngredient - _ingredientBegin;

		if(_foundIngredient == _ingredientEnd) {
			ingredientNames.push_back(_ingredient);
			_index = ingredientNames.size() - 1;

		}

		return _index;
	}
	size_t find_allergen(const std::string &_allergen) {
		const auto _allergenBegin = allergenNames.begin();
		const auto _allergenEnd = allergenNames.end();
		const auto _foundAllergen = std::find(_allergenBegin, _allergenEnd, _allergen);

		size_t _index = _foundAllergen - _allergenBegin;

		if(_foundAllergen == _allergenEnd) {
			allergenNames.push_back(_allergen);
			_index = allergenNames.size() - 1;

		}

		return _index;
	}
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

template<typename T>
std::set<T> operator&(const std::set<T> &_lhs, const std::set<T> &_rhs) {
	std::set<T> _intersection;

	std::set_intersection(
		_lhs.begin(),
		_lhs.end(),
		_rhs.begin(),
		_rhs.end(),
		std::inserter(_intersection, _intersection.begin())
	);

	return _intersection;
}

template<typename T>
std::set<T>& operator&=(std::set<T> &_lhs, const std::set<T> &_rhs) {
	_lhs = _lhs & _rhs;

	return _lhs;
}

template<typename T>
std::set<T> operator|(const std::set<T> &_lhs, const std::set<T> &_rhs) {
	std::set<T> _union;

	std::set_union(
		_lhs.begin(),
		_lhs.end(),
		_rhs.begin(),
		_rhs.end(),
		std::inserter(_union, _union.begin())
	);

	return _union;
}

template<typename T>
std::set<T>& operator|=(std::set<T> &_lhs, const std::set<T> &_rhs) {
	_lhs = _lhs | _rhs;

	return _lhs;
}

bool operator<(std::pair<std::string, size_t> _lhs, std::pair<std::string, size_t> _rhs) {
	return _lhs.first < _rhs.first;
}

input read_entries_from_file();
size_t solve_part_one(input &_entries);
std::string solve_part_two(input &_entries);

int main() {
	input _input = read_entries_from_file();

	auto _start = std::chrono::high_resolution_clock::now();
	auto _solutionPart1 = solve_part_one(_input);
	auto _end = std::chrono::high_resolution_clock::now();

	std::cout << "Part 1 solution: " << _solutionPart1 << std::endl;
	std::cout << "Part 1 time: " << std::chrono::duration<double>(_end - _start).count() << std::endl;

	_start = std::chrono::high_resolution_clock::now();
	auto _solutionPart2 = solve_part_two(_input);
	_end = std::chrono::high_resolution_clock::now();

	std::cout << "Part 2 solution: " << _solutionPart2 << std::endl;
	std::cout << "Part 2 time: " << std::chrono::duration<double>(_end - _start).count() << std::endl;

	return 0;
}

input read_entries_from_file() {
	std::fstream _file("./resources/allergen_assessment.txt", std::fstream::in);
	input _input;

	if(_file.is_open()) {
		std::string _line;
		while(std::getline(_file, _line)) {
			if(_line.empty()) continue;
			std::set<size_t> _ingredients;
			std::set<size_t> _allergens;
			size_t _position = _line.find("(contains ");
			for(auto i: split(_line.substr(0, _position - 1), " ")) {
				_ingredients.insert(_input.find_ingredient(i));

			}
			_line.pop_back();
			for(auto i: split(_line.substr(_position + 10), ", ")) {
				_allergens.insert(_input.find_allergen(i));
			}
			_input.ingredients.emplace_back(std::move(_ingredients));
			_input.allergens.emplace_back(std::move(_allergens));
		}
	}

	_file.close();

	return _input;
}

size_t solve_part_one(input &_input) {
	size_t _solution{0};
	std::vector<std::set<size_t>> _possibleIngredients(_input.allergenNames.size());
	std::vector<bool> _assignedAllergens(_input.allergenNames.size());
	std::set<size_t> _matchedIngredients;

	for(size_t _ingredient = 0; _ingredient < _input.ingredients.size(); _ingredient++) {
		for(auto _allergen: _input.allergens[_ingredient]) {
			if(!_assignedAllergens[_allergen]) {
				_assignedAllergens[_allergen] = true;
				_possibleIngredients[_allergen].insert(
					_input.ingredients[_ingredient].begin(),
					_input.ingredients[_ingredient].end()
				);

			}
			else {
				_possibleIngredients[_allergen] &= _input.ingredients[_ingredient];

			}
		}
	}

	for(auto i: _possibleIngredients) {
		_matchedIngredients |= i;
	}

	for(auto &i: _input.ingredients) {
		for(auto &j: i) {
			if(_matchedIngredients.find(j) == _matchedIngredients.end()) {
				_solution++;
			}
		}
	}

	return _solution;
}

std::string solve_part_two(input &_input) {
	std::string _solution{};
	std::vector<std::set<size_t>> _possibleIngredients(_input.allergenNames.size());
	std::vector<bool> _assignedAllergens(_input.allergenNames.size());
	std::set<size_t> _matchedIngredients;

	for(size_t _ingredient = 0; _ingredient < _input.ingredients.size(); _ingredient++) {
		for(auto _allergen: _input.allergens[_ingredient]) {
			if(!_assignedAllergens[_allergen]) {
				_assignedAllergens[_allergen] = true;
				_possibleIngredients[_allergen].insert(
					_input.ingredients[_ingredient].begin(),
					_input.ingredients[_ingredient].end()
				);
			}
			else {
				_possibleIngredients[_allergen] &= _input.ingredients[_ingredient];

			}
		}
	}

	bool _repeat = true;

	while(_repeat) {
		_repeat = false;
		for(auto &_ingredients: _possibleIngredients) {
			if(_ingredients.size() == 1) {
				_matchedIngredients.insert(*_ingredients.begin());

			}
			else {
				for(auto _toRemove: _matchedIngredients & _ingredients) {
					_ingredients.erase(_toRemove);
					_repeat = true;

				}
			}
		}
	}

	std::vector<std::pair<std::string, size_t>> _allergenNamesWithIndexes;

	for(size_t _allergen = 0; _allergen < _possibleIngredients.size(); _allergen++) {
		if(!_possibleIngredients[_allergen].empty()) {
			_allergenNamesWithIndexes.emplace_back(_input.allergenNames[_allergen], _allergen);

		}
	}

	std::sort(_allergenNamesWithIndexes.begin(), _allergenNamesWithIndexes.end());

	for(auto &i: _allergenNamesWithIndexes) {
		_solution += _input.ingredientNames[*_possibleIngredients[i.second].begin()] + ",";

	}

	return _solution.substr(0, _solution.size() - 1);
}