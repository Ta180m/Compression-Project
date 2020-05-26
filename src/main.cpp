#include "common.h"
#include "huffman.h"
#define pb push_back

struct weightstruct
{
  int word;
  int clause;
  int sentence;
  weightstruct(int a=100,int b=0,int c=0):word(a),clause(b),sentence(c){}
  weightstruct operator + (const weightstruct& w) const
  {
    return weightstruct(word+w.word,clause+w.clause,sentence+w.sentence);
  }
};

weightstruct getweight(string s)
{
  if(s[0]=='$')
    return weightstruct(1000,0,0);
  else if(s=="farmer"||s.length()==1||s=="example"||s=="is"||s=="are")return weightstruct(-100,0,0);
  else if(s=="one"||s=="two"||s=="three"||s=="four"||s=="five"||s=="six"||s=="seven"||s=="eight"||s=="nine")return weightstruct(-100,0,0);
  else return weightstruct(0,0,0);
}
struct clause
{
  vector<string> words;
  vector<weightstruct> weights;
  string s;
  weightstruct weight;
  void parse()
  {
    string word="";
    bool iseq=false;
    for(int j=0;j<s.length();j++)
    {
      if(s.at(j)=='$') iseq=!iseq;
      if(('a'<=s.at(j)&&s.at(j)<='z')||('A'<=s.at(j)&&s.at(j)<='Z')||('0'<=s.at(j)&&s.at(j)<='9')||(iseq&&s.at(j)!=' '))
        word+=('A'<=s.at(j)&&s.at(j)<='Z')?(s.at(j)-'A'+'a'):(s.at(j));
      else
      {
        if(!iseq)
        {
          if(word!="")words.pb(word);
          word="";
        }
      }
    }
    if(word!="") words.pb(word);
  }
  void calcweight()
  {
    weights.clear();
    for(auto& w: words)weights.pb(getweight(w));
    for(auto& w: weights) weight=weight+w;
  }
  string gettext(int minw)
  {
    bool iss=true;
    string ans="";
    for(int i=0;i<words.size();i++)
      if(weights[i].word>=minw)
      {
        if(iss)
        {
          iss=false;
          ans+=words[i];
        }
        else ans+=" "+words[i];
    }
    return ans;
  }
};
struct sentence
{
  vector<clause> clauses;
  string s;
  weightstruct weight;
  void parse()
  {
    string cl="";
    bool iseq=false;
    for(int j=0;j<s.length();j++)
    {
      if(s.at(j)=='$') iseq=!iseq;
      if(s.at(j)!=','|| iseq) cl+=s.at(j);
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
    weight=weightstruct();
    for(auto& c : clauses)
    {
      c.calcweight();
      weight=weight+c.weight;
    }
  }
  string gettext(int minw)
  {
    bool iss=true;
    string ans="";
    for(auto& c:clauses)
    if(c.weight.clause>=minw)
    {
      string tmp=c.gettext(minw);
      if(tmp.length()>0)
      {
        if(iss)
        {
          iss=false;
          ans=ans+tmp;
        }
        else ans+=","+tmp;
      }
    
    }
    return ans;
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
  bool iseq=false;
  while(getline(stin,line))
  {
    for(int i=0;i<line.length();i++)
    {
      b++;
      if(line.at(i)=='$') iseq=!iseq;
      if((line.at(i)!='.'&&line.at(i)!='?'&&line.at(i)!='!')|| iseq)sentencefile+=line.at(i);
      else
      {
        sentence tmp;tmp.s=sentencefile;text.pb(tmp);
        sentencefile="";
      }
    }
    b++;
    sentencefile+=' ';
  }
  for(auto& i:text)i.parse();
}

string gettextstring(int minw)
{
  bool iss=true;
  string ans="";
  for(auto& c:text)
    if(c.weight.sentence>=minw)
    {
      string tmp=c.gettext(minw);
      if(tmp.length()>0)
      {
        if(iss)
        {
          iss=false;
          ans=ans+tmp;
        }
        else ans+="."+tmp;
      }
    
    }
  return ans;
}

int main() {
  init();
  for(auto x:text)
    x.calcweight();
  cout<<gettextstring(-10000);
  

  
  // WARNING: Huffman will CRASH if you pass a string with only one unique /*aracter
  /*string orig = "test1234";
  vector<bool> enc = huffman::encode(orig);
  string dec = huffman::decode(enc);
  if (orig != dec) {
	cout << "Not a match!\n";
  }
  else {
    cout << dec << '\n';
    for (auto b : enc) cout << b;
    cout << '\n';
	cout << "Original length: " << orig.size() << '\n';
    cout << "Compressed length: " << (enc.size() + 7) / 8 << '\n';
	cout << "Percent compression: " << 100.0 - (double)100.0 * (enc.size() + 7) / 8 / orig.size() << "%\n";
  }*/
}