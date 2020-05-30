#include "common.h"
#include "huffman.h"
#define pb push_back


struct weightstruct
{
	int word;
	int clause;
	int sentence;
	weightstruct(int a=0,int b=0,int c=0):word(a),clause(b),sentence(c){}
	weightstruct operator + (const weightstruct& w) const
	{
		return weightstruct(word+w.word,clause+w.clause,sentence+w.sentence);
	}
};
map<string,int> wordlist;
set<string> adjlist;
void getlists()
{
  ifstream listfile("fullist");
  for(int i=0;i<500;i++)
  {
    string a;int b;
    listfile>>a>>b;
    wordlist[a]=b;
  }
  ifstream listfileadj("adjlist");
  for(int i=0;i<227;i++)
  {
    string a;
    listfileadj>>a;
    adjlist.insert(a);
  }
}

weightstruct getweight(string s)
{
	if(s[0]=='$')
		return weightstruct(200,50,10);
	if(s=="one"||s=="two"||s=="three"||s=="four"||s=="five"||s=="six"||s=="seven"||s=="eight"||s=="nine")return weightstruct(100,25,10);
	if(s=="example")return weightstruct(-300,-200,-100);
  

  
  if(s=="spi")
    return weightstruct(-100,-10,-200);
  if(s=="ipf")
    return weightstruct(-100,30,50);
  if(s=="please") return weightstruct(-300,0,0);
  if(adjlist.find(s)!=adjlist.end())
  {
    return weightstruct(-100,-10,-10);
  }
  if(wordlist.find(s)!=wordlist.end())
  {
    return weightstruct(-wordlist[s]/100000,-10,-5);
  }
  
  /*
	if(s=="a"||s=="is"||s=="are"||s=="to"||s=="so"||s=="as") return weightstruct(-200,0,0);
	if(s=="the")
		return weightstruct(-200,0,0);
/*
	if(s.length()==1)
		return weightstruct(-300,0,0);
	if(s.length()==2)
		return weightstruct(-50,0,0);
*/
	return weightstruct(-10,0,0);
}
void preprocessword(int stage,string& s,string& next,int pos)
{
	if(stage==-1)//after deleting low weights
	{
		if(s[0]=='$')
			s=s.substr(1,s.length()-2);
    if(s.length()>5)
    {
      string tmp;
      for(char x:s)
        if(x!='a'&&x!='e'&&x!='i'&&x!='o'&&x!='u')
          tmp+=x;
      s=tmp;
    }
	}
	if(stage==0)
	{
		string tmp;
		for(char c:s)
			tmp+=('A'<=c&&c<='Z')?(c-'A'+'a'):(c);
		s=tmp;
	}
	if(stage==1)
	{
    if(s=="sample"&&next=="input")
      s="spi",next="";
    if(s=="sample"&&next=="output")
      s="spo",next="";
    if(s=="input"&&next=="format")
      s="ipf",next="";
    if(s=="output"&&next=="format")
      s="opf",next="";

    if(s=="input")
      s="ipt";
    if(s=="output")
      s="opt";
		if(s=="and")
			s=",";
		if(s.size()>=2)
			if(s.substr(s.size()-2)=="'s")
				s=s.substr(0,s.size()-2);
	}
	if(stage==2)
	{
		if(s=="farmer"&&next=="john")
		{
			s="fj";
			next="";
		}
	}
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
			if(s.at(j)=='$')
			{
				if(iseq)
				{
					word+="$";
					words.pb(word);
					word="";
					iseq=false;
				}
				else
				{
					if(word!="") words.pb(word);
					word="$";
					iseq=true;
				}
			}
			else
			{
				if(s.at(j)!=' '&&s.at(j)!='\"'&&s.at(j)!='\"')
					word+=(s.at(j));
				//if(('a'<=s.at(j)&&s.at(j)<='z')||('A'<=s.at(j)&&s.at(j)<='Z')||('0'<=s.at(j)&&s.at(j)<='9')||(iseq&&s.at(j)!=' '))
				//  word+=('A'<=s.at(j)&&s.at(j)<='Z')?(s.at(j)-'A'+'a'):(s.at(j));
				else
				{
					if(!iseq)
					{
						if(word!="")words.pb(word);
						word="";
					}
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
	void preprocess(int stage)
	{
		for(int i=0;i<words.size();i++)
		{
			string e="";
			string* nx=&e;
			if(i+1<words.size())
				nx=&words[i+1];
			preprocessword(stage,words[i],*nx,i);
		}
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
			if((s.at(j)!=','&&s.at(j)!='('&&s.at(j)!=')'&&s.at(j)!=';'&&s.at(j)!=':')|| iseq) cl+=s.at(j);
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
	void preprocess(int stage)
	{
		for(auto& c:clauses)
			c.preprocess(stage);
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
struct file
{
	vector<sentence> text;
	void init(string tx)
	{
		string line;
		stringstream stin(tx);
		string sentencefile="";
		bool iseq=false;
		while(getline(stin,line))
		{
			if(line=="INPUT FORMAT")
			{
				sentence tmp;tmp.s=sentencefile;text.pb(tmp);
				sentencefile=line;
				while(getline(stin,line)&&line!="SAMPLE INPUT")
				{
					sentencefile+=' '+line;
				}
				sentence tmp2;tmp2.s=sentencefile;text.pb(tmp2);
				sentencefile="";
			}
			if(line=="SAMPLE INPUT")
			{
				sentence tmp;tmp.s=sentencefile;text.pb(tmp);
				sentencefile=line;
				while(getline(stin,line))
				{
					sentencefile+=' '+line;
				}
				sentence tmp2;tmp2.s=sentencefile;text.pb(tmp2);
				sentencefile="";
			}
			for(int i=0;i<line.length();i++)
			{
				if(line.at(i)=='$') iseq=!iseq;
				if((line.at(i)!='.'&&line.at(i)!='?'&&line.at(i)!='!')|| iseq) sentencefile+=line.at(i);
				else
				{
					sentence tmp;tmp.s=sentencefile;text.pb(tmp);
					sentencefile="";
				}
			}
			sentencefile+=' ';
		}
		sentence tmp;tmp.s=sentencefile;text.pb(tmp);
		for(auto& i:text)i.parse();
	}
	void preprocess(int stage)
	{
		for(auto&x : text)
			x.preprocess(stage);
	}
	void calcweight()
	{
		for(auto& x:text)
			x.calcweight();
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
};


string huffmancompress(string s)
{
  vector<bool> enc = huffman::encode(s);
  for (auto b : enc) cout << b;
  cout << endl;
  string ans;
  for (int i = 0; i < (int)enc.size(); ++i) {
    char out = 0;
    for (int j = i; j < min(i + 8, (int)enc.size()); ++j) {
      if (enc[j]) out += (1 << (j - i));
    }
    ans += out;
  }
  return ans;
}

int huffmancompresslength(string s)
{
  vector<bool> enc = huffman::encode(s);
  return (enc.size() + 7) / 8;
}

int initsize=0;
int compressrate;
int targetsize;

int main(int argc, char *argv[]) {
  getlists();
	string input="";

  string input_file = argv[1];
  //cin >> input_file;

	string line;
	compressrate = atoi(argv[2]);
	ifstream stin(input_file);
  compressrate = 100 - compressrate;

	while(getline(stin,line))
	{
		input+=line;
		input+='\n';
	}
	initsize=input.length()-1;//remove last newline

  for(char& x:input)
    if(x=='\r' || x == '\n')
      x=' ';
      
	file orig;
	orig.init(input);

	file final;
	final=orig;
	for(int i=0;i<5;i++)
	{
		final.preprocess(i);
	}
	targetsize=((compressrate)*initsize+99)/100;

	final.calcweight();
	int a=-10000;int b=10000;//(a,b]
	while(a+1<b)
	{
		int c=(a+b)/2;
		file tmp;

		tmp.init(final.gettextstring(c));
		tmp.calcweight();
		tmp.preprocess(-1);
    string s = tmp.gettextstring(-100000);
		int sz= (s == "" ? 0 : huffmancompresslength(s));
		if(sz>targetsize)
		{
			a=c;
		}
		else
		{
			b=c;
		}
	}
	file output;
	output.init(final.gettextstring(b));
	output.calcweight();
	output.preprocess(-1);
	string s = output.gettextstring(-100000);
  cout << s << '\n';
    
  //where can i find a library of words in categories can't find any
  // not really sure
  // im not getting anywhere with weighting lol
  // there is no list i can find


  //!!! are we counting input size correctly bc the \r issue?
  s = huffmancompress(s);
  int orig_size = 0;
  ifstream testin(input_file);
  char c;
  while (testin >> c) ++orig_size;
  while (100 * s.size() > compressrate * orig_size) s.pop_back();
  cout << "Original length: " << orig_size << '\n';
  cout << "Compressed length: " << s.size() << '\n';
  cout << "Percent compression: " << 100.0 - ((double)100.0 * s.size() / orig_size) << "%\n";
  cout << s << '\n'; 
  ofstream cout("test");
	cout << s << '\n'; 

	// WARNING: Huffman will CRASH if you pass a string with only one unique character
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
