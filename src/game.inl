#include "constants.hpp"

// std::visit's little friend
template<typename ... Ts>
struct Overload : Ts ... { 
	using Ts::operator() ...;
};
template<class... Ts> Overload(Ts...) -> Overload<Ts...>;

template<typename Frontend, typename AI>
void Game<Frontend, AI>::restart() {
	field.reset();
	render.reset();
	current_turn = Cell::Red;
	selected = {};
	finished = false;
}

template<typename Frontend, typename AI>
PlayerType Game<Frontend, AI>::current_player() const {
	if(current_turn == Cell::Red) {
		return player1;
	} else {
		return player2;
	}
}

template<typename Frontend, typename AI>
std::optional<Turn> Game<Frontend, AI>::process_step() {
	auto event = render.access_event();
	std::optional<Turn> turn;
	if(event) {
		std::visit(Overload {
			[this](events::Exit e) {
				active = false;
			},
			[this](events::Select e) {
				// select is ignored is game over condition is reached;
				if(finished) return;
				// select is ignored if its not a human turn to rule the world;
				if(current_player() != PlayerType::Human) return;
				if(field.access_cell(e.source) == current_turn) {
					selected = e.source;
					render.select_piece(e.source);
				} else {
					selected = {};
					render.select_reset();
				}
			},
			[this, &turn](events::Move e) {
				// move is not allowed after game over
				if(finished) return;
				// move is not allowed if human epoch is in the past;
				if(current_player() != PlayerType::Human) return;
				// move is only process if the piece was previously selected;
				if(selected) {
					turn = Turn { *selected, e.target };
					selected = {};
					render.select_reset();
				}
			},
			[this](events::Restart e) {
					restart();
			}
		}, *event);
	}
	if(current_player() == PlayerType::AI) {
		turn = ai.make_turn(*field, Cell::Blue);
		if(turn) {
			std::this_thread::sleep_for(std::chrono::milliseconds(constants::turn_delay_ms));
		}
	}
	return turn;
}

template<typename Frontend, typename AI>
void Game<Frontend, AI>::process() {
	// process a single step of events, receiving either player input or ai turn in result, or none;
	auto turn = process_step();
	bool made_turn = false;
	// process a turn and switch active player;
	if(turn) {
		if(field.move_cell(turn->src, turn->dst)) {
			current_turn = current_turn == Cell::Red ? Cell::Blue : Cell::Red;
			made_turn = true;
		}
	}
	// render results;
	auto red_index = 0u;
	auto blue_index = constants::pieces_side;
	typename Frontend::RenderBlock pieces;
	for(auto y = 0u; y < constants::field_height; ++y) {
		for(auto x = 0u; x < constants::field_width; ++x) {
			Position p(x, y);
			switch(field.access_cell(p)) {
				case Cell::Red: {
					pieces[red_index++] = p;
					break;
				}
				case Cell::Blue: {
					pieces[blue_index++] = p;
					break;
				}
			}
		}
	}
	render.process_pieces(pieces);
	if(made_turn) {
		render.play_sound();
	}
	auto winner = field.check_winner();
	if(winner != Cell::Empty) {
		render.display_victory(to_string(winner) + " is the champion!");
		finished = true;
	}
	render.display();
}

template<typename Frontend, typename AI>
void Game<Frontend, AI>::play() {
	while(active) {
		process();
	}
	render.finish();
}
