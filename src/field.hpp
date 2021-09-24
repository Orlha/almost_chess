#pragma once
#include "board.hpp"
#include "position.hpp"

class Field {
	public:
	Field();
	Cell& access_cell(Position pos);
	Cell access_cell(Position pos) const;
	void reset();
	Cell stucking_winner() const;
	Cell check_winner() const;
	bool move_cell(Position src, Position dst);
	const Board& operator*() const;

	private:
	Board data = { Cell::Empty };
};
