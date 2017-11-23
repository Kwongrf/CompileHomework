#include<iostrea>
#include<vector>
#include<string>
using namespace std;
#define NUM 20
#define SymNum 10
typedef struct Item{
  char left;
  string right;
  bool operator ==(const Item & I)const;
}Item;
char Symbol[] = {'E','T','F','+','-','*','/','(',')','N'};
Item arr[]={Item{'R',"E"},Item{'E',".E+T"},Item{'E',".E-T"},
            Item{'E',".T"},Item{'T',".T*F"},Item{'T',".T/F"},
            Item{'T',".F"},Item{'F',".(E)"},Item{'F',".N"}}
std::vector<Item> items;
Item::bool operator ==(const Item& I)const{
  if(this->left == I.left && this->right == I.right)
    return true;
  else
    return false;
}


vector<Item> closure(std::vector<Item> I)
{
  std::vector<Item> J = I;
  do{
    std::vector<Item> J_new = J;
    for (std::vector<Item>::iterator it = J_new.begin(); it != J_new.end(); it++) {
      std::string right = (*it).right;
      int pos = right.find('.')
      if(pos != right.npos && pos != right.length()-1)
      {
        char B = right[pos+1];
        for(std::vector<Item>::iterator it2= items.begin();it2!=items.end();it2++)
        {
          if((*it2).left == B)
          {
            if(find(J_new.begin(),J_new.end(),*it2) == J_new.end())
              J_new.push_back(*it);
          }
        }
      }
    }
  }while(J_new != J);
}

std::vector<Item> go(std::vector<Item> I, char X)
{
  std::vector<Item> J;
  for(std::vector<Item>::iterator it = I.begin();it!=I.end();it++)
  {
    string right = (*it).right;
    int pos1 = right.find('.');
    int pos2 = right.find('X');
    if(pos1+1 == pos2)
    {
      char left = (*it).left;
      string new_right = right.substr(0,pos1-1) + X;
      new_right = new_right +'.';
      new_right = new_right + right.substr(pos2+1)
      Item item{left,new_right};
      J.push_back(item);
    }
  }
  return J;
}
