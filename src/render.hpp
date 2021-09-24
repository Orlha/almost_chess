#pragma once
#include <array>
#include <optional>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "position.hpp"
#include "event.hpp"
#include "constants.hpp"

class Render {
	public:
	using RenderBlock = std::array<Position, constants::pieces_total>;
	Render();
	std::optional<Event> access_event();
	void process_pieces(const RenderBlock& pieces);
	void display();
	void display_victory(const std::string& text);
	void finish();
	void reset();
	void select_piece(Position p);
	void select_reset();
	void play_sound();

	private:
	RenderBlock pieces;
	sf::RenderWindow window;
	sf::RenderTexture static_render;
	sf::Texture white_texture, black_texture;
	sf::RectangleShape white_sprite, black_sprite;
	sf::Sprite background;
	sf::Text text;
	sf::Font font;
	sf::CircleShape circle;
	sf::SoundBuffer sound_buffer;
	sf::Sound sound;

	void draw_board();
	void draw_pieces();
};
