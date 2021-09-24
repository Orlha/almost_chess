#pragma once
#include <random>
#include <cstdio>
#include <optional>
#include "turn.hpp"
#include "board.hpp"
#include "constants.hpp"

class Mastermind {
	public:
	Mastermind();
	std::optional<Turn> make_turn(const Board& board, Cell side);

	private:
	void recalculate_threshold(const Board& board, Cell side);
	void reset_threshold();
	std::optional<Turn> generate_turn(const Board& board, Cell side);
	bool target_validity(const Board& board, Position target) const;
	int random(int min, int max);

	std::random_device dev;
	std::mt19937 rng;
	uint8_t validity_threshold_x = constants::field_width;
	uint8_t validity_threshold_y = constants::field_height;
};
