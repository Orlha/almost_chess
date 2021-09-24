#include "render.hpp"
#include <stdexcept>
#include <algorithm>
#include "constants.hpp"

sf::Color make_color(auto list) {
	auto data = std::data(list);
	return sf::Color(data[0], data[1], data[2]);
}

Render::Render():
	window(sf::VideoMode(constants::resolution_width, constants::resolution_height), "Almost Chess"),
	circle(constants::cell_outline_radius)
{
	auto load_sprite = [&] (auto& texture, auto path) {
		if(!texture.loadFromFile(path)) {
			throw std::runtime_error("File not found;");
		}
		texture.setSmooth(true);
		sf::RectangleShape shape;
		shape.setTexture(&texture);
		shape.setSize(sf::Vector2f(constants::cell_width, constants::cell_height));
		return shape;
	};
	static_render.create(constants::resolution_width, constants::resolution_height);
	white_sprite = load_sprite(white_texture, constants::white_piece_path);
	black_sprite = load_sprite(black_texture, constants::black_piece_path);
	if(!font.loadFromFile(constants::font_path)) {
		throw std::runtime_error("File not found;");
	}
	text.setFont(font);
	text.setCharacterSize(constants::font_size);
	text.setPosition(sf::Vector2f(constants::victory_text_left, constants::victory_text_top));
	text.setFillColor(sf::Color::Black);
	text.setStyle(sf::Text::Bold);
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineColor(sf::Color::Transparent);
	circle.setOutlineThickness(4);

	if(!sound_buffer.loadFromFile(constants::turn_sound_path)) {
		throw std::runtime_error("File not found;");
	}
	sound.setBuffer(sound_buffer);
	draw_board();
}

void Render::select_piece(Position p) {
	circle.setOutlineColor(make_color(constants::cell_color_outline));
	circle.setPosition(sf::Vector2f(p.x * constants::cell_width + 2.f, p.y * constants::cell_height + 2.f));
}

void Render::select_reset() {
	circle.setOutlineColor(sf::Color::Transparent);
}

void Render::play_sound() {
	sound.play();
}

std::optional<Event> Render::access_event() {
	sf::Event event;
	if(window.isOpen() && window.pollEvent(event)) {
		switch(event.type) {
			case sf::Event::Closed: {
				return events::Exit {};
			}
			case sf::Event::MouseButtonPressed: {
				if(event.mouseButton.button == sf::Mouse::Left) {
					if(event.mouseButton.x > constants::resolution_width) return {};
					if(event.mouseButton.y > constants::resolution_height) return {};
					auto cell_x = event.mouseButton.x / constants::cell_width;
					auto cell_y = event.mouseButton.y / constants::cell_height;
					Position pos(cell_x, cell_y);
					if(std::find(std::begin(pieces), std::end(pieces), pos) != std::end(pieces)) {
						return events::Select(pos);
					} else {
						return events::Move(pos);
					}
				}
				break;
			}
			case sf::Event::KeyPressed: {
				if(event.key.code == sf::Keyboard::R) {
					return events::Restart {};
				}
				break;
			}
			default: break;
		}
	}
	return {};
}

void Render::draw_board() {
	sf::RectangleShape white(sf::Vector2f(constants::cell_width, constants::cell_height));
	sf::RectangleShape black(sf::Vector2f(constants::cell_width, constants::cell_height));
	white.setFillColor(make_color(constants::cell_color_white));
	black.setFillColor(make_color(constants::cell_color_black));
	bool color = true;
	auto process_piece = [&] (auto& piece, auto x, auto y) {
		piece.setPosition(sf::Vector2f(constants::cell_width * x, constants::cell_height * y));
		static_render.draw(piece);
	};
	for(auto y = 0u; y < constants::field_height; ++y) {
		for(auto x = 0u; x < constants::field_width; ++x) {
			if(color) {
				process_piece(white, x, y);
			} else {
				process_piece(black, x ,y);
			}
			color = !color;
		}
		color = !color;
	}
	static_render.display();
	background.setTexture(static_render.getTexture());
}

void Render::process_pieces(const RenderBlock& pieces) {
	this->pieces = pieces;
}

void Render::draw_pieces() {
	auto process_piece = [&] (auto& piece, auto x, auto y) {
		piece.setPosition(sf::Vector2f(constants::cell_width * x, constants::cell_height * y));
		window.draw(piece);
	};
	for(auto i = 0u; i < pieces.size() / 2; ++i) {
		process_piece(white_sprite, pieces[i].x, pieces[i].y);
	}
	for(auto i = pieces.size() / 2; i < pieces.size(); ++i) {
		process_piece(black_sprite, pieces[i].x, pieces[i].y);
	}
}

void Render::display_victory(const std::string& text) {
	this->text.setString(text);
}

void Render::finish() {
	window.close();
}

void Render::reset() {
	this->text.setString("");
}

void Render::display() {
	window.clear();
	window.draw(background);
	draw_pieces();
	window.draw(text);
	window.draw(circle);
	window.display();
}
