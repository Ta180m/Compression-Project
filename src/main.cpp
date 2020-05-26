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
        string tmp=c.gettext();
        if(tmp.length()>0)
        {
          if(iss)
          {
            iss=false;
            ans+=c.gettext();
          }
          else ans+=" "+words[i];
        }
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
int main() {
  init();
  for(sentence& i:text)
  {
    for(clause& j:i.clauses)
    {
      for(auto k:j.words)
      {
        cout<<k<<endl;
      }
    }
  }

  /*string orig = "Snow has arrived on the farm, and as she does at the beginning of every winter, Bessie is building a snow-cow! Most of the time, Bessie strives to make her sculpture look as much like a real cow as possible. However, feeling artistically inspired, this year she decides to pursue a more abstract route and build a sculpture in the shape of a tree, consisting of N snowballs (1<=N<=105) connected by N-1 branches, each connecting a pair of snowballs such that there is a unique path between every pair of snowballs. Bessie has added a nose to one of the snowballs, so it represents the head of the abstract snow cow. She designates it as snowball number 1. To add more visual interest, she plans to dye some of the snowballs different colors in an artistic fashion by filling old milk pails with colored dye and splashing them onto the sculpture. Colors are identified by integers in the range 1...105, and Bessie has an unlimited supply of buckets filled with dyes of every possible color. When Bessie splashes a snowball with a bucket of dye, all the snowballs in its subtree are also splashed with the same dye (snowball y is in the subtree of snowball x if x lies on the path from y to the head snowball). By splashing each color with great care, Bessie makes sure that all colors a snowball has been splashed with will remain visible. For example, if a snowball had colors [1,2,3] and Bessie splashes it with color 4, the snowball will then have colors [1,2,3,4]. After splashing the snowballs some number of times, Bessie may also want to know how colorful a part of her snow-cow is. The \"colorfulness\" of a snowball x is equal to the number of distinct colors c such that snowball x is colored c. If Bessie asks you about snowball x, you should reply with the sum of the colorfulness values of all snowballs in the subtree of x. Please help Bessie find the colorfulness of her snow-cow at certain points in time.";
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
	cout << "Percent compression: " << 1.0 - (double)(enc.size() + 7) / 8 / orig.size() << '\n';
  }*/
}