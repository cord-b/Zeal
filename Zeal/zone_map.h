#pragma once
#include "hook_wrapper.h"
#include <stdint.h>
#include "EqStructures.h"
#include "EqUI.h"
#include "directx.h"

class ZoneMap
{
public:
	static constexpr float kMaxPositionSize = 0.05f;  // In fraction of screen size.
	static constexpr float kMaxMarkerSize = 0.05f;

	ZoneMap(class ZealService* zeal, class IO_ini* ini);
	~ZoneMap();

	// UI control interface.
	// Rect and sizes are in percentages of screen dimensions (0 to 100).
	// Setting update_default stores to the ini and triggers a ui update.
	bool is_enabled() const { return enabled; }
	void set_enabled(bool enable, bool update_default = false);
	bool set_background(int new_state, bool update_default = true); // [clear, dark, light, tan]
	bool set_alignment(int new_state, bool update_default = true); // [left, center, right]
	bool set_map_top(int top_percent, bool update_default = true);
	bool set_map_left(int left_percent, bool update_default = true);
	bool set_map_bottom(int bottom_percent, bool update_default = true);
	bool set_map_right(int right_percent, bool update_default = true);
	bool set_position_size(int new_size_percent, bool update_default = true);
	bool set_marker_size(int new_size_percent, bool update_default = true);
	bool set_zoom(int zoom_percent);  // Note: 100% = 1x.

	int get_background() const { return map_background_state; }
	int get_alignment() const { return map_alignment_state; }
	int get_map_top() const { return static_cast<int>(map_rect_top * 100 + 0.5f); }
	int get_map_left() const { return static_cast<int>(map_rect_left * 100 + 0.5f); }
	int get_map_bottom() const { return static_cast<int>(map_rect_bottom * 100 + 0.5f); }
	int get_map_right() const { return static_cast<int>(map_rect_right * 100 + 0.5f); }
	int get_position_size() const { return static_cast<int>(position_size / kMaxPositionSize * 100 + 0.5f); }
	int get_marker_size() const { return static_cast<int>(marker_size / kMaxMarkerSize * 100 + 0.5f); }
	int get_zoom() const { return static_cast<int>(zoom_factor * 100 + 0.5f); }

	void toggle_background();
	void toggle_zoom();
	void callback_render();

private:
	static constexpr int kInvalidZoneId = 0;
	static constexpr int kBackgroundStates = 4;  // 0 = clear.
	static constexpr int kAlignmentStates = 3; // 0 = Left, 1 = Center, 2 = Right.
	static constexpr float kDefaultRectTop = 0.1f;
	static constexpr float kDefaultRectLeft = 0.1f;
	static constexpr float kDefaultRectBottom = 0.4f;
	static constexpr float kDefaultRectRight = 0.4f;
	static constexpr float kDefaultPositionSize = 0.01f;
	static constexpr float kDefaultMarkerSize = 0.02f;

	// UI and parser methods.
	// Rect and sizes are in fractions of screen dimensions(0.f to 1.f).
	bool parse_command(const std::vector<std::string>& args);
	bool parse_shortcuts(const std::vector<std::string>& args);
	void parse_rect(const std::vector<std::string>& args);
	void parse_marker(const std::vector<std::string>& args);
	void parse_background(const std::vector<std::string>& args);
	void parse_zoom(const std::vector<std::string>& args);
	void parse_poi(const std::vector<std::string>& args);
	void set_marker(int y, int x);
	void clear_marker();
	bool set_map_rect(float top, float left, float bottom, float right, bool update_default = false);
	void update_ui_options();

	void load_ini(class IO_ini* ini);
	void dump();

	// The following methods execute as part of callback_render().
	void render_release_resources();
	void render_load_map();
	void render_map();
	int render_update_position_buffer();
	void render_update_marker_buffer();

	bool enabled = false;
	int map_background_state = 0;
	int map_alignment_state = 0;
	int zone_id = kInvalidZoneId;
	int marker_zone_id = kInvalidZoneId;
	int zoom_recenter_zone_id = kInvalidZoneId;
	int marker_x = 0;
	int marker_y = 0;
	
	float scale_factor = 0;  // Conversion factors for map data to screen coordinates.
	float zoom_factor = 1.f;
	float offset_x = 0;
	float offset_y = 0;
	float map_rect_top = kDefaultRectTop;
	float map_rect_left = kDefaultRectLeft;
	float map_rect_bottom = kDefaultRectBottom;
	float map_rect_right = kDefaultRectRight;
	float clip_rect_top = 0;
	float clip_rect_left = 0;
	float clip_rect_bottom = 0;
	float clip_rect_right = 0;
	float position_size = kDefaultPositionSize;
	float marker_size = kDefaultMarkerSize;

	int line_count = 0;  // # of primitives in line buffer.
	IDirect3DVertexBuffer8* line_vertex_buffer = nullptr;
	IDirect3DVertexBuffer8* position_vertex_buffer = nullptr;
	IDirect3DVertexBuffer8* marker_vertex_buffer = nullptr;
};


