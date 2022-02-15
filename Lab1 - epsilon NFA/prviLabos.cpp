#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>

using namespace std;

void splitByChar(string s, char c, vector<string> &vec)
{
   int i = 0;
   int j;
   j = s.find(c, i);
   int l = s.length();
   while (j != -1)
   {
      vec.push_back(s.substr(i, j - i));
      i = ++j;
      j = s.find(c, i);
   }
   if (i != l)
   {
      vec.push_back(s.substr(i));
   }
   return;
}

void fillEpsilon(set<string> &statesNow, map<pair<string, string>, set<string>> &transitionMap)
{
   //cout << "provjeravam Eps" << endl;
   set<string> statesTemp(statesNow); //copy values
   pair<string, string> key;
   for (auto &state : statesNow)
   {
      key.first = state;
      key.second = '$';
      if (transitionMap.count(key))
      {
         for (auto &elem : transitionMap[key])
         {
            //cout << "Eps nasao: " << elem << endl;
            if (elem != "#")
               statesTemp.insert(elem);
         }
      }
   }
   if (statesNow.size() == statesTemp.size())
   {
      return;
   }
   else
   {
      statesNow = statesTemp;
      statesTemp.clear();
      fillEpsilon(statesNow, transitionMap);
   }
}

string automateNext(string firstState, vector<string> &input, map<pair<string, string>, set<string>> &transitionMap)
{
   string output = "";
   set<string> statesNow, statesNext;
   statesNow.insert(firstState);
   fillEpsilon(statesNow, transitionMap);
   for (auto &state : statesNow)
   {
      output += (state + ",");
   }

   output.erase(output.size() - 1);

   pair<string, string> key;
   for (auto &sign : input)
   {
      for (auto &state : statesNow)
      {
         //statesNext.clear();
         key.first = state;
         key.second = sign;
         //cout << "provjeravam ima li u mapi: " << key.first << " " << key.second << endl;
         if (transitionMap.count(key) && transitionMap[key].count("#") == 0)
         {
            //cout << "nasao sam" << endl;
            for (auto &elem : transitionMap[key])
            {
               //cout << "upisujem " << elem << endl;
               statesNext.insert(elem);
               //cout << "imam " << statesNext.size() << " elemenata" << endl;
            }
         }
      }
      fillEpsilon(statesNext, transitionMap);
      statesNow.clear();
      for (auto &n : statesNext)
      {
         statesNow.insert(n);
      }
      statesNext.clear();
      output += "|";
      if (statesNow.size() == 0)
      {
         output += "#";
      }
      else
      {
         for (auto &x : statesNow)
         {
            output += (x + ",");
         }
         output.erase(output.size() - 1);
      }
   }
   return output;
}

int main(void)
{
   string firstRow, secondRow, thirdRow, fourthRow, startState;
   //INPUT 1st

   cin >> firstRow;
   //cerr << "1-------\n";
   vector<string> tempSeq;
   splitByChar(firstRow, '|', tempSeq);

   vector<vector<string>> sequences;
   vector<string> tempoary; //between calculations

   for (int i = 0; i < tempSeq.size(); i++)
   {
      tempoary.clear();
      splitByChar(tempSeq[i], ',', tempoary);
      sequences.push_back(tempoary);
   }

   /*for(int i = 0; i<sequences.size(); i++){
        for(int j=0; j<sequences[i].size(); j++){ //ISPIS 
            cout << sequences[i][j] << "***";
        }
        cout << endl;
    }*/

   //INPUT 2nd
   cin >> secondRow;
   //cerr << "2-------\n";
   vector<string> states;
   splitByChar(secondRow, ',', states);

   //INPUT 3rd
   cin >> thirdRow;
   //cerr << "3-------\n";
   vector<string> symbols;
   splitByChar(thirdRow, ',', symbols);

   //INPUT 4th
   cin >> fourthRow;
   // cerr << "4-------\n";
   vector<string> acceptable;
   splitByChar(fourthRow, ',', acceptable);

   //INPUT 5th
   cin >> startState;
   //cerr << "-------";

   //INPUT transitions
   string line;
   vector<string> splitted;
   vector<string> leftSplitted;
   vector<string> rightSplitted;
   set<string> nextStates;
   map<pair<string, string>, set<string>> transitionMap;

   while (getline(cin, line) >> line)
   {
      //cerr << "unio sam prijelaz+++++";
      line.erase(line.begin() + line.find('>'));
      splitted.clear();
      leftSplitted.clear();
      rightSplitted.clear();
      nextStates.clear();
      splitByChar(line, '-', splitted); //in first is PAIR in second is list of states n+1
      splitByChar(splitted.at(0), ',', leftSplitted);
      splitByChar(splitted.at(1), ',', rightSplitted);

      pair<string, string> key(leftSplitted.at(0), leftSplitted.at(1));
      for (int i = 0; i < rightSplitted.size(); i++)
      {
         nextStates.insert(rightSplitted.at(i));
      }
      //cout << key.first << " " <<  key.second << " -->";
      //cout << nextStates.size();
      //cout << endl;

      transitionMap.insert({key, nextStates});
   }
   /* cout << "vani! simboli->" << endl;
   for (auto &x : sequences)
   {
      for (auto &y : x)
      {
         cout << y << " ";
      }
      cout << endl;
   }
   
   cout << "prelazi: " << endl;
   for (auto &x : transitionMap)
   {
      cout << x.first.first << ", " << x.first.second << "->";
      for (auto &y : x.second)
      {
         cout << y << ", ";
      }
      cout << endl;
   }*/
   //cerr << "vani sam" << endl;

   string van;
   for (auto &vec : sequences)
   {
      van = automateNext(startState, vec, transitionMap);
      cout << van;
      cout << endl;
   }
   return 0;
}
