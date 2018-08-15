/*
 * Attempt2.cpp
 *
 *  Created on: Nov 16, 2017
 *      Author: NISHANT
 */



#include<iostream>
#include<fstream>
#include<sstream>
#include<stdlib.h>
using namespace std;

clock_t start_time;
int timeallwd;

struct knowledge{
	string pred[100];
	int nopred;
	int norg[2][100];
	string arg[1000];
}kb[10000],kb_clone[10000],newk[10000];
struct input{
	string qpred;
	int norg;
	string qarg[1000];
};


void copy_kb(int nsen)
{
	for(int i=0;i<nsen;i++)
	{
		kb_clone[i].nopred=kb[i].nopred;
		for(int j=0;j<kb[i].nopred;j++)
		{
			kb_clone[i].pred[j]=kb[i].pred[j];
			kb_clone[i].norg[0][j]=kb[i].norg[0][j];
			kb_clone[i].norg[1][j]=kb[i].norg[1][j];
			for(int k=kb[i].norg[0][j];k<kb[i].norg[0][j]+kb[i].norg[1][j];k++)
			{
				kb_clone[i].arg[k]=kb[i].arg[k];
			}
		}
	}
}

void del_pred(struct knowledge &clause,int nog,int index)
{
	for(int y=index;y<clause.nopred;y++)
	{
		clause.pred[y]=clause.pred[y+1];
	}
	for(int z=clause.norg[0][index];z<clause.norg[0][index]+clause.norg[1][index];z++)
	{
		for(int y=clause.norg[0][index];y<clause.norg[0][clause.nopred-1]+clause.norg[1][clause.nopred-1];y++)
		{
			clause.arg[y]=clause.arg[y+1];
		}
	}
	for(int y=index;y<clause.nopred;y++)
	{
		clause.norg[0][y]=clause.norg[0][y+1];
		clause.norg[1][y]=clause.norg[1][y+1];
		clause.norg[0][y]=clause.norg[0][y]-nog;
	}
	clause.nopred--;
}

void del_sen(struct knowledge resol[100],int index,int sen)
{
	for(int i=index;i<sen;i++)
		resol[i]=resol[i+1];
}

void del_sen2(int index,int sen)
{
	for(int i=index;i<sen;i++)
		newk[i]=newk[i+1];
}

void unify(struct knowledge &clause1,struct knowledge &clause2,int i,int j)
{
	for(int k=clause1.norg[0][i];k<clause1.norg[0][i]+clause1.norg[1][i];k++)
	{
		if(clause1.arg[k].front()<91)
		{
			string constant = clause1.arg[k];
			int index=k-clause1.norg[0][i];
			if(clause2.arg[clause2.norg[0][j]+index].front()>96)
			{
				string variable = clause2.arg[clause2.norg[0][j]+index];
				for(int p=0;p<clause2.norg[0][clause2.nopred-1]+clause2.norg[1][clause2.nopred-1];p++)
				{
					if(clause2.arg[p]==variable)
					{
						clause2.arg[p]=constant;
					}
				}
			}
		}
	}
}

void unify2(struct knowledge &clause1,struct knowledge &clause2,int i,int j)
{
	for(int k=clause1.norg[0][i];k<clause1.norg[0][i]+clause1.norg[1][i];k++)
	{
		if(clause1.arg[k].front()>96)
		{
			string constant = clause1.arg[k];
			int index=k-clause1.norg[0][i];
			if(clause2.arg[clause2.norg[0][j]+index].front()>96)
			{
				string variable = clause2.arg[clause2.norg[0][j]+index];
				for(int p=0;p<clause2.norg[0][clause2.nopred-1]+clause2.norg[1][clause2.nopred-1];p++)
				{
					if(clause2.arg[p]==variable)
					{
						clause2.arg[p]=constant;
					}
				}
			}
		}
	}
}

int subset(struct knowledge resol[100],int nsen1,int nsen2)
{
	int samesen=0,argcount=0,nog=0,predcount=0,npred=0;
	for(int i=0;i<nsen2;i++)
	{
		for(int j=0;j<nsen1;j++)
		{
			//Check if number of predicates equal----------------------------------------------------
			if(resol[i].nopred==newk[j].nopred)
			{
				predcount=0;
				npred=resol[i].nopred;
				for(int x=0;x<resol[i].nopred;x++)
				{
					for(int y=0;y<newk[j].nopred;y++)
					{
						//Check if predicates are equal----------------------------------------------
						if(resol[i].pred[x]==newk[j].pred[y])
						{
							nog=resol[i].norg[1][x];
							argcount=0;
							//Check if arguments equal-----------------------------------------------
							for(int m=resol[i].norg[0][x],n=newk[j].norg[0][y];m<resol[i].norg[0][x]+resol[i].norg[1][x] && n<newk[j].norg[0][y]+newk[j].norg[1][y];m++,n++)
							{
								if(resol[i].arg[m].front()>96 && newk[j].arg[n].front()>96)
									argcount++;
								else if((resol[i].arg[m].front()<91 || newk[j].arg[n].front()<91) && resol[i].arg[m]==newk[j].arg[n])
									argcount++;
							}
							if(argcount==nog)
								predcount++;
						}
					}
				}
				if(predcount==npred)
				{
					samesen++;
					del_sen(resol,i,nsen2);
					nsen2--;
					i--;
					break;
				}
			}
		}
	}
	return samesen;
}

int subset2(int nsen1,int nsen2)
{
	int samesen=0,argcount=0,nog=0,predcount=0,npred=0;
	for(int i=0;i<nsen2;i++)
	{
		for(int j=0;j<nsen1;j++)
		{
			//Check if number of predicates equal----------------------------------------------------
			if(newk[i].nopred==kb[j].nopred)
			{
				predcount=0;
				npred=newk[i].nopred;
				for(int x=0;x<newk[i].nopred;x++)
				{
					for(int y=0;y<kb[j].nopred;y++)
					{
						//Check if predicates are equal----------------------------------------------
						if(newk[i].pred[x]==kb[j].pred[y])
						{
							nog=newk[i].norg[1][x];
							argcount=0;
							//Check if arguments equal-----------------------------------------------
							for(int m=newk[i].norg[0][x],n=kb[j].norg[0][y];m<newk[i].norg[0][x]+newk[i].norg[1][x] && n<kb[j].norg[0][y]+kb[j].norg[1][y];m++,n++)
							{
								if(newk[i].arg[m].front()>96 && kb[j].arg[n].front()>96)
									argcount++;
								else if((newk[i].arg[m].front()<91 || kb[j].arg[n].front()<91) && newk[i].arg[m]==kb[j].arg[n])
									argcount++;
							}
							if(argcount==nog)
								predcount++;
						}
					}
				}
				if(predcount==npred)
				{
					samesen++;
					del_sen2(i,nsen2);
					nsen2--;
					i--;
					break;
				}
			}
		}
	}
	return samesen;
}

void unionset(struct knowledge resol[100],int nsen1,int nsen2)
{
	for(int i=nsen1;i<nsen1+nsen2;i++)
	{
		newk[i]=resol[i-nsen1];
	}
}

void unionset2(int nsen1,int nsen2)
{
	for(int i=nsen1;i<nsen1+nsen2;i++)
	{
		kb[i]=newk[i-nsen1];
	}
}

struct knowledge fol_resolve(struct knowledge cl1,struct knowledge cl2)
{
	int count=0;
	struct knowledge res;
	res.pred[0]="AwesomeNishant";
	res.nopred=0;
	res.norg[0][0]=0;
	res.norg[1][0]=0;
	res.arg[0]="AweNish";
	int flag=0;
	for(int i=0;i<cl1.nopred;i++)
	{
		for(int j=0;j<cl2.nopred;j++)
		{
			//check if ~predicate = predicate-----------------------------------------------
			if((cl1.pred[i].front()=='~' && cl2.pred[j].front()!='~') ||(cl1.pred[i].front()!='~' && cl2.pred[j].front()=='~'))
			{
				string pred1 = cl1.pred[i];
				string pred2 = cl2.pred[j];
				if(cl1.pred[i].front()=='~')
					pred1.erase(0,1);
				else
					pred2.erase(0,1);
				//check if predicates are equal---------------------------------------------
				if(pred1==pred2)
				{
					int argcount=0;
					int nog=cl1.norg[1][i];
					//check if arguments are resolvable-------------------------------------
					for(int m=cl1.norg[0][i],n=cl2.norg[0][j];m<cl1.norg[0][i]+cl1.norg[1][i] && n<cl2.norg[0][j]+cl2.norg[1][j];m++,n++)
					{
						if(cl1.arg[m].front()<91 && cl2.arg[n].front()<91 && cl1.arg[m]!=cl2.arg[n])
							break;
						argcount++;
					}
					if(nog==argcount)
					{
						count++;
						//unify constant of first clause with variables of second clause----------
						unify(cl1,cl2,i,j);
						//unify constant of second clause with variables of first clause----------
						unify(cl2,cl1,j,i);
						//assign variables of second clause to variables of first clause-----------
						unify2(cl2,cl1,j,i);
						//Delete predicate from first clause--------------------------------------
						del_pred(cl1,nog,i);
						//Delete predicate from second clause-------------------------------------
						del_pred(cl2,nog,j);
						flag=1;
						break;
					}
				}
			}
		}
		if(flag==1)
			break;
	}
	if(count!=0)
	{
		//Combine first and second clause and return------------------------
		res=cl1;
		res.nopred=res.nopred+cl2.nopred;
		for(int i=cl1.nopred;i<res.nopred;i++)
		{
			res.pred[i]=cl2.pred[i-cl1.nopred];
			res.norg[0][i]=cl2.norg[0][i-cl1.nopred]+cl1.norg[0][cl1.nopred-1]+cl1.norg[1][cl1.nopred-1];
			res.norg[1][i]=cl2.norg[1][i-cl1.nopred];
		}
		for(int i=cl1.norg[0][cl1.nopred-1]+cl1.norg[1][cl1.nopred-1];i<res.norg[0][res.nopred-1]+res.norg[1][res.nopred-1];i++)
		{
			res.arg[i]=cl2.arg[i-cl1.norg[0][cl1.nopred-1]-cl1.norg[1][cl1.nopred-1]];
		}
	}
	return res;
}

int fol_resolution(int nsen)
{
	struct knowledge res;
	int newkcount=0;
	while(1)
	{
		for(int i=0;i<nsen;i++)
		{
			for(int j=i+1;j<nsen;j++)
			{
				//Resolve sentences--------------------------------------------------------
				res=fol_resolve(kb[i],kb[j]);
				//If contradiction occurs then true----------------------------------------
				if(res.pred[0].empty())
				{
					return 1;
				}
				//If some clauses remain after resolving two sentences---------------------
				if(res.pred[0]!="AwesomeNishant")
				{

					struct knowledge resarray[100];
					resarray[0].nopred=res.nopred;
					for(int x=0;x<res.nopred;x++)
					{
						resarray[0].pred[x]=res.pred[x];
						resarray[0].norg[0][x]=res.norg[0][x];
						resarray[0].norg[1][x]=res.norg[1][x];
					}
					for(int x=0;x<res.norg[0][res.nopred-1]+res.norg[1][res.nopred-1];x++)
						resarray[0].arg[x]=res.arg[x];
					//Check if sentence in resarray already exists in newk------------------
					int nosubset=subset(resarray,newkcount,1);
					if(nosubset==0)
					{
						//Combine resarray and newk-----------------------------------------
						unionset(resarray,newkcount,1);
						newkcount++;

					}
				}
				clock_t end_time = clock();
				clock_t ticks = end_time - start_time;
				double timesec = ticks / (double) CLOCKS_PER_SEC;
				if(newkcount>9000 || timesec>timeallwd || timesec>180)
					return 0;
			}
		}
		//Check if newk subset of kb--------------------------------------------------------
		int newksubset=subset2(nsen,newkcount);
		newkcount=newkcount-newksubset;
		if(newkcount==0)
			return 0;
		//Combine newk and kb---------------------------------------------------------------
		unionset2(nsen,newkcount);
		nsen=nsen+newkcount;
		clock_t end_time = clock();
		clock_t ticks = end_time - start_time;
		double timesec = ticks / (double) CLOCKS_PER_SEC;
		if(nsen>9000 || timesec>timeallwd || timesec>180)
			return 0;
	}
	return 0;
}

int main()
{
//Declaration of variables---------------------
	string line,dummy;
	int nquery,val,nsen;
	struct input query[100];
	ifstream infile;

//Taking input from file----------------------------------
	infile.open("input.txt");

//Input queries-------------------------------------------
	getline(infile,line);
	stringstream ob1(line);
	ob1>>nquery;
	for(int i=0;i<nquery;i++)
	{
		int j=0;
		getline(infile,line,'(');
		query[i].qpred=line;
		getline(infile,line,')');
		stringstream ob2(line);
		while(getline(ob2,query[i].qarg[j],','))
			j++;
		query[i].norg=j;
		getline(infile,dummy);
	}

//Input Knowledge Base--------------------------------
	getline(infile,line);
	stringstream ob3(line);
	ob3>>nsen;
	string predicates;
	for(int i=1;i<=nsen;i++)
	{
		int j=0;
		int ind=0;
		getline(infile,line);
		stringstream ob4(line);
		while(getline(ob4,predicates,'|'))
		{
			int count=0;
			stringstream ob5(predicates);
			getline(ob5,kb[i].pred[j],'(');
			if(kb[i].pred[j].front()==' ')
				kb[i].pred[j].erase(0,1);
			getline(ob5,line,')');
			stringstream ob6(line);
			while(getline(ob6,kb[i].arg[ind],','))
			{
				if(kb[i].arg[ind].front()>96)
				{
					stringstream sing;
					sing << i;
					string str = sing.str();
					kb[i].arg[ind].append(str);
				}
				count++;
				ind++;
			}
			if(j==0)
				kb[i].norg[0][j]=0;
			else
				kb[i].norg[0][j]=kb[i].norg[0][j-1]+kb[i].norg[1][j-1];
			kb[i].norg[1][j]=count;
			j++;
		}
		kb[i].nopred=j;
	}
	infile.close();

	nsen++;
	ofstream outfile("output.txt");
	for(int i=0;i<nquery;i++)
	{
		string temp=query[i].qpred;
		string temp2="~";
		if(temp.front()=='~')
		{
			temp.erase(0,1);
			temp2=temp;
		}
		else
			temp2.append(temp);
		kb[0].pred[0]=temp2;
		kb[0].nopred=1;
		kb[0].norg[0][0]=0;
		kb[0].norg[1][0]=query[i].norg;
		for(int j=0;j<query[i].norg;j++)
			kb[0].arg[j]=query[i].qarg[j];

//Call FOL resolution----------------------------------
		copy_kb(nsen);
		timeallwd=300/nquery;
		start_time=clock();
		val=fol_resolution(nsen);
		//Write output-----------------------------------------
		if(val==1)
			outfile<<"TRUE\n";
		else
			outfile<<"FALSE\n";
	}
	outfile.close();
	return 0;
}
