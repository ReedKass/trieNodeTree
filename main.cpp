/* Name: Reed Kass-Mullet
 * Date: 11 March 2019
 * Assignment: comp15 - proj1
 * Description: the main.cpp file for the executable of the 
 *              trieNode tree class.
 */
#include "trieNodeTree.h"
#include "fstream"
#include "string"
using namespace std;

int main(int argc, char* argv[]){
  if(argc != 3)
    cerr << "Usage: ./SeqMatch [query file] [output location]\n";
  else if(argc == 3){
    trieNodeTree run(argv[1], argv[2]);
    run.driver();
  }
}
