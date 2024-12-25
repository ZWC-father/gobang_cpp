/*
 basic file operations
 - Logging file
 - Stdout on console
 - Game palyback
 */

#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <ctime>

#include "boost/functional/hash.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "boost/beast/core/detail/base64.hpp"
#include "openssl/md5.h"
#include "sqlite3.h"
#include "nfd.h"

#define CONSOLE_LOG 1
#define FILE_LOG 2
#define BOTH_LOG 3
#define LEVEL_INFO 1
#define LEVEL_WARN 2
#define LEVEL_ERROR 3


namespace chr = std::chrono;
namespace fs = std::filesystem;
using ptree = boost::property_tree::ptree;

std::string get_timestamp();
void log_crash(std::string content, int opt);

class Log{
public:
	Log(const std::string& log_file, int opt);
	~Log();
	void logging(std::string content, int level, int opt);
//	void log_crash(std::string content, int opt);

private:
	bool file_open = 0;
	std::ofstream log_file;
	void open_file(const std::string& name);
	void close_file();
};

class ReplayWrite{
public:

	ReplayWrite(const std::string& replay_name, const std::string& hash_name);
	~ReplayWrite();
	void push_back(int x, int y, int player);
	void write();
	void pop_back();
	void push_back_win(int win);
private:
	std::string replay_name, hash_name, tab_name;
	std::ofstream replay_file;
	std::vector<int>step;
	boost::hash<std::vector<int>>vec_hash;
	std::string boost_hash, MD5_hash;
	ptree pt;
	void open_replay(const std::string& name);
	void open_xml(const std::string& name);
	void update_hash();
	
};

class ReplayRead{
public:
	ReplayRead(const std::string& replay_name, const std::string& hash_name);
	~ReplayRead();
	struct Step{
		int x,y,player;
	};
	std::vector<Step> get_step();
	int get_win();
	void decode();
	
private:
	std::string replay_name, hash_name, tab_name;
	std::ifstream replay_file;
	std::vector<Step>step;
	boost::hash<std::vector<int>>vec_hash;
	std::string boost_hash, MD5_hash;
	ptree pt;
	int win = -1;
	void open_replay(const std::string& name);
	void open_xml(const std::string& name);

};

class PlayerInfo{
public:
	PlayerInfo(const std::string& db_name);
	~PlayerInfo();
	struct Info{
		std::string name;
		int total, win, lose;
	};
	Info query(const std::string& player);
	void upsert(Info info);
private:
	sqlite3* db;
	bool check_table(const char* table_name);
};

class FileSelect{
public:
	FileSelect(const char* type, const char* path);
	~FileSelect();
	bool open_folder(fs::path& relative_path);
private:
	const char* nfd_type;
	const char* nfd_path;
	nfdchar_t* out_path = NULL;
};
