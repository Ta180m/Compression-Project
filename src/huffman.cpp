#include "huffman.h"
#include "common.h"

namespace huffman {
  struct node {
    char c; int f; node * l, * r;
    node(char _c, int _f) { c = _c, f = _f, l = r = NULL; }
  };
  struct comp { bool operator()(node * a, node * b) { return a->f > b->f; } };
  node * root;
  vector<bool> path;
  vector<vector<bool>> code(256);
  void traverse(node * n) {
    if (n->l) {
      path.push_back(0);
      traverse(n->l);
      path.pop_back();
    }
    if (n->r) {
      path.push_back(1);
      traverse(n->r);
      path.pop_back();
    }
    if (n->c) code[n->c] = path;
  }

  void get_tree(node * n, vector<bool> & v) {
    if (n->l) {
      v.push_back(1);
      get_tree(n->l, v);
    }
    else v.push_back(0);
    if (n->r) {
      v.push_back(1);
      get_tree(n->r, v);
    }
    else v.push_back(0);
    if (n->c) {
      v.push_back(1);
      for (int i = 0; i < 8; ++i) v.push_back(1 & (n->c >> i));
    }
    else v.push_back(0);
  }

  int idx = 0;
  void construct_tree(node * n, vector<bool> & v) {
    if (v[idx++] == 1) {
      n->l = new node(0, 0);
      construct_tree(n->l, v);
    }
    if (v[idx++] == 1) {
      n->r = new node(0, 0);
      construct_tree(n->r, v);
    }
    if (v[idx++] == 1) {
      for (int i = 0; i < 8; ++i) n->c |= (1 << v[idx++]);
    }
  }

  void generate(vector<int> f) {
    priority_queue<node *, vector<node *>, comp> pq;
    for (int c = 'a'; c <= 'z'; ++c) {
      node * n = new node(c, f[c]);
      pq.push(n);
    }
    while (pq.size() > 1) {
      node * l = pq.top(); pq.pop();
      node * r = pq.top(); pq.pop();
      node * n = new node(0, l->f + r->f);
      n->l = l, n->r = r;
      pq.push(n);
    }
    root = pq.top();
  }

  void solve(node * n, vector<bool> & v, string & s) {
    if (n->c) {
      s += n->c;
      solve(root, v, s);
    }
    else {
      if (v[idx++] == 0) solve(n->l, v, s);
      else solve(n->r, v, s);
    }    
  }

  vector<bool> encode(string s) {
    vector<int> f(256);
    for (auto& c : s) ++f[c];
    generate(f);
    traverse(root);
    vector<bool> ret;
    get_tree(root, ret);
    for (auto& c : s) {
      for (auto b : code[c]) ret.push_back(b);
    }
    return ret;
  }

  string decode(vector<bool> v) {
    root = new node(0, 0);
    string ret;
    construct_tree(root, v);
    solve(root, v, ret);
    return ret;
  }
}