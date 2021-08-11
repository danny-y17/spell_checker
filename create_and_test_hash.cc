// Danny You
#include "quadratic_probing.h"
#include "linear_probing.h"
#include "double_hashing.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

template <typename HashTableType>
void TestFunctionForHashTable(HashTableType &hash_table, const string &words_filename, const string &query_filename) {
  //cout << "TestFunctionForHashTables..." << endl;
  //cout << "Words filename: " << words_filename << endl;
  //cout << "Query filename: " << query_filename << endl;
  hash_table.MakeEmpty();  

  ifstream file(words_filename);
  string line;
  while(getline(file,line)) {
      hash_table.Insert(line);
  }
  // # of elements
  int item_count = hash_table.get_item_count();  
  cout << "number_of_elements: " << item_count << endl;
  // table size
  int table_size = hash_table.get_table_size();
  cout << "size_of_table: " << table_size << endl;
  // load factor
  double table_size2 = table_size;
  float load_factor = item_count / table_size2;
  cout << "load_factor: " << load_factor << endl;
  // collisions
  float collision_count = hash_table.get_colli_count();
  float average_count = collision_count/item_count;
  cout << "average_collisions: " << average_count << endl;
  cout << "total_collisions: " << collision_count << endl;
  cout << endl;

    ifstream file2(query_filename);
    string line2;
    while(getline(file2,line2)) {
        hash_table.probe_reset();
        if (hash_table.Contains(line2)) {
            cout << line2 << " Found " << hash_table.get_colli_count()+1 << endl;
        }
        else {
            cout << line2  << " Not_Found " << hash_table.get_colli_count()+1 << endl;
        }
    }
}

//namespace PROTECTED {

int testHashingWrapper(int argument_count, char **argument_list) {  
    const string words_filename(argument_list[1]);  // words_filename = wordsfilename
    const string query_filename(argument_list[2]);  // query_filename = queryfilename
    const string param_flag(argument_list[3]);      // param_flag = flag
    int R = 5;                                      // default R value to be used
    if (argument_count == 5) {                      // if argument count = 5 means theres an R input
        const string rvalue(argument_list[4]);      // rvalue = R
        R = stoi(rvalue);                           // returns as interger
    }
    if (param_flag == "linear") {   // if flag is linear
       // cout << "Linear:" << endl;  // print linear
        HashTableLinear<string> linear_probing_table;
        TestFunctionForHashTable(linear_probing_table, words_filename,
                                query_filename);
    } 
    else if (param_flag == "quadratic") {   // if flag is quadratic
        //cout << "Quadratic:" << endl;       // print quadratic
        HashTable<string> quadratic_probing_table;
        TestFunctionForHashTable(quadratic_probing_table, words_filename,
                                 query_filename);

    } 
    else if (param_flag == "double") {
        //cout << "Double:" << endl;
        cout << "r_value: " << R << endl;
        HashTableDouble<string> double_probing_table;
        double_probing_table.SetR(R);
        TestFunctionForHashTable(double_probing_table, words_filename,
                                   query_filename);
    } 
    else {
        cout << "Unknown hash type " << param_flag
             << " (User should provide linear, quadratic, or double)" << endl;
    }
    return 0;
}
// Sample main for program create_and_test_hash.
// WE WILL NOT USE YOUR MAIN IN TESTING. DO NOT CODE FUNCTIONALITY INTO THE
// MAIN. WE WILL DIRECTLY CALL testHashingWrapper. ALL FUNCTIONALITY SHOULD BE
// THERE. This main is only here for your own testing purposes.
int main(int argc, char **argv) {   // argument count and argument vector
    if (argc != 4 and argc != 5) {  // if argument count is not 4 or 5 print the following
        cout << "Usage: " << argv[0]   
             << " <wordsfilename> <queryfilename> <flag>" << endl;
        cout << "or Usage: " << argv[0]
             << " <wordsfilename> <queryfilename> <flag> <rvalue>" << endl;
        return 0;
    }

    testHashingWrapper(argc, argv); //argument counts: 4 or 5 argument v: query_words, words, flag
    return 0;
}

//};  // namespace PROTECTED
