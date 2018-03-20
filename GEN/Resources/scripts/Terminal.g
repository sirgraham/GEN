/*
 ----------------------------------------------------
  Ejemplo conexion SPP ejemplo de terminal bluetooth 
  en C!  
 ---------------------------------------------------- 
*/
 

int main()
{
  string  cadena;
  char	  caracter;
	int   	counter;

	Console_Open(15,3,0,1);
	
	printf(" Iniciando comunicacion...\n"); 						
	
	if(SPP_Ini()!=0) 
		{
			return 0;
		}
		
	counter = 0;		
	
	while(1)
		{
			if(SPP_IsAccept()==0)
				{
					printf("\r Esperando aceptacion: %d segundos.",counter); 					
					delay(1000);
					counter++;													
				}
	 		 else
	  	  {							
					break;					
				}		
		}
		
	printf("\r Conectado...\n"); 							  

	EmptyString(cadena);
	
	do { 
			 caracter=Keyboard_Read(1);
			
			 if(caracter!=0) 
				 {				
					 SPP_Write(caracter);			
				 }
			
			 if(SPP_Read(cadena)==0)
				 {						
					 printf(cadena);
				 }	
							
		 } while(caracter!='X');
			
  printf(" Terminada la comunicacion.\n");
  
  SPP_End();
	
  Console_Close();
  
  return 0;
}
