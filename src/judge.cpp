#include "judge.hpp"
#include <iostream>
Judger::Judger(int xy){
	range = xy, count=0;
	board = new int*[range];
	for(int i=0;i<range;i++){
		board[i] = new int[range];
		for(int j=0;j<range;j++)board[i][j]=0;
	}
}

Judger::~Judger(){
	for(int i=0;i<range;i++)delete board[i];
}

bool Judger::update(int x, int y,int player){
//	for(int i=0;i<15;i++){
//		for(int j=0;j<15;j++)std::cout<<board[i][j]<<' ';
//		std::cout<<std::endl;
//	}
	if(x < 0 || x >= range || y < 0 || y >= range)return 0;
	if(board[x][y])return 0;
	board[x][y] = player;
	count++;
	return 1;
}

void Judger::undo(int x, int y){
	if(board[x][y]){
		board[x][y] = 0;
		count--;
	}
}

bool Judger::full(){return count == range*range;}

std::pair<int,int> Judger::judge(int x, int y, int player){
	int pt = x;
	int tp = x;
	while(pt > 0 && board[pt - 1][y] == player)pt--;
	while(tp < range-1 && board[tp + 1][y] == player)tp++;
	if(tp-pt>=4)return std::make_pair(0,x-pt);

	pt=y,tp=y;
	while(pt > 0 && board[x][pt-1] == player)pt--;
	while(tp < range-1 && board[x][tp+1] == player)tp++;
	if(tp-pt>=4)return std::make_pair(1,y-pt);
	
	int pm = x, mp = x;
	int pn = y, np = y;
	while(pm > 0 && pn > 0 && board[pm - 1][pn - 1]==player)pm--, pn--;
	while(mp < range-1 && np < range-1 && board[mp + 1][np + 1]==player)mp++, np++;
	if(mp-pm>=4)return std::make_pair(2,x-pm);
	
	pm = x, mp = x;
	pn = y, np = y;
	while(pm > 0 && pn < range-1 && board[pm - 1][pn + 1]==player)pm--, pn++;
	while(mp < range-1 && np > 0 && board[mp + 1][np - 1]==player)mp++, np--;
	if(mp-pm>=4)return std::make_pair(3,x-pm);

	return std::make_pair(-1,-1);
}
