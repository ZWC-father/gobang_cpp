#include <iostream>
#include <vector>
#include <ctime>
#include <thread>
#include <atomic>
#include <cstring>
#include <climits>
#include <utility>
#include <random>
#include <unordered_map>

constexpr int FIVE=10000000;
constexpr int FOUR=100000;
constexpr int BLOCK_FOUR=1500;
constexpr int DEAD_FOUR=200;
constexpr int THREE=1000;
constexpr int BLOCK_THREE=150;
constexpr int BETTER_BLOCK_THREE=300;
constexpr int DEAD_THREE=30;
constexpr int TWO=100;
constexpr int BLOCK_TWO=15;
constexpr int DEAD_TWO=5;
constexpr int ONE=10;
constexpr int BLOCK_ONE=1;
constexpr int DEAD_ONE=0;
constexpr int JUMP_FOUR=BLOCK_FOUR;
constexpr int JUMP_THREE=BLOCK_THREE;

constexpr int DOUBLE_BLOCK_FOUR=FOUR;//双冲四
constexpr int BLOCK_FOUR_THREE=FOUR;//冲四活三
constexpr int DOUBLE_THREE=FOUR/3;//双三
constexpr int DOUBLE_TWO=200;//双二

constexpr int BLOCK_FOUR_TYPE = 1;
constexpr int THREE_TYPE = 2;
constexpr int BLOCK_THREE_TYPE = 3;
constexpr int TWO_TYPE = 4;

constexpr int pow4[7] = {1,4,16,64,256,1024,4096};

constexpr float ratio = 1.5f;//TODO
constexpr int INIT_DIS = 1;
constexpr int SEARCH_DIS = 1;

class PVE{
public:
	PVE(int** board, int role, int dep);
	struct Ans{int posx, posy;};
	void init();
	void start();
	void interrupt();
	std::pair<bool, int> get_status();
	Ans get_ans();

private:
	int sum1,sum2;
	const int search_dep;
	char marker[15][15];
	std::atomic<int> count;
	std::atomic<bool> finished;
	std::atomic<bool> signal;
	std::atomic<Ans> ans;
	int tmp[15][15];//tmp board
	int tab[2][2][30];//score table
	int board[4][30][15];//board in four directions
	int score[2][4][30][15];//score in four directions
	bool change_mark[4][30];
	unsigned long long zobrist;
	unsigned long long hash_tab[15][15][3];

	std::unordered_map<unsigned long long,int>buf;

//	struct Special{
//        	int role,type,row,val;
//	};
	struct Position{
        	int role,posi,posj;
	}pos[8];
	struct NewPosition{
        	int posi_1,posj_1;//vector 1
        	int posi_2,posj_2; //vector 2
	};
	
//	std::vector<Special>special;
//	char spj_sta[2][5];
	inline void convert(int i, int j, NewPosition& new_pos);
	inline void marker_init();
	inline void score_init();
	inline void marker_update(int i, int j, int opt);
	inline void calc(bool target, int type, int row, int role);
	int eval(int depth);
	int dfs(int dep, int role, int prev);
	void search();
};
