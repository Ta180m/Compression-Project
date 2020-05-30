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

weightstruct getweight(string s)
{
	if(s[0]=='$')
		return weightstruct(200,100,100);
	if(s=="one"||s=="two"||s=="three"||s=="four"||s=="five"||s=="six"||s=="seven"||s=="eight"||s=="nine")return weightstruct(100,50,50);
	if(s=="example")return weightstruct(-300,-200,-200);
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
				if(s.at(j)!=' ')
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
  string ans;
  for (int i = 0; i < ((int)enc.size() + 7) / 8; ++i) {
    char out = 0;
    for (int j = i; j < min(i + 8, (int)enc.size()); ++j) {
      if (enc[j]) i += (1 << (j - i));
    }
    ans += out;
  }
}

int initsize=0;
int compressrate;
int targetsize;

int main() {
	string input="";

  string input_file = "soc1.txt";
  //cin >> input_file;

	string line;
	cin>>compressrate;
	ifstream stin(input_file);


	while(getline(stin,line))
	{
		input+=line;
		input+='\n';
	}
	initsize=input.length()-1;//remove last newline

	file orig;
	orig.init(input);

	file final;
	final=orig;
	for(int i=0;i<5;i++)
	{
		final.preprocess(i);
	}
	targetsize=((100-compressrate)*initsize+99)/100;

	final.calcweight();
	int a=-10000;int b=10000;//(a,b]
	while(a+1<b)
	{
		int c=(a+b)/2;
		file tmp;

		tmp.init(final.gettextstring(c));
		tmp.calcweight();
		tmp.preprocess(-1);
		int sz=huffmancompress(tmp.gettextstring(-100000)).length();
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
	for (auto& c : s) if (c == '\r') c = ' ';
	//printf("%s\n", s.c_str());

  
  
  /*int final_ratio = 10, orig_size = 0;
  ifstream testin(input_file);
  char c;
  while (testin >> c) ++orig_size;
  while (10 * ans.size() > final_ratio * orig_size) ans.pop_back();
  cout << ans;*/


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
