/*
 ----------------------------------------------------
  Ejemplo de envio de un fichero por bluetooth en C!  
 ---------------------------------------------------- 
*/
 
int SendFileObexPush(string npath,string nfile)
{
	int counter;
	
	printf(" Enviando %s ...",nfile); 						
	
	if(ObexPush_Send(npath,nfile)==0)
		{		
			while(ObexPush_IsSend())
				{
					if(ObexPush_IsAcceptSend()==0)
						{
							printf("\r Esperando aceptacion: %d segundos.",counter); 					
							delay(1000);
							counter++;
					
							if(counter>30) 
								{
									break;
								}
								
						}
			 		 else
			  	  {							
						  printf("\r Enviando %s: %d%% ",nfile,ObexPush_GetProgress()); 								  						  
					  }
				
					if(kbhit()==1) 
						{
							printf("\n Interrumpido por el usuario.");
							return 1;
						}									
				}
		}
		
  if(ObexPush_GetError()==0)
  	{
  		printf("\r Enviado %s.",nfile); 
  	}
   else
    {
    	if(ObexPush_GetError()==1)
    		{    			
    			printf("\r Peticion de envio no aceptada."); 
    		}
    	 else
    	 	{
    	 		printf("\r Error al enviar fichero: %s",nfile); 
    	 	}
    }
    
  return ObexPush_GetError();  
}


int main()
{
	string namefile;	
	string namefile2;	
	string path;
	int    c; 
	
	Console_Open(15,3,0,1);
	
	GetPathScript(path);
	
	namefile = "xblue";
				
	strcat(path,namefile); 
	strcat(path,".gif"); 
	
	printf(" Iniciando comunicacion...\n"); 						
	
	if(ObexPush_Ini("")!=0) 
		{
			return 0;
		}
		
	while(1)
		{
			sprintf(namefile2,"%s_%03d.gif",namefile,c+1);
			if(SendFileObexPush(path,namefile2)==1)	
				{
					printf("\n");
					break;			
				}
					
			printf("\n");	
			c++;	
		}
	  
  printf(" Terminada la comunicacion.\n"); 						
  
  ObexPush_End();
	
  getchar();
  
  Console_Close();
  
  return 0;
}
