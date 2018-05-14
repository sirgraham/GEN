

int main()
{
	int i;
	char caracter;
	
	i = 0;

	Console_Open(15,3,0,1);
	
	caracter='A';

	do{ printf("%02d\n",i);
	
			 i++;	
	
		 } while(caracter=='A');
	
	getchar();
	
	Console_Close();
	
	return 0;
}