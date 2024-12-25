//#include "judge.hpp"
#include <iostream>
#include <vector>
#include <ctime>
#include <cstring>
#include <climits>
#include <utility>
#include <random>
#include <unordered_map>
using namespace std;
char marker[15][15];
pair<int,int> ans;
constexpr int pow4[7] = {1,4,16,64,256,1024,4096};


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

/*
constexpr int FIVE=10000000;
constexpr int FOUR=1000000;
constexpr int BLOCK_FOUR=100000;
constexpr int DEAD_FOUR=2000;
constexpr int THREE=10000;
constexpr int BLOCK_THREE=3000;
constexpr int BETTER_BLOCK_THREE=6000;
constexpr int DEAD_THREE=500;
constexpr int TWO=1000;
constexpr int BLOCK_TWO=200;
constexpr int DEAD_TWO=50;
constexpr int ONE=50;
constexpr int BLOCK_ONE=20;
constexpr int DEAD_ONE=1;
*/

constexpr int DOUBLE_BLOCK_FOUR=FOUR;//双冲四
constexpr int BLOCK_FOUR_THREE=FOUR;//冲四活三
constexpr int DOUBLE_THREE=FOUR/3;//双三
constexpr int DOUBLE_TWO=200;//双二

constexpr int JUMP_FOUR=BLOCK_FOUR;
constexpr int JUMP_THREE=BLOCK_THREE;
	
constexpr int BLOCK_FOUR_TYPE = 1; 
constexpr int THREE_TYPE = 2; 
constexpr int BLOCK_THREE_TYPE = 3;  
constexpr int TWO_TYPE = 4;

constexpr float ratio = 1.5f;

int sum1,sum2;
int tmp[15][15];
int tab[2][6200];
int spj[2][6200];
int board[4][30][15];//类型，行，位置
int score[2][4][30][15];//操作对象，类型，行，位置
unsigned long long zobrist;
unsigned long long hash_tab[15][15][3];

unordered_map<unsigned long long,int>buf;

struct Special{
	int role,type,row,val;
};
struct Position{
	int role,posi,posj;
}pos[10];
struct NewPosition{
	int posi_1,posj_1;//vector 1
	int posi_2,posj_2; //vector 2
};
vector<Special>special;
char spj_sta[2][5];
bool change_mark[4][30];



inline void convert(int i,int j,NewPosition& new_pos){
	new_pos.posi_1=14-i+j,new_pos.posi_2=i+j;
	new_pos.posj_1=(i<j?i:j),new_pos.posj_2=(i+j<15?i:j);
}


inline int full_hash(int* board_tmp, int start, int len){
	int sum=0;
	for(int i=0;i<len;i++)sum+=(board_tmp[start+i]+1)*pow4[i];
	return sum;
}

inline int edge_hash(int opt,int* board_tmp, int start, int len, int role){
	int sum=0;
	for(int i=0;i<len;i++)sum+=(board_tmp[start+i]+1)*pow4[i];
	role=(role==1?3:2);
	if(opt>>1&1)sum+=role*pow4[len];
	if(opt&1)sum*=4,sum+=role;
	return sum;

}


void marker_init(){
	for(int i=0;i<15;i++){
		for(int j=0;j<15;j++){
			if(!tmp[i][j])continue;
			int xmin = max(0, i - 1);
			int xmax = min(14, i + 1);
			int ymin = max(0, j - 1);
			int ymax = min(14,j + 1);//TODO
			for(int p=xmin;p<=xmax;p++){
				for(int q=ymin;q<=ymax;q++){
					if(!tmp[p][q])marker[p][q]++;
				}
			}
		}
	}
}
//TODO
//冲三必须满足的条件： 2 1 1 1 0 0 ，即一侧必须至少有两个空格

inline void calc(bool target, int type, int row, int match_len, int role){//
	int* board_tmp=board[type][row];
	int* score_tmp=score[target][type][row];
	int* tab_tmp=tab[role-1];
	int* spj_tmp=spj[role-1];
	char* spj_sta_tmp=spj_sta[role-1];
	int len;
	if(type==0||type==1)len=15;
	else len=min(row+1,29-row);
	if(match_len-len==2){//TODO：是否根据角色选择tab
		int val=tab_tmp[edge_hash(3,board_tmp,0,len,role)];
		if(!val)return;
		for(int j=0;j<len;j++){
			if(board_tmp[j]==role)score_tmp[j]=max(score_tmp[j],val);
		}
	}else{
		int key=edge_hash(1,board_tmp,0,match_len-1,role);
		int val=tab_tmp[key];
		if(val){
			for(int j=0;j<match_len-1;j++){
				if(board_tmp[j]==role){
					score_tmp[j]=max(score_tmp[j],val);
				}
			}
			if(spj_tmp[key]){
				if(target)spj_sta_tmp[spj_tmp[key]]++;
				else special.emplace_back(role,type,row,spj_tmp[key]);
			}
		}
		
		key=edge_hash(2,board_tmp,len-match_len+1,match_len-1,role);
		val=tab_tmp[key];
		if(val){
			for(int j=len-match_len+1;j<len;j++){
				if(board_tmp[j]==role){
					score_tmp[j]=max(score_tmp[j],val);
				}
			}
			if(spj_tmp[key]){
				if(target)spj_sta_tmp[spj_tmp[key]]++;
				else special.emplace_back(role,type,row,spj_tmp[key]);
			}
		}
		
		for(int i=0;i+match_len-1<len;i++){
			key=full_hash(board_tmp,i,match_len);
			//cout<<"row: "<<row<<" match_len: "<<match_len<<" i: "<<i<<endl; 
			val=tab_tmp[key];
			if(!val)continue;
			for(int j=0;j<match_len;j++){
				if(board_tmp[i+j]==role){
					score_tmp[i+j]=max(score_tmp[i+j],val);
				}
			}
			if(spj_tmp[key]){
				if(target)spj_sta_tmp[spj_tmp[key]]++;
				else special.emplace_back(role,type,row,spj_tmp[key]);
			}
		}
	}
	
	
}

int eval(int depth){
	NewPosition new_pos[depth];
	int sum1_tmp=sum1, sum2_tmp=sum2;
	memset(spj_sta,0,sizeof spj_sta);
	memset(change_mark,0,sizeof change_mark);
	//0->冲4
	//1->活3
	//2->冲3
	//3->活2
	for(int i=0;i<depth;i++){
		convert(pos[i].posi,pos[i].posj,new_pos[i]);
		int role=pos[i].role;
		board[0][pos[i].posi][pos[i].posj]=role;
		board[1][pos[i].posj][pos[i].posi]=role;
		board[2][new_pos[i].posi_1][new_pos[i].posj_1]=role;
		board[3][new_pos[i].posi_2][new_pos[i].posj_2]=role;
	}

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
/*
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
			for(int p=3;p<=6;p++){
				calc(1,0,posi,p,1);
				calc(1,0,posi,p,2);
			}
			for(int j=0;j<15;j++){
				if(board[0][posi][j]==1)sum1_tmp+=score[1][0][posi][j]-score[0][0][posi][j];
				else if(board[0][posi][j]==2)sum2_tmp+=score[1][0][posi][j]-score[0][0][posi][j];
				score[1][0][posi][j]=0;
			}
			change_mark[0][posi]=1;
		}

		if(!change_mark[1][pos[i].posj]){
			int posi=pos[i].posj;
			for(int p=3;p<=6;p++){
				calc(1,1,posi,p,1);
				calc(1,1,posi,p,2);
			}
			for(int j=0;j<15;j++){
				if(board[1][posi][j]==1)sum1_tmp+=score[1][1][posi][j]-score[0][1][posi][j];
				else if(board[1][posi][j]==2)sum2_tmp+=score[1][1][posi][j]-score[0][1][posi][j];
				score[1][1][posi][j]=0;
			}
			change_mark[1][posi]=1;
		}

		if(!change_mark[2][new_pos[i].posi_1]){
			int posi=new_pos[i].posi_1;
			for(int p=3;p<=min(6,min(31-posi,3+posi));p++){
				calc(1,2,posi,p,1);
				calc(1,2,posi,p,2);
			}
			int siz=min(1+posi,29-posi);
			for(int j=0;j<siz;j++){
				if(board[2][posi][j]==1)sum1_tmp+=score[1][2][posi][j]-score[0][2][posi][j];
				else if(board[2][posi][j]==2)sum2_tmp+=score[1][2][posi][j]-score[0][2][posi][j];
				score[1][2][posi][j]=0;
			}
			change_mark[2][posi]=1;
		}

		if(!change_mark[3][new_pos[i].posi_2]){
			int posi=new_pos[i].posi_2;
			for(int p=3;p<=min(6,min(31-posi,3+posi));p++){
				calc(1,3,posi,p,1);
				calc(1,3,posi,p,2);
			}
			int siz=min(1+posi,29-posi);
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
	return sum1_tmp-ratio*sum2_tmp;
}

inline void marker_update(int i, int j, int opt){
	int xmin = max(0, i - 1);
	int xmax = min(14, i + 1);
	int ymin = max(0, j - 1);
	int ymax = min(14,j + 1);//TODO
	for(int p=xmin;p<=xmax;p++){
		for(int q=ymin;q<=ymax;q++){
			if(p==i && q==j)continue;
			if(!tmp[p][q])marker[p][q]+=opt;
		}
	}
}


int dfs(int dep,int role,int prev){
//	counter++;
	if(dep >= 5){
//		counter2++;
		if(buf.contains(zobrist))return buf[zobrist];
		return buf[zobrist]=eval(5);
//		return eval(5);
	}
	//get valuable point
	//int val = role ? INT_MAX : INT_MIN;
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
						cout<<tmp_val<<endl;
						if(tmp_val>val){
							ans=make_pair(i,j);
							val=tmp_val;
						}
					}else val = max(val, dfs(dep+1, role&1?2:1, val));
				}else val = min(val, dfs(dep+1, role&1?2:1, val));
				zobrist^=hash_tab[i][j][role];

				marker_update(i, j, -1);
				marker[i][j] = mark_tmp;
				tmp[i][j] = 0;

				if(role&1){
					if(val>=prev)return val;
				}else{
					if(val<=prev)return val;
				}
			

			}
		}
	}
	return val;
	
}



//记录在某个方向上，是否有
void score_init(){
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
		//(1,0)  (2,1)
	}	

	for(int i=0;i<15;i++){
		for(int j=0;j<=i;j++)board[3][i][j]=tmp[j][i-j];
	}
	
	for(int i=1;i<15;i++){
		for(int j=0;j<15-i;j++)board[3][14+i][j]=tmp[i+j][14-j];
	}
	

	for(int i=0;i<15;i++){
		for(int j=3;j<=6;j++)calc(0,0,i,j,1),calc(0,0,i,j,2);
		for(int j=0;j<15;j++){
			if(board[0][i][j]==1)sum1+=score[0][0][i][j];
			else if(board[0][i][j]==2)sum2+=score[0][0][i][j];
		}
	}
//	cout<<sum1<<" "<<sum2<<endl;
	
	for(int i=0;i<15;i++){
		for(int j=3;j<=6;j++)calc(0,1,i,j,1),calc(0,1,i,j,2);
		for(int j=0;j<15;j++){
			if(board[1][i][j]==1)sum1+=score[0][1][i][j];
			else if(board[1][i][j]==2)sum2+=score[0][1][i][j];
		}
	}
//	cout<<sum1<<" "<<sum2<<endl;

	for(int i=0;i<29;i++){
		for(int j=3;j<=min(6,min(31-i,i+3));j++)calc(0,2,i,j,1),calc(0,2,i,j,2);
		int len=min(i+1,29-i);
		for(int j=0;j<len;j++){
			if(board[2][i][j]==1)sum1+=score[0][2][i][j];
			else if(board[2][i][j]==2)sum2+=score[0][2][i][j];
		}

	}
//	cout<<sum1<<" "<<sum2<<endl;

	for(int i=0;i<29;i++){
		for(int j=3;j<=min(6,min(31-i,i+3));j++)calc(0,3,i,j,1),calc(0,3,i,j,2);
		int len=min(i+1,29-i);
		for(int j=0;j<len;j++){
			if(board[3][i][j]==1)sum1+=score[0][3][i][j];
			else if(board[3][i][j]==2)sum2+=score[0][3][i][j];
		}
	}


}

void init(){
	memset(score,0,sizeof score);
	memset(board,0,sizeof board);
	memset(marker,0,sizeof marker);
	memset(pos,0,sizeof pos);
	memset(spj_sta,0,sizeof spj_sta);
	memset(change_mark,0,sizeof change_mark);
	buf.clear();
	special.clear();
	zobrist=0;
	sum1=sum2=0;
	score_init();
	marker_init();
	mt19937_64 rand_gen(time(0));
	for(int i=0;i<15;i++){
		for(int j=0;j<15;j++){
			for(int k=1;k<3;k++)hash_tab[i][j][k]=rand_gen();
		}
	}
}

int main(){
	tab[0][682] = FIVE;
	tab[0][1705] = FOUR;
	tab[0][3753] = BLOCK_FOUR;
	tab[0][1707] = BLOCK_FOUR;
	tab[0][3755] = DEAD_FOUR;
	tab[0][425] = THREE;
	tab[0][427] = BLOCK_THREE;
	tab[0][937] = BLOCK_THREE;
	tab[0][1451] = BETTER_BLOCK_THREE;
	tab[0][3749] = BETTER_BLOCK_THREE;
	tab[0][939] = DEAD_THREE;
	tab[0][105] = TWO;
	tab[0][107] = BLOCK_TWO;
	tab[0][233] = BLOCK_TWO;
	tab[0][235] = DEAD_TWO;
	tab[0][25] = ONE;
	tab[0][27] = BLOCK_ONE;
	tab[0][57] = BLOCK_ONE;
	tab[0][59] = DEAD_ONE;
	tab[0][618] = JUMP_FOUR;
	tab[0][666] = JUMP_FOUR;
	tab[0][678] = JUMP_FOUR;
	tab[0][166] = JUMP_THREE;
	tab[0][154] = JUMP_THREE;
	
	tab[1][1023] = FIVE;
	tab[1][2045] = FOUR;
	tab[1][3069] = BLOCK_FOUR;
	tab[1][2046] = BLOCK_FOUR;
	tab[1][3070] = DEAD_FOUR;
	tab[1][509] = THREE;
	tab[1][510] = BLOCK_THREE;
	tab[1][765] = BLOCK_THREE;
	tab[1][1534] = BETTER_BLOCK_THREE;
	tab[1][3061] = BETTER_BLOCK_THREE;
	tab[1][766] = DEAD_THREE;
	tab[1][125] = TWO;
	tab[1][126] = BLOCK_TWO;
	tab[1][189] = BLOCK_TWO;
	tab[1][190] = DEAD_TWO;
	tab[1][29] = ONE;
	tab[1][30] = BLOCK_ONE;
	tab[1][45] = BLOCK_ONE;
	tab[1][46] = DEAD_ONE;
	tab[1][895] = JUMP_FOUR;
	tab[1][991] = JUMP_FOUR;
	tab[1][1015] = JUMP_FOUR;
	tab[1][247] = JUMP_THREE;
	tab[1][223] = JUMP_THREE;
	
	spj[0][1707] = BLOCK_FOUR_TYPE; //冲4（右空）
	spj[0][3753] = BLOCK_FOUR_TYPE; //冲4（左空）
	spj[0][425] = THREE_TYPE; //活3
	spj[0][427] = BLOCK_THREE_TYPE; //冲3（右空）
	spj[0][937] = BLOCK_THREE_TYPE; //冲3（左空）
	spj[0][105] = TWO_TYPE; //活2

	spj[1][2046] = BLOCK_FOUR_TYPE; //冲4（右空）
	spj[1][3069] = BLOCK_FOUR_TYPE; //冲4（左空）
	spj[1][509] = THREE_TYPE; //活3
	spj[1][510] = BLOCK_THREE_TYPE; //冲3（右空）
	spj[1][765] = BLOCK_THREE_TYPE; //冲3（左空）
	spj[1][125] = TWO_TYPE; //活2
	
	int x,y;
	
/*	tmp[7][7]=2;
	tmp[7][6]=2;
	tmp[8][8]=2;
	tmp[6][6]=1;
	tmp[7][5]=1;
	tmp[8][6]=1;


	for(int i=0;i<15;i++){
		for(int j=0;j<15;j++)scanf("%d",&tmp[i][j]);
	}
	init();
	cout<<sum1<<" "<<sum2<<endl;
	int s1=0;
	int s2=0;
	int s3=0;
	int s4=0;

	for(auto it:special){
                s1+=it.role;
		s2+=it.type;
		s3+=it.row;
		s4+=it.val;
        }
	cout<<s1<<" "<<s2<<" "<<s3<<" "<<s4<<endl;
	exit(0);
*/
	while(~scanf("%d%d",&x,&y)){
		tmp[x][y]=2;
		init();
		cout<<"DEBUG: "<<dfs(0,1,INT_MAX)<<endl;
		tmp[ans.first][ans.second]=1;
		for(int i=0;i<15;i++){
			for(int j=0;j<15;j++)cout<<tmp[i][j]<<' ';
			cout<<endl;
		}
		cout<<endl;
	}
	
	//for(int i=0;i<15;i++)cout<<score[0][1][i]<<' ';
	/*
	for(int i=2;i<4;i++){
		for(int j=0;j<29;j++){
			for(int k=0;k<min(1+j,29-j);k++){
				cout<<score[0][i][j][k]<<' ';
			}
			cout<<endl;
		}
		cout<<endl;
	}
	*/
//	for(auto& it:special){
///		cout<<it.role<<' '<<it.type<<' '<<it.row<<' '<<it.val<<endl;
//	}
//	cout<<endl;
	
	return 0;
}

/*
 
0 2 0 0 0 1 2 0 0 0 0 2 1 0 1
2 0 0 0 0 0 1 0 0 2 0 0 0 2 0
1 0 2 0 0 0 2 1 2 0 0 0 1 1 2
2 0 0 0 0 1 1 0 0 0 0 0 2 0 0
1 0 2 0 2 0 2 0 0 1 2 1 0 1 1
2 1 1 2 2 0 1 0 0 2 2 1 0 2 1
0 0 0 0 0 0 2 0 0 2 0 1 0 0 2
1 0 0 0 1 2 1 1 0 1 0 2 1 0 1
2 2 1 0 0 0 1 1 0 1 1 0 0 0 2
0 1 0 0 1 0 2 0 1 2 0 2 0 0 1
0 0 0 2 2 1 0 1 2 1 0 0 0 2 1
2 0 0 1 0 1 0 1 2 1 0 0 1 1 0
0 0 0 0 0 1 1 2 2 0 1 0 0 2 0
1 2 0 0 2 0 2 0 2 0 1 0 0 0 2
0 2 0 0 0 1 2 0 1 0 1 0 1 0 1
 

0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
1 1 1 2 0 0 1 1 1 0 1 2 1 1 2
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0


0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
2 2 2 1 0 0 2 2 2 0 2 1 2 2 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

1 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 1 0 0 0 0 0 0 0 0 1 0 0 0 0
0 0 1 0 0 0 0 0 0 0 0 1 0 0 0
0 0 0 2 0 0 0 0 0 0 0 0 1 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 1 0 0 0 0 1 0 0 0 0 0 0 0 0
0 0 1 0 0 0 0 1 0 0 0 0 0 0 0
0 0 0 1 0 0 0 0 1 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 1 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 2 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 1 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 1 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 2


0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 1 0 0 0 0 0 0 1 0
0 0 0 0 0 1 0 0 0 0 0 0 1 0 0
0 0 0 0 1 0 0 0 0 0 0 2 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 1 0 0 0 0 0 0
0 0 0 0 0 0 0 1 0 0 0 0 0 0 1
0 0 0 0 0 0 1 0 0 0 0 0 0 1 0
0 0 0 0 0 0 0 0 0 0 0 0 1 0 0
0 0 0 0 1 0 0 0 0 0 0 1 0 0 0
0 0 0 2 0 0 0 0 0 0 0 0 0 0 0
0 0 1 0 0 0 0 0 0 0 0 0 0 0 0
0 1 0 0 0 0 0 0 0 0 0 0 0 0 0
2 0 0 0 0 0 0 0 0 0 0 0 0 0 0

 */
/*
1 1 1 1 1    10000000  682  1023

0 1 1 1 1 0   1000000  1705 2045
0 1 1 1 1 2   100000  3753  3069
2 1 1 1 1 0   100000  1707  2046
2 1 1 1 1 2   2000 3755  3070

0 1 1 1 0     10000  425  509
2 1 1 1 0     3000   427  510
2 1 1 1 0 0   6000   1451  1534
0 1 1 1 2     3000  937   765
0 0 1 1 1 2   6000   3749  3061
2 1 1 1 2     500  939  766

0 1 1 0       1000  105  125
2 1 1 0       200  107  126
0 1 1 2       200  233  189
2 1 1 2       50  235  190

0 1 0         50 25  29
2 1 0         20 27  30
0 1 2         20 57  45
2 1 2         1 59  46

1 1 1 0 1    1000000 618  895
1 1 0 1 1    1000000 666  991
1 0 1 1 1    1000000 678  1015

1 0 1 1  166  247
1 1 0 1  154  223


*/
