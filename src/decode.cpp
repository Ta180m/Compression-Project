#include "common.h"
#include "huffman.h"

/*int main() {
  ifstream enc("êõz=Çãñx¼Þïw»]®W«Õj5F£Ñèôz=žO'“Éär¹\\®×k5šÍf³Y¬V«Õj5†Ãa°X¬ÖkµZ­V«UªUªUªÕjµÚív»Ýî÷ûý~?");
  stringstream ss;
  ss << enc.rdbuf();
  string s = ss.str();
  vector<bool> v;
  for (auto& c : s) {
    //v.push_back(c == '1');
    for (int i = 0; i < 8; ++i) v.push_back(1 & (c >> i));
  }
  cout << huffman::decode(v);
}*/