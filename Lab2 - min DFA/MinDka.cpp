#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <queue>

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

void findAccesible(map<pair<string, string>, string> &transitionMap, set<string> &accesibleStates, string firstState)
{
   accesibleStates.insert(firstState);
   for (auto &entry : transitionMap)
   {
      if (entry.first.first == firstState)
      {
         accesibleStates.insert(entry.second);
         //cout << "dodajem od prvoga: " << entry.second << endl;
      }
   } //added for first state "neighbours"
   int sizeOld, sizeNew;
   do
   {
      sizeOld = accesibleStates.size();
      for (auto &state : accesibleStates)
      {
         for (auto &entry : transitionMap)
         {
            if (entry.first.first == state)
            {
               accesibleStates.insert(entry.second);
               //cout << "dodajem: " << entry.first.first << ", " << entry.first.second << " -> " << entry.second << endl;
            }
         }
      }
      sizeNew = accesibleStates.size();

   } while (sizeOld < sizeNew);
}
void leaveAccesible(map<pair<string, string>, string> &transitionMap, set<string> &accesibleStates, map<pair<string, string>, string> &newMap)
{
   for (auto &entry : transitionMap)
   {
      if (accesibleStates.count(entry.first.first))
      {
         pair<string, string> key(entry.first.first, entry.first.second);
         newMap.insert({key, entry.second});
      }
   }
}
void minimizeDFA(map<pair<string, string>, string> &newMap, set<string> &accesibleStates, set<string> &acceptable, vector<string> &symbols, string startState)
{
   set<string> nonAcp, yesAcp;
   map<string, int> grouping;
   queue<set<string>> groupQ;
   for (auto &e : accesibleStates)
   {
      if (acceptable.count(e))
      {
         yesAcp.insert(e);
      }
      else
      {
         nonAcp.insert(e);
      }
   }
   groupQ.push(nonAcp);
   groupQ.push(yesAcp);
   int numOld, numNew;
   do
   {
      int id = 0;
      numOld = groupQ.size();
      set<string> temp;
      //setting group IDs for all states in Q
      for (int i = 0; i < numOld; i++)
      {
         temp = groupQ.front();
         groupQ.pop();
         for (auto &e : temp)
         {
            grouping[e] = id;
         }
         id++;
         groupQ.push(temp);
      }

      //iterating through Q and searching for states we can join
      for (int i = 0; i < numOld; i++) //for every group in Q
      {
         temp = groupQ.front();
         groupQ.pop();
         set<string> unused = temp;
         while (!unused.empty())
         {
            string first = *(unused.begin());
            unused.erase(first);
            set<string> equWithFirst;
            equWithFirst.insert(first);
            for (string s : unused)
            {
               bool eq = true;
               for (auto &symbol : symbols)
               {
                  if (grouping[newMap.at(make_pair(first, symbol))] != grouping[newMap.at(make_pair(s, symbol))])
                  {
                     eq = false;
                     break;
                  }
               }
               if (eq)
               {
                  equWithFirst.insert(s);
               }
            }
            for (string e : equWithFirst)
            {
               unused.erase(e);
            }
            groupQ.push(equWithFirst);
         }
      }
      numNew = groupQ.size();
   } while (numOld < numNew);

   queue<set<string>> copyQ = groupQ;
   set<string> out;
   string sOut = "";
   set<string> ordering;
   while (!copyQ.empty())
   {
      out = copyQ.front();
      copyQ.pop();
      ordering.insert(*(out.begin()));
   }
   for (auto &a : ordering)
   {
      sOut += a + ",";
   }
   sOut = sOut.substr(0, sOut.size() - 1);
   cout << sOut << endl;

   sOut = "";
   for (auto &s : symbols)
   {
      sOut += s + ",";
   }
   sOut = sOut.substr(0, sOut.size() - 1);
   cout << sOut << endl;

   copyQ = groupQ;
   sOut = "";
   out.clear();
   ordering.clear();
   while (!copyQ.empty())
   {
      out = copyQ.front();
      copyQ.pop();
      string st = *(out.begin());
      if (acceptable.count(st))
      {
         ordering.insert(st);
      }
   }
   for (auto &a : ordering)
   {
      sOut += a + ",";
   }
   sOut = sOut.substr(0, sOut.size() - 1);
   cout << sOut << endl;

   copyQ = groupQ;
   sOut = "";
   out.clear();
   while (!copyQ.empty())
   {
      out = copyQ.front();
      copyQ.pop();
      if (out.count(startState))
      {
         cout << *(out.begin()) << endl;
      }
   }

   map<pair<string, string>, string> mapOut;
   for (auto &entry : newMap)
   {
      string s1, s2, s3;
      s2 = entry.first.second;
      copyQ = groupQ;
      out.clear();
      while (!copyQ.empty())
      {
         out = copyQ.front();
         copyQ.pop();
         if (out.count(entry.first.first))
         {
            s1 = *(out.begin());
         }
      }
      copyQ = groupQ;
      out.clear();
      while (!copyQ.empty())
      {
         out = copyQ.front();
         copyQ.pop();
         if (out.count(entry.second))
         {
            s3 = *(out.begin());
         }
      }
      pair<string, string> keyOut(s1, s2);
      mapOut.insert({keyOut, s3});
   }
   for (auto &entry : mapOut)
   {
      cout << entry.first.first << "," << entry.first.second << "->" << entry.second << endl;
   }
}
int main(void)
{
   string firstRow, secondRow, thirdRow, fourthRow, startState;
   //INPUT 1st

   getline(cin, firstRow);
   //cerr << "1-------\n";
   vector<string> states;
   splitByChar(firstRow, ',', states);
   //cout << "stanje: " << states.at(0) << endl;

   //INPUT 2nd
   getline(cin, secondRow);
   vector<string> symbols;
   splitByChar(secondRow, ',', symbols);
   //cout << "simboli: " << symbols.at(0) << endl;

   //INPUT 3rd
   getline(cin, thirdRow);
   vector<string> acceptable;
   if (thirdRow.empty())
   {
      //cout << "hHEHEHHEHEHEH" << endl;
   }
   else
   {
      splitByChar(thirdRow, ',', acceptable);
      //cout << "prihvatljivi " << acceptable.at(0) << endl;
   }

   //INPUT 4th
   getline(cin, startState);
   //cout << "pocetno: " << startState << endl;

   //INPUT transitions
   string line;
   vector<string> splitted;
   vector<string> leftSplitted;
   map<pair<string, string>, string> transitionMap;

   while (getline(cin, line))
   {
      line.erase(line.begin() + line.find('>'));
      splitted.clear();
      leftSplitted.clear();
      splitByChar(line, '-', splitted);
      splitByChar(splitted.at(0), ',', leftSplitted);

      pair<string, string> key(leftSplitted.at(0), leftSplitted.at(1));

      transitionMap.insert({key, splitted.at(1)});
   }
   //END OF INPUT
   set<string> accesibleStates;
   findAccesible(transitionMap, accesibleStates, startState);
   //out << accesibleStates.size() << endl;
   map<pair<string, string>, string> newMap;
   leaveAccesible(transitionMap, accesibleStates, newMap);
   set<string> acceptableSet;
   for (auto &e : acceptable)
   {
      acceptableSet.insert(e);
   }
   minimizeDFA(newMap, accesibleStates, acceptableSet, symbols, startState);
   return 0;
}