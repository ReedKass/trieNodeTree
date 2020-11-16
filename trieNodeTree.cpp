/* Name: Reed Kass-Mullet
 * Date: 7 March 2019
 * Assignment: Comp15 Proj1
 * Description: Write a Trie Node Tree with multiple functions
 *
 * BtrieNodeTree.cpp
 ==========================================================================*/

#include "trieNodeTree.h"

using namespace std;

//BIG 3

//Constructor
trieNodeTree::trieNodeTree(string ifname, string ofname){
  root = make_node(NULL);
  //  commandFile.open(ifname);
  //  outputFile.open(ofname);
  cfile = ifname;
  ofile = ofname;
}

//Destructor
trieNodeTree::~trieNodeTree(){
  delete_Tree(root);
  root = NULL;
}

//Copy Constructor
trieNodeTree::trieNodeTree(const trieNodeTree &source){
  root = copy(source.root);
}

//Assignment Overload
trieNodeTree &trieNodeTree::operator= (const trieNodeTree &source){
  if(this != &source){
    delete_Tree(root);
    root = copy(source.root);
  }
  return *this;
}

// Input: trieNode Node, the node being copied
// Return: trieNode, the new deep copy of node
// Does: Recursively creates a deep copy of the entire trieNode tree
trieNode *trieNodeTree::copy(trieNode *node){
  if(node == NULL)
    return NULL;
  trieNode *to_copy = make_node(node->parent);
  to_copy->word_end = node->word_end;
  for(int i = 0; i < 4; i++){
    to_copy->children[i] = copy(node->children[i]);
  }
  return to_copy;
}

// Input: trieNode node, the current node being deleted
// Return: nothing
// Does: deletes the current node after recursively deleting all nodes beneath
//       the current one
void trieNodeTree::delete_Tree(trieNode *node){
  if(node != NULL){
    for(int i = 0; i < 4; i++){
      delete_Tree(node->children[i]);
    }
    delete node;
  }
}

// Input: trieNode parent, the parent of the node being created
// Return: trieNode, the new node that has been created
// Does: Creates a new trieNode on the heap and returns it will all children
//       set to NULL and word_end set to false.
trieNode *trieNodeTree::make_node(trieNode *parent){
  trieNode *insertee = new trieNode;
  insertee->word_end = false;
  insertee->parent = parent;
  for(int i = 0; i < 4; i++)
    insertee->children[i] = NULL;
  return insertee;
}

// Input: none
// Return: nothing
// Does: drives the entire class, uses a while loop to draw from the set 
//       command file and run through if statements to deduce what operation
//       is necessary and what functions should be called
void trieNodeTree::driver(){
  string comm = "";
  commandFile.open(cfile);
  outputFile.open(ofile);
  if(commandFile.fail() || outputFile.fail())
    cout << "ERROR: File cannot be opened\n";
  else
    while(getline(commandFile, comm))
      if(comm[0] == 105){//"i"
	outputFile << comm.substr(2) << " inserted\n";
	insert(root, comm.substr(2));
      }else if((comm[0] + comm[1]) == 225){//"pq"
	print(prefix_query(root, comm.substr(3)));
      }else if(comm[0] == 113){
	query_driver_call(comm.substr(2));
      }else if(comm[0] == 114){//"r"
	if(remove(root, comm.substr(2)))
	  outputFile << comm.substr(2) << " removed\n";
	else 
	  outputFile << comm.substr(2) << ": No match found\n";
      }else if(comm[0] == 110){//"n"
	outputFile << "There are " << count_nodes(root) - 1<< 
	  " nodes in the database\n";
      }else if(comm[0] == 112){//"p"
	print(root);
      }else if(comm[0] == 100){//"d"
	delete_Tree(root);
	root = make_node(NULL);
      }else if(comm[0] == 102)//"f"
	passFile(comm.substr(2));
}

// Input: string fname, the new name of the output file
// Return: nothing
// Does: changes the output file to the string passes to it and makes sure that
//       the file is opened correctly.
void trieNodeTree::passFile(string fname){
  outputFile.close();
  outputFile.open(fname);
  if(outputFile.fail())
    cout << "ERROR: File cannot be opened\n";
  else
    outputFile << "Output file has been changed to " << fname << endl;
}

//Abstraction for public call
void trieNodeTree::remove(string word){
  remove(root, word);
}

// Input: trieNode node, the node currently being inspected
//       string word, the word being searched for
// Return: nothing
// Does: recursively searches for the word in question and then calls remove
//       helper to delete all necessary nodes. If no nodes must be deleted
//       then it will simply flip the word_end boolean to false.
bool trieNodeTree::remove(trieNode* node, string word){
  if(node == NULL)
    return false;
  else if(word.length() != 0 && node != NULL){
    if(word[0] == A){
      return remove(node->children[0], word.substr(1));
    }else if(word[0] == C){
      return remove(node->children[1], word.substr(1));
    }else if(word[0] == G){
      return remove(node->children[2], word.substr(1));
    }else if(word[0] == T){
      return remove(node->children[3], word.substr(1));
    }
  }else if(word.length() == 0 && node->word_end){
    bool last_node = true;
    for(int i = 0; i < 4; i++){
      if(node->children[i] != NULL)
	last_node = false;
    }
    if(is_leaf(node)){
      remove_helper(node->parent, node);
      return true;
    }else{
      node->word_end = false;
      return true;
    }
  }else if (word.length() == 0 && node->word_end == false)
    return false;

  cerr << "ERROR\n";
  return false;
}

// Input: trieNode node, the node currently being inspected
//        trieNode prev, the node that was last looked at, this node is the one
//        which is deleted if necessary.
// Return: nothing
// Does: aids the remove function by recurevily climbing up the tree until the
//       code equals the root or the node in question is being used by another
//       string in storage, deleting previous nodes as it goes.
void trieNodeTree::remove_helper(trieNode *node, trieNode *prev){
  bool in_use = false;
  for(int i = 0; i < 4; i++)
    if(node->children[i] != NULL && node->children[i] != prev)
      in_use = true;
  if(node == root || (node->word_end || in_use)){
    for(int i = 0; i < 4; i++){
      if (node->children[i] == prev){
	delete node->children[i];
	node->children[i] = NULL;
      }
    }
  } else{
    delete prev;
    remove_helper(node->parent, node);
  }
}

//Abstraction for public call
void trieNodeTree::insert(string word){
  insert(root, word);
}

// Input: trieNode node, the node the recusion is currently at
//        string word, the word being searched for
// Return: nothing
// Does: recursivey inserts string word by either creating new nodes or by
//       changing the current node's word_end to true;
void trieNodeTree::insert(trieNode* node, string word){
  if(word.length() != 0){
    if(word[0] == A){//"A"
      insert_helper(node, word, 0);
    }
    else if(word[0] == C){//"C"
      insert_helper(node, word, 1);
    }
    else if(word[0] == G){//"G"
      insert_helper(node, word, 2);
    }
    else if(word[0] == T){//"T"
      insert_helper(node, word, 3);
    }
  }else if(word.length() == 0){
    node->word_end = true;
  }
}

void trieNodeTree::insert_helper(trieNode* node, string word, int direction){
  if(node->children[direction] == NULL){
    node->children[direction] = make_node(node);
    insert(node->children[direction], word.substr(1));
  }else if(node->children[direction] != NULL){
    insert(node->children[direction], word.substr(1));
  }
}

// Input: trieNode node, the node currently being printed from
// Return: nothing
// Does: uses recursion to print all strings stored in the tree
void trieNodeTree::print(trieNode* node){
  if(node != NULL){
    if(node->word_end){
      outputFile << print_helper(node) << endl;
    }
    for(int i = 0; i < 4; i++){
      print(node->children[i]);
    }
    //if(node->word_end){
    //  outputFile << print_helper(node) << endl;
    //}
  }
}

// Input: trieNode node, the node that is currently being printed
// Return: the string of all concatinated values found within the printed 
//         string
// Does: uses recursion to climb to trieNode Tree and concatinate to a return
//       string the values of each character in the stored tree.
string trieNodeTree::print_helper(trieNode *node){
  string temp = "";
  if(node->parent == NULL)
    return "";
  else if(node->parent != NULL){
    if(node == node->parent->children[0]){
      temp = "A";
    }
    else if(node == node->parent->children[1]){
      temp = "C";
    }
    else if(node == node->parent->children[2]){
      temp = "G";
    }
    else if(node == node->parent->children[3]){
      temp = "T";
    }
    return print_helper(node->parent) + temp;
  }
  cerr << "error\n";
  return "";
}

//Absraction for public call
int trieNodeTree::count_nodes(){
  return count_nodes(root) - 1;
}

// Input: trieNode node, the node currently being counted
// Return: int value of a node and all nodes under it
// Does: uses recursion to count all nodes present in the tree, root included
int trieNodeTree::count_nodes(trieNode *node){
  int cnt = 0;
  if(node != NULL){
    cnt = cnt + 1;
    for(int i = 0; i < 4; i++){
      cnt = cnt + count_nodes(node->children[i]);
    }
  }
  else
    return 0;
  return cnt;
}

void trieNodeTree::query_driver_call(string word){
  string quer = query(root, word);
  if(quer != "No match found"){
    outputFile << "Match found: " << quer 
	       << " " << percent(word,quer) << "%\n";
  }else
    outputFile << "No match found\n";
}

//Abrasction for public call
string trieNodeTree::query(string word){
  return query(root, word);
}

// Input: trieNode node, the node currently being searched at
//        string word, the word being queried for
// Return: the string of the word that is the closest match to word
// Does: queries the tree for the word that either matches word or comes 
//       closest to it.
string trieNodeTree::query(trieNode *node, string word){
  if(node != NULL){
    if(word.length() == 0 && node->word_end){
      return print_helper(node);
    }else if(word[0] == A && node->children[0] != NULL){
      return query(node->children[0], word.substr(1));
    }else if(word[0] == C && node->children[1] != NULL){
      return query(node->children[1], word.substr(1));
    }else if(word[0] == G && node->children[2] != NULL){
      return query(node->children[2], word.substr(1));
    }else if(word[0] == T && node->children[3] != NULL){
      return query(node->children[3], word.substr(1));
    }else{
      if(is_leaf(node))
	return print_helper(node);
      else
	return query_helper(node);
    }
  }
  cerr << "ERROR\n";
  return NULL;
}

// Input: trieNode node, the node that is being currently searched through
// return: the string of the closest match to the queried word
// Does: recursively climbs the tree in search of the word that most closely
//       matches the string being searched for
string trieNodeTree::query_helper(trieNode *node){
  if(node == NULL)
    return "No match found";
  if(node->word_end)
    return print_helper(node);
  else{
    vector<string> contains;
    contains = query_helper_contains(node, contains);
    string shortest = contains[0];
    for(unsigned long i = 0; i < contains.size(); i++)
      if(contains[i].length() < shortest.length())
	shortest = contains[i];
    return shortest;
  }
}

// Input: trieNode node, the node currently being searched through
//        vector<string> current, a DA of all words founds so far from the
//        origional node
// Returns: a vector<string> which contains all words located from underneath
//          a node passed to the function
// Does: uses recursion to search through the tree and add words to a vector
//       which is then returned.
vector<string> trieNodeTree::query_helper_contains(trieNode *node, 
					       vector<string> current){
  if(node->word_end){
    current.push_back(print_helper(node));
  }
  for(int i = 0; i < 4; i++)
    if(node->children[i] != NULL)
      current = query_helper_contains(node->children[i], current);
  return current;
}

// Input: trieNode node, the node being inspected to see if it is a leaf
// Returns: boolean value, true if node is a leaf, false if not
// Does: uses a for loop to determine and return wether or not the node
//       is a leaf (has no children).
bool trieNodeTree::is_leaf(trieNode *node){
  for(int i = 0; i < 4; i++)
    if(node->children[i] != NULL)
      return false;
  return true;
}

//Abstraction for public call
void trieNodeTree::prefix_query(string prefix){
  print(prefix_query(root, prefix));
}

// Input: trieNode Node, the node currently being inspected
//        string prefix, the string which represents the prefix which is being
//        searched for
// Return: none
// Does: uses recursion to search through the trieNode Tree to find and print
//       all words which match the prefix passed to the function.
trieNode *trieNodeTree::prefix_query(trieNode *node, string prefix){
  if(node == NULL){
    return node;
  }else if(prefix.length() == 0){
    //print(node);
    return node;
  }else if(prefix[0] == A){
    return prefix_query(node->children[0], prefix.substr(1));
  }else if(prefix[0] == C){
    return prefix_query(node->children[1], prefix.substr(1));
  }else if(prefix[0] == G){
    return prefix_query(node->children[2], prefix.substr(1));
  }else if(prefix[0] == T){
    return prefix_query(node->children[3], prefix.substr(1));
  }else {
    cerr << "ERROR\n";
    return NULL;
  }
}

//Input: string word, the queried word
//       string result, the word returned by query
// Return: the double value of the percent match between words
// Does: finds and returns the double value of the percent match between word
//       and result
double trieNodeTree::percent(string word, string result){
  double count = 0;
  int length = 0;
  if (word.length() > result.length())
    length = result.length();
  else
    length = word.length();
  for(int i = 0; i < length; i++){
    if(word[i] == result[i]){
      count++;
    }
  }
  if(word.length() >= result.length())
    return (count/(double)word.length()) * 100;
  else
    return (count/(double)result.length()) * 100;
}
