
string pepe;
string pepe2;
	 
int main() 
{ 	
  pepe  = "hola radiola";
  pepe2 = "fkdlfkd";
    
  printf("result %s \n",prueba(pepe,pepe2));   
 
  // getchar();
  
  return 0;
} 


string prueba(string pp,string data)
{
  string mas;
  
  printf("1) %s\n",pp);    
  printf("2) %s\n",data);    
  
  strcpy(mas,pp);
  strcat(mas,data);
      
  return mas;
}
