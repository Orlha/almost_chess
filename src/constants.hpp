#pragma once

namespace constants {

static constexpr auto resolution_width = 600u;
static constexpr auto resolution_height = 600u;
static constexpr auto cell_width = 75.f;
static constexpr auto cell_height = 75.f;
static constexpr auto cell_outline_radius = 35.5f;
static constexpr auto cell_color_white = { 0x8a, 0x78, 0x5d };
static constexpr auto cell_color_black = { 0xdc, 0xd3, 0xea };
static constexpr auto cell_color_outline = { 250, 255, 148 };
static constexpr auto framerate_limit = 60u;
static constexpr auto field_width = 8u;
static constexpr auto field_height = 8u;
static constexpr auto pieces_side = 9u;
static constexpr auto pieces_total = pieces_side + pieces_side;
static constexpr auto pieces_count_x = 3;
static constexpr auto pieces_count_y = 3;
static constexpr auto font_size = 48u;
static constexpr auto victory_text_left = 100;
static constexpr auto victory_text_top  = 250;
static constexpr auto turn_delay_ms = 200u;

static constexpr auto threshold_win_x = 5;
static constexpr auto threshold_win_y = 5;

static constexpr auto white_piece_path = "resources/white.png";
static constexpr auto black_piece_path = "resources/black.png";
static constexpr auto font_path = "resources/kaushan_script.otf";
static constexpr auto turn_sound_path = "resources/turn.ogg";

}
