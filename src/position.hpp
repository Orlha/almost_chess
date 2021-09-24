#pragma once
#include <cstdint>

struct Position {
	uint8_t x;
	uint8_t y;
	bool operator==(const Position&) const = default;
};
