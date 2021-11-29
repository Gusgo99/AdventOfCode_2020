#include "headers.hpp"

struct input {
	std::map<int64_t, std::vector<std::string>> images;
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

std::string min_string(const std::string &_str) {
	return std::min(
		std::string{_str.rbegin(), _str.rend()},
		_str
	);
}

std::map<std::string, std::set<int64_t>> map_border_to_id(const input &_input) {
	std::map<std::string, std::set<int64_t>> _connectionToId;
	
	for(auto &[_id, _image]: _input.images) {
		_connectionToId[min_string(_image.front())].insert(_id);
		_connectionToId[min_string(_image.back())].insert(_id);
		std::string _left, _right;
		for(auto &_line: _image) {
			_left += _line[0];
			_right += _line.back();
		}
		_connectionToId[min_string(_left)].insert(_id);
		_connectionToId[min_string(_right)].insert(_id);
	}

	return _connectionToId;
}

std::map<int64_t, std::set<std::string>> map_id_to_border(
	const std::map<std::string, std::set<int64_t>> &_borderToId
) {
	std::map<int64_t, std::set<std::string>> _idToConnection;

	for(auto &[_connection, _ids]: _borderToId) {
		for(auto &_id: _ids) {
			_idToConnection[_id].insert(_connection);
		}
	}

	return _idToConnection;
}

void transpose_image(std::vector<std::string> &_image) {
	const size_t _newRowSize = _image.size();
	const size_t _newColumnSize = _image[0].size();
	const size_t _maxSize = std::max(_newRowSize, _newColumnSize);

	_image.resize(_maxSize);
	for(auto &i: _image) i.resize(_maxSize);

	for(size_t i = 0; i < _image.size(); i++) {
		for(size_t j = i; j < _image[i].size(); j++) {
			std::swap(_image[i][j], _image[j][i]);
		}
	}

	_image.resize(_newColumnSize);
	for(auto &i: _image) i.resize(_newRowSize);
}

void transform_to_match(const std::string &_top, std::vector<std::string> &_image) {
	const std::string _minTop = min_string(_top);

	if(min_string(_image.back()) == _minTop) std::reverse(_image.begin(), _image.end());

	if(min_string(_image.front()) == _minTop) {
		if(_image.front() != _top) {
			for(auto &i: _image) {
				std::reverse(i.begin(), i.end());
			}
		}
		return;
	}
	transpose_image(_image);
	transform_to_match(_top, _image);
}

input read_entries_from_file();
uint64_t solve_part_one(const input &_entries);
uint64_t solve_part_two(input _entries);

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
	std::fstream _file("./resources/jurassic_jigsaw.txt", std::fstream::in);
	input _input;

	if(_file.is_open()) {
		int64_t _id;
		std::vector<std::string> _image;
		std::string _line;
		while(std::getline(_file, _line)) {
			if(_line.empty()) {
				_input.images[_id] = _image;
				_image.clear();

			}
			else if(_line.find("Tile") != std::string::npos) {
				_id = std::stoll(_line.substr(5));

			}
			else {
				_image.push_back(_line);

			}
		}
	}

	_file.close();

	return _input;
}

uint64_t solve_part_one(const input &_input) {
	uint64_t _solution = 1;
	std::map<int64_t, size_t> _repeatCount;
	std::map<std::string, std::set<int64_t>> _connectionToId = map_border_to_id(_input);

	for(auto &[_str, _id]: _connectionToId) {
		if(_id.size() == 1) {
			_repeatCount[*_id.begin()]++;

		}
	}

	for(auto [_id, _count]: _repeatCount) {
		if(_count == 2) _solution *= _id;

	}

	return _solution;
}

uint64_t solve_part_two(input _input) {
	auto _connectionToId = map_border_to_id(_input);
	auto _idToConnection = map_id_to_border(_connectionToId);
	std::map<int64_t, size_t> _repeatCount;

	for(auto &[_str, _id]: _connectionToId) {
		if(_id.size() == 1) {
			_repeatCount[*_id.begin()]++;

		}
	}

	// [Column][Row] cause it was easier, sadly.
	std::vector<std::vector<int64_t>> _image;

	for(auto [_id, _count]: _repeatCount) {
		if(_count == 2) {
			_image.emplace_back(std::vector{_id});

			auto &_cornerImage = _input.images[_id];
			
			if(_connectionToId[min_string(_cornerImage.front())].size() != 1) {
				std::reverse(_cornerImage.begin(), _cornerImage.end());

			}

			std::string _leftCorner;
			for(auto &i: _cornerImage) _leftCorner += i.front();

			if(_connectionToId[min_string(_leftCorner)].size() != 1) {
				for(auto &i: _cornerImage) {
					std::reverse(i.begin(), i.end());

				}
			}

			break;
		}
	}

	size_t _usedImages = 1;

	while(_input.images.size() != _usedImages) {
		int64_t _lastId = _image.back().back();
		std::string _top = _input.images[_lastId].back();

		while(_connectionToId[min_string(_top)].size() == 2) {
			auto _nextId = _connectionToId[min_string(_top)].begin();
			if(*_nextId == _lastId) _nextId++;

			_image.back().push_back(*_nextId);
			_usedImages++;

			transform_to_match(_top, _input.images[*_nextId]);

			_top = _input.images[*_nextId].back();
			_lastId = *_nextId;
		}

		if(_input.images.size() != _usedImages) {
			auto &_topLastRow = _input.images[_image.back().front()];

			std::string _side;
			for(auto i: _topLastRow) {
				_side += i.back();
			}

			auto _firstId = _connectionToId[min_string(_side)].begin();
			if(*_firstId == _image.back().front()) _firstId++;
			_image.emplace_back(std::vector{*_firstId});
			_usedImages++;

			transform_to_match(_side, _input.images[*_firstId]);
			transpose_image(_input.images[_image.back().back()]);
		}
	}

	for(auto &[_id, _img]: _input.images) {
		_img.erase(_img.begin());
		_img.pop_back();
		for(auto &i: _img) {
			i = i.substr(1, i.size() - 2);
		}
	}

	std::vector<std::string> _finalImage;

	for(size_t i = 0; i < _image.size(); i++) {
		for(size_t j = 0; j < _input.images[_image[i][0]].size(); j++) {
			_finalImage.emplace_back();
			for(size_t k = 0; k < _image.size(); k++) {
				_finalImage.back() += _input.images[_image[k][i]][j];

			}
		}
	}

	const size_t _monsterSize = 15;
	std::vector<std::string> _monster = {
		"                  # ",
		"#    ##    ##    ###",
		" #  #  #  #  #  #   "
	};

	std::set<size_t> _monsterPositions;

	for(auto inv: {0, 1, 0, 1, 0, 1, 0, 1}) {
		for(size_t i = 0; i < _finalImage.size() - _monster.size(); i++) {
			for(size_t j = 0; j < _finalImage[0].size() - _monster[0].size(); j++) {
				std::set<size_t> _matchedPositions;
				for(size_t k = 0; k < _monster.size(); k++) {
					for(size_t l = 0; l < _monster[0].size(); l++) {
						if(_finalImage[i + k][j + l] == _monster[k][l]) {
							const size_t _positionFound = (i + k) * _finalImage.size() + j + l;
							_matchedPositions.insert(_positionFound);
						}
					}
				}
				if(_matchedPositions.size() == _monsterSize) {
					_monsterPositions.insert(_matchedPositions.begin(), _matchedPositions.end());
				}
			}
		}
		
		if(inv == 0) std::reverse(_monster.begin(), _monster.end());
		else transpose_image(_monster);

	}

	uint64_t _solution = 0;

	for(auto &i: _finalImage) _solution += std::count(i.begin(), i.end(), '#');

	_solution -= _monsterPositions.size();

	return _solution;
}