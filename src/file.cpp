#include "file.hpp"

std::string get_timestamp(){
        auto now = chr::system_clock::now();
        int dis_ms = chr::duration_cast<chr::milliseconds>(now.time_since_epoch()).count()
                - chr::duration_cast<chr::seconds>(now.time_since_epoch()).count() * 1000;
        time_t tt = chr::system_clock::to_time_t(now);
        auto time_tm = localtime(&tt);
        char str_time[100] = {0};
        sprintf(str_time, "%d-%02d-%02d+%02d:%02d:%02d.%03d", time_tm->tm_year + 1900,
                time_tm->tm_mon + 1, time_tm->tm_mday, time_tm->tm_hour,
                time_tm->tm_min, time_tm->tm_sec, dis_ms);
        return std::string(str_time);
}

void log_crash(std::string content, int opt){
        std::string output = "[" + get_timestamp() + "] [CRASH] " + content;
        if(opt & 1)std::cerr << output << std::endl;
        if(opt > 2){
                std::ofstream tmp;
                tmp.open(get_timestamp() + ".crash.log");
                if(tmp.is_open())tmp << output << std::endl;
                tmp.close();
        }
}

Log::Log(const std::string& name, int opt){
	if(opt > 1)open_file(name);
}

Log::~Log(){
	if(file_open && log_file.is_open())log_file.close();
}

void Log::logging(std::string content, int level, int opt){
	std::string level_string;
	if(level == 1)level_string = "] [INFO] ";
	else if(level == 2) level_string = "] [WARN] ";
	else level_string = "] [ERROR] ";
	std::string output = "[" + get_timestamp() + level_string + content;
	
	if(opt & 1)std::cout << output << std::endl;
	
	if(opt > 1){
		if(file_open)log_file << output << std::endl;
		else throw std::runtime_error("Log file not open yet!");
	}
}

void Log::open_file(const std::string& name){
	std::string file_name = get_timestamp() + "." + name + ".log";
	//std::string file_name = "/root/testlog.log";
	
	fs::path file_path(file_name);
	if (fs::exists(file_path)) {
		throw std::runtime_error("Log file exist!");
	}
	
	log_file.exceptions(std::ios::failbit | std::ios::badbit);
	log_file.open(file_name, std::ios::out | std::ios::trunc);
	
	if(log_file.is_open() == 0){
		throw std::runtime_error("Failed to create log file!");
	}
	file_open = 1;
}

void Log::close_file(){
	file_open = 0;
	if(log_file.is_open())log_file.close();
}

ReplayWrite::ReplayWrite(const std::string& replay_name, const std::string& hash_name){	
	open_xml(hash_name);
	open_replay(replay_name);
}

ReplayWrite::~ReplayWrite(){
	replay_file.close();
}

void ReplayWrite::open_replay(const std::string& name){
	replay_name = "replay/" + get_timestamp() + name + ".replay";
	//std::string file_name = "/root/testlog.log";
	if(fs::exists("replay")){
		if(!fs::is_directory("replay")){
			fs::remove("replay");
			fs::create_directories("replay");
		}
	}else{
		fs::create_directories("replay");
	}
	
	fs::path file_path(replay_name);
	if (fs::exists(file_path)) {
		throw std::runtime_error("Replay file exist!");
	}
	
	replay_file.exceptions(std::ios::failbit | std::ios::badbit);
	replay_file.open(replay_name, std::ios::out | std::ios::trunc);
	
	if(replay_file.is_open() == 0){
		throw std::runtime_error("Failed to create replay file!");
	}
}

void ReplayWrite::open_xml(const std::string& name){
	hash_name = name + ".xml";
	
	fs::path file_path(hash_name);
	if (!fs::exists(file_path)){
		std::ofstream file_tmp(hash_name);
		file_tmp.close();
	}

	boost::property_tree::read_xml(hash_name, pt);	

	if(pt.get_child_optional(tab_name = name)){
		for (const auto& node : pt.get_child(tab_name)){
			if(node.first != "file")throw std::runtime_error("Hash file was modified!");
			if(!node.second.get_optional<std::string>("file_name") ||
			   !node.second.get_optional<std::string>("boost_hash") ||
			   !node.second.get_optional<std::string>("MD5_hash")){
				throw::std::runtime_error("Hash file was modified!");
			}
			//if(node.first.get_child_optional("filename"))exit(0);
//			else{
//				if(node.second.get<std::string>("hash") ||
		}
	}

}

void ReplayWrite::update_hash(){
	ptree node;
	node.put("file_name", replay_name);
	node.put("boost_hash", boost_hash);
	node.put("MD5_hash", MD5_hash);
	pt.add_child(tab_name + ".file", node);
}

void ReplayWrite::push_back(int x, int y, int player){
	step.push_back(player * 10000 + x * 100 + y);
}

void ReplayWrite::push_back_win(int win){
	step.push_back(win);//Warning: type [1,4]
}

void ReplayWrite::pop_back(){
	step.pop_back();
}

std::string calc_MD5(const std::string& data) {
	unsigned char md[MD5_DIGEST_LENGTH]; // 存储 MD5 的二进制结果
	MD5(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), md);
	
	std::ostringstream oss;
	for(int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
		oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(md[i]);
	}
	return oss.str();
}


void ReplayWrite::write(){
	std::string cache, output;
	cache.push_back('#');
	for(auto& it:step){
		cache += std::to_string(it);
		cache.push_back(',');
	}
	cache.pop_back();
	cache.push_back('#');
	output.resize(boost::beast::detail::base64::encoded_size(cache.size()));
	auto siz = boost::beast::detail::base64::encode(output.data(), cache.data(), cache.size());
	output.resize(siz);
	replay_file<<output;

	size_t tmp = 0;
	tmp = vec_hash(step);
	boost::hash_combine(tmp, replay_name);
	boost_hash = std::to_string(tmp);
	MD5_hash = calc_MD5(output);

	update_hash();
	boost::property_tree::write_xml(hash_name, pt);
}

ReplayRead::ReplayRead(const std::string& replay_name, const std::string& hash_name){	
	open_replay(replay_name);
	open_xml(hash_name);
}

ReplayRead::~ReplayRead(){
	replay_file.close();
}

void ReplayRead::open_replay(const std::string& name){
	replay_name = name;//relative path
	
	fs::path file_path(name);
	if (!fs::exists(file_path)) {
		throw std::runtime_error("Replay File Not Exist!");
	}
	
	replay_file.exceptions(std::ios::failbit | std::ios::badbit);
	replay_file.open(replay_name);
	
	if(replay_file.is_open() == 0){
		throw std::runtime_error("Failed to Open Replay File!");
	}
}

void ReplayRead::open_xml(const std::string& name){
	hash_name = name + ".xml";//same name as ReplayWrite
	
	fs::path file_path(hash_name);
	if (!fs::exists(file_path)){
		throw std::runtime_error("XML File Not Exist!");
	}

	boost::property_tree::read_xml(hash_name, pt);	

	if(pt.get_child_optional(tab_name = name)){
		for (const auto& node : pt.get_child(tab_name)){
			if(node.first != "file")throw std::runtime_error("Hash File Was Modified!");
			if(!node.second.get_optional<std::string>("file_name") ||
			   !node.second.get_optional<std::string>("boost_hash") ||
			   !node.second.get_optional<std::string>("MD5_hash")){
				throw::std::runtime_error("Hash File Was Modified!");
			}
			if(node.second.get<std::string>("file_name") == replay_name){
				boost_hash = std::string(node.second.get<std::string>("boost_hash"));
				MD5_hash = std::string(node.second.get<std::string>("MD5_hash"));
			}

			//if(node.first.get_child_optional("filename"))exit(0);
//			else{
//				if(node.second.get<std::string>("hash") ||
		}
	}
	if(MD5_hash == "" || boost_hash == "")throw std::runtime_error("No Hash Info in XML File!");

}

void ReplayRead::decode(){
	std::string cache, output;
	std::vector<int>step_cache;
	replay_file>>cache;
//	std::cerr<<cache<<std::endl;
//	std::cerr<<MD5_hash<<std::endl;
	if(calc_MD5(cache) != MD5_hash)throw std::runtime_error("MD5 Hash Not Matched!");
	

	output.resize(boost::beast::detail::base64::decoded_size(cache.size()));
	auto siz = boost::beast::detail::base64::decode(output.data(), cache.data(), cache.size());
	output.resize(siz.first);

	if(output.size() > 1){
		int i = 1;
		while(i < output.size()){
			int x = 0;
			while(i < output.size()){
				if(isdigit(output[i]))x = x * 10 + output[i] - '0';
				else{
					i++;
					break;
				}
				i++;
			}
			step_cache.push_back(x);
		}
		
	}else throw std::runtime_error("Decode Error!");
	
	size_t tmp = 0;
	tmp = vec_hash(step_cache);
	boost::hash_combine(tmp, replay_name);
	if(boost_hash != std::to_string(tmp))throw std::runtime_error("Boost Hash Not Matched!");
	
	for(auto x:step_cache){
			if(x < 10000){
				win = x;
			}else{
				step.emplace_back(x % 10000 / 100, x % 100, x / 10000);
			}
	}

	if(win == -1)throw std::runtime_error("No Winner Info!");

}

std::vector<ReplayRead::Step> ReplayRead::get_step(){
	return step;
}

int ReplayRead::get_win(){
	return win;
}

bool PlayerInfo::check_table(const char* table_name){
	const std::string checkSQL = "SELECT name FROM sqlite_master WHERE type='table' AND name=?;";
	sqlite3_stmt* stmt;

	if(sqlite3_prepare_v2(db, checkSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
		throw std::runtime_error("SQL init error!");
	}

	sqlite3_bind_text(stmt, 1, table_name, -1, SQLITE_STATIC);

	bool exists = 0;
	if(sqlite3_step(stmt) == SQLITE_ROW)exists = 1;

	sqlite3_finalize(stmt);
	return exists;
}

PlayerInfo::PlayerInfo(const std::string& db_name){
	if(sqlite3_open(db_name.c_str(), &db) != SQLITE_OK)throw std::runtime_error("Failed to open DB");
	if(!check_table("players")){
		const std::string createSQL = R"(
                CREATE TABLE IF NOT EXISTS players (
                name TEXT PRIMARY KEY,
                total INTEGER NOT NULL,
                win INTEGER NOT NULL,
                lose INTEGER NOT NULL);)";
		char* errMsg = nullptr;
        	if (sqlite3_exec(db, createSQL.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK){
			throw std::runtime_error(std::string(errMsg));
		}
	}
}

PlayerInfo::~PlayerInfo(){
	sqlite3_close(db);
}

PlayerInfo::Info PlayerInfo::query(const std::string& player){
	const std::string selectSQL = "SELECT * FROM players WHERE name = ?;";
	sqlite3_stmt* stmt;

	sqlite3_prepare_v2(db, selectSQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_text(stmt, 1, player.c_str(), -1, SQLITE_STATIC);

	Info info;
	
	if(sqlite3_step(stmt) == SQLITE_ROW){
		info.name = player;
		info.total = sqlite3_column_int(stmt, 1);
		info.win = sqlite3_column_int(stmt, 2);
		info.lose = sqlite3_column_int(stmt, 3);
	}else{
		info = (Info){player, -1, -1, -1};
	}

	sqlite3_finalize(stmt);
	return info;
}

void PlayerInfo::upsert(Info info){
	const std::string upsertSQL = "INSERT OR REPLACE INTO players (name, total, win, lose) VALUES (?, ?, ?, ?);";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, upsertSQL.c_str(), -1, &stmt, nullptr);


	sqlite3_bind_text(stmt, 1, info.name.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, info.total);
	sqlite3_bind_int(stmt, 3, info.win);
	sqlite3_bind_int(stmt, 4, info.lose);

	if (sqlite3_step(stmt) != SQLITE_DONE){
		sqlite3_finalize(stmt);
		throw std::runtime_error(std::string(sqlite3_errmsg(db)));
	}

	sqlite3_finalize(stmt);
}

FileSelect::FileSelect(const char* type, const char* path){
	nfd_type = type, nfd_path = path;
}

FileSelect::~FileSelect(){
	if(out_path != NULL)free(out_path);
}

bool FileSelect::open_folder(fs::path& relative_path) {
	nfdresult_t res = NFD_OpenDialog(nfd_type, nfd_path, &out_path);
	if(res == NFD_ERROR)throw std::runtime_error(std::string(std::string(NFD_GetError())));
	if(res == NFD_CANCEL)return 0;
	fs::path select_path(out_path);
	relative_path = fs::relative(select_path, fs::current_path());
	return 1;
}

