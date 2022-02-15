#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <tuple>
#include <stack>
#include <algorithm>

using namespace std;
string printStack(stack<string> &s)
{
   stack<string> temp = s;
   string out = "";
   if (s.empty())
   {
      out += "$";
      return out;
   }
   while (!s.empty())
   {
      string w = s.top();
      out += w;
      s.pop();
   }
   s = temp;
   return out;
}
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

string automateNext(string firstState, vector<string> &input, map<tuple<string, string, string>, pair<string, string>> &transitionMap, string startStack, vector<string> &acceptable)
{
   tuple<string, string, string> keyEnd; //using after last input
   string output = "";
   string stateNow = firstState;
   stack<string> Stack;
   Stack.push(startStack);
   output += stateNow + "#" + printStack(Stack) + "|";
   for (auto &sign : input)
   {
      if (Stack.empty())
      {
         goto finishing;
      }
      tuple<string, string, string> keyE(stateNow, "$", Stack.top());
      while (transitionMap.count(keyE))
      {
         pair<string, string> valueE = transitionMap[keyE];
         if (Stack.empty())
         {
            goto finishing;
         }
         else
         {
            Stack.pop();
            stateNow = valueE.first;
            string nextStackE = valueE.second;
            if (nextStackE != "$")
            {
               for (int i = nextStackE.length() - 1; i >= 0; i--)
               {
                  string s = "";
                  s += nextStackE[i];
                  Stack.push(s);
               }
            }
            output += stateNow + "#" + printStack(Stack) + "|";
            get<0>(keyE) = stateNow;
            get<2>(keyE) = Stack.top();
         }
      }
      if (Stack.empty())
      {
         goto finishing;
      }
      tuple<string, string, string> key(stateNow, sign, Stack.top());
      if (transitionMap.count(key) != 0)
      {
         pair<string, string> value = transitionMap[key];
         Stack.pop();
         stateNow = value.first;
         string nextStack = value.second;
         if (nextStack != "$")
         {
            for (int i = nextStack.length() - 1; i >= 0; i--)
            {
               string s = "";
               s += nextStack[i];
               Stack.push(s);
            }
         }
         output += stateNow + "#" + printStack(Stack) + "|";
      }
      else
      {
         goto finishing;
      }
   }
   get<0>(keyEnd) = stateNow;
   get<1>(keyEnd) = "$";
   get<2>(keyEnd) = Stack.top();
   while (count(acceptable.begin(), acceptable.end(), stateNow) != 1)
   {
      if (transitionMap.count((keyEnd)) == 0)
         break;
      else
      {
         pair<string, string> value = transitionMap[keyEnd];
         Stack.pop();
         stateNow = value.first;
         string nextStack = value.second;
         if (nextStack != "$")
         {
            for (int i = nextStack.length() - 1; i >= 0; i--)
            {
               string s = "";
               s += nextStack[i];
               Stack.push(s);
            }
         }
         output += stateNow + "#" + printStack(Stack) + "|";
         get<0>(keyEnd) = stateNow;
         get<2>(keyEnd) = Stack.top();
      }
   }
   if (count(acceptable.begin(), acceptable.end(), stateNow))
   {
      output += "1";
   }
   else
   {
      output += "0";
   }
   return output;

finishing:
   output += "fail|0";
   return output;
}

int main(void)
{
   string firstRow, secondRow, thirdRow, fourthRow, startState, thirdRowB, startStack;
   stack<string> Stack;
   //INPUT 1st

   getline(cin, firstRow);
   //cerr << "1-------\n";
   vector<string> tempSeq;
   splitByChar(firstRow, '|', tempSeq);

   vector<vector<string>> sequences;
   vector<string> tempoary; //between calculations

   for (int i = 0; i < (int)tempSeq.size(); i++)
   {
      tempoary.clear();
      splitByChar(tempSeq[i], ',', tempoary);
      sequences.push_back(tempoary);
   }

   //INPUT 2nd
   getline(cin, secondRow);
   //cerr << "2-------\n";
   vector<string> states;
   splitByChar(secondRow, ',', states);

   //INPUT 3rd
   getline(cin, thirdRow);
   //cerr << "3-------\n";
   vector<string> symbols;
   splitByChar(thirdRow, ',', symbols);

   getline(cin, thirdRowB);
   vector<string> stackElems;
   splitByChar(thirdRowB, ',', stackElems);

   //INPUT 4th
   getline(cin, fourthRow);
   // cerr << "4-------\n";
   vector<string> acceptable;
   splitByChar(fourthRow, ',', acceptable);

   //INPUT 5th
   getline(cin, startState);
   //cerr << "-------";

   getline(cin, startStack);

   //INPUT transitions
   string line;
   vector<string> splitted;
   vector<string> leftSplitted;
   vector<string> rightSplitted;
   map<tuple<string, string, string>, pair<string, string>> transitionMap;

   while (getline(cin, line))
   {
      //cerr << "unio sam prijelaz+++++";
      line.erase(line.begin() + line.find('>'));
      splitted.clear();
      leftSplitted.clear();
      rightSplitted.clear();
      splitByChar(line, '-', splitted); //in first is PAIR in second is list of states n+1
      splitByChar(splitted.at(0), ',', leftSplitted);
      splitByChar(splitted.at(1), ',', rightSplitted);

      tuple<string, string, string> key(leftSplitted.at(0), leftSplitted.at(1), leftSplitted.at(2));
      //cout << leftSplitted.at(0) << " " << leftSplitted.at(1) << "" << leftSplitted.at(2);
      pair<string, string> value(rightSplitted.at(0), rightSplitted.at(1));

      transitionMap.insert({key, value});
   }

   string van = "";
   for (auto &vec : sequences)
   {
      van = automateNext(startState, vec, transitionMap, startStack, acceptable);
      cout << van << endl;
   }
   return 0;
}
