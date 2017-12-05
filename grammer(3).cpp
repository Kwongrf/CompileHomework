#include<iostream>
#include<vector>
#include<string.h>
#include<stack>
#include<algorithm>
using namespace std;
#define NUM 20
#define SymNum 10
#define NAN 999
typedef struct Item{
  char left;
  string right;
  bool operator ==(const Item & I)const;
}Item;
char Symbol[] = {'E','T','F','+','-','*','/','(',')','N'};
Item arr[]={Item{'R',".E"},Item{'E',".E+T"},Item{'E',".E-T"},
            Item{'E',".T"},Item{'T',".T*F"},Item{'T',".T/F"},
            Item{'T',".F"},Item{'F',".(E)"},Item{'F',".N"}};
std::vector<Item> items;
bool Item::operator ==(const Item& I)const{
  if(this->left == I.left && this->right == I.right)
    return true;
  else
    return false;
}
int relation[NUM][SymNum];
int action[NUM][SymNum-2];
int _goto[NUM][3];//3涓潪缁堢粨绗?

int order(char c)
{
  switch(c){
    case '+':
    case 'E':
      return 0;
    case '-':
    case 'T':
      return 1;
    case '*':
    case 'F':
      return 2;
    case '/':
      return 3;
    case '(':
      return 4;
    case ')':
      return 5;
    case 'N':
      return 6;
    case '$':
      return 7;
    default:
      return -1;
  }
}
std::vector<int>FOLLOW(char X)
{
  std::vector<int> v;
  switch (X) {
  	case 'R':
  		v.push_back(order('$'));
  		break;
    case 'E':
      v.push_back(order('+'));//'+'
      v.push_back(order('-'));//'-'
      v.push_back(order(')'));//')'
      v.push_back(order('$'));//'$'
      break;
    case 'T':
      v.push_back(order('+'));//'+'
      v.push_back(order('-'));//'-'
      v.push_back(order('*'));//'*'
      v.push_back(order('/'));//'/'
      v.push_back(order(')'));//')'
      v.push_back(order('$'));//'$'
      break;
    case 'F':
      v.push_back(order('+'));//'+'
      v.push_back(order('-'));//'-'
      v.push_back(order('*'));//'*'
      v.push_back(order('/'));//'/'
      v.push_back(order(')'));//')'
      v.push_back(order('$'));//'$'
      break;
    default:
      break;
  }
  return v;
}
void printItems(std::vector<Item> v)
{
  //cout<<"printItems"<<endl;
  int count=0;
  for(std::vector<Item>::iterator it = v.begin();it!=v.end();it++)
  {
    cout<<(*it).left<<"->"<<(*it).right<<'\t';
    count++;
    if(count%4==0)
    	cout<<endl;
  }
  cout<<endl;
  //cout<<"printItems end"<<endl;
}
void printCollection(std::vector<std::vector<Item> > C)
{
  //cout<<"printCollection"<<endl;
  int count = 0;
  for(std::vector<std::vector<Item> >::iterator it = C.begin();it!=C.end();it++)
  {

    cout<<"\nI"<<count<<endl;

    printItems(*it);
    for(int j = 0;j < SymNum ;j++)
    {
    	if(relation[count][j]!=-1)
        cout<<"I"<<count<<"--"<<Symbol[j]<<"-->I"<<relation[count][j]<<endl;
	  }
    count++;
  }
  //cout<<"printCollection end"<<endl;
}

vector<Item> closure(std::vector<Item> I)
{
  //cout<<"start closure"<<endl;
  //printItems(I);
  std::vector<Item> J = I;
  std::vector<Item> J_new;
  do{
    J_new = J;
    for (std::vector<Item>::iterator it = J_new.begin(); it != J_new.end(); it++)
    {
      std::string right = (*it).right;
      int pos = right.find('.');
      if(pos != right.npos && pos != right.length()-1)
      {
        char B = right[pos+1];
        for(std::vector<Item>::iterator it2= items.begin();it2!=items.end();it2++)
        {
          if((*it2).left == B)
          {
            if(find(J_new.begin(),J_new.end(),*it2) == J_new.end())
              J.push_back(*it2);
          }
        }
      }
    }
  }while(J_new != J);
  //cout<<"end closure"<<endl;
  return J_new;

}

std::vector<Item> go(std::vector<Item> I, char X)
{
  //cout<<"start go"<<endl;
  //printItems(I);
  std::vector<Item> J;
  bool flag = false;
  for(std::vector<Item>::iterator it = I.begin();it!=I.end();it++)
  {
    string right = (*it).right;
    int pos1 = right.find('.');
    int pos2 = right.find(X);
    if(pos1+1 == pos2 && pos2 != right.npos)
    {
      flag = true;
      char left = (*it).left;
      string new_right = right.substr(0,pos1) + X;
      new_right = new_right +'.';
      new_right = new_right + right.substr(pos2+1);
      //cout<<"good:"<<new_right<<endl;
      Item item{left,new_right};
      J.push_back(item);
    }
  }
  //cout<<"end go"<<endl;
  //printItems(J);
  if(flag)
    return closure(J);
  else
    return J;
}
std::vector<std::vector<Item> > CannonicalCollection()
{
  cout <<"start CannonicalCollection" <<endl;
  std::vector<std::vector<Item> >C;
  std::vector<std::vector<Item> >C_new;
  std::vector<Item> v;
  v.push_back(items[0]);
  C.push_back(closure(v));
  do {
    C_new = C;
    for(std::vector<vector<Item> >::iterator it = C_new.begin(); it!=C_new.end();it++ )
    {
    	//printItems(*it);

      for(int i = 0;i < SymNum ;i++)
      {
        std::vector<Item> v = go(*it, Symbol[i]);

        if(v.size() > 0)
        {
          //printItems(v);
          std::vector<std::vector<Item> >::iterator pos = find(C.begin(),C.end(), v);
          if(pos == C.end())
          {
            C.push_back(v);
            relation[distance(C_new.begin(),it)][i] = C.size()-1;
          }
          else
          {
            relation[distance(C_new.begin(),it)][i] = distance(C.begin(),pos);
            //cout<<"?"<<relation[distance(C_new.begin(),it)][i]<<endl;
          }
        }
      }

    }
  } while(C.size()!=C_new.size());
  cout<<"end CannonicalCollection"<<endl;
  return C;
}

void analysisTable(std::vector<std::vector<Item> > C)
{
  int size = C.size();
  for(int i = 0;i< size;i++)
  {
    for(int j=0;j<SymNum;j++)
    {
      if(relation[i][j]!=-1)
      {
        if(j<3)
        {
          _goto[i][j] = relation[i][j];
        }
        else
        {
          action[i][j-3] = relation[i][j];
        }
      }
    }
  }
  for(std::vector<std::vector<Item> >::iterator it = C.begin();it!=C.end();it++)
  {
    for(std::vector<Item>::iterator it2 = (*it).begin();it2!=(*it).end();it2++)
    {
      string right = (*it2).right;
      char left = (*it2).left;
      if(right.find('.') == right.length()-1)
      {
        string new_right = '.'+right.substr(0,right.length()-1);
        Item item{left,new_right};
        //cout<<"new_right:"<<new_right<<endl;
        std::vector<Item>::iterator pos = find(items.begin(),items.end(),item);
        std::vector<int> v = FOLLOW(left);
        for(std::vector<int>::iterator it3 = v.begin();it3!=v.end();it3++)
        {
          action[distance(C.begin(),it)][*it3] = -1*distance(items.begin(),pos);
        }
      }
    }
  }
  cout<<'+'<<'\t'<<'-'<<'\t'<<'*'<<'\t'<<'/'<<'\t'<<'('<<'\t'<<')'<<'\t'<<'N'<<'\t'<<'$'<<'\t'<<'E'<<"  "<<'T'<<"  "<<'F'<<endl;
  for(int i = 0 ;i < size;i++)
  {
    //cout<<i<<'\t';
    for(int j = 0;j < SymNum-2; j++)
    {
      if(size>action[i][j])
        cout<<action[i][j]<<'\t';
      else
        cout<<"-"<<'\t';
    }
    for(int j = 0;j < 3; j++)
    {
      if (_goto[i][j]<size) {
          cout<<_goto[i][j]<<"  ";
      }
      else
        cout<<"-"<<"  ";

    }

    cout<<endl;
  }
}

//浠ヤ笅涓鸿瘝娉曞垎鏋愮▼搴?
//鍏ㄥ眬鍙橀噺
int state;
char C;
string token;
char* forward;
void error()
{
  cout<<"ERROR"<<endl;
  exit(-1);
}
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
	get_char();//直锟接改憋拷C
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


void SLR1()
{
  stack<int>State;
  stack<char>Symbols;
  State.push(0);
  string buffer;
  cout<<"Input your sentence:"<<endl;
  cin>>buffer;
  buffer = buffer+'$';
  cout<<buffer<<endl;
  string retstr = lexicalAnalysis(buffer);
  const char* cbuffer=retstr.c_str();
  char* bufp = strdup(cbuffer);
  do{
    int S = State.top();

    if(action[S][order(*bufp)] != NAN)
    {
      if(action[S][order(*bufp)] > 0)//shift
      {
        State.push(action[S][order(*bufp)]);
        Symbols.push(*bufp);
        bufp++;
      }
      else if(action[S][order(*bufp)] < 0)//reduce
      {
        int i = -1*action[S][order(*bufp)];
        Item item = items[i];
        string right = item.right;
        for(int j = 0;j<right.length()-1;j++)
        {
          State.pop();
          Symbols.pop();
        }
        char left = item.left;
        Symbols.push(left);
        int S_ = State.top();
        State.push(_goto[S_][order(left)]);
        //cout<<right.substr(1)<<endl;
        cout<<left<<"->"<<right.substr(1)<<endl;
      }
      else if(action[S][order(*bufp)] == 0)//ACC
      {
        cout<<"Success!";
        return;
      }
      else
        error();
    }
  }while(1);
}
int main() {
  for(int i =0;i < 9;i++)
    items.push_back(arr[i]);
  for(int i = 0; i < NUM ;i++)
    for(int j = 0;j < SymNum;j++)
    {
      relation[i][j] = -1;
      if(j<SymNum-2)
        action[i][j]=NAN;
      if(j<3)
        _goto[i][j]=NAN;
    }
  std::vector<std::vector<Item> > C = CannonicalCollection();
  /*
  for(int i = 0; i < NUM ;i++)
  {
    for(int j = 0;j < SymNum;j++)
        cout<<relation[i][j]<<' ';
    cout<<endl;
  }*/
  printCollection(C);
  analysisTable(C);
  SLR1();
  return 0;
}
