#include "game.hpp"
#include "render.hpp"
#include "ai.hpp"

int main() {
	Game<Render,Mastermind> {}.play();
}
