#define DEBUG

#ifdef DEBUG

#define Print(x) std::cerr<<(x)
#define Println(x) std::cerr<<(x)<<std::endl

#elif

#define Print(x)
#define Println(x)

#endif
/*
 This the test file for GUI API.
 */
#define STB_IMAGE_IMPLEMENTATION
#include "gui.h"
#include <chrono>

Window* main_window;
bool is_running = 1;
bool change;
int count;
void click(int bt){
	Print("Button: ");
	Print(bt);
	if(bt == 0)change = 1;
	Println(" Clicked.");
}

void board(std::pair<int, int> bt){
	Print("(");
	Print(bt.first);
	Print(",");
	Print(bt.second);
	Println(") Clicked");
}

void add_button(){
	Window::Scale scale = {std::make_pair(0.1,0.72),std::make_pair(0.3,0.1),
		               std::make_pair(0.01,0.015),0.04,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
	Window::Color color = {ImVec4(1.0f,1.0f,1.0f,1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
		               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};

	Window::Content content = {32, "YJX AK IOI\n   (PVP)", NULL};
	Window::Info info = {0, BUTTON_TYPE, MAIN_WINDOW};
	main_window->add_widget(info, scale, color, content);

	
	
	
	Window::Scale scale2 = {std::make_pair(0.6,0.72),std::make_pair(0.3,0.1),
		               std::make_pair(0.01,0.015),0.04,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
	Window::Color color2 = {ImVec4(1.0f,1.0f,1.0f,1.0f),ImVec4(0.2f, 0, 0.7f, 1.0f),
		               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};

	Window::Content content2 = {32, "WTY AK IOI\n    (PVE)", NULL};
	Window::Info info2 = {1, BUTTON_TYPE, MAIN_WINDOW};
	main_window->add_widget(info2, scale2, color2, content2);

	
	
	

	Window::Scale scale3 = {std::make_pair(0.05,0.05),std::make_pair(0.3,0.05),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        Window::Color color3 = {ImVec4(0,0,1.0f,1.0f),ImVec4(0, 0, 0, 0),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};

        Window::Content content3 = {32, "Player XHYH", NULL};
        Window::Info info3 = {2, TEXT_TYPE, PVP_BOARD_WINDOW};
        main_window->add_widget(info3, scale3, color3, content3);




	Window::Scale scale4 = {std::make_pair(0.1,0.72),std::make_pair(0.3,0.1),
                               std::make_pair(0.01,0.01),0.03,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        Window::Color color4 = {ImVec4(0,1.0f,1.0f,1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};

        Window::Content content4 = {32, "ZWC AK IOI\n   (PVP)", NULL};
        Window::Info info4 = {3, TEXT_TYPE, PVP_BOARD_WINDOW};
        main_window->add_widget(info4, scale4, color4, content4);


	Window::Scale scale5 = {std::make_pair(0.1,0.72),std::make_pair(0.35,0.1),
                               std::make_pair(0.01,0.01),0.05,ImVec2(0,0), ImVec2(0,0), ImVec2(0,0)};
        Window::Color color5 = {ImVec4(0,0,0,1.0f),ImVec4(0.2f, 0.5f, 0.7f, 1.0f),
                               ImVec4(0.3f,0.6f, 0.8f, 1.0f),ImVec4(0.1f, 0.4f, 0.6f, 1.0f)};

        Window::Content content5 = {8, "XHYH", NULL};
        Window::Info info5 = {4, TEXT_INPUT_TYPE, PVP_SETTING_WINDOW};
        main_window->add_widget(info5, scale5, color5, content5);
	

	

}
void renderer(){
	try{
		main_window = new Window("YJX AK IOI", 0.75f, 0.5f, 0.3f, 0.7f);
	}catch(std::exception& e){
		std::cerr<<e.what()<<std::endl;
		return;
	}
	main_window->imgui_init();
	main_window->background_init("resources/picture01.png");
	add_button();
	main_window->widget_init();
	main_window->change_widget_size();

	main_window->chess_init(std::make_pair(0.7f, 0.7f), "resources/chess_black.png", "resources/chess_white.png");
	/*
	main_window->add_button(0.04, "YJX AK IOI\n   (PVP)",std::make_pair(0,MAIN_WINDOW),std::make_pair(0.1,0.72),
			        std::make_pair(0.3,0.1), std::make_pair(0.01,0.01),
				ImVec4(0.2f, 0.5f, 0.7f, 1.0f), ImVec4(0.3f, 0.6f, 0.8f, 1.0f),
	                        ImVec4(0.1f, 0.4f, 0.6f, 1.0f));

	main_window->add_button(0.04, "WTY AK IOI\n   (PVE)",std::make_pair(1,MAIN_WINDOW),std::make_pair(0.6,0.72),
			        std::make_pair(0.3,0.1), std::make_pair(0.01,0.01),
				ImVec4(0.2f, 0.5f, 0.7f, 1.0f), ImVec4(0.3f, 0.6f, 0.8f, 1.0f),
	                        ImVec4(0.1f, 0.4f, 0.6f, 1.0f));
	
	main_window->add_button(0.04, "ZWC DDoS CCF\n  (Replay)",std::make_pair(2,MAIN_WINDOW),std::make_pair(0.25,0.87),
			        std::make_pair(0.5,0.1), std::make_pair(0.01,0.01),
				ImVec4(0.2f, 0.5f, 0.7f, 1.0f), ImVec4(0.3f, 0.6f, 0.8f, 1.0f),
	                        ImVec4(0.1f, 0.4f, 0.6f, 1.0f));
	
			*/
	main_window->set_button_callback(click);
	
	Window::Chess chess = {0, std::make_pair(1,1), BLACK_CHESS};
	main_window->push_chess(chess);
	chess = {1,std::make_pair(5, 3), WHITE_CHESS};
	main_window->push_chess(chess);
	main_window->pop_chess(1);

	while(1){
		if(main_window->render(MAIN_WINDOW) == -1)break;
		if(change == 1)break;
	}

//	while(change){
//		if(main_window->render(PVP_SETTING_WINDOW) == -1){
//			delete main_window;
//			exit(0);
//		}
//		printf("%s\n", std::get<Window::Content>(main_window->widgets[4]).str);
//		
//	}
	
	Window::BoardInfo info = {std::make_pair(0.097222222f, 0.159722222f),
		                  std::make_pair(0.907407407f, 0.767361111f), 14};
	main_window->board_init(info);

	if(change){
		main_window->background_init("resources/picture02.png");
		//main_window->change_widget_size();

	}
	main_window->set_board_callback(board);
	auto start = std::chrono::high_resolution_clock::now();
	while(change){
		if(main_window->render(PVP_BOARD_WINDOW) == -1)break;
		count++;
		auto end = std::chrono::high_resolution_clock::now();
		if(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() >= 1000){
			std::cerr<<count<<" FPS"<<std::endl;
			start = std::chrono::high_resolution_clock::now();
			count = 0;
		}
	}

	delete main_window;
	
	is_running = 0;

	return;
}

int main(){
	renderer();

	while(is_running);
	system("sleep 1");
	return 0;	
}
