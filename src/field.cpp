#include "field.hpp"
#include <cassert>
#include "constants.hpp"

Field::Field() {
	reset();
}

Cell& Field::access_cell(Position pos) {
	return data[pos.x * constants::field_height + pos.y];
}

Cell Field::access_cell(Position pos) const {
	return data[pos.x * constants::field_height + pos.y];
}

void Field::reset() {
	// set all pieces to their default positions;
	Board tmp = { Cell::Empty };
	std::copy(std::begin(tmp), std::end(tmp), std::begin(data));
	for(auto y = 0u; y < 3; ++y) {
		for(auto x = 0u; x < 3; ++x) {
			access_cell(Position(x, y)) = Cell::Blue;
		}
	}
	for(auto y = constants::field_height - constants::pieces_count_y; y < constants::field_height; ++y) {
		for(auto x = constants::field_width - constants::pieces_count_x; x < constants::field_width; ++x) {
			access_cell(Position(x, y)) = Cell::Red;
		}
	}
}

Cell Field::stucking_winner() const {
	// check if any particular side has all their pieces hard-stuck
	// in which case return the opposite side as a winner;
	auto stuck_red = 0;
	auto stuck_blue = 0;

	// checks if one particular position is ready to accept a cell
	auto is_valid = [&] (Position p) {
		if(p.x >= constants::field_width || p.y >= constants::field_height) return false;
		return access_cell(p) == Cell::Empty;
	};

	// check if one piece has all their directions blocked for movement
	auto is_stuck = [&] (Position p) {
		Position sides[4] = { p, p, p, p };
		--sides[0].x;
		++sides[1].x;
		--sides[2].y;
		++sides[3].y;
		for(auto side: sides) {
			if(is_valid(side)) {
				return false;
			}
		}
		return true;
	};

	// calculate "stuckiness" of each side
	for(auto y = 0u; y < constants::field_height; ++y) {
		for(auto x = 0u; x < constants::field_width; ++x) {
			Position p(x, y);
			switch(access_cell(p)) {
				case Cell::Red: {
					if(is_stuck(p)) {
						++stuck_red;
					}
					break;
				}
				case Cell::Blue: {
					if(is_stuck(p)) {
						++stuck_blue;
					}
					break;
				}
			}
		}
	}

	// if stuck threshold is reached, another side is a winner
	if(stuck_red == constants::pieces_side) return Cell::Blue;
	if(stuck_blue == constants::pieces_side) return Cell::Red;
	// otherwise, stuck-win-condition hasn't been reached;
	return Cell::Empty;
}

Cell Field::check_winner() const {
	auto victor_red = [this] {
		for(auto y = 0u; y < constants::pieces_count_y; ++y) {
			for(auto x = 0u; x < constants::pieces_count_x; ++x) {
				if(access_cell(Position(x, y)) != Cell::Red) {
					return false;
				}
			}
		}
		return true;
	} ();
	auto victor_blue = [this] {
		for(auto y = constants::field_height - constants::pieces_count_y; y < constants::field_height; ++y) {
			for(auto x = constants::field_width - constants::pieces_count_x; x < constants::field_width; ++x) {
				if(access_cell(Position(x, y)) != Cell::Blue) {
					return false;
				}
			}
		}
		return true;
	} ();
	// if either red a blue has reached the enemy territory then they have won
	if(victor_red) {
		return Cell::Red;
	}
	if(victor_blue) {
		return Cell::Blue;
	}
	// otherwise try to decide if a stucking-victory has been reached;
	return stucking_winner();
}

bool Field::move_cell(Position src, Position dst) {
	assert(access_cell(src) != Cell::Empty && access_cell(dst) == Cell::Empty);
	auto diff = std::abs(src.x - dst.x) + std::abs(src.y - dst.y);
	if(diff > 1) return false;
	std::swap(access_cell(src), access_cell(dst));
	return true;
}

const Board& Field::operator*() const {
	return data;
}
