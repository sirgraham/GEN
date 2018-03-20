/* 
*/ 
 
int i, j; // global vars  
char ch; 
string pepe;
string pepe2;
 
int main() 
{ 
  int i, j; // local vars  
  
  Console_Open(15,3,0,1);
  
  pepe = "hola radiola";
  
  pepe2 = prueba("Esto es una Prueba",pepe);
  
  strcat(pepe2," + mas");
  
  cout << "cadena:" << pepe2;

  // Call a "standard library' function. 
  cout << "Mini C++ Demo Program.\n\n"; 
  
 
  // Call a programmer-defined function. 
  print_alpha(); 
 
  cout << "\n"; 
 
  // Demonstrate do and for loops. 
  cout << "Use loops.\n"; 
  do { 
    cout << "Enter a number (0 to quit): "; 
    cin >> i; 
 
    // Demonstrate the if 
    if(i < 0 ) { 
      cout << "Numbers must be positive, try again.\n"; 
    } 
    else { 
      for(j = 0; j <= i; ++j) { 
        cout << j << " summed is "; 
        cout << sum(j) << "\n"; 
      } 
    } 
  } while(i != 0); 
 
  cout << "\n"; 
 
  // Demonstrate the break in a loop. 
  cout << "Break from a loop.\n"; 
  for(i=0; i < 100; i++) { 
    cout << i << "\n"; 
    if(i == 5) { 
      cout << "Breaking out of loop.\n"; 
      break; 
    } 
  } 
 
  cout << "\n"; 
 
  // Demonstrate the switch 
  cout << "Use a switch.\n"; 
  for(i=0; i < 6; i++) { 
    switch(i) { 
      case 1: // can stack cases 
      case 0: 
        cout << "1 or 0\n"; 
        break; 
      case 2: 
        cout << "two\n"; 
        break; 
      case 3: 
        cout << "three\n"; 
        break; 
      case 4: 
        cout << "four\n"; 
        cout << "4 * 4 is "<< 4*4 << "\n"; 
//        break; // this break is optional 
      // no case for 5 
    } 
  } 
  cout << "\n"; 
 
  cout << "Use a library function to generate "  
       << "10 random integers.\n"; 
  for(i=0; i < 10; i++) { 
    cout << rand() << " "; 
  } 
 
  cout << "\n"; 
  cout << "Done!\n"; 
  
  getchar();
  
  Console_Close();
 
  return 0; 
} 
 
// Sum the values between 0 and num. 
// This function takes a parameter. 
int sum(int num) 
{ 
  int running_sum; 
 
  running_sum = 0; 
 
  while(num) { 
    running_sum = running_sum + num; 
    num--; 
  } 
  return running_sum; 
} 
 
// Print the alphabet. 
int print_alpha() 
{ 
  cout << "This is the alphabet:\n"; 
 
  for(ch = 'A'; ch<='Z'; ch++) { 
    putchar(ch); 
  } 
  cout << "\n"; 
 
  return 0; 
}

string prueba(string pp,string data)
{
  cout << pp << "\n\n";  
      
  return pp;
}
