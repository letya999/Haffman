#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <cstring>

using namespace std;

void encode(void);
void decode(void); 

class Node {
	public:
     int a;
     unsigned char c;
     Node *left, *right; 
     Node() {
     	left=right=NULL;
	 }
     Node(Node *L, Node *R) {
      left =  L;
      right = R;
      a = L->a + R->a; 
	 }
};

struct MyCompare {
    bool operator()(const Node* l, const Node* r) 
	const {return l->a < r->a;}
};

vector<bool> code; 
map<unsigned char,vector<bool> > table;
char name[50], name1[50],name2[50]; 
Node* root;
int count=0;
char buf=0;

void BuildTable(Node *root)
{   
  if(root->left!=NULL) {  
    code.push_back(0);
    BuildTable(root->left);
  }
     
  if(root->right!=NULL) {  
    code.push_back(1);
    BuildTable(root->right);
  }
  if (root->left == NULL && root->right == NULL) {
    table[root->c]=code;
}      
    code.pop_back();
}

void encode(void) {
  ifstream f(name, ios::binary);
  map<unsigned char,int> m;  
  char c;
  while (f.get(c))
    m[c]++;
////// ?????????? ????????? ???? ? ?????? list            
  list<Node*> t;
  for(map<unsigned char,int>::iterator itr=m.begin(); itr!=m.end(); ++itr) { 
    Node*p = new Node;
    p->c = itr->first;
    p->a = itr->second;  
    t.push_back(p); 
   }
//////  ??????? ??????      
  while (t.size()!=1) {
  	t.sort(MyCompare());
    Node *SonL = t.front();
    t.pop_front();
    Node *SonR = t.front(); 
    t.pop_front();
    Node *parent = new Node(SonL,SonR); 
    t.push_back(parent);
  }
//root - ????????? ?? ??????? ??????
  root = t.front();     
////// ??????? ???? ??????-???:           
  BuildTable(root);   
// ?????????? ????????? ????? ? ?????? ?????
  f.clear(); f.seekg(0); 
////// ??????? ???? ? ???? output.txt
  strcat(name1,"_compressed.txt");
  ofstream g(name1, ios::binary);
  while(f.get(c)) {
    vector<bool> x = table[c];
    for(int n=0; n<x.size(); n++) {
      buf = buf | x[n]<<(7-count);   
      count++;   
      if (count==8) { 
		count=0;
		g.put(buf);
	    buf=0; 
	  } 

	}
  }
  f.close();
  g.close(); 
}

void decode(void) {
  ifstream F(name1, ios::binary);    
  strcat(name2,"_decompressed.txt");
  ofstream res(name2, ios::binary);
  Node *p = root;
  count=0;
  unsigned char byte; 
  byte = F.get();
    
  while(!F.eof()) {
   	bool b = byte & (1 << (7-count) ) ; 
      if(b) p=p->right;
	  else p=p->left;
      if(p->left==NULL && p->right==NULL) {
	    res.put(p->c);
	    p=root;
	  }
      count++;
      if(count==8) {
	    count=0;
	    byte = F.get();
	  }
	}
  F.close();
  res.close();
}

int main (int argc, char *argv[])
{
  setlocale(LC_ALL, "rus"); 
  cin>>name;
  strcpy(name1, name); strcpy(name2, name);
  strcat(name,".txt");
  encode();
  decode();
  return 0;
}
