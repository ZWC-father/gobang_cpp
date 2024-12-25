/*
UI structure

Main_Window
   - PVP Setting
       - PVP Window
   - PVE Setting
       - PVE Window
   - Playback Setting
       - PlayBack Window


Main_Window
 - PVP Button
 - PVE Button
 - Playback Button

PVP Setting
  - Player name
  - Board size

PVE Setting
  - Player role
  - Difficulty
  - Board size

Playback Setting
  - File selection
  - Playback mode

PVP Window
  - Board
  - Score (Based on player database)

PVE Window
  - Board
  - Score (Based on player database)

Replay Window
  - Continue Button
  - Speed Set (Pause Button)

All Sub menu has a RETURN Button
*/

#define MAX_BUFFER 128

#define BLACK_CHESS 0
#define WHITE_CHESS 1

#define BUTTON_TYPE 0
#define TEXT_TYPE 1
#define TEXT_INPUT_TYPE 2

#define MAIN_WINDOW 1
#define PVP_SETTING_WINDOW 2
#define PVE_SETTING_WINDOW 4
#define REPLAY_SETTING_WINDOW 8
#define PVP_BOARD_WINDOW 16
#define PVE_BOARD_WINDOW 32
#define REPLAY_BOARD_WINDOW 64
#define HELP_WINDOW 128
#define SETTING_WINDOW (PVP_SETTING_WINDOW | PVE_SETTING_WINDOW | REPLAY_SETTING_WINDOW)
#define BOARD_WINDOW (PVP_BOARD_WINDOW | PVE_BOARD_WINDOW | REPLAY_BOARD_WINDOW)

#include <tuple>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <functional>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"

#include "stb_image.h"

class Window{
public:
	struct Scale{
		std::pair<float, float> pos_scale;      //const
                std::pair<float, float> size_scale;     //const
                std::pair<float, float> padding_scale;  //const
                float font_scale;//font_scale (based on height)
		ImVec2 pos;
                ImVec2 size;
                ImVec2 padding;
	};
	struct Color{
		ImVec4 font_color;
		ImVec4 back_color;
                ImVec4 hovered_color;
                ImVec4 active_color;
	};
	struct Content{
		size_t size_limit;//size limit (for variable text)
		char str[MAX_BUFFER+3];
		ImFont* font;
	};
	struct Info{
		int id,type,when_load;
		bool hidden;
	};
	struct BoardInfo{
		std::pair<float, float>start_scale;
		std::pair<float, float>end_scale;
		int size;
	}board_info;
	struct Chess{
		int id; //unique
		std::pair<int, int>pos; //(refer to board_info)
		bool color;
	};
//	using pii = std::pair<int, int>;
//	using pff = std::pair<float, float>;
	using Widget = std::tuple<Info, Scale, Color, Content>;
	
	//int: size limit (for input text)
	
	std::vector<Widget>widgets;
	std::vector<Chess>chesses;
	std::pair<float, float>chess_scale;//compared with board button widget
					    //should be less than 1.0
					    //as button is fullsized

	using ButtonCallback = std::function<void(int)>;
	using BoardCallback = std::function<void(std::pair<int, int>)>;
	ButtonCallback button_callback;
	BoardCallback board_callback;

	Window(const char* name, const float ratio = 0.75, const float scale = 0.5, 
	       const float min_scale = 0.3, const float max_scale = 0.8);

	~Window();
	void imgui_init();
	void background_init(const char* filename);
	void widget_init();
	void chess_init(std::pair<float, float> scale,
			const char* filename_black, const char* filename_white);
	void board_init(BoardInfo info);
	int render(int window_loaded);
	
	void set_button_callback(ButtonCallback cb);
	void set_board_callback(BoardCallback cb);
	void add_widget(Info info, Scale scale, Color color, Content);
	void push_chess(Chess chess);
	void pop_chess(int count);
	void change_widget_size();
//	void render_board(int size,);
//	void render_setting(int type,);
//	void render_playback(int type,);
//	void resize(int pixels);
private:
	int window_w, window_h;
	int original_w, original_h;
	
	std::pair<int, int> display_size;
	
	SDL_DisplayID* displays;//Display list (only use primary in current version)
	SDL_Window* window;//SDL main window pointer
	SDL_GLContext gl_context;//OpenGL context (created by SDL)

	GLuint texture;//backgound picture
	GLuint texture_black, texture_white;//chess texture

	ImGuiWindowFlags window_flags;//ImGui setting
	ImGuiStyle style;//ImGui global style

	GLuint LoadTexture(const char* filename, int& out_width, int& out_height);
	std::pair<int,int> get_resolution();
	void render_main();
	void render_board(int window_loaded);
	void render_setting(int window_loaded);
	void load_widget(int window_loaded);
	void load_board_button_chess();
	void clear_font();

};
