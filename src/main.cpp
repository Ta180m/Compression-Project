#include "common.h"
#include "huffman.h"
#define pb push_back

int getweight(string s)
{
  return 0;
}
struct clause
{
  vector<string> words;
  vector<int> weights;
  string s;
  int weight;
  void parse()
  {
    string word="";
    for(int j=0;j<s.length();j++)
    {
      if(s.at(j)!=' ')word+=s.at(j);
      else
      {
        if(word!="")words.pb(word);
        word="";
      }
    }
    if(word!="") words.pb(word);
  }
  void calcweight()
  {
    for(auto& w: words)weights.pb(getweight(w));
    weight=-100000;
    for(auto& w : weights)if(w>weight)weight=w;
  }
};
struct sentence
{
  vector<clause> clauses;
  string s;
  int weight;
  void parse()
  {
    string cl="";
    //bool iseq=false;
    for(int j=0;j<s.length();j++)
    {
      //if(s.at(j)=='$') iseq=!iseq;
      if(s.at(j)!=',') cl+=s.at(j);
      else
      {
        clause tmp;
        tmp.s=cl;clauses.pb(tmp);cl="";
      }
    }
    clause tmp;
    tmp.s=cl;clauses.pb(tmp);
    for(auto& c:clauses) c.parse();
    int numclause=0;
    for(int i=0;i<clauses.size();i++)
      if(clauses[i].words.size()!=0)
      {
        auto tmp=clauses[i];
        clauses[numclause]=tmp;
        numclause++;
      }
    clauses.resize(numclause);
  }
  void calcweight()
  {
    weight=-100000;
    for(auto& c : clauses)if(c.weight>weight)weight=c.weight;
  }
};
vector<sentence> text;
vector<int> value;
string line;
int b,c;
void init()
{
  cin>>c;
  ifstream stin("soc1.txt");
  string sentencefile="";
  while(getline(stin,line))
  {
    for(int i=0;i<line.length();i++)
    {
      b++;
      if(line.at(i)!='.')sentencefile+=line.at(i);
      else
      {
        sentence tmp;tmp.s=sentencefile;text.pb(tmp);
        sentencefile="";
      }
    }
    b++;
    sentencefile+=' ';
  }
}
void clsep()
{
  for(auto& i:text)i.parse();
}
int main() {
  init();
  clsep();
  /*vector<bool> test = huffman::encode("test");
  for (auto b : test) cout << b;
  string s = huffman::decode(test);
  cout << s << '\n';*/
}