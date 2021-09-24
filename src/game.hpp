#pragma once
#include <optional>
#include <variant>
#include <thread>
#include "event.hpp"
#include "field.hpp"
#include "position.hpp"
#include "board.hpp"
#include "turn.hpp"

enum class PlayerType {
	Human,
	AI,
};

template<typename Frontend, typename AI>
class Game {
	public:
	void play();

	private:
	AI ai;
	Frontend render;
	PlayerType player1 = PlayerType::Human;
	PlayerType player2 = PlayerType::AI;
	Field field;
	Cell current_turn = Cell::Red;
	std::optional<Position> selected;
	bool active = true;
	bool finished = false;

	void restart();
	PlayerType current_player() const;
	std::optional<Turn> process_step();
	void process();
};

// moved to cpp for reasability, for still requires template instantiation
#include "game.inl"
