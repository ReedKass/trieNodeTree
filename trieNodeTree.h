/* 
 * Name: Reed Kass-Mullet
 * Date: 7 March 2019
 * Assignment: COMP15-proj1
 ===========================================================================*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <sstream>
#include <cmath>

using namespace std;

struct trieNode {
  trieNode *children[4];
  bool word_end;
  trieNode *parent;
};

//Function descriptions are located in the .cpp file
class trieNodeTree {
public:

trieNodeTree();

trieNodeTree(string fname, string ofname);

~trieNodeTree();

trieNodeTree(const trieNodeTree &source);

trieNodeTree &operator= (const trieNodeTree &source);

void driver();
int count_nodes();
void remove(string word);
void insert(string word);
string query(string word);
void prefix_query(string prefix);

private:

const int A = 65;
const int C = 67;
const int G = 71;
const int T = 84;

ifstream commandFile;
string cfile;
ofstream outputFile;
string ofile;

trieNode *root;
trieNode *make_node(trieNode *parent);
int count_nodes(trieNode *node);
bool remove(trieNode *node, string word);
void remove_helper(trieNode *node, trieNode *prev);
void insert(trieNode *node, string word);
void insert_helper(trieNode *node, string word, int direction);
void query_driver_call(string word);
string query(trieNode *node, string word);
string query_helper(trieNode *node);
vector<string> query_helper_contains(trieNode *node, vector<string> current);
bool is_leaf(trieNode *node);
trieNode *prefix_query(trieNode *node, string prefix);
double percent(string word, string result);
void print(trieNode *node);
string print_helper(trieNode *node);
void delete_Tree(trieNode *node);
trieNode *copy(trieNode *node);
void passFile(string fname);
};
