#define DEBUG

#ifdef DEBUG

#define Print(x) std::cerr<<(x)
#define Println(x) std::cerr<<(x)<<std::endl

#elif

#define Print(x)
#define Println(x)

#endif

#define STB_IMAGE_IMPLEMENTATION
#include "gui.hpp"
#include "file.hpp"
#include "judge.hpp"
#include "pve.hpp"
#include <thread>
#include <mutex>
#include <queue>
#include <format>
#include <ranges>

#include "widgets_table.h"

//thread <1>: gui
//thread <2>: pve
//thread <3>: file

int board[15][15];
int now_window;
int switch_window = MAIN_WINDOW;
bool running = 1;
struct LogContent{
	std::string content;
	int level, opt;
};
struct Chess{
	int x, y;
	bool color;
};
Window* window;
Log* logger;
PlayerInfo* player_info;
Judger* judge;
PVE* pve;
ReplayWrite* replay_write;
ReplayRead* replay_read;
FileSelect* file_select;
Window::BoardInfo board_info = (Window::BoardInfo){std::make_pair(0.02706f, 0.12533f),
std::make_pair(0.97294f, 0.83407f), 15};

PVE::Ans ai_ans;

std::mutex log_lock, ai_lock, replay_write_lock;
std::atomic<int> dialog_signal(0);
std::vector<Chess> chesses;
std::queue<LogContent> log_queue;
std::filesystem::path path_buff;

int now_player;
bool now_color;
int board_signal;
int ai_signal;
//-1 -> full
//1 -> player_a win
//2 -> player_b win

std::string player_a_name, player_b_name, player_human_name;
PlayerInfo::Info player_a_info, player_b_info, player_human_info;
int replay_delay;
int chess_count;
std::vector<Chess> replay_buff;
std::vector<ReplayRead::Step> replay_read_buff;
std::pair<std::string, std::string>replay_player_buff, replay_read_player_buff;
bool replay_win_buff, replay_first_buff, replay_signal;
int replay_pos;
bool replay_read_win, replay_real_win;
bool replay_ready;
std::atomic<int>ai_difficulty(0);

/*
void add_widget(){
	Window::Scale scale = {std::make_pair(0.1,0.72),std::make_pair(0.3,0.1),
                               std::make_pair(0.01,0.015),0.04,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        Window::Color color = {ImVec4(1.0f,1.0f,1.0f,1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        Window::Content content = {32, "PVP", NULL};
        Window::Info info = {0, BUTTON_TYPE, MAIN_WINDOW, 0};
        window->add_widget(info, scale, color, content);


        scale = (Window::Scale){std::make_pair(0.6,0.72),std::make_pair(0.3,0.1),
                               std::make_pair(0.01,0.015),0.04,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f,1.0f,1.0f,1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "PVE", NULL};
        info = (Window::Info){1, BUTTON_TYPE, MAIN_WINDOW, 0};
        window->add_widget(info, scale, color, content);


        scale = (Window::Scale){std::make_pair(0.1,0.3),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 0, 1.0f, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "Player A Name:", NULL};
        info = (Window::Info){2, TEXT_TYPE, PVP_SETTING_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.6,0.3),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 0, 1.0f, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "Player B Name:", NULL};
        info = (Window::Info){3, TEXT_TYPE, PVP_SETTING_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.05,0.4),std::make_pair(0.4,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){16, "", NULL};//YJX
        info = (Window::Info){4, TEXT_INPUT_TYPE, PVP_SETTING_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.55,0.4),std::make_pair(0.4,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){16, "", NULL};//WTY
        info = (Window::Info){5, TEXT_INPUT_TYPE, PVP_SETTING_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.1,0.8),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "Player A First", NULL};
        info = (Window::Info){6, BUTTON_TYPE, PVP_SETTING_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.6,0.8),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "Player B First", NULL};
        info = (Window::Info){7, BUTTON_TYPE, PVP_SETTING_WINDOW, 0};
        window->add_widget(info, scale, color, content);
	
	scale = (Window::Scale){std::make_pair(0,0),std::make_pair(0.15,0.04),
                               std::make_pair(0.01,0.01),0.025,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "<-- Back", NULL};
        info = (Window::Info){8, BUTTON_TYPE, PVP_SETTING_WINDOW, 0};
        window->add_widget(info, scale, color, content);


        scale = (Window::Scale){std::make_pair(0.38,0.2),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 0, 1.0f, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "Player Name:", NULL};
        info = (Window::Info){9, TEXT_TYPE, PVE_SETTING_WINDOW, 0};
        window->add_widget(info, scale, color, content);

	
	scale = (Window::Scale){std::make_pair(0.3,0.3),std::make_pair(0.4,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){16, "", NULL};//XHYH
        info = (Window::Info){10, TEXT_INPUT_TYPE, PVE_SETTING_WINDOW, 0};
        window->add_widget(info, scale, color, content);

	
	scale = (Window::Scale){std::make_pair(0.1,0.8),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "Player First", NULL};
        info = (Window::Info){11, BUTTON_TYPE, PVE_SETTING_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.6,0.8),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "AI First", NULL};
        info = (Window::Info){12, BUTTON_TYPE, PVE_SETTING_WINDOW, 0};
        window->add_widget(info, scale, color, content);

	
	scale = (Window::Scale){std::make_pair(0,0),std::make_pair(0.15,0.04),
                               std::make_pair(0.01,0.01),0.025,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "<-- Back", NULL};
        info = (Window::Info){13, BUTTON_TYPE, PVE_SETTING_WINDOW, 0};
        window->add_widget(info, scale, color, content);

	
	
        scale = (Window::Scale){std::make_pair(0.35,0.35),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "Select Replay File:", NULL};
        info = (Window::Info){14, BUTTON_TYPE, REPLAY_SETTING_WINDOW, 0};
        window->add_widget(info, scale, color, content);

	
	
	scale = (Window::Scale){std::make_pair(0.02,0.4),std::make_pair(0.96,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 0, 0, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){128, "", NULL};//yjx.replay
        info = (Window::Info){15, TEXT_TYPE, REPLAY_SETTING_WINDOW, 1};
        window->add_widget(info, scale, color, content);



	scale = (Window::Scale){std::make_pair(0.35,0.8),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "Replay!", NULL};
        info = (Window::Info){16, BUTTON_TYPE, REPLAY_SETTING_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0,0),std::make_pair(0.15,0.04),
                               std::make_pair(0.01,0.01),0.025,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "<-- Back", NULL};
        info = (Window::Info){17, BUTTON_TYPE, REPLAY_SETTING_WINDOW, 0};
        window->add_widget(info, scale, color, content);

	
	scale = (Window::Scale){std::make_pair(0.35,0.85),std::make_pair(0.3,0.1),
                               std::make_pair(0.01,0.015),0.04,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f,1.0f,1.0f,1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "Replay", NULL};
	info = (Window::Info){18, BUTTON_TYPE, MAIN_WINDOW, 0};
        window->add_widget(info, scale, color, content);
	

	scale = (Window::Scale){std::make_pair(0,0),std::make_pair(0.15,0.04),
                               std::make_pair(0.01,0.01),0.025,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "<-- Back", NULL};
        info = (Window::Info){19, BUTTON_TYPE, PVP_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.33,0.65),std::make_pair(0.4,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 0, 0, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "", NULL};//PVP Warning
        info = (Window::Info){20, TEXT_TYPE, PVP_SETTING_WINDOW, 1};
        window->add_widget(info, scale, color, content);

	
	scale = (Window::Scale){std::make_pair(0.07,0.04),std::make_pair(0.35,0.050),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(0, 0, 1.0f, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "", NULL};//YJX
        info = (Window::Info){21, TEXT_TYPE, PVP_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.62,0.04),std::make_pair(0.35,0.050),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(0, 0, 1.0f, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "", NULL};//WTY
        info = (Window::Info){22, TEXT_TYPE, PVP_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.05,0.09),std::make_pair(0.32,0.050),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(0, 0, 1.0f, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "", NULL};//XX Win XX Lost
        info = (Window::Info){23, TEXT_TYPE, PVP_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.62,0.09),std::make_pair(0.32,0.050),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(0, 0, 1.0f, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "", NULL};//XX Win XX Lost
        info = (Window::Info){24, TEXT_TYPE, PVP_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.47,0.05),std::make_pair(0.12,0.070),
                               std::make_pair(0.01,0.01),0.04,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 0, 0, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "<--", NULL};
        info = (Window::Info){25, TEXT_TYPE, PVP_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.1,0.85),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "Retract", NULL};
        info = (Window::Info){26, BUTTON_TYPE, PVP_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.6,0.85),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "Give up", NULL};
        info = (Window::Info){27, BUTTON_TYPE, PVP_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.37,0.9),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 0, 0, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "", NULL};//Player # Win
        info = (Window::Info){28, TEXT_TYPE, PVP_BOARD_WINDOW, 1};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.07,0.04),std::make_pair(0.35,0.050),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(0, 0, 1.0f, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "", NULL};//YJX
        info = (Window::Info){29, TEXT_TYPE, PVE_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.05,0.09),std::make_pair(0.32,0.050),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(0, 0, 1.0f, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "", NULL};//XX Win XX Lost
        info = (Window::Info){30, TEXT_TYPE, PVE_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);
	
	
	scale = (Window::Scale){std::make_pair(0.72,0.05),std::make_pair(0.2,0.06),
                               std::make_pair(0.01,0.01),0.04,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(0, 0, 1.0f, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "AI", NULL};
        info = (Window::Info){31, TEXT_TYPE, PVE_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.47,0.05),std::make_pair(0.12,0.070),
                               std::make_pair(0.01,0.01),0.04,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 0, 0, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "<--", NULL};
        info = (Window::Info){32, TEXT_TYPE, PVE_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.1,0.85),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "Retract", NULL};
        info = (Window::Info){33, BUTTON_TYPE, PVE_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.6,0.85),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "Give up", NULL};
        info = (Window::Info){34, BUTTON_TYPE, PVE_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0,0),std::make_pair(0.15,0.04),
                               std::make_pair(0.01,0.01),0.025,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "<-- Back", NULL};
        info = (Window::Info){35, BUTTON_TYPE, PVE_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.37,0.9),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 0, 0, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "", NULL};//Player # Win
        info = (Window::Info){36, TEXT_TYPE, PVE_BOARD_WINDOW, 1};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.33,0.65),std::make_pair(0.4,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 0, 0, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "", NULL};//PVE Warning
        info = (Window::Info){37, TEXT_TYPE, PVE_SETTING_WINDOW, 1};
        window->add_widget(info, scale, color, content);

//-------------------------------------------------------------------------------------------------

	scale = (Window::Scale){std::make_pair(0.07,0.04),std::make_pair(0.35,0.050),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(0, 0, 1.0f, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "", NULL};//YJX
        info = (Window::Info){38, TEXT_TYPE, REPLAY_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.62,0.04),std::make_pair(0.35,0.050),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(0, 0, 1.0f, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "", NULL};//WTY
        info = (Window::Info){39, TEXT_TYPE, REPLAY_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.05,0.09),std::make_pair(0.32,0.050),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(0, 0, 1.0f, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "", NULL};//XX Win XX Lost
        info = (Window::Info){40, TEXT_TYPE, REPLAY_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.62,0.09),std::make_pair(0.32,0.050),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(0, 0, 1.0f, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "", NULL};//XX Win XX Lost
        info = (Window::Info){41, TEXT_TYPE, REPLAY_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.47,0.05),std::make_pair(0.12,0.070),
                               std::make_pair(0.01,0.01),0.04,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 0, 0, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "<--", NULL};
        info = (Window::Info){42, TEXT_TYPE, REPLAY_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.1,0.85),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "Pre Step", NULL};
        info = (Window::Info){43, BUTTON_TYPE, REPLAY_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.6,0.85),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "Next Step", NULL};
        info = (Window::Info){44, BUTTON_TYPE, REPLAY_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


	scale = (Window::Scale){std::make_pair(0.3,0.9),std::make_pair(0.4,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 0, 0, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "", NULL};//Player # Win
        info = (Window::Info){45, TEXT_TYPE, REPLAY_BOARD_WINDOW, 1};
        window->add_widget(info, scale, color, content);


        scale = (Window::Scale){std::make_pair(0,0),std::make_pair(0.15,0.04),
                               std::make_pair(0.01,0.01),0.025,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "<-- Back", NULL};
        info = (Window::Info){46, BUTTON_TYPE, REPLAY_BOARD_WINDOW, 0};
        window->add_widget(info, scale, color, content);


//---------------------------------------------------------------------------------------------

        scale = (Window::Scale){std::make_pair(0.3,0.4),std::make_pair(0.25,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 0, 1.0f, 1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){32, "Difficulty(1~4):", NULL};
        info = (Window::Info){47, TEXT_TYPE, PVE_SETTING_WINDOW, 0};
        window->add_widget(info, scale, color, content);

        
	scale = (Window::Scale){std::make_pair(0.6,0.4),std::make_pair(0.05,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        color = (Window::Color){ImVec4(1.0f, 1.0f, 1.0f, 1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};
        content = (Window::Content){1, "", NULL};
        info = (Window::Info){48, TEXT_INPUT_TYPE, PVE_SETTING_WINDOW, 0};
        window->add_widget(info, scale, color, content);
}
*/

void push_log(const std::string str,int level, int opt){
	log_lock.lock();
	log_queue.emplace(str, level, opt);
	log_lock.unlock();
	
}

void push_log(const char* str, int level, int opt){
	log_lock.lock();
	log_queue.emplace(std::string(str), level, opt);
	log_lock.unlock();
}

std::string add_space(int cnt, const char* tmp){
	return std::format("{0}{1}", std::string(cnt, ' '), tmp);
}

std::string string_center(std::string str,int len){
	if(str.length()>=len)return str;
	int cnt = (len - str.length()) / 2;
	return std::format("{0}{1}", std::string(cnt, ' '), str);
}

void clean_path_buff(){
	replay_read_player_buff.first.clear(), replay_read_player_buff.second.clear();
	path_buff.clear();
}

std::pair<int, int> matrix_convert(std::pair<int, int> pos){
	return std::make_pair(pos.second, pos.first);
}

void change_player(){
	now_color = !now_color;
	now_player = 3 - now_player;
	
	int id;
	if(now_window == PVP_BOARD_WINDOW)id = 25;
	else if(now_window == PVE_BOARD_WINDOW)id = 32;
	else id = 42;

	if(now_player == 1)strcpy(std::get<Window::Content>(window->widgets[id]).str, "<--");
	else strcpy(std::get<Window::Content>(window->widgets[id]).str, "-->");

	push_log("Player Changed", LEVEL_INFO, CONSOLE_LOG);
}


void clear_str_buffer(int widget_id){
	memset(std::get<Window::Content>(window->widgets[widget_id]).str, 0,
	sizeof std::get<Window::Content>(window->widgets[widget_id]).str);
}


void show_warning(bool opt, int widget_id, const std::string& content){
	memset(std::get<Window::Content>(window->widgets[widget_id]).str, 0, sizeof std::get<Window::Content>(window->widgets[widget_id]).str);
	strncpy(std::get<Window::Content>(window->widgets[widget_id]).str, content.c_str(), content.length());
	std::get<Window::Info>(window->widgets[widget_id]).hidden = !opt;
}


std::string convert_player_info(PlayerInfo::Info& info, int len){
//	if(info.total == -1)return string_center(std::string("Win: 0, Lose: 0"), len);
	if(info.total == -1)info.total = info.win = info.lose = 0;
	std::string tmp;
	tmp = "Win: ";
	if(info.win > 99)tmp += "99+";
	else tmp += std::to_string(info.win);
	tmp += ", Lose: ";
	if(info.lose > 99)tmp += "99+";
	else tmp += std::to_string(info.lose);
	return string_center(tmp, len);
}

void retract(){
	judge->undo(chesses.back().x, chesses.back().y);//matrix convert
	window->pop_chess(1);
	chesses.pop_back();
	change_player();
}

void log_player_info_update(std::string name, PlayerInfo::Info info){
        push_log(std::string("Update Player Info: ") + name +
        std::string(", Total: ") + std::to_string(info.total) +
        std::string(", Win: ") + std::to_string(info.win) +
        std::string(", Lose: " + std::to_string(info.lose)),
        LEVEL_INFO, CONSOLE_LOG);
}

void update_replay(int window_loaded){
	replay_write_lock.lock();
	replay_buff.clear();
	replay_buff = chesses;
	if(window_loaded == PVP_BOARD_WINDOW)replay_player_buff = std::make_pair(player_a_name, player_b_name);
	else replay_player_buff = std::make_pair(player_human_name, std::string("AI"));
	if(board_signal > 0)replay_win_buff = 1;//TODO
	else replay_win_buff = 0;
	replay_first_buff = (now_player & 1) ^ ((replay_buff.size() % 2) ^ 1);
	replay_signal = 1;
	replay_write_lock.unlock();
	push_log("Replay Info Updated", LEVEL_INFO, CONSOLE_LOG);
}

void update_database(int winner){
	//std::cout<<player_a_info.total<<" "<<player_b_info.total<<std::endl;
	if(now_window == PVP_BOARD_WINDOW){
		if(winner == 1 || winner == 100) player_a_info.win++, player_b_info.lose++;
		else if(winner == 2 || winner == 200)player_b_info.win++, player_a_info.lose++;
		player_a_info.total++, player_b_info.total++;
		player_info->upsert(player_a_info), player_info->upsert(player_b_info);
	}else{
		if(winner == 1 || winner == 100)player_human_info.win++;
		else if(winner == 2 || winner == 200)player_human_info.lose++;
		player_human_info.total++;
		player_info->upsert(player_human_info);
	}
	if(now_window == PVP_BOARD_WINDOW){
		log_player_info_update(player_a_info.name, player_a_info);
		log_player_info_update(player_b_info.name, player_b_info);
	}else{
		log_player_info_update(player_human_info.name, player_human_info);
	}

}

void clean_pvp_board(){
	for(int i = 21; i <= 24; i++)clear_str_buffer(i);
	chesses.clear();
	player_a_name = player_b_name = "";
	player_a_info = player_b_info = {"",0,0,0};
	now_color = board_signal = 0;
	window->chesses.clear();
	if(judge != nullptr){
		delete judge;
		judge = nullptr;
	}
	push_log("PVP Board Info Cleaning Done", LEVEL_INFO, CONSOLE_LOG);
}

void clean_pve_board(){
	for(int i = 29; i <= 30; i++)clear_str_buffer(i);
	chesses.clear();
	player_human_name = "";
	player_human_info = {"",0,0,0};
	now_color = board_signal = ai_signal = 0;
	window->chesses.clear();
	if(judge != nullptr){
		delete judge;
		judge = nullptr;
	}
	push_log("PVE Board Info Cleaning Done", LEVEL_INFO, CONSOLE_LOG);
}

void clean_replay_board(){
	for(int i = 38; i <= 41; i++)clear_str_buffer(i);
	chesses.clear();
	player_a_name = player_b_name = "";
	player_a_info = player_b_info = {"",0,0,0};
	now_color = board_signal = ai_signal = 0;
	window->chesses.clear();
	replay_pos = 0;
	show_warning(0, 45, "");
	push_log("Replay Board Info Cleaning Done", LEVEL_INFO, CONSOLE_LOG);
}

void show_replay_info(){
	if(replay_real_win){
		if(now_player == 1)show_warning(1, 45, add_space(9, "Player A Win").c_str());
		else{
			if(player_b_name == "AI")show_warning(1, 45, add_space(16, "AI Win").c_str());
			else show_warning(1, 45, add_space(9, "Player B Win").c_str());
		}
	}else if(replay_read_win){
		if(now_player == 2 || player_b_name == "AI")show_warning(1, 45, add_space(5, "Player A Give up"));
		else show_warning(1, 45, add_space(5, "Player B Give up"));
	}else if(replay_read_buff.size() == 225){
		show_warning(1, 45, add_space(7, "Board is Full"));
	}else{
		show_warning(1, 45, "No One Win the Game");
	}

}


void show_ai_message(bool opt){
	strcpy(std::get<Window::Content>(window->widgets[36]).str, " AI Thinking...");
	std::get<Window::Info>(window->widgets[36]).hidden = !opt;
}

bool check_real_win(){
	judge = new Judger(15);
	for(auto& it:replay_read_buff){
		judge->update(it.x, it.y, it.player);
		auto sta = judge->judge(it.x, it.y, it.player);
		if(~sta.first){
			if(judge != nullptr){
				delete judge;
				judge = nullptr;
			}
			return 1;
		}
	}
	if(judge != nullptr){
		delete judge;
		judge = nullptr;
	}
	return 0;
}

void replay_next_step(){
	if(replay_pos >= replay_read_buff.size())return;
//	std::cout<<replay_pos<<std::endl;
	std::pair<int, int> ans(replay_read_buff[replay_pos].x, replay_read_buff[replay_pos].y);
	std::pair<int, int> r_ans = matrix_convert(ans);
	Window::Chess chess = (Window::Chess){chess_count++, r_ans, now_color};
	window->push_chess(chess);
	chesses.emplace_back(ans.first, ans.second, now_color);
	push_log(std::string("Replay Next Step: (") + std::to_string(ans.first) + ',' +
		 std::to_string(ans.second) + ')', LEVEL_INFO, CONSOLE_LOG);
	if(replay_pos == replay_read_buff.size() - 1){
		show_replay_info();
		std::get<Window::Info>(window->widgets[42]).hidden = 1;
	}
	change_player();
	replay_pos++;
}

void replay_pre_step(){
	if(replay_pos < 1)return;
	show_warning(0, 45, "");
	push_log("Replay Previous Step", LEVEL_INFO, CONSOLE_LOG);
	std::get<Window::Info>(window->widgets[42]).hidden = 0;
	replay_pos--;
	window->pop_chess(1);
	chesses.pop_back();
	change_player();
}

void click(int bt){//注意：回调函数可能的顺序
	push_log(std::string("Button Click: ") + std::to_string(bt), LEVEL_INFO, CONSOLE_LOG);
	if(bt == 0){
		switch_window = PVP_SETTING_WINDOW;
	}else if(bt == 1){
		switch_window = PVE_SETTING_WINDOW;
	}else if(bt == 8){
		switch_window = MAIN_WINDOW;
	}else if(bt == 13){
		switch_window = MAIN_WINDOW;
	}else if(bt == 18){
		switch_window = REPLAY_SETTING_WINDOW;
	}else if(bt == 17){
		if(dialog_signal.load())return;
		switch_window = MAIN_WINDOW;
	}else if(bt == 6 || bt == 7){
		switch_window = PVP_BOARD_WINDOW;
		now_player = (bt == 6 ? 1 : 2);
	}else if(bt == 19){
		if(chesses.size() && !replay_signal){//TODO:replay_signal should be thread safe
			update_replay(PVP_BOARD_WINDOW);
		}
		update_database(board_signal);
		clean_pvp_board();
		switch_window = MAIN_WINDOW;//TODO: Do some clean or update before exit
	}else if(bt == 26){
		if(board_signal || chesses.empty())return;
		retract();
		push_log("Retract a Chess", LEVEL_INFO, CONSOLE_LOG);
	}else if(bt == 27){
		if(board_signal)return;
		board_signal = 3 - now_player;
	}else if(bt == 11 || bt == 12){
		switch_window = PVE_BOARD_WINDOW;
		now_player = (bt == 11 ? 1 : 2);
	}else if(bt == 35){
		ai_lock.lock();
		if(ai_signal == 2)ai_signal = -1;
		else if(ai_signal == 1 || ai_signal == 3)ai_signal = 0;
		ai_lock.unlock();
		if(chesses.size() && !replay_signal){
			update_replay(PVE_BOARD_WINDOW);
		}
		update_database(board_signal);
		clean_pve_board();
		switch_window = MAIN_WINDOW;
	}else if(bt == 34){
		if(board_signal)return;
		ai_lock.lock();
		if(ai_signal == 2)ai_signal = -1;
		else if(ai_signal == 1 || ai_signal == 3)ai_signal = 0;
		ai_lock.unlock();
		board_signal = 2;
	}else if(bt == 33){
		if(board_signal || chesses.size() < 2)return;
		ai_lock.lock();
		if(ai_signal == 2){
			ai_signal = -1;
			retract();
			push_log("Retract a Chess", LEVEL_INFO, CONSOLE_LOG);
		}else if(ai_signal == 1 || ai_signal == 3){
			ai_signal = 0;
			retract();
			push_log("Retract a Chess", LEVEL_INFO, CONSOLE_LOG);
		}else{
			retract();
			retract();
			push_log("Retract Two Chesses", LEVEL_INFO, CONSOLE_LOG);
		}
		ai_lock.unlock();
		show_ai_message(0);
	}else if(bt == 14){
		if(dialog_signal.load())return;
		clean_path_buff();
		dialog_signal.store(1);
	}else if(bt == 16){
		if(dialog_signal.load() || !replay_ready)return;
		switch_window = REPLAY_BOARD_WINDOW;
	}else if(bt == 46){
		clean_replay_board();
		switch_window = MAIN_WINDOW;
	}else if(bt == 43){
		replay_pre_step();
	}else if(bt == 44){
		replay_next_step();
	}
}

//GUI与主程序，PVE和Judge必须变换坐标

void show_win_message(int winner){
	if(now_window == PVP_BOARD_WINDOW){
		if(winner == 1)strcpy(std::get<Window::Content>(window->widgets[28]).str,
		add_space(4, "Player A Win").c_str()), push_log("Player A Win", LEVEL_INFO, CONSOLE_LOG);
		else strcpy(std::get<Window::Content>(window->widgets[28]).str,
		add_space(4, "Player B Win").c_str()), push_log("Player B Win", LEVEL_INFO, CONSOLE_LOG);
		std::get<Window::Info>(window->widgets[28]).hidden = 0;
	}else{
		if(winner == 1)strcpy(std::get<Window::Content>(window->widgets[36]).str,
		add_space(4, "Player A Win").c_str()), push_log("Player A Win", LEVEL_INFO, CONSOLE_LOG);
		else strcpy(std::get<Window::Content>(window->widgets[36]).str,
		add_space(8, "AI Win").c_str()), push_log("AI Win", LEVEL_INFO, CONSOLE_LOG);
		std::get<Window::Info>(window->widgets[36]).hidden = 0;
	}
}

void show_full_message(){
	int id = (now_window == PVP_BOARD_WINDOW ? 28 : 36);
	strcpy(std::get<Window::Content>(window->widgets[id]).str, add_space(4, "Board is Full").c_str());
	std::get<Window::Info>(window->widgets[id]).hidden = 0;

}

void show_player_info(int window_loaded){
	std::string tmp;
	if(window_loaded == PVP_BOARD_WINDOW){
		tmp = string_center(player_a_name, 16);
		//std::cout<<tmp<<std::endl;
		strncpy(std::get<Window::Content>(window->widgets[21]).str, tmp.c_str(), tmp.length());
		tmp = string_center(player_b_name, 20);
		strncpy(std::get<Window::Content>(window->widgets[22]).str, tmp.c_str(), tmp.length());
		tmp = convert_player_info(player_a_info, 20);
		strncpy(std::get<Window::Content>(window->widgets[23]).str, tmp.c_str(), tmp.length());
		tmp = convert_player_info(player_b_info, 20);
		strncpy(std::get<Window::Content>(window->widgets[24]).str, tmp.c_str(), tmp.length());
		std::get<Window::Info>(window->widgets[25]).hidden = 0;
		std::get<Window::Info>(window->widgets[28]).hidden = 1;
		if(now_player == 1)strcpy(std::get<Window::Content>(window->widgets[25]).str, "<--");
		else strcpy(std::get<Window::Content>(window->widgets[25]).str, "-->");
	}else if(window_loaded == PVE_BOARD_WINDOW){
		tmp = string_center(player_human_name, 16);
		strncpy(std::get<Window::Content>(window->widgets[29]).str, tmp.c_str(), tmp.length());
		tmp = convert_player_info(player_human_info, 20);
		strncpy(std::get<Window::Content>(window->widgets[30]).str, tmp.c_str(), tmp.length());
		std::get<Window::Info>(window->widgets[32]).hidden = 0;
		std::get<Window::Info>(window->widgets[36]).hidden = 1;
	}else{
		tmp = string_center(player_a_name, 16);
		//std::cout<<tmp<<std::endl;
		strncpy(std::get<Window::Content>(window->widgets[38]).str, tmp.c_str(), tmp.length());
		tmp = string_center(player_b_name, 20);
		strncpy(std::get<Window::Content>(window->widgets[39]).str, tmp.c_str(), tmp.length());
		tmp = convert_player_info(player_a_info, 20);
		strncpy(std::get<Window::Content>(window->widgets[40]).str, tmp.c_str(), tmp.length());
		if(player_b_name != std::string("AI"))tmp = convert_player_info(player_b_info, 20);
		else tmp = "No Info for AI";
		strncpy(std::get<Window::Content>(window->widgets[41]).str, tmp.c_str(), tmp.length());
		std::get<Window::Info>(window->widgets[42]).hidden = 0;
		std::get<Window::Info>(window->widgets[45]).hidden = 1;
		if(now_player == 1)strcpy(std::get<Window::Content>(window->widgets[42]).str, "<--");
		else strcpy(std::get<Window::Content>(window->widgets[42]).str, "-->");
	}
}

void board_click(std::pair<int, int> pos){
	push_log(std::string("Board Click: (") + std::to_string(pos.first) + ',' +
	std::to_string(pos.second) + ')', LEVEL_INFO, CONSOLE_LOG);
	if(board_signal || now_window == REPLAY_BOARD_WINDOW || 
	  (now_window == PVE_BOARD_WINDOW && ai_signal)){
		push_log("Invalid Board Click, Ignored!", LEVEL_WARN, CONSOLE_LOG);
		return;
	}
	Window::Chess chess;
	std::pair<int, int> r_pos = matrix_convert(pos);

	if(!judge->update(r_pos.first, r_pos.second, now_player)){
		push_log("Invalid Board Click, Ignored!", LEVEL_WARN, CONSOLE_LOG);
		return;
	}
		//注意：处理比赛结束的函数不应有更改渲染参数的内容，
		//因为处理点击的回调函数由渲染进程调用
		//应当在当前帧结束后再处理结束事务
		

	chess = (Window::Chess){chess_count++, pos, now_color};
	window->push_chess(chess);
	chesses.emplace_back(r_pos.first, r_pos.second, now_color);
		
	std::pair<int, int> status = judge->judge(r_pos.first, r_pos.second, now_player);
//	std::cout<<status.first<<std::endl;
	if(~status.first)board_signal = now_player;
	else if(judge->full()){
		push_log(std::string("Board is full!"), LEVEL_INFO, CONSOLE_LOG);
		board_signal = -1;
	}
		
	change_player();
	if(!board_signal && now_window == PVE_BOARD_WINDOW){
		show_ai_message(1);
		ai_lock.lock();
		ai_signal = 1;
		ai_lock.unlock();
	}
}

void push_ai_chess(){
	std::pair<int, int> ans(ai_ans.posx, ai_ans.posy);
	std::pair<int, int> r_ans = matrix_convert(ans);
	Window::Chess chess = (Window::Chess){chess_count++, r_ans, now_color};
	window->push_chess(chess);
	chesses.emplace_back(ans.first, ans.second, now_color);
	
	judge->update(ans.first, ans.second, now_player);
	std::pair<int, int> status = judge->judge(ans.first, ans.second, now_player);
        if(~status.first)board_signal = now_player;
        else if(judge->full()){
                //push_log(std::string("Board is full!"), LEVEL_INFO, CONSOLE_LOG);
                board_signal = -1;
        }

}

void log_player_info(std::string name, PlayerInfo::Info info){
	push_log(std::string("Query Player Info: ") + name +
	std::string(", Total: ") + std::to_string(info.total) +
	std::string(", Win: ") + std::to_string(info.win) +
	std::string(", Lose: " + std::to_string(info.lose)),
	LEVEL_INFO, CONSOLE_LOG);
}

void player_query(int window_loaded){
	if(player_info == nullptr)return;
	std::string tmp;
	if(window_loaded == PVP_SETTING_WINDOW){
		tmp = std::string(std::get<Window::Content>(window->widgets[4]).str);
		if(tmp != player_a_name){
			player_a_info = player_info->query(player_a_name = tmp);
			show_warning(0, 20, std::string(""));
			log_player_info(player_a_name, player_a_info);
		}
		tmp = std::string(std::get<Window::Content>(window->widgets[5]).str);
		if(tmp != player_b_name){
			player_b_info = player_info->query(player_b_name = tmp);
			show_warning(0, 20, std::string(""));
			log_player_info(player_b_name, player_b_info);
		}
	}else if(window_loaded == PVE_SETTING_WINDOW){//window_loaded == PVE_SETTING_WINDOW
		tmp = std::string(std::get<Window::Content>(window->widgets[10]).str);
		if(tmp != player_human_name){
			player_human_info = player_info->query(player_human_name = tmp);
			show_warning(0, 37, std::string(""));
			log_player_info(player_human_name, player_human_info);
		}
		tmp = std::string(std::get<Window::Content>(window->widgets[48]).str);
		if(tmp.empty())ai_difficulty.store(0);
		else if(ai_difficulty.load() != tmp[0] - '0'){
			ai_difficulty.store(tmp[0] - '0');
			show_warning(0, 37, std::string(""));
		}
	}else if(window_loaded == REPLAY_SETTING_WINDOW){
		player_a_info = player_info->query(player_a_name);
		log_player_info(player_a_name, player_a_info);
		if(player_b_name != std::string("AI")){
			player_b_info = player_info->query(player_b_name);
			log_player_info(player_b_name, player_b_info);
		}else push_log("Query Player Info: Player B is AI", LEVEL_INFO, CONSOLE_LOG);
		

		if(player_a_info.total == -1 || player_b_info.total == -1){
			throw std::runtime_error("No player info, don't modify the replay-file name!");
		}
	}
//	Println(player_a_name);	
}



void update_ai_chess(){
	ai_lock.lock();
	push_ai_chess();
	show_ai_message(0);
	change_player();
	ai_signal = 0;
	ai_lock.unlock();
}

void gen_first_chess(){
	ai_ans = (PVE::Ans){7,7};
	update_ai_chess();
}


bool check_player_name(int window_loaded){
	if(window_loaded == PVP_BOARD_WINDOW){
		if(player_a_name.empty() || player_b_name.empty())return 0;
		if(player_a_name == player_b_name)return 0;
		if(player_a_name == std::string("AI") || player_b_name == std::string("AI"))return 0;
		if(player_a_name.contains('#') || player_b_name.contains('#'))return 0;
	}else{
		if(player_human_name.empty())return 0;
		if(player_human_name == std::string("AI"))return 0;
		if(player_human_name.contains('#'))return 0;
	}
	return 1;
}

void read_replay(){
	replay_read = new ReplayRead(path_buff.string().c_str(), "sums");
	replay_read->decode();
	replay_read_buff = replay_read->get_step();
	replay_read_win = replay_read->get_win();
	std::string tmp = path_buff.string();
	std::vector<std::string> tokens;
	for(auto part : tmp | std::views::split('#'))tokens.emplace_back(std::string(part.begin(), part.end()));

	if(tokens.size() == 4)replay_read_player_buff = std::make_pair(tokens[1], tokens[2]);
	else{
		delete replay_read;
		throw std::runtime_error("Invalid Player Info in File Name");
	}

	if(replay_read_player_buff.first.size() > 17 || replay_read_player_buff.second.size() > 17){
		delete replay_read;
		throw std::runtime_error("Invalid Player Info in File Name");
	}
	
	push_log(std::string("Replay File Decode Success, Total Steps: ") +
		 std::to_string(replay_read_buff.size()), LEVEL_INFO, CONSOLE_LOG);
	push_log(std::string("Player Name: ") + replay_read_player_buff.first + "  " +
		 replay_read_player_buff.second, LEVEL_INFO, CONSOLE_LOG);
	push_log(std::string("Finished or Not: ") + std::to_string(replay_read_win), LEVEL_INFO, CONSOLE_LOG);
//	for(auto& it:replay_read_buff){
//		std::cout<<it.x<<" "<<it.y<<" "<<it.player<<std::endl;
//	}
//	std::cout<<replay_read_win<<std::endl;
//	std::cout<<replay_read_player_buff.first<<" "<<replay_read_player_buff.second<<std::endl;
	delete replay_read;
}

void log_switch_window(){
	push_log(std::string("Window Switch to: ") + std::to_string(now_window), LEVEL_INFO, CONSOLE_LOG);
}

void gui_main(){
	try{
                window = new Window("Gobang Cpp", 0.75f, 0.5f, 0.3f, 0.8f);
		window->imgui_init();
		window->chess_init(std::make_pair(0.7f, 0.7f), "resources/chess_black.png",
				"resources/chess_white.png");
		add_widget(window);
		window->widget_init();
		window->set_button_callback(click);
		window->set_board_callback(board_click);
        }catch(std::exception& e){
 		log_crash(std::string("GUI Init Failed:\n") + e.what(), BOTH_LOG);
		exit(134);
                //TODO:logging crash!
        }
	int count = 0;
	auto start = std::chrono::high_resolution_clock::now();
	while(running){
		if(board_signal){
//			if(switch_window == now_window){//TODO
//			}else{
			if(board_signal == 1 || board_signal == 2){
				show_win_message(board_signal);
//				update_database(board_signal);
				push_log(std::string("Game Finished, Board Signal: ") + std::to_string(board_signal),
				LEVEL_INFO, CONSOLE_LOG);
				board_signal *= 100;
			}else if(board_signal == -1){
				show_full_message();
//				update_database(-1);
				push_log(std::string("Board is Full, Board Signal: ") + std::to_string(board_signal),
				LEVEL_INFO, CONSOLE_LOG);
				board_signal *= 100;
			}
			std::get<Window::Info>(window->widgets[25]).hidden = 1;
			std::get<Window::Info>(window->widgets[32]).hidden = 1;
//			}
		}
		
		if(ai_signal == 3){
			update_ai_chess();
		}
		
		if(dialog_signal.load() == -1){
			show_warning(1, 15, add_space(34, "Invalid Path or File Name"));
			dialog_signal.store(0);
			push_log("NFD: Invalid Path or File", LEVEL_WARN, CONSOLE_LOG);
		}else if(dialog_signal.load() == 2){
			push_log(std::string("NFD: File Path: ") + path_buff.string(), LEVEL_INFO, CONSOLE_LOG);
			if(path_buff.string().length() <= 100){
				show_warning(1, 15, path_buff.string());
				try{
					read_replay();
					player_a_name = replay_read_player_buff.first;
					player_b_name = replay_read_player_buff.second;
					player_query(REPLAY_SETTING_WINDOW);
					replay_ready = 1;
				}catch(std::exception& e){
					push_log(std::string("ReplayReader: File Open/Decode Failed."), 
					LEVEL_WARN, CONSOLE_LOG);
					push_log(e.what(), LEVEL_WARN, CONSOLE_LOG);
					show_warning(1, 15,
					add_space(2, "File Open/Decode Failed. Don't Modify the Path and Content"));
				}
			}
			else{
				show_warning(1, 15, add_space(34, "Invalid Path or File Name"));
				push_log("File Name too Long!", LEVEL_WARN, CONSOLE_LOG);
			}
			dialog_signal.store(0);
		}
	
		if(switch_window != now_window){
			if(switch_window == MAIN_WINDOW){
				window->background_init("resources/picture01.png");
				now_window =  switch_window;
				log_switch_window();
			}else if(switch_window == PVP_SETTING_WINDOW){
				window->background_init("resources/picture02.png");
				now_window =  switch_window;
				log_switch_window();
			}else if(switch_window == PVE_SETTING_WINDOW){
				window->background_init("resources/picture03.png");
				now_window =  switch_window;
				log_switch_window();
			}else if(switch_window == REPLAY_SETTING_WINDOW){
				window->background_init("resources/picture04.png");
				replay_ready = 0;
				show_warning(0, 15, "");
				now_window =  switch_window;
				log_switch_window();
			}else if(switch_window == PVP_BOARD_WINDOW){
				if(check_player_name(PVP_BOARD_WINDOW)){
					window->background_init("resources/picture05.png");
					window->board_init(board_info);
					show_player_info(PVP_BOARD_WINDOW);
					judge = new Judger(15);
					now_window = switch_window;
					log_switch_window();
				}else{
					show_warning(1, 20, std::string("Invalid Player Name"));
					push_log("Invalid Player Name", LEVEL_WARN, CONSOLE_LOG);
					switch_window = now_window;
				}
			}else if(switch_window == PVE_BOARD_WINDOW){
				if(!check_player_name(PVE_BOARD_WINDOW)){
					show_warning(1, 37, std::string("Invalid Player Name"));
					push_log("Invalid Player Name", LEVEL_WARN, CONSOLE_LOG);
					switch_window = now_window;
				}else if(ai_difficulty.load() > 4 || ai_difficulty.load() < 1){
					show_warning(1, 37, add_space(4, "Invalid Difficulty").c_str());
					push_log("Invalid Difficulty", LEVEL_WARN, CONSOLE_LOG);
					switch_window = now_window;
				}else{
					window->background_init("resources/picture05.png");
					window->board_init(board_info);
					show_player_info(PVE_BOARD_WINDOW);
					judge = new Judger(15);
					now_window = switch_window;
					log_switch_window();
					if(now_player == 2)gen_first_chess();
				}
			}else if(switch_window == REPLAY_BOARD_WINDOW){
				window->background_init("resources/picture05.png");
				window->board_init(board_info);
				now_player = replay_read_buff[0].player;
				show_player_info(REPLAY_BOARD_WINDOW);
				if(replay_real_win = check_real_win()){
					push_log("Replay Real Win or Not: Yes", LEVEL_INFO, CONSOLE_LOG);
				}else{
					push_log("Replay Real Win or Not: No", LEVEL_INFO, CONSOLE_LOG);
				}
				now_window = switch_window;
				log_switch_window();
			}
		}
		if(window->render(now_window) == -1){//TODO：增加中断AI进程的部分
			if(now_window == PVP_BOARD_WINDOW || now_window == PVE_BOARD_WINDOW){
				if(chesses.size() && !replay_signal)update_replay(now_window);
				update_database(board_signal);
			}

			running = 0;
		}
		if(now_window == PVP_SETTING_WINDOW || now_window == PVE_SETTING_WINDOW){
			player_query(now_window);
		}
//		std::cout<<now_window<<std::endl;
		
		count++;
                auto end = std::chrono::high_resolution_clock::now();
                if(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() >= 5000){
//			std::cerr<<now_window<<std::endl;
			push_log(std::string("Renderer: ") + std::to_string(count/5) + " FPS (Avg)", LEVEL_INFO, CONSOLE_LOG);
                        start = std::chrono::high_resolution_clock::now();
                        count = 0;
                }

		
	}
	
	//clean();//TODO: Do you want to do some f**king stuff?

	delete window;
}

void logging(){
	logger->logging(log_queue.front().content, log_queue.front().level, log_queue.front().opt);
	log_queue.pop();
}

void write_replay(){
	replay_write_lock.lock();
	if(replay_signal){
		std::string file_name = '#' + replay_player_buff.first + '#' + replay_player_buff.second + '#';
		replay_write = new ReplayWrite(file_name, "sums");
		int now_player = replay_first_buff + 1;
		for(auto& it:replay_buff){
			replay_write->push_back(it.x, it.y, now_player);
			now_player = 3 - now_player;
		}
		replay_write->push_back_win(replay_win_buff);
		replay_write->write();
//		std::cout<<replay_win_buff<<std::endl;
		delete replay_write;
		replay_signal = 0;
		push_log(std::string("Replay Writes to File: ") + file_name, LEVEL_INFO, CONSOLE_LOG);
		push_log(std::string("Total Steps: ") + std::to_string(replay_buff.size()), LEVEL_INFO, CONSOLE_LOG);
		push_log(std::string("Win or Not: ") + std::to_string(replay_win_buff), LEVEL_INFO, CONSOLE_LOG); 
	}
	replay_write_lock.unlock();
}

void log_main(){
	try{
		logger = new Log("test_log", CONSOLE_LOG);
	}catch(std::exception& e){
 		log_crash(std::string("GUI Init Failed:\n") + e.what(), BOTH_LOG);
		exit(134);
	}
	while(running){
		log_lock.lock();
		while(log_queue.size())logging();
		log_lock.unlock();
		write_replay();
		std::this_thread::sleep_for(std::chrono::milliseconds(20));	
	}
	while(log_queue.size())logging();
	delete logger;
	write_replay();
}

void ai_algo(){
	std::chrono::high_resolution_clock::time_point start;
	while(running){
		ai_lock.lock();
		if(ai_signal == 1){
			start = std::chrono::high_resolution_clock::now();
			pve = new PVE(judge->board, 1 + now_color, 2 + ai_difficulty.load());//TODO
			pve->init();
			pve->start();
			ai_signal = 2;
			push_log(std::string("Start Searching... Depth: ") +
				 std::to_string(2 + ai_difficulty.load()), LEVEL_INFO, CONSOLE_LOG);
		}else if(ai_signal == 2){
			std::pair<bool, int> status = pve->get_status();
			if(status.first){
				ai_ans = pve->get_ans();
				delete pve;
				ai_signal = 3;
				auto end = std::chrono::high_resolution_clock::now();
				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
				push_log(std::string("DFS Finished, Elapsed Time: ") +
				std::to_string(duration.count()) + "ms", LEVEL_INFO, CONSOLE_LOG);
				push_log(std::string("Searching Result: (") + std::to_string(ai_ans.posx) +
					',' + std::to_string(ai_ans.posy) + ')', LEVEL_INFO, CONSOLE_LOG);
			}
		}else if(ai_signal == -1){
			pve->interrupt();
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			delete pve;
			ai_signal = 0;
			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			push_log(std::string("DFS Aborted, Elapsed Time: ") +
			std::to_string(duration.count()) + "ms", LEVEL_INFO, CONSOLE_LOG);
		}
		ai_lock.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

void file_dialog(){
	while(running){
		if(dialog_signal.load() == 1){
			file_select = new FileSelect("replay", "replay/");
			std::filesystem::path path;
			try{
				if(file_select->open_folder(path)){
					//dialog_lock.lock();
					path_buff = path;
					//dialog_lock.unlock();
					dialog_signal.store(2);
				}else dialog_signal.store(-1);
			}catch(std::exception& e){
				push_log(std::string("NFD Exception: ") + e.what(), LEVEL_ERROR, CONSOLE_LOG);
				dialog_signal.store(-1);
			}
			delete file_select;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		
	}
}

int main(int argc, char **argv){
	std::thread Gui(gui_main);
	std::thread Log(log_main);
	std::thread AI(ai_algo);
	std::thread dialog(file_dialog);
	Gui.detach();
	Log.detach();
	AI.detach();
	dialog.detach();
	try{
		player_info = new PlayerInfo("playerinfo.db");
	}catch(std::exception& e){
		log_crash(std::string("Sqlite3 Init Failed: ") + e.what(), BOTH_LOG);
		exit(134);
	}
	while(running)std::this_thread::sleep_for(std::chrono::milliseconds(50));
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	delete player_info;
	return 0;
}
