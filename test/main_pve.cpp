#include "pve.hpp"
PVE* pve;
int board[15][15];
int main(){
	int x, y;
	while(~scanf("%d%d", &x, &y)){
		board[x][y] = 2;
		pve = new PVE(board, 1);
		pve->init();
		pve->start();
		auto sta = pve->get_status();
		while(!sta.first){
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			sta = pve->get_status();
		}
		auto ans = pve->get_ans();
		board[ans.posx][ans.posy] = 1;
		for(int i=0;i<15;i++){
			for(int j=0;j<15;j++)std::cout<<board[i][j]<<' ';
			std::cout<<std::endl;
		}
		delete pve;
	}
	return 0;
}
