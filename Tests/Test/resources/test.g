/* 
*/ 
 
int i, j; // global vars  
char ch; 
string pepe;
string pepe2;
 
int main() 
{ 
  int i, j; // local vars  
  
  pepe = "hola radiola";
  
  pepe2 = Prueba("Esto es una Prueba");
  
  StrCat(pepe2," + mas");
  
  Printf("cadena: %s", pepe2);

  // Call a "standard library' function. 
 	Printf("Mini C++ Demo Program.\n\n"); 
  
 
  // Call a programmer-defined function. 
  Print_Alpha(); 
 
  Printf("\n"); 
 
  // Demonstrate do and for loops. 
  Printf("Use loops.\n"); 
  do { 
    		Printf("Enter a number (0 to quit): "); 
    		//cin >> i; 
 
    // Demonstrate the if 
    if(i < 0 ) { 
      Printf("Numbers must be positive, try again.\n"); 
    } 
    else { 
      for(j = 0; j <= i; ++j) { 
        Printf("%d summed is %d \n", j , Sum(j)); 
      } 
    } 
  } while(i != 0); 
 
  Printf("\n"); 
 
  // Demonstrate the break in a loop. 
  Printf("Break from a loop.\n"); 
  for(i=0; i < 100; i++) { 
    Printf("%d\n", i); 
    if(i == 5) { 
      Printf("Breaking out of loop.\n"); 
      break; 
    } 
  } 
 
  Printf("\n"); 
 
  // Demonstrate the switch 
  Printf("Use a switch.\n"); 
  for(i=0; i < 6; i++) { 
    switch(i) { 
      case 1: // can stack cases 
      case 0: 
        Printf("1 or 0\n"); 
        break; 
      case 2: 
        Printf("two\n"); 
        break; 
      case 3: 
        Printf("three\n"); 
        break; 
      case 4: 
        Printf("four\n"); 
        Printf("4 * 4 is %d\n", 4*4); 
//        break; // this break is optional 
      // no case for 5 
    } 
  } 
  Printf("\n"); 
 
  Printf("Use a library function to generate 10 random integers.\n"); 
  for(i=0; i < 10; i++) { 
    Printf("%d ", Rand()); 
  } 
 
  Printf("\n"); 
  Printf("Press a key!\n"); 
  
  GetChar();
  
  Printf("Done!\n"); 
   
  return 0; 
} 
 
// Sum the values between 0 and num. 
// This function takes a parameter. 
int Sum(int num) 
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
int Print_Alpha() 
{ 
  Printf("This is the alphabet:\n"); 
 
  for(ch = 'A'; ch<='Z'; ch++) { 
    PutChar(ch); 
  } 
  
  Printf("\n"); 
 
  return 0; 
}

string Prueba(string pp)
{
	Printf("[%s]", pp);
  Printf("\n\n");  
      
  return pp;
}
