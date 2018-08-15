/*
 * Final3.cpp
 *
 *  Created on: Sep 20, 2017
 *      Author: NISHANT
 */

#include<iostream>
#include<deque>
#include<vector>
#include<fstream>
#include<sstream>
#include<math.h>
#include<stdlib.h>
using namespace std;

clock_t start_time;

int check(int arr[1000][1000],int row,int col,int n)
{
	if(arr[row][col]==2)
		return 0;
	for(int i=row;i>=0;i--)
	{
		if(arr[i][col]==2)
			break;
		if(arr[i][col]==1)
		{
			return 0;
		}
	}
	for(int i=col;i<n;i++)
	{
		if(arr[row][i]==2)
			break;
		if(arr[row][i]==1)
		{
			return 0;
		}
	}
	for(int i=row,j=col;i>=0 && j>=0;i--,j--)
	{
		if(arr[i][j]==2)
			break;
		if(arr[i][j]==1)
		{
			return 0;
		}
	}
	for(int i=row,j=col;i>=0 && j<n;i--,j++)
	{
		if(arr[i][j]==2)
			break;
		if(arr[i][j]==1)
		{
			return 0;
		}
	}
	return 1;
}

int probfunc(double prob)
{
	double val = (double) (rand()%10000)/10000;
	if(val<prob)
		return 1;
	return 0;
}

int numOfConflicts(int arr[1000][1000], int n, int p)
{
	int attacked=0;
	for(int row=0;row<n;row++)
	{
		for(int col=0;col<n;col++)
		{
			if(arr[row][col]==1)
			{
				for(int i=row-1;i>=0;i--)
				{
					if(arr[i][col]==2)
						break;
					if(arr[i][col]==1)
					{
						attacked++;
						break;
					}
				}
				for(int i=col-1;i>=0;i--)
				{
					if(arr[row][i]==2)
						break;
					if(arr[row][i]==1)
					{
						attacked++;
						break;
					}
				}
				for(int i=row-1,j=col-1;i>=0 && j>=0;i--,j--)
				{
					if(arr[i][j]==2)
						break;
					if(arr[i][j]==1)
					{
						attacked++;
						break;
					}
				}
				for(int i=row-1,j=col+1;i>=0 && j<n;i--,j++)
				{
					if(arr[i][j]==2)
						break;
					if(arr[i][j]==1)
					{
						attacked++;
						break;
					}
				}
			}
		}
	}
	return attacked;
}

int SA(int arr[1000][1000],int n,int p)
{
	srand(time(0));
	int numqueens=0,posn[2][100];
	double T=10000,iter=2;
	while(numqueens!=p)
	{
		int ipos = rand()%n;
		int jpos = rand()%n;
		if(arr[ipos][jpos]==0)
		{
			arr[ipos][jpos]=1;
			posn[0][numqueens]=ipos;
			posn[1][numqueens]=jpos;
			numqueens++;
		}
	}
	double energy1 = numOfConflicts(arr,n,p);
	if(energy1==0)
		return 1;
	while(1)
	{
		clock_t end_time = clock();
		clock_t ticks = end_time - start_time;
		double timesec = ticks / (double) CLOCKS_PER_SEC;
		T=1/log(iter);
		iter++;
		if(iter>10000000 || timesec>270)
			return 0;
		double energy1 = numOfConflicts(arr,n,p);
		int queenum=rand()%p;
		int iloc = rand()%n;
		int jloc = rand()%n;
		while(arr[iloc][jloc]!=0)
		{
			iloc = rand()%n;
			jloc = rand()%n;
		}
		int vpos=posn[0][queenum];
		int hpos=posn[1][queenum];
		arr[vpos][hpos]=0;
		arr[iloc][jloc]=1;
		posn[0][queenum]=iloc;
		posn[1][queenum]=jloc;
		double energy2 = numOfConflicts(arr,n,p);
		if(energy2==0)
			return 1;
		double delta = energy2-energy1;
		if(delta>0)
		{
			double prob = exp(-delta/T);
			if(!probfunc(prob))
			{
				arr[vpos][hpos]=1;
				arr[iloc][jloc]=0;
				posn[0][queenum]=vpos;
				posn[1][queenum]=hpos;
			}
		}
	}
}

int dfs(int arr[1000][1000],int n,int p)
{
	int rowpar,colpar,lizp=0,noOfChild[100],state=1,i=0,j=0,flag=0;
	for(int q=0;q<100;q++)
	{
		noOfChild[q]=0;
	}
	deque <int> queue1;
	vector<int> stack1;
	if(lizp==p)
		return 1;
	if(arr[0][0]==2)
	{
		while(arr[i][j]==2)
		{
			j++;
			if(j==n)
			{
				i++;
				j=0;
			}
		}
	}
	while(arr[i][j]!=2 && j<n)
	{
		queue1.push_front(j);
		queue1.push_front(i);
		j++;
	}
	if(j==n)
	{
		j=0;
		i++;
	}
	while(arr[i][j]==2)
	{
		j++;
		if(j==n)
		{
			i++;
			j=0;
		}
	}
	deque<int>::iterator it2d;
	vector<int>::iterator it3d;
	while(1)
	{
		clock_t end_time = clock();
		clock_t ticks = end_time - start_time;
		double timesec = ticks / (double) CLOCKS_PER_SEC;
		if(queue1.size()==0 || (i==n && j>0) || timesec>210)
			flag=1;
		if(flag==1)
		{
			flag=SA(arr,n,p);
			return flag;
		}
		int row=queue1.front();
		queue1.pop_front();
		int col=queue1.front();
		queue1.pop_front();
		noOfChild[state-1]--;

		if(check(arr,row,col,n))
		{
			arr[row][col]=1;
			stack1.push_back(row);
			stack1.push_back(col);
			lizp++;
			if(lizp==p)
				return 1;

			while(arr[i][j]!=2 && j<n && i<n)
			{
				queue1.push_front(j);
				queue1.push_front(i);
				noOfChild[state]++;
				j++;
			}
			if(j==n)
			{
				j=0;
				i++;
			}
			while(arr[i][j]==2)
			{
				j++;
				if(j==n)
				{
					i++;
					j=0;
				}
			}
			state++;
		}
		while(noOfChild[state-1]==0)
		{
			state--;
			colpar=stack1.back();
			stack1.pop_back();
			rowpar=stack1.back();
			stack1.pop_back();
			arr[rowpar][colpar]=0;
			i=rowpar;
			j=colpar;
			while(arr[i][j]!=2 && j<n)
			{
				j++;
			}
			if(j==n)
			{
				j=0;
				i++;
			}
			while(arr[i][j]==2)
			{
				j++;
				if(j==n)
				{
					i++;
					j=0;
				}
			}
			lizp--;
		}
	}
}

int bfs(int arr[1000][1000],int n,int p)
{
	int rowpar,colpar,lizp=0,noOfChild[100],i=0,j=0,flag=0;
	deque< deque<int> > queue1;
	deque <int> deq1;
	deque <int> deq2;
	vector<int> stack1;
	if(lizp==p)
		return 1;
	if(arr[0][0]==2)
	{
		while(arr[i][j]==2)
		{
			j++;
			if(j==n)
			{
				i++;
				j=0;
			}
		}
	}
	while(arr[i][j]!=2 && j<n)
	{
		deq1.push_front(j);
		deq2.push_front(i);
		queue1.push_back(deq2);
		queue1.push_back(deq1);
		deq1.pop_front();
		deq2.pop_front();
		j++;
	}

	if(j==n)
	{
		j=0;
		i++;
	}
	while(arr[i][j]==2)
	{
		j++;
		if(j==n)
		{
			i++;
			j=0;
		}
	}
	deque< deque<int> >::iterator it2d;
	while(1)
	{
		clock_t end_time = clock();
		clock_t ticks = end_time - start_time;
		double timesec = ticks / (double) CLOCKS_PER_SEC;
		if(queue1.size()==0 || (i==n && j>0) || timesec>210)
			flag=1;
		if(flag==1)
		{
			flag=SA(arr,n,p);
			return flag;
		}
		deq1 = queue1.front();
		int row = deq1.front();
		queue1.pop_front();
		deq2 = queue1.front();
		int col=deq2.front();
		queue1.pop_front();

		for(int it=1;it<deq1.size();it++)
		{
			int rpos=deq1[it];
			int cpos=deq2[it];
			arr[rpos][cpos]=1;
			lizp++;
		}
		if(check(arr,row,col,n))
		{
			arr[row][col]=1;
			lizp++;
			if(lizp==p)
				return 1;
			i=row;
			j=col;
			while(arr[i][j]!=2 && j<n && i<n)
			{
				j++;
			}
			if(j==n)
			{
				j=0;
				i++;
			}
			while(arr[i][j]==2)
			{
				j++;
				if(j==n)
				{
					i++;
					j=0;
				}
			}
			while(arr[i][j]!=2 && j<n && i<n)
			{

				deq1.push_front(i);
				deq2.push_front(j);
				queue1.push_back(deq1);
				queue1.push_back(deq2);
				deq1.pop_front();
				deq2.pop_front();
				j++;
			}
			if(j==n)
			{
				j=0;
				i++;
			}
			while(arr[i][j]==2)
			{
				j++;
				if(j==n)
				{
					i++;
					j=0;
				}
			}
		}
		for(int it=0;it<deq1.size();it++)
		{
			int rpos=deq1[it];
			int cpos=deq2[it];
			arr[rpos][cpos]=0;
		}
		deq1.clear();
		deq2.clear();
		lizp=0;
	}
}

int main()
{
	start_time=clock();
	int arr[1000][1000],n=0,p=0,val,i=0,j;
	string line, met;
	char ch,ch2;
	ifstream infile;
	infile.open("input.txt");
	while(!infile.eof())
	{
		if(i==0)
		{
			infile.get(ch);
			ch2=ch;
			getline(infile,line);
			met=line;
		}
		else if(i==1)
		{
			getline(infile,line);
			stringstream ob1(line);
			ob1>>n;
		}
		else if(i==2)
		{
			getline(infile,line);
			stringstream ob2(line);
			ob2>>p;
		}
		else
		{
			for(int x=0;x<n+1;x++)
			{
				infile.get(ch);
				int bin=(int)ch-48;
				if(bin==0||bin==2)
				{
					arr[i-3][x]=bin;
				}
			}
		}
		i++;
	}
	infile.close();
	int seg=0;
	for(i=0;i<n;i++)
	{

		for(j=0;j<n;j++)
		{

			while(arr[i][j]==2 && i<n)
			{
				j++;
				if(j==n)
				{
					j=0;i++;
				}
			}
			if(arr[i][j]==0 && j!=n-1)
			{
				seg++;
			}
			while(arr[i][j]!=2)
			{
				j++;
				if(j==n && i<n)
				{
					seg++;
					j=0;i++;
				}
			}
		}
	}
	int allwd=seg-p;
	if(allwd<0)
		val=0;
	else if(ch2=='D')
		val = dfs(arr,n,p);
	else if(ch2=='B')
		val=bfs(arr,n,p);
	else
		val=SA(arr,n,p);
		ofstream outfile("output.txt");
		if(val==1)
		{
			outfile<<"OK\n";
			for(i=0;i<n;i++)
			{
				for(j=0;j<n;j++)
				{
					outfile<<arr[i][j];
				}
				outfile<<"\n";
			}
		}
		else
			outfile<<"FAIL\n";
		outfile.close();
	return 0;
}






























