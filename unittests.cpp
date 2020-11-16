/* Name: Reed Kass-Mullet
 * Date: 7 March 2019
 * Assignment: COMP15 proj1 - unit testing
 * Description: Unit testing for my trieNode Tree class
 */
#include <string>
#include <iostream>
#include "trieNodeTree.h"
#include <string>

using namespace std;

bool test_query(string cfile, string ofile);
bool test_insert(string cfile, string ofile);
bool test_prefix_query(string cfile);
bool test_remove(string cfile, string ofile);
bool test_count_nodes(string cfile, string ofile);
bool test_copy_constructor(string cfile, string ofile);

int main(){
  string commandFile = "no_Commands.txt";
  string outputFile = "unittestoutput.txt";
  bool results[6];
  bool pass = true;

  results[0] = test_query(commandFile, outputFile);
  results[1] = test_insert(commandFile, outputFile);
  results[2] = test_prefix_query(commandFile);
  results[3] = test_remove(commandFile, outputFile);
  results[4] = test_count_nodes(commandFile, outputFile);
  results[5] = test_copy_constructor(commandFile, outputFile);

  for(int i = 0; i < 6; i++){
    if (!results[i]){
      cout << i + 1 << " Failed the test\n";
      pass = false;
    } else
      cout << i + 1 << " Passed the test\n";
  }
  if(pass)
    cout << "All Tests Pass!\n";
  else
    cout << "Functions have Failed\n";
  cerr << "FIN\n";
}

//Tests query function
bool test_query(string cfile, string ofile){
  trieNodeTree tester(cfile, ofile);
  bool correct = true;
  tester.insert("AAA");
  if(tester.query("AAA") != "AAA")
    correct = false;
  if(tester.query("A") != "AAA")
    correct = false;
  if(tester.query("AAAA") != "AAA")
    correct = false;
  tester.insert("CTG");
  tester.insert("CTA");
  tester.insert("C");
  if(tester.query("CT") != "CTA")
    return false;
  return correct;
}

//tests insert function
bool test_insert(string cfile, string ofile){
  trieNodeTree tester(cfile, ofile);
  bool correct = true;
  if(tester.count_nodes() != 0)
    correct = false;
  tester.insert("AA");
  if(tester.count_nodes() != 2)
    correct = false;
  tester.insert("AAAA");
  if(tester.count_nodes() != 4)
    correct = false;
  return correct;
}

//tests prefix_query function
bool test_prefix_query(string cfile){
  trieNodeTree tester(cfile, "prefix_unittesting.txt");
  tester.insert("AAA");
  tester.insert("AAAGTA");
  tester.insert("CAAA");
  tester.insert("AA");
  tester.prefix_query("AAA");

  ifstream inFile;
  inFile.open("prefix_unittesting.txt");
  string check;
  getline(inFile, check); //Also checks for correct ordering in print function
  if(check != "AAA")
    return false;
  getline(inFile, check);
  if(check != "AAAGTA")
    return false;
  return true;
  inFile.close();
}

//tests remove function
bool test_remove(string cfile, string ofile){
  trieNodeTree tester(cfile,ofile);
  tester.insert("ACGTA");
  tester.insert("AAA");
  if(tester.count_nodes() != 7)
    return false;
  tester.remove("AAA");
  if(tester.count_nodes() != 5)
    return false;
  tester.remove("ACGTA");
  if(tester.count_nodes() != 0)
    return false;
  return true;
}

// tests count_nodes function
bool test_count_nodes(string cfile, string ofile){
  trieNodeTree tester(cfile, ofile);
  tester.insert("AAA");
  if(tester.count_nodes() != 3)
    return false;
  tester.insert("AA");
  if(tester.count_nodes() != 3)
    return false;
  return true;
}

//Tests copy constructor & Assignment Overload
bool test_copy_constructor(string cfile, string ofile){
  trieNodeTree tester(cfile, ofile);
  tester.insert("AAA");
  trieNodeTree copied = tester;
  tester.remove("AAA");
  if(copied.count_nodes() != 3)
    return false;
  return true;
}
