#include "file.hpp"
int main (int argc, char **argv){
/*
	Log* logger = nullptr;
	try{
		logger = new Log("gomoku_game", CONSOLE_LOG | FILE_LOG);
		
	}catch(const std::exception& e){
		logger->crash(std::string("Logger init failed: ") + e.what(), CONSOLE_LOG | FILE_LOG);
		return 1;
	}

	logger->logging("logging test", LEVEL_INFO, CONSOLE_LOG | FILE_LOG);
	logger->logging("logging test2", LEVEL_WARN, CONSOLE_LOG);
	logger->logging("logging test3", LEVEL_ERROR, FILE_LOG);

	delete logger;

	
//	try{
//		logger = new Log("gomoku_game", CONSOLE_LOG | FILE_LOG);
//		
//	}catch(const std::exception& e){
//		logger->crash(std::string("Logger init failed: ") + e.what(), CONSOLE_LOG | FILE_LOG);
//		return 1;
//	}

	
	ReplayWrite* replay;
	replay = new ReplayWrite("game_replay", "sums");
	replay->push_back(1,1,1);
	replay->push_back(1,2,2);
	//replay->pop_back();
	replay->push_back_win(1);
	replay->write();
	delete replay;


	ReplayRead* replay;
	replay = new ReplayRead("replay/2024-12-20+17:34:21.440_game_replay.replay", "sums");
	//std::cerr<<"OK"<<std::endl;
	replay->decode();
	std::vector<ReplayRead::Step> ans = replay->get_step();
	bool win = replay->get_win();
	
	for(auto it:ans){
		std::cout<<it.x<<' '<<it.y<<' '<<it.player<<std::endl;
	}
	std::cout<<win<<std::endl;
*/
	PlayerInfo* player;
	player = new PlayerInfo("playerinfo.db");
	player->upsert((PlayerInfo::Info){"YJX",114514,250,250});
	PlayerInfo::Info info = player->query("YJX");
	std::cout<<info.total<<" "<<info.win<<" "<<info.lose<<std::endl;
	info.name = "WTY";
	player->upsert(info);
	delete player;

	FileSelect* file;
	file = new FileSelect("replay", "replay/");
	fs::path p;
	if(file->open_folder(p)){
		std::cerr<<p.string()<<std::endl;
	}
	delete file;

	return 0;
}
