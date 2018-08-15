/*
 * Attempt10.cpp
 *
 *  Created on: Oct 16, 2017
 *      Author: NISHANT
 */


#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<stdlib.h>
#include<algorithm>
#include<math.h>
using namespace std;

struct choices{
	int count;
	int rindex;
	int cindex;
};

double timeg;
int allwd_depth=4;
int flag=0;
int choose[1][2];
vector<int> visited;
int max_val(int arr[100][100], int n, int val,int alpha,int beta,int depth);
int min_val(int arr[100][100], int n, int val,int alpha,int beta,int depth);

void copy_arr(int arr1[100][100],int arr2[100][100],int n)
{
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			arr2[i][j]=arr1[i][j];
		}
	}
}

int max(int a,int b)
{
	if(a>=b)
		return a;
	return b;
}

int min(int a,int b)
{
	if(a<b)
		return a;
	return b;
}

bool comp_count(choices a,choices b)
{
	return a.count>b.count;
}


void cal_depth(int branch)
{
	if(timeg<=2)
		allwd_depth=2;
	else if(timeg>2 && timeg<=15)
	{
		if(branch>230)
			allwd_depth=2;
		else if(branch>95 && branch<=230)
			allwd_depth=4;
		else if(branch>45 && branch<=95)
			allwd_depth=4;
		else if(branch>25 && branch<=45)
			allwd_depth=6;
		else if(branch<=25)
			allwd_depth=6;
	}
	else if(timeg>15 && timeg<=60)
	{
		if(branch>=500)
			allwd_depth=2;
		else if(branch>230 && branch<500)
			allwd_depth=4;
		else if(branch>95 && branch<=230)
			allwd_depth=4;
		else if(branch>60 && branch<=95)
			allwd_depth=4;
		else if(branch>50 && branch<=60)
			allwd_depth=6;
		else if(branch>20 && branch<=50)
			allwd_depth=6;
		else if(branch<=20)
			allwd_depth=8;
	}
	else if(timeg>60 && timeg<=120)
	{
		if(branch>=575)
			allwd_depth=2;
		else if(branch>230 && branch<575)
			allwd_depth=4;
		else if(branch>95 && branch<=230)
			allwd_depth=4;
		else if(branch>65 && branch<=95)
			allwd_depth=6;
		else if(branch>50 && branch<=65)
			allwd_depth=6;
		else if(branch>30 && branch<=50)
			allwd_depth=6;
		else if(branch<=30)
			allwd_depth=8;
	}
	else if(timeg>120 && timeg<=240)
	{
		if(branch>=575)
			allwd_depth=2;
		else if(branch>330 && branch<575)
			allwd_depth=4;
		if(branch>230 && branch<=330)
			allwd_depth=4;
		else if(branch>95 && branch<=230)
			allwd_depth=4;
		else if(branch>65 && branch<=95)
			allwd_depth=6;
		else if(branch>50 && branch<=65)
			allwd_depth=6;
		else if(branch>30 && branch<=50)
			allwd_depth=8;
		else if(branch<=30)
			allwd_depth=8;
	}
	else if(timeg>240)
	{
		if(branch>=600)
			allwd_depth=2;
		else if(branch>330)
			allwd_depth=4;
		else if(branch>150 && branch<=330)
			allwd_depth=4;
		else if(branch>95 && branch<=150)
			allwd_depth=6;
		else if(branch>65 && branch<=95)
			allwd_depth=6;
		else if(branch>50 && branch<=65)
			allwd_depth=8;
		else if(branch>30 && branch<=50)
			allwd_depth=8;
		else if(branch<=30)
			allwd_depth=10;
	}
}

void gravity(int arr[100][100],int n)
{
	for(int j=0;j<n;j++)
	{
		for(int i=0;i<n;i++)
		{
			if(arr[i][j]==-6)
			{
				int x=i;
				while(x!=0)
				{
					arr[x][j]=arr[x-1][j];
					arr[x-1][j]=-6;
					x--;
				}
			}
		}
	}
}

int dfs_val(int arr[100][100],int n, int row, int col,int x, int val)
{
	arr[row][col]= -6;
	val=val+1;
	visited.push_back(row);
	visited.push_back(col);
	if(row>0 && arr[row-1][col]==x)
		val=dfs_val(arr,n,row-1,col,x,val);
	if(col<n-1 && arr[row][col+1]==x)
		val=dfs_val(arr,n,row,col+1,x,val);
	if(row<n-1 && arr[row+1][col]==x)
		val=dfs_val(arr,n,row+1,col,x,val);
	if(col>0 && arr[row][col-1]==x)
		val=dfs_val(arr,n,row,col-1,x,val);
	return val;
}

int dfs_branching(int arr[100][100],int n, int row, int col,int x, int count)
{
	arr[row][col]= -6;
	count=count+1;
	if(row>0 && arr[row-1][col]==x)
		count=dfs_branching(arr,n,row-1,col,x,count);
	if(col<n-1 && arr[row][col+1]==x)
		count=dfs_branching(arr,n,row,col+1,x,count);
	if(row<n-1 && arr[row+1][col]==x)
		count=dfs_branching(arr,n,row+1,col,x,count);
	if(col>0 && arr[row][col-1]==x)
		count=dfs_branching(arr,n,row,col-1,x,count);
	return count;
}

void dfs2_reverse(int arr[100][100], int x, int val2)
{
	if(val2!=0)
	{
		int col=visited.back();
		visited.pop_back();
		int row=visited.back();
		visited.pop_back();
		arr[row][col]=x;
		dfs2_reverse(arr,x,val2-1);
	}
}

int max_val(int arr[100][100], int n, int val,int alpha,int beta,int depth)
{

	if(depth==allwd_depth+1)
		return val;
	int arr_clone[100][100],arr_clone2[100][100],index=0,score=-100000,ret_val;
	choices max_ch[1000];
	gravity(arr,n);
	copy_arr(arr,arr_clone2,n);
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			while(arr_clone2[i][j]==-6 && i<n)
			{
				j++;
				if(j==n)
				{
					j=0;
					i++;
				}
			}
			if(i==n)
				break;
			int cnt=dfs_branching(arr_clone2,n,i,j,arr_clone2[i][j],0);
			max_ch[index].count=cnt;
			max_ch[index].rindex=i;
			max_ch[index].cindex=j;
			index++;
		}
	}
	if(flag==0)
		cal_depth(index);
	flag=1;
	if(allwd_depth==1)
		cout<<index<<endl;
	if(index==0)
		return val;
	sort(max_ch,max_ch+index,comp_count);
	for(int i=0;i<index;i++)
	{
		int row=max_ch[i].rindex;
		int col=max_ch[i].cindex;
		int elem = arr[row][col];
		copy_arr(arr,arr_clone,n);
		int val2=dfs_val(arr_clone,n,row,col,elem,0);
		val=val+pow(val2,2);
		ret_val=min_val(arr_clone,n,val,alpha,beta,depth+1);
		if(ret_val>=score && depth==1)
		{
			choose[0][0]=row;
			choose[0][1]=col;
		}
		score = max(score,ret_val);
		val=val-pow(val2,2);
		if(score>=beta)
			return score;
		alpha=max(alpha,score);
	}
	return score;
}

int min_val(int arr[100][100], int n, int val,int alpha,int beta,int depth)
{

	if(depth==allwd_depth+1)
		return val;
	int arr_clone[100][100],arr_clone2[100][100],index=0,score=10000000,ret_val;
	choices min_ch[1000];
	gravity(arr,n);
	copy_arr(arr,arr_clone2,n);
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			while(arr_clone2[i][j]==-6 && i<n)
			{
				j++;
				if(j==n)
				{
					j=0;
					i++;
				}
			}
			if(i==n)
				break;
			int cnt=dfs_branching(arr_clone2,n,i,j,arr_clone2[i][j],0);
			min_ch[index].count=cnt;
			min_ch[index].rindex=i;
			min_ch[index].cindex=j;
			index++;
		}
	}
	if(index==0)
		return val;
	sort(min_ch,min_ch+index,comp_count);
	for(int i=0;i<index;i++)
	{
		int row=min_ch[i].rindex;
		int col=min_ch[i].cindex;
		int elem = arr[row][col];
		copy_arr(arr,arr_clone,n);
		int val2=dfs_val(arr_clone,n,row,col,elem,0);
		val=val-pow(val2,2);
		ret_val=max_val(arr_clone,n,val,alpha,beta,depth+1);
		score = min(score,ret_val);
		val=val+pow(val2,2);
		if(score<=alpha)
			return score;
		beta=min(beta,score);
	}
	return score;
}

int main()
{
	int arr[100][100],n,p,val,i=0,j,alpha,beta,depth=1;
	double t;
	string line, met;
	char ch;
	ifstream infile;
	infile.open("input.txt");
	while(!infile.eof())
	{
		if(i==0)
		{
			getline(infile,line);
			stringstream ob1(line);
			ob1>>n;
		}
		else if(i==1)
		{
			getline(infile,line);
			stringstream ob2(line);
			ob2>>p;

		}
		else if(i==2)
		{
			getline(infile,line);
			stringstream ob3(line);
			ob3>>t;
		}
		else
		{
			for(int x=0;x<n+1;x++)
			{
				infile.get(ch);
				int bin=(int)ch-48;
				if((bin>=0 && bin<9) || bin==-6)
				{
					arr[i-3][x]=bin;
				}
			}
		}
		i++;
	}
	infile.close();
	timeg = t;
	val=0;
	alpha=-10000000;
	beta=10000000;
	val=max_val(arr,n,val,alpha,beta,depth);
	int rowin=choose[0][0];
	int colin=choose[0][1];
	int final_value=dfs_val(arr,n,rowin,colin,arr[rowin][colin],0);
	gravity(arr,n);
	ofstream outfile("output.txt");
	char cval=char(colin+65);
	int rval=rowin+1;
	outfile<<cval<<rval<<"\n";
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			if(arr[i][j]==-6)
				outfile<<"*";
			else
				outfile<<arr[i][j];
		}
		outfile<<"\n";
	}
	outfile.close();
	return 0;
}

