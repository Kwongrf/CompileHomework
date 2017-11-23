#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string>
using namespace std;
#define ID 0
#define NUM 1
#define RELOP 2 //��ϵ�����
#define LOGOP 3 //�߼������
#define SHFOP 4 //��λ�����
#define ASSOP 5 //��ֵ�����
#define SLFOP 6 //�����Լ������
#define MEMOP 7 //��Ա�����

//��ϵ����
#define LT 0
#define LE 1
#define EQ 2
#define NE 3
#define GT 4
#define GE 5

//�߼�����
#define AND 6
#define OR  7
#define NOT 8

//��λ����
#define LS 9 // <<
#define RS 10 // >>

//��ֵ����
#define AS 11    // =
#define ADDAS 12 // +=
#define SUBAS 13 // -=
#define MULAS 14 // *=
#define DIVAS 15 // /=
#define MODAS 16 // %=
#define ANDAS 17 // &=
#define EORAS 18 // ^=
#define ORAS 19  // |=
#define LSAS 20  // <<=
#define RSAS 21 // >>=

//�����Լ�
#define ADDONE 22 //++
#define SUBONE 23 //--

//��Ա
#define PTR 24//->
#define NOM 25//.

//ȫ�ֱ���
int state;
char C;
int iskey;
string token;
char* forward;
char buffer[2048];


int int_counter;
int flt_counter;


int total;//�������ַ�
int ch_counter;//���ַ���

int ordop_counter; //δ���������
int relop_counter;
int logop_counter;
int shfop_counter;
int assop_counter;
int slfop_counter;
int memop_counter;

int key_counter;//�ؼ����ַ���
int word_counter;//�Զ����ַ���

int line_counter;//�������

//���ű�
struct Node{
	string lexeme;
	int count;
	Node* next=NULL;

	void print_list()
	{
		Node* current = this->next;
		int i = 0;
		while(current!=NULL)
		{
			cout <<current->lexeme<<":"<<current->count<<"\t";
			i++;
			if(i % 5 == 0) cout<<endl;
			current = current->next;
		}
		cout<<endl;
	}

	//���ʶ�����߸��������
	Node* table_insert()
	{
		Node* current = this;
		while(current->next != NULL)
		{
			current = current->next;
			if(current->lexeme == token)
			{

	        current->count = current->count+1;
	        //current->count++;
	        return current;
	      }
		}
		Node* newNode = new Node();
		newNode->lexeme = token;
		newNode->count = 1;
		newNode->next = NULL;
		current->next = newNode;
		return newNode;
	}

};
typedef Node* LinkList;
struct charNode{
	char ch;
	int count;
	charNode* next = NULL;
	void print_list()
	{
		charNode* current = this->next;
		int i = 0;
		while(current!=NULL)
		{
			if(current->ch != '\n'&&current->ch != '\r'&&current->ch != '\t')
			{
				cout <<current->ch<<":"<<current->count<<"\t";
				i++;
				if(i % 5 == 0) cout<<endl;
				current = current->next;
			}
			else if(current->ch == '\n')
			{
				cout<<"���з�"<<":"<<current->count<<"\t";
				i++;
				if(i % 5 == 0) cout<<endl;
				current = current->next;
			}
			else if(current->ch == '\r')
			{
				cout<<"��ո�"<<":"<<current->count<<"\t";
				i++;
				if(i % 5 == 0) cout<<endl;
				current = current->next;
			}
			else if(current->ch == '\t')
			{
				cout<<"�Ʊ��"<<":"<<current->count<<"\t";
				i++;
				if(i % 5 == 0) cout<<endl;
				current = current->next;
			}
		}
		cout<<endl;
	}

	//����ͨ�����
	void table_insert(char c)
	{
		charNode* current = this;
		while(current->next != NULL)
		{
			current = current->next;
			if(current->ch == c)
			{
	        	current->count++;
	        	return;
	        	//return current;
	        }
		}
		charNode* newNode = new charNode();
		newNode->ch = c;
		newNode->count = 1;
		newNode->next = NULL;
		current->next = newNode;
		//return newNode;
	}
};
typedef charNode* cLinkList;
struct intNode{
	int num;
	int count;
	int type = 1;//type=0���֣�type=1������
	intNode* next=NULL;
	void print_list()
	{
		if(type == 0)
		{
			intNode* current = this->next;
			int i = 0 ;
			while(current!=NULL)
			{
				cout <<current->num<<":"<<current->count<<"\t";
				i++;
				if(i % 5 == 0) cout<<endl;
				current = current->next;
			}
			cout<<endl;
		}
		else
		{
			intNode* current = this->next;
			int i = 0 ;
			while(current!=NULL)
			{
				switch (current->num) {
					case LT:
						cout <<"LT:"<<current->count<<"\t";
						break;
					case LE:
						cout <<"LT:"<<current->count<<"\t";
						break;
					case EQ:
						cout <<"EQ:"<<current->count<<"\t";
						break;
					case NE:
						cout <<"NE:"<<current->count<<"\t";
						break;
					case GT:
						cout <<"GT:"<<current->count<<"\t";
						break;
					case GE:
						cout <<"GE:"<<current->count<<"\t";
						break;
					case AND:
						cout <<"AND:"<<current->count<<"\t";
						break;
					case OR:
						cout <<"OR:"<<current->count<<"\t";
						break;
					case NOT:
						cout <<"NOT:"<<current->count<<"\t";
						break;
					case LS:
						cout <<"LS:"<<current->count<<"\t";
						break;
					case RS:
						cout <<"RS:"<<current->count<<"\t";
						break;
					case AS:
						cout <<"AS:"<<current->count<<"\t";
						break;
					case ADDAS:
						cout <<"ADDAS:"<<current->count<<"\t";
						break;
					case SUBAS:
						cout <<"SUBAS:"<<current->count<<"\t";
						break;
					case MULAS:
						cout <<"MULAS:"<<current->count<<"\t";
						break;
					case DIVAS:
						cout <<"DIVAS:"<<current->count<<"\t";
						break;
					case MODAS:
						cout <<"MODAS:"<<current->count<<"\t";
						break;
					case ANDAS:
						cout <<"ANDAS:"<<current->count<<"\t";
						break;
					case EORAS:
						cout <<"EORAS:"<<current->count<<"\t";
						break;
					case ORAS:
						cout <<"ORAS:"<<current->count<<"\t";
						break;
					case LSAS:
						cout <<"LSAS:"<<current->count<<"\t";
						break;
					case RSAS:
						cout <<"RSAS:"<<current->count<<"\t";
						break;
					case ADDONE:
						cout <<"ADDONE:"<<current->count<<"\t";
						break;
					case SUBONE:
						cout <<"SUBONE:"<<current->count<<"\t";
						break;
					case PTR:
						cout <<"PTR:"<<current->count<<"\t";
						break;
					case NOM:
						cout <<"NOM:"<<current->count<<"\t";
						break;

				}
				i++;
				if(i % 5 == 0) cout<<endl;
				current = current->next;
			}
			cout<<endl;
		}

	}

	//������
	void table_insert(int num)
	{
		intNode* current = this;
		while(current->next != NULL)
		{
			current = current->next;
			if(current->num == num)
			{

	        current->count++;
	        return;
	        //return current;
	      }
		}
		intNode* newNode = new intNode();
		newNode->num = num;
		newNode->count = 1;
		newNode->next = NULL;
		current->next = newNode;
		//return newNode;
	}
};
typedef intNode* iLinkList;
struct floatNode{
	float num;
	int count;
	floatNode* next=NULL;
	void print_list()
	{
		floatNode* current = this->next;
		int i = 0;
		while(current!=NULL)
		{
			cout <<current->num<<":"<<current->count<<"\t";
			i++;
			if(i % 5 == 0) cout<<endl;
			current = current->next;
		}
		cout<<endl;
	}

	//�渡����
	void table_insert(float num)
	{
		floatNode* current = this;
		while(current->next != NULL)
		{
			current = current->next;
			if(current->num == num)
			{

	        current->count++;
	        return;
	        //return current;
	      }
		}
		floatNode* newNode = new floatNode();
		newNode->num = num;
		newNode->count = 1;
		newNode->next = NULL;
		current->next = newNode;
		//return newNode;
	}
};
typedef floatNode* fLinkList;
//�ؼ��ʱ�
string key_words[]={"auto","break","case","char","const","continue","default","do",
					"double","else","enum","extern","float","for","goto","if","int",
					"long","register","return","short","signed","sizeof","static",
					"struct","switch","typedef","union","unsigned","void","volatile","while"};


LinkList table_hdptr= new Node();//���ű� ����ͷָ��

LinkList key_hp = new Node;
cLinkList ordop_hp= new charNode;
iLinkList relop_hp = new intNode;
iLinkList logop_hp = new intNode;
iLinkList shfop_hp = new intNode;
iLinkList assop_hp = new intNode;
iLinkList slfop_hp = new intNode;
iLinkList memop_hp = new intNode;

iLinkList int_hp = new intNode;
fLinkList flt_hp = new floatNode;

void get_char()
{
	ch_counter++;
	//if (ch_counter > total) return;
	C = *forward;
	forward++;

	if (C == '\n')
	{
		line_counter++;
	}
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

bool letter()
{
	if ((C >= 'A' && C <= 'Z')||(C >= 'a' && C <= 'z'))
		return true;
	else
		return false;
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
	if(*(forward-1) == '\n')
		line_counter--;
	else if (*(forward-2)=='\n')
		line_counter--;
	forward-=2;
	ch_counter-=2;
	get_char();//ֱ�Ӹı�C
	
}

int reserve()
{
	int i;
	//cout<<token<<endl;
	for(i=0;i<32;i++)//�ؼ�����Ϊ32
	{
		if (token == key_words[i])
		{
			return i;
		}
	}
	if (i == 32)
		return -1;
}

int SToI(string s)
{
	return atoi(s.c_str());
}

float SToF(string s)
{
	return atof(s.c_str());//FIXME �޷������Ȼ��ɸ�������
}




void error()
{
	
	char* tmp = forward-1;
	int col = 1;
	if(line_counter!=0) 
	{
		while(*tmp!='\n')
		{
			tmp--;
			col++;
		}
	}
	else
	{
		while(tmp != buffer)
		{
			tmp--;
			col++;
		}
	}
	
	cout<<"�����ڵ�"<<line_counter+1<<"��"<<"����"<<col-1<<"��"<<endl;
	while(C!=' '&&C!=EOF &&C!='\n')
	{
		get_char();
	}
}


void return_back(char ch)//�������
{
  ordop_counter++;
  ordop_hp->table_insert(ch);

}
void return_back(int key)//�ǹؼ���
{
	key_counter++;
  Node* pos = key_hp->table_insert();
}
void return_back(int kind ,int attr)//������ �� ��ϵ����� �����������
{
  switch (kind) {
    case NUM:
      int_hp->table_insert(attr);
      int_counter++;
      break;
    case RELOP:
      relop_hp->table_insert(attr);
      relop_counter++;
      break;
    case LOGOP:
      logop_hp->table_insert(attr);
      logop_counter++;
      break;
    case SHFOP:
      shfop_hp->table_insert(attr);
      shfop_counter++;
      break;
    case ASSOP:
      assop_hp->table_insert(attr);
      assop_counter++;
      break;
    case SLFOP:
      slfop_hp->table_insert(attr);
      slfop_counter++;
      break;
    case MEMOP:
      memop_hp->table_insert(attr);
      memop_counter++;
      break;
    default:
      cout<<"error"<<endl;
      break;
  }
}
void return_back(int kind , float num)//�Ǹ�����
{
  flt_hp->table_insert(num);
	//cout<<num<<endl;
  flt_counter++;
}
void return_back(int id , Node* p )//���Զ����ʶ��
{
	//cout<<p->lexeme<<endl;
  word_counter++;
}

void compile()
{
	//int ct=0;
	do{
		//cout<<ct++<<endl;
		switch( state ){
			case 0:
				token = "";
				get_char();
				get_nbc();
				switch(C){
					case '_':
					case 'a':
					case 'b':
					case 'c':
					case 'd':
					case 'e':
					case 'f':
					case 'g':
					case 'h':
					case 'i':
					case 'j':
					case 'k':
					case 'l':
					case 'm':
					case 'n':
					case 'o':
					case 'p':
					case 'q':
					case 'r':
					case 's':
					case 't':
					case 'u':
					case 'v':
					case 'w':
					case 'x':
					case 'y':
					case 'z':
					case 'A':
					case 'B':
					case 'C':
					case 'D':
					case 'E':
					case 'F':
					case 'G':
					case 'H':
					case 'I':
					case 'J':
					case 'K':
					case 'L':
					case 'M':
					case 'N':
					case 'O':
					case 'P':
					case 'Q':
					case 'R':
					case 'S':
					case 'T':
					case 'U':
					case 'V':
					case 'W':
					case 'X':
					case 'Y':
					case 'Z':state = 1;break;
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':state = 2;break;//FIXME ״̬У��
					case '<':state = 8;break;
					case '>':state = 10;break;
					case '=':state = 12;break;
					case '+':state = 13;break;
					case '-':state = 14;break;
					case '*':state = 15;break;
					case '/':state = 16;break;
					case '!':state = 20;break;
					case '&':state = 21;break;
					case '|':state = 22;break;
					case '%':state = 23;break;
					case '^':state = 24;break;
					case '?':state = 25;break;
					case '.':state = 26;break;
					case '\"':state = 27;break;
					case '\'':state = 28;break;
					case '(':
					case ')':
					case '{':
					case '}':
					case '[':
					case ']':
					case ';':
					case ':':
					case ',':
					case '~':
					case '#':
					case '\n':
					case '\t':
					case '\r':state = 0;return_back(C);break;

					default :state =29;break;

				}
				break;
			case 1:
				cat();
				get_char();
				if( letter() || digit() || C == '_')
					state = 1;
				else
				{

					retract();
					state = 0;
					iskey = reserve();
					if(iskey != -1)
						return_back(iskey);
					else
					{
						Node* identry = table_hdptr->table_insert();
						return_back(ID,identry);
					}
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
						return_back(NUM,SToI(token));
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
						return_back(NUM,SToF(token));
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
					return_back(NUM,SToF(token));
				}
				break;
			case 8: // '<'
				cat();
				get_char();
				switch(C){
					case '=':
						cat();
						state = 0;
						return_back(RELOP,LE);
						break;//����<=�ļǺź���Ϥ
					case '<':
						state = 9;
						break;
					default:
						retract();
						state = 0;
						return_back(RELOP,LT);
						break;
				}
				break;
			case 9: //"<<"
				cat();
				get_char();
				switch(C){
					case '=':
						cat();
						state = 0;
						return_back(ASSOP,LSAS);
						break;
					default:
						retract();
						state = 0;
						return_back(SHFOP,LS);
						break;
				}
				break;
			case 10: // '>'
				cat();
				get_char();
				switch(C){
					case '=':
						cat();
						state = 0;
						return_back(RELOP,GE);
						break;//����>=�ļǺź���Ϥ
					case '>':
						state = 11;
						break;
					default:
						retract();
						state = 0;
						return_back(RELOP,GT);
						break;
				}
				break;
			case 11: // ">>"
				cat();
				get_char();
				switch(C){
					case '=':
						cat();
						state = 0;
						return_back(ASSOP,RSAS);
						break;
					default:
						retract();
						state = 0;
						return_back(SHFOP,RS);
						break;
				}
				break;
			case 12: // '='
				cat();
				get_char();
				if( C == '=')
				{
					cat();
					state = 0;
					return_back(RELOP,EQ);
				}
				else
				{
					retract();
					state = 0;
					return_back(ASSOP,AS);
				}
				break;
			case 13: // '+'
				cat();
				get_char();
				switch(C){
					case '=':
						cat();
						state = 0;
						return_back(ASSOP,ADDAS);
						break;
					case '+':
						cat();
						state = 0;
						return_back(SLFOP,ADDONE);
						break;
					default:
						retract();
						state = 0;
						return_back(C);
						break;
				}
			case 14: //'-'
				cat();
				get_char();
				switch(C){
					case '=':
						cat();
						state = 0;
						return_back(ASSOP,SUBAS);
						break;
					case '-':
						cat();
						state = 0;
						return_back(SLFOP,SUBONE);
						break;
					case '>':
						cat();
						state = 0;
						return_back(MEMOP,PTR);
						break;
					default:
						retract();
						state = 0;
						return_back(C);
						break;
				}
				break;
			case 15: // '*'
				cat();
				get_char();
				if(C == '=')
				{
					cat();
					state = 0;
					return_back(ASSOP,MULAS);
				}
				else
				{
					retract();
					state = 0;
					return_back(C);
				}
				break;
			case 16: // '/'
				cat();
				get_char();
				switch(C){
					case '=':
						cat();
						state = 0;
						return_back(ASSOP,DIVAS);
						break;
					case '*':
						state = 17;
						break;
					case '/':
						state = 19;
						break;
					default:
						retract();
						state = 0;
						return_back(C);
						break;

				}
				break;
			case 17:
				get_char();
				while (C != '*')
					get_char(); //ֱ������ '*'
				state = 18;
				break;
			case 18:
				get_char();
				if (C == '/')
					state = 0;
				else
					state = 17;
				break;
			case 19:
				get_char();
				while(C != '\n')
					get_char(); //ֱ������
				//get_char();
				state = 0;
				break;
			case 20: // '!'
				cat();
				get_char();
				if( C == '=')
				{
					cat();
					state = 0;
					return_back(RELOP,NE);
				}
				else
				{
					retract();
					state = 0;
					return_back(LOGOP,NOT);
				}
				break;
			case 21: //'&'
				cat();
				get_char();
				switch(C){
					case '&':
						cat();
						state = 0;
						return_back(LOGOP,AND);
						break;
					case '=':
						cat();
						state = 0;
						return_back(ASSOP,ANDAS);
						break;
					default:
						retract();
						state = 0;
						return_back(C);
						break;
				}
				break;
			case 22:
				cat();
				get_char();
				switch(C){
					case '|':
						cat();
						state = 0;
						return_back(LOGOP,OR);
						break;
					case '=':
						cat();
						state = 0;
						return_back(ASSOP,ORAS);
						break;
					default:
						retract();
						state = 0;
						return_back(C);
						break;
				}
				break;
			case 23: // '%'
				cat();
				get_char();
				if (C == '=')
				{
					cat();
					state = 0;
					return_back(ASSOP,MODAS);
				}
				else
				{
					retract();
					state = 0;
					return_back(C);
				}
				break;
			case 24: // '^'
				cat();
				get_char();
				if (C == '=')
				{
					cat();
					state = 0;
					return_back(ASSOP,EORAS);
				}
				else
				{
					retract();
					state = 0;
					return_back(C);
				}
				break;
			case 25: // '?'
				return_back(C);
				state = 0;
				break;
			case 26:// '.'
				return_back(MEMOP,NOM);
				state = 0;
				break;
			case 27://'\"'
				get_char();
				while(C != '\"')
				{	
					cat();
					get_char(); //ֱ������
				}
				state = 0;
				break;
			case 28://'\''
				get_char();
				while(C != '\'')
				{	
					cat();
					get_char(); //ֱ������
				}
				state = 0;
				break;
			case 29: //����  
				error();
				state = 0;
				break;
		}

	}while(C!=EOF);
}




void printout()
{
	cout<<"���ַ�����"<<ch_counter+line_counter-1<<endl;
	cout<<"��������"<<line_counter<<endl;
	cout<<"�ؼ��ֳ�������"<<key_counter<<endl;
	cout<<"���ֵĹؼ��ּ�������"<<endl;
	key_hp->print_list();

	cout<<"�Զ����ʶ������������"<<word_counter<<endl;
	cout<<"���ֵı�ʶ����������"<<endl;
	table_hdptr->print_list();

	cout<<"���ֵ���ͨ�������������"<<endl;
	ordop_hp->print_list();
	cout<<"���ֵĹ�ϵ�������������"<<endl;
	relop_hp->print_list();
	cout<<"���ֵ��߼��������������"<<endl;
	logop_hp->print_list();
	cout<<"���ֵ���λ�������������"<<endl;
	shfop_hp->print_list();
	cout<<"���ֵĸ�ֵ�������������"<<endl;
	assop_hp->print_list();
	cout<<"���ֵ������Լ��������������"<<endl;
	slfop_hp->print_list();
	cout<<"���ֵĳ�Ա�������������"<<endl;
	memop_hp->print_list();
	cout<<"���ֵ��������ּ�������"<<endl;
	int_hp->print_list();
	cout<<"���ֵĸ������ּ�������"<<endl;
	flt_hp->print_list();
}

int main()
{
	forward = buffer;

	state = 0;

	int_counter = 0;
	flt_counter = 0;
	ch_counter = 0;//���ַ���
	ordop_counter = 0; //δ���������
	relop_counter = 0;
	logop_counter = 0;
	shfop_counter = 0;
	assop_counter = 0;
	slfop_counter = 0;
	memop_counter = 0;

	key_counter = 0;//�ؼ����ַ���
	word_counter = 0;//�Զ����ַ���
	line_counter = 0;//�������

	int_hp->type = 0;
	string dir = "test.c";
	cout<<"������Ҫ��ȡ���ļ���ַ:(����-1��Ĭ�϶�ȡ��ǰĿ¼�µ�test.c)"<<endl;
	string tmp;
	cin>>tmp;
	if(tmp != "-1")
		dir = tmp;
	//���ļ���ȡ
	ifstream in(dir.data());
    if (! in.is_open())
    { cout << "Error opening file"; exit (1); }
    total=0;
    in>>noskipws;
	while (!in.eof())        
	{
		if( (in>>buffer[total])=='\0'){
            break;
        }
		++total;
		//cout<<"���ڶ����"<<total<<"���ַ�"<<endl;
	}
	buffer[total] = EOF;
	//cout<<total<<endl;
	in.close();               //��ȡ��Ϻ�,�ر��ļ�
	compile();
	printout();
	system("pause") ;
    return 0;

}
