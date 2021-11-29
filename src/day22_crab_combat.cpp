#include "headers.hpp"

struct input {
	std::vector<std::vector<int64_t>> decks;
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
int64_t solve_part_two(input &_entries);

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
	std::ifstream _file("./resources/crab_combat.txt");
	input _input;

	if(_file.is_open()) {
		std::string _line;
		std::vector<int64_t> _deck;
		while(std::getline(_file, _line)) {
			if(_line.empty()) {
				_input.decks.emplace_back(std::move(_deck));
				
			}
			else if(_line[0] != 'P') {
				_deck.push_back(std::stoll(_line));

			}
		}
		if(!_deck.empty()) _input.decks.emplace_back(std::move(_deck));
	}

	_file.close();

	return _input;
}

int64_t solve_part_one(input &_input) {
	int64_t _solution = 0;

	std::deque<int64_t> _p1Deque{_input.decks.front().begin(), _input.decks.front().end()};
	std::deque<int64_t> _p2Deque{_input.decks.back().begin(), _input.decks.back().end()};

	while(!_p1Deque.empty() && !_p2Deque.empty()) {
		const auto _cardP1 = _p1Deque.front();
		const auto _cardP2 = _p2Deque.front();

		_p1Deque.pop_front();
		_p2Deque.pop_front();

		if(_cardP1 > _cardP2) {
			_p1Deque.push_back(_cardP1);
			_p1Deque.push_back(_cardP2);

		}
		else {
			_p2Deque.push_back(_cardP2);
			_p2Deque.push_back(_cardP1);

		}
	}

	for(size_t i = 0; i < _p1Deque.size(); i++) _solution += (_p1Deque.size() - i) * _p1Deque[i];
	for(size_t i = 0; i < _p2Deque.size(); i++) _solution += (_p2Deque.size() - i) * _p2Deque[i];

	return _solution;
}

int64_t solve_part_two(input &_input) {
	int64_t _solution = 0;

	std::deque<int64_t> _p1Deque{_input.decks.front().begin(), _input.decks.front().end()};
	std::deque<int64_t> _p2Deque{_input.decks.back().begin(), _input.decks.back().end()};

	std::set<std::pair<std::deque<int64_t>, std::deque<int64_t>>> _playedGames;
	std::stack<std::tuple<
		std::deque<int64_t>,
		std::deque<int64_t>,
		std::set<std::pair<std::deque<int64_t>, std::deque<int64_t>>>
	>> _nonFinishedGames;

	while(!_p1Deque.empty() && !_p2Deque.empty()) {
		auto _roundState = std::pair{_p1Deque, _p2Deque};
		if(_playedGames.find(_roundState) != _playedGames.end()) {
			_p2Deque.clear();

		}
		else {
			_playedGames.insert(std::move(_roundState));

			const auto _cardP1 = _p1Deque.front();
			const auto _cardP2 = _p2Deque.front();

			if(
				(_p1Deque.size() > size_t(_cardP1)) &&
				(_p2Deque.size() > size_t(_cardP2))
			) {
				_nonFinishedGames.push({std::move(_p1Deque), std::move(_p2Deque), std::move(_playedGames)});

				auto& [_p1, _p2, _g] = _nonFinishedGames.top();

				_p1Deque = std::deque(_p1.begin(), _p1.begin() + _cardP1 + 1);
				_p2Deque = std::deque(_p2.begin(), _p2.begin() + _cardP2 + 1);

			}
			else if(_cardP1 > _cardP2) {
				_p1Deque.push_back(_cardP1);
				_p1Deque.push_back(_cardP2);

			}
			else {
				_p2Deque.push_back(_cardP2);
				_p2Deque.push_back(_cardP1);

			}
			_p1Deque.pop_front();
			_p2Deque.pop_front();
		}

		if(!_nonFinishedGames.empty() && (_p1Deque.empty() || _p2Deque.empty())) {
			bool _p1Won = _p2Deque.empty();

			auto& [_p1, _p2, _games] = _nonFinishedGames.top();

			_p1Deque = std::move(_p1);
			_p2Deque = std::move(_p2);
			_playedGames = std::move(_games);
			_nonFinishedGames.pop();

			const auto _cardP1 = _p1Deque.front();
			const auto _cardP2 = _p2Deque.front();
			_p1Deque.pop_front();
			_p2Deque.pop_front();

			if(_p1Won) {
				_p1Deque.push_back(_cardP1);
				_p1Deque.push_back(_cardP2);

			}
			else {
				_p2Deque.push_back(_cardP2);
				_p2Deque.push_back(_cardP1);

			}
		}
	}

	for(size_t i = 0; i < _p1Deque.size(); i++) _solution += (_p1Deque.size() - i) * _p1Deque[i];
	for(size_t i = 0; i < _p2Deque.size(); i++) _solution += (_p2Deque.size() - i) * _p2Deque[i];

	return _solution;
}