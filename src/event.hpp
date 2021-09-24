#pragma once
#include <variant>
#include "position.hpp"

namespace events {

struct Exit {};

struct Select {
	Position source;
};

struct Move {
	Position target;
};

struct Restart {};

}

using Event = std::variant<events::Exit,events::Select,events::Move,events::Restart>;
