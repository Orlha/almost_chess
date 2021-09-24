#pragma once
#include <string>
#include "constants.hpp"

enum class Cell {
	Empty,
	Red,
	Blue,
};

static std::string cell_strings[] {
	"Empty", "Red", "Blue"
};

static const std::string& to_string(Cell value) {
	return cell_strings[static_cast<std::underlying_type_t<Cell>>(value)];
}


using Board = Cell[constants::field_height * constants::field_width];
