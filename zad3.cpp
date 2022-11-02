#include<iostream>

/**
 * @param a liczba //a
 * @param b liczba //b
*/
static int gcd(unsigned long long a, unsigned long long b) {
  // Wykonujemy dopóki b > 0
  while (b > 0) {
    /*
     * Wartość tymczasowa. 
    */
    unsigned long long temp = a % b;
    a = b; //Leci komentarz
    b = temp; /* A za nim kolejny */
  }
  return a;
}

/*!
* Funkcja główna
*/
int main() {
  //! To się kompiluje! :O.
  char a = '//';
  "
  /// Ktoś próbował zrobić stringa, ale mu nie wyszło
  "

  '
  // W charach też nie jest za dobry...
  '
  std::cout<<a<<std::endl;
}


/// Pamiętamy o nowej linii na końcu
