#include "gui.hpp"
#define EXTERNAL_FONT
#define DEBUG

#ifdef DEBUG

#define Print(x) std::cerr<<(x)
#define Println(x) std::cerr<<(x)<<std::endl

#elif

#define Print(x)
#define Println(x)

#endif


/*
         if(SDL_Init(SDL_INIT_VIDEO) == 0){
                throw std::runtime_error(std::string("SDL init error: ") + SDL_GetError());
        }

        int num_displays;
        displays = SDL_GetDisplays(&num_displays);//use primary display
        if(displays == NULL || num_displays == 0){
                SDL_Quit();
                throw std::runtime_error(std::string("SDL no available display: ") + SDL_GetError());
        }

        display_mode = SDL_GetDesktopDisplayMode(*displays);
        display_width = display_mode->w, display_height = display_mode->h;
        ratio = (float)display_width / display_height;

        if(ratio < min_ratio || ratio > max_ratio){
                SDL_free(displays);
                SDL_Quit();
                throw std::runtime_error(std::string("Resolution not supported!"));
        }

        width = display_width * scale, height = display_height * scale;
        create_window(title);
 */


std::pair<int,int> Window::get_resolution(){
        if(SDL_Init(SDL_INIT_VIDEO) == 0){
                throw std::runtime_error(std::string("SDL Init Error: ") + SDL_GetError());
        }

        int num_displays;
        displays = SDL_GetDisplays(&num_displays);//use primary display
        if(displays == NULL || num_displays == 0){
                SDL_Quit();
                throw std::runtime_error(std::string("SDL No Available Display: ") + SDL_GetError());
        }

        const SDL_DisplayMode* display_mode = SDL_GetDesktopDisplayMode(*displays);
	return std::make_pair(display_mode->w, display_mode->h);
//      display_width = display_mode->w, display_height = display_mode->h;
	
}

Window::~Window(){
	    if(texture)glDeleteTextures(1, &texture);
	    if(texture_black)glDeleteTextures(1, &texture_black);
	    if(texture_white)glDeleteTextures(1, &texture_white);

	    ImGui_ImplOpenGL3_DestroyFontsTexture();
	    ImGui_ImplOpenGL3_Shutdown();
	    ImGui_ImplSDL3_Shutdown();
	    ImGui::DestroyContext();

	    SDL_GL_DestroyContext(gl_context);
	    SDL_DestroyWindow(window);
	    SDL_Quit();
	    widgets.clear();
}

Window::Window(const char* name, const float ratio, const float scale, 
	       const float min_scale, const float max_scale){

	display_size = get_resolution();


#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
	// GL 3.2 Core + GLSL 150
	const char* glsl_version = "#version 150";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
	
	// Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//TODO:Check opengl renderer settings
	
	Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
	int h_pix = std::min(display_size.first, display_size.second);
	int w_pix = h_pix * ratio;
	
	window = SDL_CreateWindow(name, w_pix * scale, h_pix * scale, window_flags);
	SDL_SetWindowMinimumSize(window, w_pix * min_scale, h_pix * min_scale);
	SDL_SetWindowMaximumSize(window, w_pix * max_scale, h_pix * max_scale);
	SDL_SetWindowAspectRatio(window, ratio, ratio);//TODO:Ratio tolerant range
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	
	window_w = w_pix * scale, window_h = h_pix * scale;
	original_w = window_w, original_h = window_h;
	texture = texture_black = texture_white = 0;

	//TODO
	
	gl_context = SDL_GL_CreateContext(window);//TODO
	if (!gl_context) {
		SDL_DestroyWindow(window);
		SDL_Quit();
		throw std::runtime_error(std::string("OpenGL Init Error: ") + SDL_GetError());
	}
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1);
	SDL_ShowWindow(window);	

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void Window::imgui_init(){
	 window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground;
	 style = ImGui::GetStyle();
	 style.WindowPadding = ImVec2(0, 0);
	 style.FramePadding = ImVec2(0, 0);//TODO
	 style.FrameBorderSize = 0.0f;
}

void Window::background_init(const char* filename){
	int out_width, out_height;
	if(texture)glDeleteTextures(1, &texture);
	texture = LoadTexture(filename, out_width, out_height);
}

void Window::clear_font(){
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
        ImGui_ImplOpenGL3_DestroyFontsTexture();
}

void Window::widget_init(){
	clear_font();
	ImGuiIO& io = ImGui::GetIO();
	for(auto& it:widgets){
		Scale& scale = std::get<Scale>(it);
#ifdef EXTERNAL_FONT
		std::get<Content>(it).font = io.Fonts->AddFontFromFileTTF("resources/font.ttf",
				                       original_h * scale.font_scale);
#elif
		ImFontConfig config;
		config.SizePixels = original_h * scale.font_scale;
		std::get<Content>(it).font = io.Fonts->AddFontDefault(&config);
#endif
		io.Fonts->Build();

		scale.pos = ImVec2(scale.pos_scale.first * original_w,
				   scale.pos_scale.second * original_h);
		scale.size = ImVec2(scale.size_scale.first * original_w,
				    scale.size_scale.second * original_h);
		scale.padding = ImVec2(scale.padding_scale.first * original_w,
				       scale.padding_scale.second * original_h);

	}

}

void Window::chess_init(std::pair<float, float> scale,
		        const char* filename_black, const char* filename_white){
	int out_width, out_height;
	if(texture_black)glDeleteTextures(1, &texture_black);
	if(texture_white)glDeleteTextures(1, &texture_white);
	texture_black = LoadTexture(filename_black, out_width, out_height);
	if(abs(out_width - out_height) >= 5)throw std::runtime_error("Chess Picture Error!");
	texture_white = LoadTexture(filename_white, out_width, out_height);
	if(abs(out_width - out_height) >= 5)throw std::runtime_error("Chess Picture Error!");
	chess_scale = scale;
}

void Window::board_init(BoardInfo info){
	board_info = info;
}


void Window::set_button_callback(ButtonCallback cb){
	button_callback = cb;
}

void Window::set_board_callback(BoardCallback cb){
	board_callback = cb;
}

void Window::add_widget(Info info, Scale scale, Color color, Content content){
//	scale.pos = ImVec2(scale.pos_scale.first * window_w, scale.pos_scale.second * window_h);
//	scale.size = ImVec2(scale.size_scale.first * window_w, scale.size_scale.second * window_h);
//	scale.padding = ImVec2(scale.padding_scale.first * window_w, scale.padding_scale.second * window_h);

//	ImGuiIO& io = ImGui::GetIO();
//	ImFontConfig config;
//	config.SizePixels = window_h * scale.font_scale;
//	content.font = io.Fonts->AddFontDefault(&config);

	if(content.size_limit > MAX_BUFFER)content.size_limit = MAX_BUFFER;
	widgets.emplace_back(info, scale, color, content);

//	io.Fonts->Build();
}

void Window::push_chess(Chess chess){
	chesses.push_back(chess);
}

void Window::pop_chess(int count){
	for(int i=0;i<count;i++){
		if(chesses.size())chesses.pop_back();
		else return;
	}
}


void Window::change_widget_size(){//change font scale and other scale
	for(auto& it:widgets){
		Scale& scale = std::get<Scale>(it);
		scale.pos = ImVec2(scale.pos_scale.first * window_w, 
				   scale.pos_scale.second * window_h);
		scale.size = ImVec2(scale.size_scale.first * window_w,
				    scale.size_scale.second * window_h);
		scale.padding = ImVec2(scale.padding_scale.first * window_w,
				       scale.padding_scale.second * window_h);
	}
//	ImGui_ImplOpenGL3_CreateFontsTexture();
//	io.Fonts->Build();
}
//widget size will be calculated before render
//font is resized when rendering

void Window::load_widget(int window_loaded){
	for(auto& it:widgets){
		//load font
		Info& info = std::get<Info>(it);
		if(info.when_load == window_loaded && !info.hidden){
			Scale& scale = std::get<Scale>(it);
			Color& color = std::get<Color>(it);
			Content& content = std::get<Content>(it);
			content.font->Scale = (float)window_h/original_h;

			ImGui::PushFont(content.font);
			ImGui::PushStyleColor(ImGuiCol_Text, color.font_color);
			ImGui::PushStyleColor(ImGuiCol_Button, color.back_color);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color.hovered_color);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, color.active_color);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, color.back_color); 
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, scale.padding);
			ImGui::SetCursorPos(scale.pos);

			std::string label = "##";
			label += std::to_string(info.id);
			if(info.type == BUTTON_TYPE){
				if(ImGui::Button(content.str, scale.size))button_callback(info.id);
			}else if(info.type == TEXT_TYPE){
				//printf("%s\n",lable);
				ImGui::InputTextMultiline(label.c_str(), content.str, content.size_limit + 1,
						          scale.size, ImGuiInputTextFlags_ReadOnly);
			}else if(info.type == TEXT_INPUT_TYPE){
				//ImGui::InputText(label, content.str, content.size_limit + 1);
				ImGui::InputTextEx(label.c_str(), NULL, content.str, (int)content.size_limit + 1,
						   scale.size, ImGuiInputTextFlags_None, NULL, NULL);
				//Internal API
			}

			ImGui::PopStyleVar(1);
			ImGui::PopStyleColor(5);
			ImGui::PopFont();

		}
	}
}

void Window::render_main(){
	ImGui::Image((ImTextureID)(intptr_t)texture, ImVec2((float)window_w, (float)window_h));
	load_widget(MAIN_WINDOW);
}


void Window::render_board(int window_loaded){
	ImGui::Image((ImTextureID)(intptr_t)texture, ImVec2((float)window_w, (float)window_h));
	load_widget(window_loaded);
	load_board_button_chess();//render button after chess
}

void Window::render_setting(int window_loaded){
	ImGui::Image((ImTextureID)(intptr_t)texture, ImVec2((float)window_w, (float)window_h));
	load_widget(window_loaded);
}


void Window::load_board_button_chess(){
	int siz = board_info.size;
	float start_w = window_w * board_info.start_scale.first;
	float start_h = window_h * board_info.start_scale.second;
	float step_w = window_w * (board_info.end_scale.first - board_info.start_scale.first) / siz;
	float step_h = window_h * (board_info.end_scale.second - board_info.start_scale.second) / siz;
	char label[8] = "##";
	for(int i=0; i<siz; i++){
		for(int j=0; j<siz; j++){
			ImGui::SetCursorPos(ImVec2(start_w + step_w * i, start_h + step_h * j));
			label[2] = i + 'a', label[3] = j + 'a';//TODO
			if(ImGui::InvisibleButton(label, ImVec2(step_w, step_h))){//TODO
				//std::cerr<<'('<<i<<','<<j<<") Clicked!"<<std::endl;
				board_callback(std::make_pair(i,j));
			}
		}
	}

	float chess_step_w = step_w * chess_scale.first;
	float chess_step_h = step_h * chess_scale.second;
	float offset_w = (step_w - chess_step_w) / 2;
	float offset_h = (step_h - chess_step_h) / 2;
	
	for(auto& it:chesses){
		GLuint texture_chess = (it.color == BLACK_CHESS ? texture_black : texture_white);
		ImGui::SetCursorPos(ImVec2(start_w + step_w * it.pos.first + offset_w,
			                   start_h + step_h * it.pos.second + offset_h));
		ImGui::Image((ImTextureID)(intptr_t)texture_chess,
			     ImVec2(chess_step_w, chess_step_h));
	}

}

int Window::render(int window_loaded){
	SDL_Event event;
        while (SDL_PollEvent(&event)){
                ImGui_ImplSDL3_ProcessEvent(&event);
                if (event.type == SDL_EVENT_QUIT)return -1;
                if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)return -1;
                if (event.type == SDL_EVENT_WINDOW_RESIZED){
			window_w = event.window.data1;
                        window_h = event.window.data2;
			change_widget_size();
		}
	}

	ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

	ImGui::GetStyle() = style;//prevent style from being changed (reset every time)
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2((float)window_w, (float)window_h), ImGuiCond_Always);

	ImGui::Begin("##", nullptr, window_flags);
	
	if(window_loaded == MAIN_WINDOW)render_main();
	else if(window_loaded & SETTING_WINDOW)render_setting(window_loaded);
	else render_board(window_loaded);

	ImGui::End();
	ImGui::Render();
        
	glViewport(0, 0, window_w, window_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);//We don't want to see it.
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);

	return 0;
}

GLuint Window::LoadTexture(const char* filename, int& out_width, int& out_height) {
	int channels;
	unsigned char* data = stbi_load(filename, &out_width, &out_height, &channels, 0);
	if (!data) {
		throw std::runtime_error("Texture " + std::string(filename) + " Load Failed!");
	}

	
	GLuint image_texture = 0;

	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Upload pixels into texture
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, out_width, out_height, 0,
                 (channels == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
	

	/*
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, out_width, out_height, 0,
                 (channels == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	*/

	stbi_image_free(data);
	return image_texture;
}


