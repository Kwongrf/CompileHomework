#include <iostream>
#include <set>
#include <string.h>
#include <stack>
using namespace std;

char Terminal[8] = {'+','-','*','/','(',')','N','$'};
struct Nonterminal{
  set<char> First;
  set<char> Follow;
  //vector<string> expression;
};
struct Nonterminal E,R,T,Y,F;
void error()
{
  cout<<"ERROR"<<endl;
  exit(-1);
}
set<char> FIRST(string S)
{
  const char * mystr=S.c_str();
  char c = mystr[0];
  switch (c)
  {
    case 'E':
      return E.First;
    case 'R':
      return R.First;
    case 'T':
      return T.First;
    case 'Y':
      return Y.First;
    case 'F':
      return F.First;
    default:
      set<char> s;
      s.insert(c);
      return s;
  }
}
set<char> FOLLOW(char A)
{
  switch (A)
  {
    case 'E':
      return E.Follow;
    case 'R':
      return R.Follow;
    case 'T':
      return T.Follow;
    case 'Y':
      return Y.Follow;
    case 'F':
      return F.Follow;
    default:
      error();
  }
}
int order(char A)
{
  switch (A)
  {
    case 'E':
    case '+':
      return 0;
    case 'R':
    case '-':
      return 1;
    case 'T':
    case '*':
      return 2;
    case 'Y':
    case '/':
      return 3;
    case 'F':
    case '(':
      return 4;
    case ')':
      return 5;
    case 'N':
      return 6;
    case '$':
      return 7;
    default:
      error();
  }
}

string G[10][2]={
                 {"E","TR"},
                 {"R","+TR"},
                 {"R","-TR"},
                 {"R","@"},
                 {"T","FY"},
                 {"Y","*FY"},
                 {"Y","/FY"},
                 {"Y","@"},
                 {"F","(E)"},
                 {"F","N"}
               };
string M[5][8];
void initM()
{
  for(int i=0;i<5;i++)
  {
    for(int j=0;j<8;j++)
    {
      M[i][j] = "";
    }
  }
  //以上为初始化过程

  for(int i=0;i<10;i++)
  {
    string left = G[i][0];
    const char * mystr=left.c_str();
    char c = mystr[0];
    string right = G[i][1];
    set<char> first = FIRST(right);
    for(int j=0;j<8;j++)
    {
      set<char>::iterator pos = first.find(Terminal[j]);
      if(pos != first.end())
      {
        M[order(c)][j] = right;
      }
    }
    set<char>::iterator pos1 = first.find('@');
    if(pos1 != first.end())
    {
      int k = 0;
      set<char> follow = FOLLOW(c);
      for(int k=0;k<8;k++)
      {
        set<char>::iterator pos2 = follow.find(Terminal[k]);
        if(pos2 != follow.end())
        {
          M[order(c)][k] = right;
        }
      }
    }
  }

  for(int i=0;i<5;i++)
  {
    for(int j=0;j<8;j++)
    {
      if(M[i][j] == "")
        M[i][j] = "X";
      cout<<M[i][j]<<"\t";
    }
    cout<<endl;
  }
}

int state;
char C;
string token;
char* forward;

void get_char()
{
	//if (ch_counter > total) return;
	C = *forward;
	forward++;
}


void get_nbc()
{
	while (C == ' ')
	{
		get_char();
	}//FIXME EOF
}

void cat()
{
	token = token + C;
}

bool digit()
{
	if ((C >= '0' && C <= '9'))
		return true;
	else
		return false;
}

void retract()
{
	forward-=2;
	get_char();//ֱ�Ӹı�C
}

string lexicalAnalysis(string buffer)//for num and +,-,*,/,(,)
{
	string retstr;
	const char* cbuffer=buffer.c_str();
  	forward = strdup(cbuffer);
  	state = 0;
	do{
		//cout<<ct++<<endl;
		switch( state ){
			case 0:
				token = "";
				get_char();
				get_nbc();
				switch(C){
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':state = 2;break;
					case '+':
					case '-':
					case '*':
					case '/':
					case '(':
					case ')':retstr = retstr+C;break;
					}
				break;
			case 2:
				cat();
				get_char();
				switch(C){
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':state = 2;break;
					case '.':state = 3;break;
					case 'E':
					case 'e':state = 5;break;
					default:
						retract();
						state = 0;
						retstr = retstr +'N';
						break;
				}
				break;
						case 3:
				cat();
				get_char();
				if(digit())
					state = 4;
				else
				{
					error();
					state = 0;
				}
				break;
			case 4:
				cat();
				get_char();
				switch(C){
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':state = 4;break;
					case 'E':
					case 'e':state = 5;break;
					default:
						retract();
						state = 0;
						retstr = retstr +'N';
						break;
				}
				break;
			case 5:
				cat();
				get_char();
				switch(C){
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':state = 7;break;
					case '+':
					case '-':state = 6;break;
					default:
						retract();
						error();
						state = 0;
						break;
				}
				break;
			case 6:
				cat();
				get_char();
				if(digit())
					state = 7;
				else
				{
					retract();
					error();
					state = 0;
					break;
				}
				break;
			case 7:
				cat();
				get_char();
				if(digit())
					state = 7;
				else
				{
					retract();
					state = 0;
					retstr = retstr +'N';
				}
				break;
			}
		}while(C!='$');
		retstr = retstr +'$';
		cout<<retstr<<endl;
		return retstr;
}
void LL1()
{
  stack<char> Analysis;
  Analysis.push('$');
  Analysis.push('E');
  string buffer;
  cout<<"Input your sentence:"<<endl;
  cin>>buffer;
  buffer = buffer+'$';
  cout<<buffer<<endl;
  string retstr = lexicalAnalysis(buffer);
  const char* cbuffer=retstr.c_str();
  char* bufp = strdup(cbuffer);
  char X = Analysis.top();
  do{
    int flag = 0;
    for(int i = 0;i<8;i++)
    {
      if(X == Terminal[i])
      {
        flag = 1;
        if(X == *bufp)
        {
    	  cout<<X<<endl;
          Analysis.pop();
          bufp++;
        }
        else
          error();
      }
    }
    if(flag == 0)
    {
        string str;
        if(M[order(X)][order(*bufp)] != "X")
        {
          if (M[order(X)][order(*bufp)] == "@")
          {
          	cout<<X<<"->"<<"@"<<endl;
          	Analysis.pop();
		  }
		  else
		  {
		  	str = M[order(X)][order(*bufp)];
	        int len = str.length();
	        const char* cstr=str.c_str();
	        char* rear = (char*)cstr+len-1;
	        Analysis.pop();
	        while(rear+1!=cstr)
	        {

	            Analysis.push(*rear);
	            rear--;
	        }
	        cout<<X<<"->"<<str<<endl;
			}

        }
        else
          error();
    }
    X = Analysis.top();
  }while(X!='$');
}
int main()
{
  E.First.insert('(');
  E.First.insert('N');
  R.First.insert('+');
  R.First.insert('-');
  R.First.insert('@');
  T.First.insert('(');
  T.First.insert('N');
  Y.First.insert('*');
  Y.First.insert('/');
  Y.First.insert('@');
  F.First.insert('(');
  F.First.insert('N');

  E.Follow.insert('$');
  E.Follow.insert(')');
  R.Follow.insert('$');
  R.Follow.insert(')');
  T.Follow.insert('+');
  T.Follow.insert('-');
  T.Follow.insert('$');
  T.Follow.insert(')');
  Y.Follow.insert('+');
  Y.Follow.insert('-');
  Y.Follow.insert('$');
  Y.Follow.insert(')');
  F.Follow.insert('*');
  F.Follow.insert('/');
  F.Follow.insert('+');
  F.Follow.insert('-');
  F.Follow.insert('$');
  F.Follow.insert(')');

  initM();
  LL1();


}
