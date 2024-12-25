/*
This section is for competition judge
Both for PVP and PVE
 */

#include <cstring>
#include <utility>
#define PLAYER_A 1
#define PLAYER_B 2

class Judger{
public:
	int **board;
	int count;

	Judger(int xy);
	~Judger();
	bool update(int x, int y, int player);
	bool full();
	void undo(int x, int y);
	std::pair<int,int> judge(int x, int y, int player);
	
private:
	int range;
//	bool prohibit();
};

