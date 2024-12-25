void add_widget(Window* window){
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

