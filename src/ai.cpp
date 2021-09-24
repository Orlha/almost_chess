#include "ai.hpp"
#include "constants.hpp"

Mastermind::Mastermind():
	rng(dev())
{}

std::vector<Position> collect_pieces(const Board& board, Cell side) {
	std::vector<Position> result;
	for(auto y = 0u; y < constants::field_height; ++y) {
		for(auto x = 0u; x < constants::field_width; ++x) {
			if(board[x * constants::field_height + y] == side) {
				result.emplace_back(x, y);
			}
		}
	}
	return result;
}

void Mastermind::recalculate_threshold(const Board& board, Cell side) {
	auto validity_x = constants::field_width;
	auto validity_y = constants::field_height;
	auto pieces = collect_pieces(board, side);

	auto low_y = 0, low_x = 0, high_y = 0, high_x = 0;
	auto low_y_leftover = 0, low_x_leftover = 0;

	for(auto p : pieces) {
		if(p.x >= constants::threshold_win_x && p.y == constants::field_height - 1) ++low_y;
		if(p.x >= constants::threshold_win_x && p.y == constants::field_height - 2) ++high_y;
		if(p.y >= constants::threshold_win_y && p.x == constants::field_width  - 1) ++low_x;
		if(p.y >= constants::threshold_win_y && p.x == constants::field_width  - 2) ++high_x;
		if(p.x <  constants::threshold_win_x && p.y == constants::field_height - 1) ++low_y_leftover;
		if(p.y <  constants::threshold_win_y && p.x == constants::field_width  - 1) ++low_x_leftover;
	}
	if(low_y == constants::pieces_count_y) {
		--validity_y;
		if(high_y == constants::pieces_count_y && low_y_leftover == 0) {
			--validity_y;
		}
	}
	if(low_x == constants::pieces_count_x) {
		--validity_x;
		if(high_x == constants::pieces_count_x && low_x_leftover == 0) {
			--validity_x;
		}
	}
	validity_threshold_x = validity_x;
	validity_threshold_y = validity_y;
}

void Mastermind::reset_threshold() {
	validity_threshold_x = constants::field_width;
	validity_threshold_y = constants::field_height;
}

std::optional<Turn> Mastermind::make_turn(const Board& board, Cell side) {
	recalculate_threshold(board, side);
	auto turn = generate_turn(board, side);
	if(!turn) {
		reset_threshold();
		turn = generate_turn(board, side);
	}
	return turn;
}

std::optional<Turn> Mastermind::generate_turn(const Board& board, Cell side) {
	std::vector<Position> pieces = collect_pieces(board, side);
	std::vector<Position> stuck_pieces;

	// remove all the pieces that do not pass the movement threshold;
	std::erase_if(pieces, [&] (Position p) {
		return (p.x >= validity_threshold_x && p.y >= constants::threshold_win_y) ||
			   (p.y >= validity_threshold_y && p.x >= constants::threshold_win_x);
	});

	while(!pieces.empty()) {
		// generate posible aggressive movements;
		auto piece_index = random(0, pieces.size() - 1);
		Position source = pieces[piece_index];
		Position right = source; ++right.x;
		Position down = source; ++down.y;
		// select one of the directions randomly;
		if(target_validity(board, right) && target_validity(board, down)) {
			auto movement_vector = random(0, 1);
			if(movement_vector == 0) {
				return Turn { source, right };
			} else {
				return Turn { source, down };
			}
		}
		if(target_validity(board, right)) return Turn { source, right };
		if(target_validity(board, down)) return Turn { source, down };
		// piece is soft-stuck if we haven't determined the turn by now
		// move the piece to the container of the stuck pieces
		stuck_pieces.push_back(source);
		pieces.erase(pieces.begin() + piece_index);
	}

	while(!stuck_pieces.empty()) {
		// generate possible defensive movements;
		auto piece_index = random(0, stuck_pieces.size() - 1);
		Position source = stuck_pieces[piece_index];
		Position left = source; --left.x;
		Position up = source; --up.y;
		// select one of the directions randomly;
		if(target_validity(board, left) && target_validity(board, up)) {
			auto movement_vector = random(0, 1);
			if(movement_vector == 0) {
				return Turn { source, left };
			} else {
				return Turn { source, up };
			}
		}
		if(target_validity(board, left)) return Turn { source, left };
		if(target_validity(board, up)) return Turn { source, up };
		// piece is hard-stuck if we haven't determined the turn by now
		// remove the piece from the selection
		stuck_pieces.erase(stuck_pieces.begin() + piece_index);
	}
	// at this point AI has failed to produce any valid turn within the specified threshold;
	return {};
}

bool Mastermind::target_validity(const Board& board, Position target) const {
	if(target.x >= validity_threshold_x ||  target.y >= validity_threshold_y) return false;
	return board[target.x * constants::field_height + target.y] == Cell::Empty;
}

int Mastermind::random(int min, int max) {
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
	return dist(rng);
}
