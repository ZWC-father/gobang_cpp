#include "pve.hpp"

PVE::PVE(int** board, int role, int dep) : search_dep(dep){
	for(int i=0;i<15;i++){
		for(int j=0;j<15;j++){
			if(board[i][j])tmp[i][j]=(role==1?board[i][j]:3-board[i][j]);
			else tmp[i][j]=0;
		}
	}
}

void PVE::start(){
	std::thread worker(&PVE::search, this);
	worker.detach();
}

void PVE::interrupt(){
	signal.store(1, std::memory_order_relaxed);
}

std::pair<bool, int> PVE::get_status(){
	return std::make_pair(finished.load(std::memory_order_relaxed), count.load(std::memory_order_relaxed));
}

PVE::Ans PVE::get_ans(){
	return ans.load();
}

void PVE::init(){
	for(int i=0;i<=1;i++){
                for(int j=0;j<=1;j++){
                        for(int k=5;k<30;k++)tab[i][j][k]=FIVE;
                }
        }

	tab[0][0][4]=FOUR;
        tab[0][1][4]=tab[1][0][4]=BLOCK_FOUR;
        tab[1][1][4]=DEAD_FOUR;
        tab[0][0][3]=THREE;
        tab[0][1][3]=tab[1][0][3]=BLOCK_THREE;
        tab[1][1][3]=DEAD_THREE;
        tab[0][0][2]=TWO;
        tab[0][1][2]=tab[1][0][2]=BLOCK_TWO;
        tab[1][1][2]=DEAD_TWO;
        tab[0][0][1]=ONE;
        tab[0][1][1]=tab[1][0][1]=BLOCK_ONE;
        tab[1][1][1]=DEAD_ONE;

	memset(score,0,sizeof score);
        memset(board,0,sizeof board);
        memset(marker,0,sizeof marker);
        memset(pos,0,sizeof pos);
        //memset(spj_sta,0,sizeof spj_sta);
        memset(change_mark,0,sizeof change_mark);
        zobrist=0;
        sum1=sum2=0;
        count.store(0, std::memory_order_relaxed);
	finished.store(0, std::memory_order_relaxed);
	signal.store(0, std::memory_order_relaxed);

	score_init();
        marker_init();
	std::mt19937_64 rand_gen(time(0));
        for(int i=0;i<15;i++){
                for(int j=0;j<15;j++){
                        for(int k=1;k<3;k++)hash_tab[i][j][k]=rand_gen();
                }
        }

}

void PVE::marker_init(){
        for(int i=0;i<15;i++){
                for(int j=0;j<15;j++){
                        if(!tmp[i][j])continue;
                        int xmin = std::max(0, i - INIT_DIS);
                        int xmax = std::min(14, i + INIT_DIS);
                        int ymin = std::max(0, j - INIT_DIS);
                        int ymax = std::min(14,j + INIT_DIS);//TODO
                        for(int p=xmin;p<=xmax;p++){
                                for(int q=ymin;q<=ymax;q++){
                                        if(!tmp[p][q])marker[p][q]++;
                                }
                        }
                }
        }
}

void PVE::score_init(){
	for(int i=0;i<15;i++){
                for(int j=0;j<15;j++)board[0][i][j]=tmp[i][j];
        }


        for(int i=0;i<15;i++){
                for(int j=0;j<15;j++)board[1][i][j]=tmp[j][i];
        }

        for(int i=0;i<15;i++){
                for(int j=0;j<15-i;j++)board[2][14-i][j]=tmp[i+j][j];
        }

        for(int i=1;i<15;i++){
                for(int j=0;j<15-i;j++)board[2][14+i][j]=tmp[j][i+j];
        }

        for(int i=0;i<15;i++){
                for(int j=0;j<=i;j++)board[3][i][j]=tmp[j][i-j];
        }

        for(int i=1;i<15;i++){
                for(int j=0;j<15-i;j++)board[3][14+i][j]=tmp[i+j][14-j];
        }

        for(int i=0;i<15;i++){
                calc(0,0,i,1),calc(0,0,i,2);
                for(int j=0;j<15;j++){
                        if(board[0][i][j]==1)sum1+=score[0][0][i][j];
                        else if(board[0][i][j]==2)sum2+=score[0][0][i][j];
                }
        }
        
	for(int i=0;i<15;i++){
                calc(0,1,i,1),calc(0,1,i,2);
                for(int j=0;j<15;j++){
                        if(board[1][i][j]==1)sum1+=score[0][1][i][j];
                        else if(board[1][i][j]==2)sum2+=score[0][1][i][j];
                }
        }

        for(int i=0;i<29;i++){
                calc(0,2,i,1),calc(0,2,i,2);
                int siz=std::min(i+1,29-i);
                for(int j=0;j<siz;j++){
                        if(board[2][i][j]==1)sum1+=score[0][2][i][j];
                        else if(board[2][i][j]==2)sum2+=score[0][2][i][j];
                }

        }

        for(int i=0;i<29;i++){
                calc(0,3,i,1),calc(0,3,i,2);
                int siz=std::min(i+1,29-i);
                for(int j=0;j<siz;j++){
                        if(board[3][i][j]==1)sum1+=score[0][3][i][j];
                        else if(board[3][i][j]==2)sum2+=score[0][3][i][j];
                }
        }

}


inline void PVE::convert(int i,int j,NewPosition& new_pos){
        new_pos.posi_1=14-i+j,new_pos.posi_2=i+j;
        new_pos.posj_1=(i<j?i:j),new_pos.posj_2=(i+j<15?i:j);
}

inline void PVE::marker_update(int i, int j, int opt){
	int xmin = std::max(0, i - SEARCH_DIS);
        int xmax = std::min(14, i + SEARCH_DIS);
        int ymin = std::max(0, j - SEARCH_DIS);
        int ymax = std::min(14,j + SEARCH_DIS);//TODO
        for(int p=xmin;p<=xmax;p++){
                for(int q=ymin;q<=ymax;q++){
                        if(p==i && q==j)continue;
                        if(!tmp[p][q])marker[p][q]+=opt;
                }
        }
}

inline void PVE::calc(bool target, int type, int row, int role){//
        int* board_tmp=board[type][row];
        int* score_tmp=score[target][type][row];
        int len=(type==0||type==1?15:std::min(29-row,row+1));
//      char* spj_sta_tmp=spj_sta[role-1];
        int i=0,j=0,tmp;
        int sum[len]={board_tmp[0]==role};
        while(i<len){
                while(i<len&&board_tmp[i]!=role)i++;
                if(i==len)break;
                j=i+1;
                while(j<len&&board_tmp[j]==role)j++;
                tmp=tab[i==0||board_tmp[i-1]][j==len||board_tmp[j]][j-i];
                if(tmp==BLOCK_THREE&&
                ((i>1&&!board_tmp[i-1]&&!board_tmp[i-2])||
                (j+1<len&&!board_tmp[j]&&!board_tmp[j+1])))tmp=BETTER_BLOCK_THREE;
                //This is a special judge, BLOCK_THREE must be a unique value
                for(int k=i;k<j;k++)score_tmp[k]=std::max(score_tmp[k],tmp);
                i=j+1;
        }

        for(int i=1;i<len;i++){
                if(board_tmp[i]==role){
                        sum[i]=sum[i-1]+1;
                        if(i>3&&board_tmp[i-4]==role&&sum[i]-sum[i-4]==3){
                                for(int k=i-4;k<=i;k++){
                                        score_tmp[k]=(board_tmp[k]?std::max(score_tmp[k],JUMP_FOUR):0);
                                }
                        }else if(i>2&&board_tmp[i-3]==role&&sum[i]-sum[i-3]==2){
                                for(int k=i-3;k<=i;k++){
                                        score_tmp[k]=(board_tmp[k]?std::max(score_tmp[k],JUMP_THREE):0);
                                }
                        }
                }else if(!board_tmp[i])sum[i]=sum[i-1];
                else sum[i]=0;
        }
}

int PVE::eval(int depth){
	NewPosition new_pos[depth];
	int sum1_tmp=sum1, sum2_tmp=sum2;
	//memset(spj_sta,0,sizeof spj_sta);
	memset(change_mark,0,sizeof change_mark);
	for(int i=0;i<depth;i++){
		convert(pos[i].posi,pos[i].posj,new_pos[i]);
		int role=pos[i].role;
		board[0][pos[i].posi][pos[i].posj]=role;
		board[1][pos[i].posj][pos[i].posi]=role;
		board[2][new_pos[i].posi_1][new_pos[i].posj_1]=role;
		board[3][new_pos[i].posi_2][new_pos[i].posj_2]=role;
	}
/*
	for(auto& it:special){
		bool flag=0;
		switch (it.type){
			case 0:
				for(int i=0;i<depth;i++){
					if(it.row==pos[i].posi){flag=1;break;}
				}
				break;
			case 1:
				for(int i=0;i<depth;i++){
					if(it.row==pos[i].posj){flag=1;break;}
				}
				break;
			case 2:
				for(int i=0;i<depth;i++){
					if(it.row==new_pos[i].posi_1){flag=1;break;}
				}
				break;
			default:
				for(int i=0;i<depth;i++){
					if(it.row==new_pos[i].posi_2){flag=1;break;}
				}
				break;
		}
		if(!flag)spj_sta[it.role-1][it.val]++;
	}


	for(int i=0;i<2;i++){
		for(int j=0;j<15;j++){
			for(int k=0;k<15;k++){
				cout<<board[i][j][k]<<' ';
			}
			cout<<endl;
		}
		cout<<endl;
	}
	for(int i=2;i<4;i++){
		for(int j=0;j<29;j++){
			for(int k=0;k<min(1+j,29-j);k++){
				cout<<board[i][j][k]<<' ';
			}
			cout<<endl;
		}
		cout<<endl;
	}

*/

	for(int i=0;i<depth;i++){
		if(!change_mark[0][pos[i].posi]){
			int posi=pos[i].posi;
			calc(1,0,posi,1);
			calc(1,0,posi,2);
			for(int j=0;j<15;j++){
				if(board[0][posi][j]==1)sum1_tmp+=score[1][0][posi][j]-score[0][0][posi][j];
				else if(board[0][posi][j]==2)sum2_tmp+=score[1][0][posi][j]-score[0][0][posi][j];
				score[1][0][posi][j]=0;
			}
			change_mark[0][posi]=1;
		}

		if(!change_mark[1][pos[i].posj]){
			int posi=pos[i].posj;
			calc(1,1,posi,1);
			calc(1,1,posi,2);
			for(int j=0;j<15;j++){
				if(board[1][posi][j]==1)sum1_tmp+=score[1][1][posi][j]-score[0][1][posi][j];
				else if(board[1][posi][j]==2)sum2_tmp+=score[1][1][posi][j]-score[0][1][posi][j];
				score[1][1][posi][j]=0;
			}
			change_mark[1][posi]=1;
		}

		if(!change_mark[2][new_pos[i].posi_1]){
			int posi=new_pos[i].posi_1;
			calc(1,2,posi,1);
			calc(1,2,posi,2);
			int siz=std::min(1+posi,29-posi);
			for(int j=0;j<siz;j++){
				if(board[2][posi][j]==1)sum1_tmp+=score[1][2][posi][j]-score[0][2][posi][j];
				else if(board[2][posi][j]==2)sum2_tmp+=score[1][2][posi][j]-score[0][2][posi][j];
				score[1][2][posi][j]=0;
			}
			change_mark[2][posi]=1;
		}

		if(!change_mark[3][new_pos[i].posi_2]){
			int posi=new_pos[i].posi_2;
			calc(1,3,posi,1);
			calc(1,3,posi,2);
			int siz=std::min(1+posi,29-posi);
			for(int j=0;j<siz;j++){
				if(board[3][posi][j]==1)sum1_tmp+=score[1][3][posi][j]-score[0][3][posi][j];
				else if(board[3][posi][j]==2)sum2_tmp+=score[1][3][posi][j]-score[0][3][posi][j];
				score[1][3][posi][j]=0;
			}
			change_mark[3][posi]=1;
		}
	}
	
/*	

	if(spj_sta[0][BLOCK_FOUR_TYPE]>=2)sum1+=DOUBLE_BLOCK_FOUR;
	if(spj_sta[0][BLOCK_FOUR_TYPE] && spj_sta[0][THREE_TYPE])sum1+=BLOCK_FOUR_THREE;
	if(spj_sta[0][THREE_TYPE]>=2)sum1+=DOUBLE_THREE;
	if(spj_sta[0][TWO_TYPE]>=2)sum1+=DOUBLE_TWO;


	if(spj_sta[1][BLOCK_FOUR_TYPE]>=2)sum2+=DOUBLE_BLOCK_FOUR;
	if(spj_sta[1][BLOCK_FOUR_TYPE] && spj_sta[1][THREE_TYPE])sum2+=BLOCK_FOUR_THREE;
	if(spj_sta[1][THREE_TYPE]>=2)sum2+=DOUBLE_THREE;
	if(spj_sta[1][TWO_TYPE]>=2)sum2+=DOUBLE_TWO;
	
*/
	
//	for(int i=1;i<=4;i++)cout<<(int)spj_sta[0][i]<<endl;
//	cout<<sum1_tmp<<' '<<sum2_tmp<<endl;
	for(int i=0;i<depth;i++){
		board[0][pos[i].posi][pos[i].posj]=0;
		board[1][pos[i].posj][pos[i].posi]=0;
		board[2][new_pos[i].posi_1][new_pos[i].posj_1]=0;
		board[3][new_pos[i].posi_2][new_pos[i].posj_2]=0;
	}

	count.fetch_add(1,std::memory_order_relaxed);
	return sum1_tmp-ratio*sum2_tmp;
}


int PVE::dfs(int dep,int role,int prev){
	if(dep >= search_dep){
                if(buf.contains(zobrist))return buf[zobrist];
                return buf[zobrist]=eval(search_dep);
        }
        
	int val=(role&1?INT_MIN:INT_MAX);
        for(int i=0;i<15;i++){
                for(int j=0;j<15;j++){
                        if(marker[i][j]){
                                tmp[i][j] = role;
                                
				int mark_tmp=marker[i][j];
                                marker_update(i, j, 1);
                                marker[i][j]=0;
                                
				pos[dep]=(Position){role,i,j};
                                zobrist^=hash_tab[i][j][role];
                                
				if(role&1){
                                        if(!dep){
                                                int tmp_val=dfs(dep+1,role&1?2:1,val);
                                                if(tmp_val>val){
                                                        ans.store((Ans){i,j});
                                                        val=tmp_val;
                                                }
                                        }else val = std::max(val, dfs(dep+1, role&1?2:1, val));
                                }else val = std::min(val, dfs(dep+1, role&1?2:1, val));
                                
				zobrist^=hash_tab[i][j][role];
                                
				marker_update(i, j, -1);
                                marker[i][j] = mark_tmp;
                                
				tmp[i][j] = 0;

                                if(role&1){
                                        if(val>=prev)return val;
                                }else{
                                        if(val<=prev)return val;
                                }

				if(signal.load(std::memory_order_relaxed))return val;
                        }
                }
        }
        return val;
}

void PVE::search(){
	dfs(0,1,INT_MAX);
	finished.store(1,std::memory_order_relaxed);
}

