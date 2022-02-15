#include <iostream>

using namespace std;

int indeks = 0;
string ulazniNiz;

bool S();
bool A();
bool B();
bool C();

int main(void)
{
   getline(cin, ulazniNiz);
   int velicinaNiza = ulazniNiz.length();

   bool rez = S();
   if (rez && indeks == velicinaNiza)
   {
      cout << "\nDA\n";
   }
   else
   {
      cout << "\nNE\n";
   }
   return 0;
}

bool S()
{
   cout << "S";
   if (ulazniNiz[indeks] == 'a')
   {
      indeks += 1;
      return A() && B();
   }
   if (ulazniNiz[indeks] == 'b')
   {
      indeks += 1;
      return B() && A();
   }
   return false;
}

bool A()
{
   cout << "A";
   if (ulazniNiz[indeks] == 'b')
   {
      indeks += 1;
      return C();
   }
   if (ulazniNiz[indeks] == 'a')
   {
      indeks += 1;
      return true;
   }
   return false;
}

bool B() // ccSbc
{
   cout << "B";
   if (ulazniNiz[indeks] == 'c')
   {
      indeks += 1;
      if (ulazniNiz[indeks] == 'c')
      {
         indeks += 1;
         if (S())
         {
            if (ulazniNiz[indeks] == 'b')
            {
               indeks += 1;
               if (ulazniNiz[indeks] == 'c')
               {
                  indeks += 1;
                  return true;
               }
            }
         }
      }
      return false;
   }
   return true;
}

bool C()
{
   cout << "C";
   return A() && A();
}