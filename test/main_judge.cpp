#include "judge.hpp"
#include<cstdio>
#include<iostream>
int tab[16][16];
int main(int argc,char **argv){
	int range=argv[1][0]-'0';

	Judger* jud;
	jud = new Judger(range);
	
	int x,y,p;
	while(~scanf("%d%d%d",&x,&y,&p)){
		if(!jud->update(x,y,p)){
			std::cerr<<"Fuck you"<<std::endl;
			continue;
		}
		if(jud->full())return 114514;
		auto sta=jud->judge(x,y,p);
		if(~sta.first){
			std::cerr<<sta.first<<"  "<<sta.second<<std::endl;
			break;
		}
		for(int i=0;i<range;i++){
			for(int j=0;j<range;j++){
				std::cerr<<jud->board[i][j]<<' ';
			}
			std::cerr<<std::endl;
		}
		std::cerr<<std::endl;
	}

	delete jud;

	return 0;
}
