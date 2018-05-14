//------------------------------------------------------------------------------------------
//	GScript.cpp
//	
/**	
// \class GScript
//   
//  GScript Class para ejecución de scripts for Windows32
//
//	@author	 Diego Martinez
//	@version 18/Octubre/2010
*/	
//------------------------------------------------------------------------------------------

#include <GScript.h>

#include <winsock2.h>
#include <windows.h>
#include <iostream> 
#include <fstream> 
#include <GDebug.h>


//------------------- static

 std::map<std::string, int> GSCRIPT::ReferenceCountLocatedMemory;

//-------------------------------------------------------------------
//  ParseException::ParseException
/*  constructor de excepcion
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			3/11/2010
//	@return 			constructor
*/
//-------------------------------------------------------------------
GSCRIPT::ParseException::ParseException(GSCRIPT::PARSE_ERROR e, GSCRIPT * t) 
{
err = e; 

char * bc=0;
Line=1;
char * pos=0;
FUNC * f=0;
if (t->CallStack.size()>0)
{
f=t->CallStack.back();
long long desp=(t->PC-f->Body);
pos=(f->FunctionStart+desp);
}
else
pos=t->PC;

	for (bc=t->Program;bc!=pos;bc++)
	{
		if (*bc=='\n') 
			{
				Line++;
				Column=-1;
			}
		Column++;
	};

	if (f!=0)
	{
	sprintf(Text,"Function: %s \r\n%s (%d,%d) : ",f->name.c_str(),t->file.c_str(),Line,Column); 
	}
	else
	{
	sprintf(Text,"Function: <GLOBAL> \r\n%s (%d,%d) : ",t->file.c_str(),Line,Column);			
	}
	if (t->ErrorCodes.find(e)!=t->ErrorCodes.end())				
	strcat(Text,(t->ErrorCodes[e]).c_str());
	strcat(Text," Token: ");
	strcat(Text,t->token.value);
	strcat(Text,"\r\n");

	//-------- Show Call Stack
	strcat(Text,"\r\nCallStack:\r\n");
	while (!t->CallStack.empty())
	{
		f=t->CallStack.back();
		t->CallStack.pop_back();
		strcat(Text,"\t\t");
		strcat(Text,f->name.c_str());
		strcat(Text,"\r\n");
	}
	strcat(Text,"\r\n");

}

//-------------------------------------------------------------------
//  GSCRIPT::GSCRIPT
/*  constructor por defecto
//	inicializa los comandos
//
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2010
//	@return 			constructor
*/
//-------------------------------------------------------------------
GSCRIPT::GSCRIPT() 
{
	this->InitCommands	();
	this->InitDatatypes	();
	this->InitErrorCodes();
	this->EntryPoint="main";
	BreakSignaled=false;
	this->ScopePush(this->GetGlobalScope());
		
	strcpy(Zero.name,"Zero");
	Zero.v_type=INTEGER;
	strcpy(Zero.value,"0");
}
GSCRIPT::~GSCRIPT()
{
	delete(Program);

	GetGlobalScope()->LocalVariables.clear();
	this->ScopeStack.clear();
	this->FunctionTable.clear();
	this->KeyWords.clear();

	while (!this->ParseStack.empty())
		this->ParseStack.pop();

	this->DataTypes.clear();
}
//-------------------------------------------------------------------
//  GSCRIPT::LoadProgramFromFile
/*  Carga el fichero de disco
//
//	TO DO: usar GFILE para la carga de datos
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/10/2010
//	
//	@param				std::string sfname : nombre del fichero a cargar
//
//	@return 			int : resultado de la carga
*/
//-------------------------------------------------------------------
int GSCRIPT::LoadProgramFromFile(std::string fname) 
{ 
  int i=0; 

  std::ifstream in(fname.c_str(), std::ios::in | std::ios::binary); 
  if(!in)  return 1; 

  long begin,end;

  begin = in.tellg();
  in.seekg (0, std::ios::end);
  end = in.tellg();
  in.seekg (0, std::ios::beg);
  long size=end-begin;

  if (size<=0) return 1;
  
  this->Program=new char[size+1];
  if (this->Program==0) return 1;

  char * p=this->Program;
 
  do { 		*p = in.get(); 		p++; 		i++;   } while(!in.eof()); 
 
  if (*(p-2) == 0x1a) 	*(p-2) = '\0';   else 	*(p-1) = '\0'; 
 
  in.close(); 
 
  this->Program[size]=0;
  file=fname;
  return 0; 
} 
//-------------------------------------------------------------------
//  GSCRIPT::LinkMap
/*  Vincula un mapa de std::strings externo al script
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/10/2010
//	
//	@param				std::string name : nombre de la variable en el script
//						std::map<std::string,std::string>* map: puntero a mapa de strings 
*/
//------------------------------------------------------------------
void	GSCRIPT::LinkMap (std::string name,std::map<std::string,std::string> * map)
{
	this->LinkMaps[name]=map;

	VAR v;
	strcpy(v.name,name.c_str());
	v.v_type=MAP;

	this->GetGlobalScope()->LocalVariables[name]=v;

	for (std::map<std::string,std::string>::iterator it=(*map).begin();
		it!=(*map).end();
		it++)	
	    {
			VAR * p=LookUpMap((char*)name.c_str(),(char*)((*it).first).c_str());
			*p=(char*)((*it).second).c_str();				
		}
}


//-------------------------------------------------------------------
//  GSCRIPT::PreScan
/*  Scanea el script buscando variables globales y funciones
//	Desmonta el script en fragmentos de scope
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/10/2010
//	
//	@param				
//	@return 			int : resultado del scan
*/
//------------------------------------------------------------------
int GSCRIPT::PreScan() 
{
	this->PC=this->Program;

	while (token.token_class!=END)
	{
		GetToken();
		if (token.token_type==DATATYPE)
		{
			//podria ser una definición de una global o una funcion
			this->ParsePush(token);
			GetToken();

			if (token.token_type==IDENTIFIER)
			{
				//hemos encontrado una definicion, pero aún no sabemos si es una función o una variable
				this->ParsePush(token);				
				
				if ((*this->PC)=='(') //function declaration
				{
					//definimos la funcion (para ello, vamos a usar el propio parse stack que contiene 
					//actualmente el	|NOMBRE|
					//					|RETURN|
					DefineFunction(this->PC);
				}
				else
				{
					//definimos la funcion (para ello, vamos a usar el propio parse stack que contiene 
					//actualmente el	|NOMBRE|
					//					|DATATYPE|
					//como aun estamos en PreScan, asignamos como scope el global
					DefineVariable(this->PC,GSCRIPT::GetGlobalScope());
					this->PC=token.PC;
					Atom();
				}

			}
			else
				throw(ParseException(EXPECTED_IDENTIFIER,this));
		}
	}

	return 0;
}

//-------------------------------------------------------------------
//  GSCRIPT::GetToken()
/*  Scanea el siguiente intervalo de token del script usando el PC como iterador
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/10/2010
//	
//	@param				
//	@return 			int : resultado del scan
*/
//------------------------------------------------------------------
int GSCRIPT::GetToken()
{
	
	this->token.token_class=GSCRIPT::UNDEFTOK;
	this->token.token_type=GSCRIPT::UNDEFTT;

	 //--------------------------- caracteres skipables 
		
	while (
		(isspace(*this->PC) || 
		*this->PC=='\t' ||
		*this->PC=='\r' ||
		*this->PC=='\n' 
		)
		&& *this->PC) ++this->PC; //avanzamos el ProgramCounter por cada espacio	

	this->token.PC=this->PC; //apuntador al token (util para retroceder usando el stack o stepback)
	//----------------------------- hemos llegado al final del programa?
	if (*this->PC=='\0')
	{
		*(token.value)='\0';
		token.token_class=END;
		token.token_type=DELIMITER;
		return DELIMITER;
	}
	else
	//----------------------------- Chequeamos si hemos entrado en un scope
	if (IsScopeDelimiter(this->PC))
	{
		*token.value=*this->PC;
		*(token.value+1)='\0';
		token.token_class=BLOCK;
		++this->PC;
		return token.token_type=DELIMITER;
	}
	else
	//----------------------------- Chequeamos si hemos encontrado un numero
	 //es un numero negativo?
			
	if (isdigit(*this->PC))
	{
		int v=0;
		do //<<---- Achtung! TO DO: HEX , OCTAL, BINARY //empezamos por 0 para entrar aqui
		{
			this->token.value[v]=*this->PC;
			++this->PC;
			v++;
		}while (!IsDelimiter(this->PC));

				this->token.value[v]='\0'; //hemos copiado el "numero" al value del token		

				this->token.token_class	=VALUE;
		return	this->token.token_type	=NUMBER;
	}
	else
	//--------------------------- Leemos un comentario
	if (IsComment(this->PC))
	{
		return this->ReadComment().token_type;
	}
	else
	//----------------------------------------------------------------------- Read Identifier of keyword
	if (isalpha(*PC))
	{
		int v=0;
		while (!IsDelimiter(PC))
		{
			token.value[v++]=*PC;
			PC++;
		}
		token.value[v++]='\0';
		token.token_type=ALPHA;

		IdentifyAlphaToken();
		return token.token_type;				
	}
	else
	//------------------------------------------------------------------------ CASTING
	if (*PC=='<')
	{
		ParsePush(token);
		++PC;
		GetToken();	
		if (token.token_type==DATATYPE && *(PC)=='>')
		{
			++PC; //avanzamos el >
			token.token_class=SYNTAX;
			token.PC=ParseTop().PC;
			ParsePop();
			return token.token_type=CASTING;
		}
		this->PC=token.PC;
		ParsePop();
	}
	else
	//----------------------------------------------------------------------- string literal
	if(*PC == '\'') 
	{
		if (*(PC+2)=='\'')
		{
			this->PC++;
			token.value[0]=*this->PC;
			token.value[1]='\0';

			this->PC++;
			this->PC++;
			token.token_type=CONSTANT;
			token.token_class=VALUE;
			return token.token_type;
		}
	}
	else
	if(*PC == '"') 
	{ 
    PC++; 
    int v=0;
	while (*PC!='"')
	{
    if(*PC == '\r' || *PC == 0) 
      throw ParseException(QUOTE_EXPECTED,this); 

	if (*PC == '\\' && *(PC+1)=='"')
	{
	  token.value[v++]='"';
	  PC++;
	}
	else
	token.value[v++]=*PC;
	PC++;
	}
	PC++;

		token.value[v++]='\0';
		return token.token_type=LITERAL;	
  } 
	else
	//----------------------------------------------------------------------- Operators
	if (*PC=='+' && *(PC+1)=='+')
	{
		token.value[0]=*PC; 
		token.value[1]=*(PC+1);
		token.value[2]='\0'; 
		PC++; 	    	
		PC++;
		token.token_class=INC;
	    return (token.token_type = OPERAND); 
	}
	else
	if (*PC=='-' && *(PC+1)=='-')
	{
		token.value[0]=*PC; 
		token.value[1]=*(PC+1);
		token.value[2]='\0'; 
		PC++; 	    	
		PC++;
		token.token_class=DEC;
	    return (token.token_type = OPERAND); 
	}
	else
	if(strchr("+-*/%^", *PC)) 
	{
	switch(*PC)
	{
	    case '+': token.token_class=SUM;break;
	    case '-': token.token_class=SUB;break; 
		case '*': token.token_class=MUL;break;
		case '/': token.token_class=DIV;break;
		case '%': token.token_class=MOD;break;
		case '^': token.token_class=EXP;break;
	}	    
	    token.value[0]=*PC; 

		PC++; 

	    token.value[1] = '\0'; 
	    return (token.token_type = OPERAND); 
	} 
	else
	//--------------------------- logic operators
	if (*(this->PC)=='=' && *(this->PC+1)=='=')
	{
		this->PC++;
		this->PC++;
		token.token_class=EQ;
		return (token.token_type = OPERAND); 
	}
	else
	if (*(this->PC)=='<' && *(this->PC+1)!='=')
	{
		this->PC++;
		token.token_class=LS;
		return (token.token_type = OPERAND); 
	}
	else
	if (*(this->PC)=='>' && *(this->PC+1)!='=')
	{
		this->PC++;
		token.token_class=GT;
		return (token.token_type = OPERAND); 
	}
	else
	if (*(this->PC)=='>' && *(this->PC+1)=='=')
	{
		this->PC++;
		this->PC++;
		token.token_class=GE;
		return (token.token_type = OPERAND); 
	}	
	else
	if (*(this->PC)=='<' && *(this->PC+1)=='=')
	{
		this->PC++;
		this->PC++;
		token.token_class=LE;
		return (token.token_type = OPERAND); 
	}	
	else
	if (*(this->PC)=='!' && *(this->PC+1)=='=')
	{
		this->PC++;
		this->PC++;
		token.token_class=NE;
		return (token.token_type = OPERAND); 
	}	
	else
	//--------------------------- Es un delimitador
	if (*(this->PC)=='=')
	{
		*token.value=*this->PC;
		*(token.value+1)='\0';
		token.token_class=ASSIGNMENT;
		++this->PC;
		return token.token_type=OPERAND;
	}

	if (*(this->PC)=='!')
	{
		*token.value=*this->PC;
		++this->PC;
		token.token_class=NEG;
		return token.token_type=OPERAND;
	}
	//--------------------------- Es un delimitador
	if (IsDelimiter(this->PC))
	{
		*token.value=*this->PC;
		*(token.value+1)='\0';
		token.token_class=SYNTAX;
		++this->PC;
		return token.token_type=DELIMITER;
	}
	


	throw (ParseException(UNRECOGNIBLE_TOKEN,this));

	
return 0;
}
//-------------------------------------------------------------------
//  GSCRIPT::IsScopeDelimiter(char * c)
//	indica si el caracter actual es un delimitador de scope
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/10/2010
//	
//	@param				char c: caracter a chequear
//	@return 			int : true si es cierto
//------------------------------------------------------------------
int GSCRIPT::IsScopeDelimiter(char * c)
{
	return strchr("{}",*c)!=NULL;
}
//-------------------------------------------------------------------
//  GSCRIPT::IsDelimiter(char * c)
//	indica si el caracter actual es un delimitador 
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/10/2010
//	
//	@param				char c: caracter a chequear
//	@return 			int : true si es cierto
//------------------------------------------------------------------
int GSCRIPT::IsDelimiter(char * c)
{
	 if(strchr(" !:;,+-<>'/*%^=()[]", *c) || *c == 9 || *c == '\r' || *c == 0) 
	 return true;

	 return false;
}
//-------------------------------------------------------------------
//  GSCRIPT::IsScopeDelimiter(char * c)
//	indica si el caracter actual es un delimitador de comentario
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/10/2010
//	
//	@param				char c: caracter a chequear
//	@return 			int : true si es cierto
//------------------------------------------------------------------
int GSCRIPT::IsComment(char * c)
{
	if (*c=='/' && *(c+1)=='/') return true;
	if (*c=='/' && *(c+1)=='*') return true;
	return false;
}
//-------------------------------------------------------------------
//  GSCRIPT::IdentifyAlphaToken(char * c)
//	indica si el token es un identificador alpha conocido
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			27/10/2010
//	
//	@return 			int : false si no es reconocido
//------------------------------------------------------------------
int GSCRIPT::IdentifyAlphaToken()
{
	//-------------------------------------------- verificamos comandos especificos
	if  (LookUpCommand(this->token.value))
	{
		token.token_type=KEYWORD;
		CKEYWORD k=this->KeyWords[this->token.value];		
		token.token_class=k.tok.token_class;
		return true;
	}

	//-------------------------------------------- verificamos comandos especificos	
	if  (LookUpDatatype(this->token.value))
	{
		token.token_type=DATATYPE;
		token.token_class=UNDEFTOK;
		return true;
	}

	//-------------------------------------------- verificamos comandos especificos	
	if  (strcmp(this->token.value,"false")==0 || strcmp(this->token.value,"true")==0)
	{
		token.token_type=BOOLEAN;
		token.token_class=VALUE;
		return true;
	}
	//-------------------------------------------- Si no, es un identificador
		token.token_type=IDENTIFIER;
		token.token_class=UNDEFTOK;

	return false;
}



//-------------------------------------------------------------------
//  GSCRIPT::ReadComment(char * c)
//	indica si el caracter actual es un delimitador de scope
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/10/2010
//	
//	@param				char c: caracter a chequear
//	@return 			int : true si es cierto
//------------------------------------------------------------------
GSCRIPT::TOKEN GSCRIPT::ReadComment()
{
		//comentario de linea
		if (*(this->PC+1)=='/') 
		{
			int v=0;
			
			do
			{
				this->token.value[v++]=*this->PC;
				++this->PC;
			}
			while (*this->PC!='\0' && *this->PC!='\n');

			this->token.value[v+1]='\0';
					this->token.token_class=COMMENT;
					this->token.token_type=DELIMITER;
			return this->token;
		}

		//comentario de bloque
		if (*(this->PC+1)=='*') 
		{
			int v=0;
			do
			{
				this->token.value[v++]=*this->PC;
				++this->PC;
			}
			while (*this->PC!='\0' && !(*this->PC=='*' && *(this->PC+1)=='/'));

			if (*this->PC=='*') ++(++this->PC);

				this->token.value[v++]='\0';
				this->token.token_class=COMMENT;
				this->token.token_type=DELIMITER;

			return this->token;
		}

		token.token_type=IDENTIFIER;
		token.token_class=UNDEFTOK;
		return this->token;
}
//-------------------------------------------------------------------
//  GSCRIPT::LookUpDatatype()
//	busca el tipo de dato en la lista de tipos de dato
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/10/2010
//	
//	@return 			true si lo encuentra, false si no es reconocido

//------------------------------------------------------------------
int	GSCRIPT::LookUpDatatype(char *d)
{
	if (this->DataTypes.find(d)==this->DataTypes.end())
		return false;

	return true;
}
//-------------------------------------------------------------------
//  GSCRIPT::LookUpCommand()
//	busca el tipo de dato en la lista de tipos de dato
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/10/2010
//	
//	@return 			true si lo encuentra, false si no es reconocido

//------------------------------------------------------------------
int	GSCRIPT::LookUpCommand(char *d)
{
	if (this->KeyWords.find(d)==this->KeyWords.end())
		return false;

	return true;
}
//-------------------------------------------------------------------
//  GSCRIPT::InitKeywords()
//	inicializa el mapa de palabras claves
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/10/2010
//	
//	@return 			0
//------------------------------------------------------------------
int GSCRIPT::InitCommands()
{
	this->KeyWords["if"]	=CKEYWORD("if",		IF,		0);
	this->KeyWords["else"]	=CKEYWORD("else",	ELSE,	0);
	this->KeyWords["for"]	=CKEYWORD("for",	FOR,	0);
	this->KeyWords["do"]	=CKEYWORD("do",		DO,		0);
	this->KeyWords["while"]	=CKEYWORD("while",	WHILE,	0);
	this->KeyWords["return"]=CKEYWORD("return",	RETURN,	0);
	this->KeyWords["switch"]=CKEYWORD("switch",	SWITCH,	0);
	this->KeyWords["break"]	=CKEYWORD("break",	BREAK,	0);
	this->KeyWords["case"]	=CKEYWORD("case",	CASE,	0);
	this->KeyWords["default"]=CKEYWORD("default",DEFAULT,0);

	return 0;
}

//-------------------------------------------------------------------
//  GSCRIPT::InitDatatypes()
//	inicializa el mapa de tipos de dato
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2010
//	
//	@return 			0
//------------------------------------------------------------------
int GSCRIPT::InitDatatypes()
{
	this->DataTypes["void"]		=NONE;
	this->DataTypes["int"]		=INTEGER;
	this->DataTypes["integer"]	=INTEGER;
	this->DataTypes["number"]	=INTEGER;
	this->DataTypes["long"]		=INTEGER;

	this->DataTypes["char"]		=CHAR;
	this->DataTypes["byte"]		=CHAR;
	this->DataTypes["string"]	=STRING;
	this->DataTypes["buffer"]	=STRING;
	this->DataTypes["str"]		=STRING;
	this->DataTypes["alpha"]	=STRING;

	this->DataTypes["float"]	=FLOAT;
	this->DataTypes["double"]	=FLOAT;

	this->DataTypes["object"]	=OBJECT;
	this->DataTypes["obj"]		=OBJECT;

	this->DataTypes["proc"]		=PROCEDURE;
	this->DataTypes["procedure"]=PROCEDURE;

	this->DataTypes["map"]		 =MAP;
	this->DataTypes["dictionary"]=MAP;
	this->DataTypes["vector"]	 =MAP;
	this->DataTypes["array"]	 =MAP;

	this->DataTypes["pointer"]	 =MEMORY;

	this->DataTypes["bool"]		=BOOL;
	this->DataTypes["BOOL"]		=BOOL;

	
	return 0;
}

//-------------------------------------------------------------------
//  GSCRIPT::InitErrorCodes()
//	inicializa el mapa de errores
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2010
//	
//	@return 			0
//------------------------------------------------------------------
int GSCRIPT::InitErrorCodes()
{	

	this->ErrorCodes[SYNTAX_ERROR]				="Error de Sintaxis";
	this->ErrorCodes[NO_EXPRESSION]				="No se ha encontrado una expresión";
	this->ErrorCodes[EXPECTED_IDENTIFIER]		="Se esperaba un Identificador";
	this->ErrorCodes[NOT_A_VARIABLE]			="Identificador desconocido";
	this->ErrorCodes[DUPLICATE_VARIABLE]		="Nombre de Variable Duplicada";
	this->ErrorCodes[DUPLICATE_FUNCTION]		="Nombre de Función Duplicada"; 
    this->ErrorCodes[SEMICOLON_EXPECTED]		="Se esperaba un ';' "; 
	this->ErrorCodes[UNBALANCED_BRACES]			="LLaves no emparejadas "; 
    this->ErrorCodes[FUNCTION_UNDEFINED]		="Funcion desconocida"; 
    this->ErrorCodes[TYPE_EXPECTED]				="Se esperaba un especificador de tipo de dato"; 
    this->ErrorCodes[INVALID_RET]				="La función necesita un valor de retorno"; 
    this->ErrorCodes[PARENTHESIS_EXPECTED]		="Se esperaba un parentesis"; 
    this->ErrorCodes[WHILE_EXPECTED]			="Se esperaba el While correspondiente al par \"do-while\""; 
    this->ErrorCodes[QUOTE_EXPECTED]			="Se esperaba unas Comillas al final del literal (\")"; 
    this->ErrorCodes[DIVISION_BY_ZERO]			="División por Cero"; 
    this->ErrorCodes[BRACE_EXPECTED]			="Se esperaba una llave de inicio de bloque de control"; 
    this->ErrorCodes[COLON_EXPECTED]			="Se esperaba una coma";
	this->ErrorCodes[INCOMPATIBLE_TYPES]		="Tipos de datos incompatibles para una operacion aritmetica";
	this->ErrorCodes[UNFLUSHABLE_TYPE]			="No hay ningun tipo de operador para resolver un << del tipo especificado ";
	this->ErrorCodes[SHIFT_EXPECTED]			="Se esperaba un operador de flujo";
	this->ErrorCodes[SINGLE_QUOTE_EXPECTED]		="Se esperaba una comilla simple tras la constante (')";
	this->ErrorCodes[POLAND_REVERSE_ERROR]		="Se ha encontrado un error en la expresion Arithmetica al convertirla a Polaca Inversa";
	this->ErrorCodes[NO_ENTRYPOINT]				="No se ha encontrado el punto de entrada en el script";
	this->ErrorCodes[MAP_SUBVALUE_ERROR]		="Se esta usando un Mapa de valores sin usar el indicador de subvalor [...]";
	this->ErrorCodes[NOT_A_MAP]					="Se esta usando un subvalor [...] en un tipo que no es un mapa";	
	this->ErrorCodes[INCORRECT_ARGUMENTS]		="Numero de Argumentos incorrectos al realizar la llamada a la función";	
	this->ErrorCodes[UNABLE_TO_LOCATE_MEMORY]	="El interprete no tiene memoria suficiente para interpretar el script";
	this->ErrorCodes[UNRECOGNIBLE_TOKEN]		="Encontrado token no reconocible";
	this->ErrorCodes[UNRELATED_BREAK]			="Break no relacionado";
	this->ErrorCodes[NO_MEMORY]					="El interprete no tiene suficiente memoria para continuar";
	
	return 0;
}

//-------------------------------------------------------------------
//  GSCRIPT::DefineVariable()
/*  Empleando el ParseStack y la actual posición de PC
//	Define una variable en el actual SCOPE
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2010
//	
//	@param				char * PC	: Actual apuntador de scope
						SCOPE * sc	: Actual scope al que añadir la variable

//	@return 			int : resultado del scan
*/
//------------------------------------------------------------------
int GSCRIPT::DefineVariable(char * PC, GSCRIPT::SCOPE * scope)
{
	VAR Variable;

	TOKEN tok=this->ParseTop();
	this->ParsePop();

	//----------------------- comprobamos si existe la variable en el scope
	if (scope->LocalVariables.find(tok.value)!=scope->LocalVariables.end())
		throw(ParseException(DUPLICATE_VARIABLE,this));
		//ya existe!!! error!

	strcpy(Variable.name,tok.value);
	
	tok=this->ParseTop();
	this->ParsePop();
	
	Variable.v_type=this->DataTypes[tok.value];

	scope->LocalVariables[Variable.name]=Variable;
	return 0;
}

//-------------------------------------------------------------------
//  GSCRIPT::DefineFuntion()
/*  Empleando el ParseStack y la actual posición de PC
//	Define una Funcion en el actual SCOPE
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2010
//	
//	@param				char * PC	: Actual apuntador de scope
						SCOPE * sc	: Actual scope al que añadir la variable

//	@return 			int : resultado del scan
*/
//------------------------------------------------------------------
int GSCRIPT::DefineFunction(char * Point)
{
	char * FunctionEnd;
	char * ParameterList;

	FUNC * Function=new FUNC();

	
	ParameterList=Point; //guardamos el inicio de los parametros

	SCOPE ParameterListScope;

	//guardamos el token para luego
	this->ParsePush(token);

	GetToken();
	if (token.value[0]!='(')
			throw(ParseException(PARENTHESIS_EXPECTED,this));
	
	VAR p;

	do
	{	
		GetToken();
		if (token.token_type==DATATYPE)
		{
		p.v_type=this->DataTypes[token.value];
		GetToken();

			if (token.token_type==IDENTIFIER)
			{
			strcpy(p.name,token.value);			
			Function->Arguments.push_back(p);
			GetToken();
			if (token.value[0]==')') break; //fin de parametros
			}
		}
		else
			if (token.token_type!=DELIMITER)
				throw(ParseException(SYNTAX_ERROR,this));
	}
	while (token.value[0]==',');

	//si no hemos terminado en ) mal!
	if ((*token.value)!=')')
		throw(ParseException(PARENTHESIS_EXPECTED,this));
	

	//-------------------------- BUSCAMOS EL CUERPO DE LA FUNCION
		


	GetToken(); //cogemos lo que deberia ser un '{'

	if (token.value[0]!='{')
		throw(ParseException(BRACE_EXPECTED,this)); //no hemos encontrado el {

	Function->FunctionStart=this->PC;

	int Brackets=1;
	int Size=0;
	while (true)
	{	
		++this->PC;
		++Size;

		if ((*this->PC)=='{')
			Brackets++;

		if ((*this->PC)=='}')
			Brackets--;
		

		if (Brackets==0)
			break;

		if (*this->PC=='/0' || *this->PC==0)
			throw(ParseException(UNBALANCED_BRACES,this)); //unbalanced brackets
	}

	++this->PC;
	FunctionEnd=this->PC;
	//-----------------------------------------------------------

	//ahora tenemos todas las variables de definición en ParameterListScope.
	TOKEN f;

	//sacamos el nombre de la funcion del parsestack
	this->ParsePop();
	f=this->ParseTop();

	Function->name=f.value;

if (this->FunctionTable.find(Function->name)!=this->FunctionTable.end()) //la funcion ya existe
			throw (ParseException(DUPLICATE_FUNCTION,this));


	//sacamos el return type de la funcion del parsestack
	this->ParsePop();
	f=this->ParseTop();

	//copiamos el punto de entrada PC de la funcion en LOC
	Function->Loc=f.PC;



	Function->Ret=this->DataTypes[f.value]; //tipo de retorno

	//ponemos el PC en el final de la función
	Function->Body=new char[Size+1];

	if (Function->Body==NULL)
		throw ParseException(UNABLE_TO_LOCATE_MEMORY,this);

	memcpy(Function->Body,Function->FunctionStart,Size);//hacemos una copia del body

	Function->Body[Size]='\0';

	Function->Scope=new SCOPE();

	this->PC=FunctionEnd;

		this->FunctionTable[Function->name]=Function; //tenemos la funcion lista
	
	
	return 0;
}

//-------------------------------------------------------------------
//  GSCRIPT::Execute()
/*  Ejecuta el script entrando en el entrypoint
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/11/2010
//	
//	@return 			VAR  : resultado del script
*/
//------------------------------------------------------------------
GSCRIPT::VAR GSCRIPT::Execute()
{
	while (!this->ParseStack.empty())
		this->ParsePop();

	this->CallStack.clear();
	this->ScopeStack.clear();

	this->ScopePush(this->GetGlobalScope());

	if (this->FunctionTable.find(this->EntryPoint)==this->FunctionTable.end())
		throw (ParseException(NO_ENTRYPOINT,this));

	FUNC *f=this->FunctionTable[this->EntryPoint];

	token.token_class=UNDEFTOK;
	token.token_type=UNDEFTT;

	this->PC=this->Program;

	GDebug("Executing: "+f->name+"\r\n");
	this->CallStack.push_back(f);
//		VAR Return=ExecuteFunction(f);					//el execute se encarga de pushear el scope y popearlo al final	
	this->CallStack.pop_back();

	ProceedPersistentData();

	return RetRegister;
	//return	ExecuteFunction(f);
		
}
//-------------------------------------------------------------------
//  GSCRIPT::CallFunction()
/*  Ejecuta la función
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/11/2010
//	
//	@return 			VAR  : resultado de la funcion
*/
//------------------------------------------------------------------
GSCRIPT::VAR GSCRIPT::CallFunction(std::string name)
{
	GDebug("Calling Function:"+name);
	
	if (this->FunctionTable.find(name)==this->FunctionTable.end())
		throw (ParseException(FUNCTION_UNDEFINED,this));

	FUNC *f=this->FunctionTable[name];

	char * Prev=0;
	Prev=this->PC;

	//---------------------- Get Arguments
	int nArguments=0;

	GetToken();
	if (*token.value!='(') //error
			throw (ParseException(PARENTHESIS_EXPECTED,this));

	GetToken();
	if (*token.value!=')') // tiene parametros	
	{
		this->ScopePush(f->Scope);
		this->PC=token.PC;
		do
		{			
			VAR v=EvaluateExpression(); 
			this->LocalVariablesStack.push_back(v);
			GetToken();

			nArguments++;
		}
		while (*token.value==',');
		this->ScopePop();
	}		
	
	if (*token.value!=')') //debe haber terminado en parenthesis
		throw (ParseException(PARENTHESIS_EXPECTED,this));

	if (nArguments!=f->Arguments.size())
		throw (ParseException(INCORRECT_ARGUMENTS,this));

	Prev=this->PC;
	//---------------------- Metemos los argumentos a la scope de la funcion

	f->Scope->LocalVariables.clear();

	for (int e=0;e<nArguments;e++)
	{
		VAR * v=&(LocalVariablesStack[e]);
		strcpy(v->name,f->Arguments[e].name);
		f->Scope->LocalVariables[v->name]=*v;
	}

	LocalVariablesStack.clear();
	//---------------------- Llamamos a la función con los argumentos en la pila


	this->CallStack.push_back(f);
		VAR Return=ExecuteFunction(f);					//el execute se encarga de pushear el scope y popearlo al final	
	this->CallStack.pop_back();
	this->PC=Prev;

	GDebug("RET from "+name);
	return Return;
}


//-------------------------------------------------------------------
//  GSCRIPT::ExecuteFunction()
/*  Ejecuta la función
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/11/2010
//	
//	@return 			VAR  : resultado de la funcion
*/
//------------------------------------------------------------------
GSCRIPT::VAR GSCRIPT::ExecuteFunction(FUNC *f)
{
	GDebug("\tExecuting: "+f->name);

	char * Previous=this->PC;

	this->PC=f->Body;

	this->ScopePush(f->Scope);

	RetRegister.v_type=NONE;
	strcpy(RetRegister.value,"");

	ExecuteBlock();

	token.token_type=UNDEFTT;
	token.token_class=UNDEFTOK;

	this->ScopePop();

	GDebug("\tReturning: "+f->name+" ("+this->RetRegister.value+")");
	return RetRegister;
}

//-------------------------------------------------------------------
//  GSCRIPT::ExecuteBlock()
/*  Ejecuta el bloque
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/11/2010
//	
//	@return 			VAR  : resultado de la funcion
*/
//------------------------------------------------------------------
void GSCRIPT::ExecuteBlock()
{
	FUNC * f=this->CallStack.back();

	bool SingleLine=true;

	while (true) 
	{
		if (token.token_class==END) 		
		{
			if (RetRegister.v_type!=f->Ret)
				throw(ParseException(INVALID_RET,this));
			break;//terminado de interpretar la funcion
		}	
		GetToken();

		this->ParsePush(token);
//------------------------------------------------------------------------- Scopes!
		if (token.token_class==BLOCK)
		{
			if (token.value[0]=='{')
			{
			SCOPE * Scope=new SCOPE();
			this->ScopePush(Scope);
			SingleLine=false;
			}
			else
			if (token.value[0]=='}')
			{
				if (this->GetScopeLevel()<2)
					throw(ParseException(UNBALANCED_BRACES,this));
				delete(this->ScopeTop());
				this->ScopePop();
				this->ParsePop();
				break;
			}
		}
		else
//------------------------------------------------------------------------- Definiciones
		if (token.token_type==DATATYPE)
		{
			//podria ser una definición de variable
			this->ParsePush(token);
			GetToken();

			if (token.token_type==IDENTIFIER)
			{
				//hemos encontrado una definicion, pero aún no sabemos si es una función o una variable
				this->ParsePush(token);				
								
					//definimos la funcion (para ello, vamos a usar el propio parse stack que contiene 
					//actualmente el	|NOMBRE		|
					//					|DATATYPE	|

				DefineVariable(this->PC,this->GetScope());	
				this->PC=token.PC; //devolvemos el identificador para que se procese su expression y poder assignar en la declaracion
			}
			else
				throw(ParseException(EXPECTED_IDENTIFIER,this));	
		}
		else
//---------------------------------------------------------------------------- Expresiones
		if (token.token_type==IDENTIFIER) //hemos encontrado un identificador
		{		
				SCOPE * test=this->GetScope();
				this->GetScope()->PolishStack.clear();
				this->PC=this->token.PC; //back
				ParsePolish();
		}
		else
//---------------------------------------------------------------------------- Palabras clave
		if (token.token_type==KEYWORD)
			ProcessKeyword(token.value);

		this->ParsePop();
	}
}
//-------------------------------------------------------------------
//  GSCRIPT::ProcessKeyword()
/*  Procesa la palabra clave
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			8/11/2010
//	
*/
//------------------------------------------------------------------
void GSCRIPT::ProcessKeyword(char * vname)
{	
	CKEYWORD k=this->KeyWords[vname];	

	FUNC * f=0;	
	VAR r;

	char * Increment=0;

	int Branch=1;

	switch(k.tok.token_class)
	{
	case IF:
				f=CallStack.back();			
				r=EvaluateExpression();
				if (!((bool)r))
				{
				SkipBranch();
				GetToken();
				
				if (strcmp(token.value,this->KeyWords["else"].tok.value)==0)							
					ExecuteBlock();
				else
					this->PC=token.PC;
				}
				else				
					ExecuteBlock();
				
				return;
				break;
	case ELSE:
				SkipBranch();
				return;
				break;
	case DO:
				this->ParsePush(token);				
				ExecuteBlock();
				GetToken();
				if (token.token_class!=WHILE)
					throw (ParseException(WHILE_EXPECTED,this));
				r=EvaluateExpression();
				if (((bool)r)==true && !BreakSignaled)
				{
					token=this->ParseTop();
					this->PC=token.PC;
				}
				else
				{
					BreakSignaled=false;
					this->ParsePop();
				}
				return;
	case FOR:
				GetToken();
				if (token.value[0]!='(')
					throw (ParseException(PARENTHESIS_EXPECTED,this));
						//---------------- Inicializador
				EvaluateExpression();

				if (token.value[0]!=';') 
					throw (ParseException(SEMICOLON_EXPECTED,this));

				while (true)
				{
				GetToken();
						//---------------- condicional
				this->ParsePush(token);
				r=EvaluateExpression();

				if (token.value[0]!=';') 
					throw (ParseException(SEMICOLON_EXPECTED,this));
		
				GetToken();				
						//---------------- increment
				Increment=this->PC;
				
					while (true)
					{
					if (*this->PC=='\0') 
						throw (ParseException(PARENTHESIS_EXPECTED,this));

					if (*this->PC=='\n') 
						throw (ParseException(PARENTHESIS_EXPECTED,this));

					if (*this->PC==')')
						break;

					this->PC++;
					}

				if (((bool)r)!=true || BreakSignaled)
				{
					BreakSignaled=false;
					break;
				}
				
				ExecuteBlock();
				this->PC=Increment;

							//----------------- Incremento post iteración
				EvaluateExpression();

				token=this->ParseTop();
				this->PC=token.PC;
				this->ParsePop();
				}
				
				SkipBranch();
				this->ParsePop();
				return;
				break;
	case WHILE:
				r=EvaluateExpression();
				if (((bool)r)==true && !BreakSignaled)
				{
				ExecuteBlock();
				token=this->ParseTop();
				this->PC=token.PC;
				this->ParsePop();
				}
				else
				{
				SkipBranch();
				BreakSignaled=false;
				this->ParsePop();
				}
				return;
				break;
	case RETURN:
				f=CallStack.back();			
				r=EvaluateExpression();
				RetRegister.v_type=f->Ret;
				ProceedAssignment(&RetRegister,r); //resolvemos la evaluación			

				token.token_type=DELIMITER;
				token.token_class=END;				
				return;
				break;

	case SWITCH:				
				r=EvaluateExpression();
				GetToken();
				if (token.value[0]=='{')
				{
					SCOPE * sc=new SCOPE();
					this->ScopePush(sc);
				}
				else
					throw ParseException(SYNTAX_ERROR,this);

				for(;;)
				{
					VAR c;
					GetToken();
					if (token.token_class==CASE)
					{
					c=EvaluateExpression();	
					if (c==r)
						break; //seguimos hasta el siguiente case					
					}
					else
					if (token.token_class==DEFAULT)
					{
						this->PC=token.PC;
						break;
					}
					
				}
				return;
	case CASE:	
				r=EvaluateExpression();	
	case DEFAULT:				
				GetToken();
				if (token.value[0]!=':') 
					throw ParseException(SYNTAX_ERROR,this);
				return; //ignoramos los case, ya que así podemos caer en cascada
				break;
	case BREAK:
				BreakSignaled=true;				
	case CONTINUE:		
				SkipBranch(1);
				return;
				break;
	default:
				break;
	}
	//--------------------------- si hemos llegado aqui, no ha reconocido la palabra clave
	throw (ParseException(SYNTAX_ERROR,this));
}
//-------------------------------------------------------------------
//  GSCRIPT::SkipBranch()
/*  Ignora el siguiente scope (usado en iteradores y clausulas)
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2010
//	
*/
//------------------------------------------------------------------
void GSCRIPT::SkipBranch(int Branch)
{	
	while (true)
	{
		GetToken();
		if (token.token_class==END || *this->PC=='\0')
			throw(ParseException(UNBALANCED_BRACES,this));

		if (token.value[0]=='{')
		{
			Branch++;
		}
		else
		if (token.value[0]=='}')
		{
			Branch--;
			if (Branch==0)
				return;
		}
	}
}
//-------------------------------------------------------------------
//  GSCRIPT::LookUpVariable()
/*  Busca la variable en el scope local, y empieza a echar atras el scope hasta encontrarla
//	o excepcion NOT_VAR si no la encuentra
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2010
//	
//	@return 			VAR : resultado del scan
*/
//------------------------------------------------------------------
GSCRIPT::VAR* GSCRIPT::LookUpVariable(char * vname)
{
	SCOPE * sc;

	size_t n=GetScopeLevel();

	for (int e=(int)(n-1);e>=0;e--)
	{
		sc=(this->ScopeStack[e]);
		if (sc->LocalVariables.find(vname)!=sc->LocalVariables.end())
			return &sc->LocalVariables[vname];	
	}

	throw(ParseException(NOT_A_VARIABLE,this));

	return 0;
}
//-------------------------------------------------------------------
//  GSCRIPT::LookUpFunction()
/*  Busca la Funcion
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2010
//	
//	@return 			VAR : resultado del scan
*/
//------------------------------------------------------------------
int GSCRIPT::LookUpFunction(char * vname)
{
	if (this->FunctionTable.find(vname)!=this->FunctionTable.end())
		return true;

	return false;
}
//-------------------------------------------------------------------
//  GSCRIPT::EvaluateExpression()
/*  Evalua la expresión actual hasta encontrar un delimitador
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2010
//	
//	@return 			VAR : resultado del scan
*/
//------------------------------------------------------------------
GSCRIPT::VAR GSCRIPT::EvaluateExpression()
{
	this->GetScope()->PolishStack.clear();
	ParsePolish();
	VAR v=EvaluatePolish();
return v;
}
//-------------------------------------------------------------------
//  GSCRIPT::ParsePolish()
/*  Evalua la expresión para crear el stack de la notación polaca inversa
//	es una funcion recursiva
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2010
//	
//	@post				El PolishStack contiene la expresion evaluable en notación polaca inversa
//	@return 			void
*/
//------------------------------------------------------------------
void GSCRIPT::ParsePolish()
{

while (true)
{
	GetToken();

		if (token.value[0]=='(')
		{
			this->GetScope()->Parenthesis++;
			if (*(this->PC)=='-') //entra un numero negativo, hay que meter un 0
				this->GetScope()->PolishStack.push_back(VAR(0));
			continue;
		}
		if (token.value[0]==')')
		{			
			if (this->GetScope()->Parenthesis>0)
			{
			this->GetScope()->Parenthesis--;
			return;
			}
			else 
				this->PC=token.PC;
				return;
		}

	if (token.token_type==DELIMITER || token.token_class==END)
	{
		if (this->GetScope()->Parenthesis!=0)
			throw(ParseException(PARENTHESIS_EXPECTED,this));

		this->PC=token.PC;
		return;
	}

	if (token.token_type==OPERAND)
	{
		VAR op;
		op.v_type=POLISH_OP;
		strcpy(op.name,token.value);
		op.value[0]=token.token_class;
		op.value[1]=0;
		ParsePolish();
		this->GetScope()->PolishStack.push_back(op);
	}
	else
	this->GetScope()->PolishStack.push_back(Atom());
}
}
//-------------------------------------------------------------------
//  GSCRIPT::EvaluatePolish()
/*  Evalua la expresión actual hasta encontrar un delimitador
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2010
//	
//	@return 			VAR : resultado del scan
*/
//------------------------------------------------------------------
GSCRIPT::VAR GSCRIPT::EvaluatePolish()
{

VAR Resolution;
std::stack<VAR> resolve;	
    
if (GDebug::IsDebug())
{
	GDebug("-------\r\n");
	for (size_t e=0;e<this->GetScope()->PolishStack.size();e++)
	{
		char s[100];
		VAR v=this->GetScope()->PolishStack[e];
		sprintf(s,"Polish[%d]: %s %s\r\n",e,v.name,v.value);
		GDebug(s);
	}
}

	int t=0;

	if (this->GetScope()->PolishStack.size()==0)
		throw ParseException(POLAND_REVERSE_ERROR,this);	

	if (this->GetScope()->PolishStack.back().v_type!=POLISH_OP)
		return this->GetScope()->PolishStack.back();


	resolve.push(Zero);  //para operaciones que incluyan negativos
	for (unsigned int e=0;e<this->GetScope()->PolishStack.size();e++)
	{		
		VAR current=this->GetScope()->PolishStack[e];

		switch(current.v_type)
		{
		case STRING:
		case FLOAT:
		case BOOL:							
		case INTEGER: 
					resolve.push(current);
					break;
		case POLISH_OP:
						switch(current.value[0])
						{
						case INC: 
						case DEC:
						case NEG:
						case SUB:
								break;
						default:
						if ((resolve.size()-1)<2) 
								throw ParseException(POLAND_REVERSE_ERROR,this);
						}

						Resolution=resolve.top();
						resolve.pop();

						switch(current.value[0])
						{
						case LS:  
							    Resolution=(resolve.top() < Resolution); 
								resolve.pop();
								resolve.push(Resolution);break;						
						case LE:
							    Resolution=(resolve.top() <= Resolution); 
								resolve.pop();
								resolve.push(Resolution);break;	
						case GT:
								Resolution=(resolve.top() > Resolution); 
								resolve.pop();
								resolve.push(Resolution);break;	
						case GE:
 							    Resolution=( resolve.top() >= Resolution); 
								resolve.pop();
								resolve.push(Resolution);break;	
						case EQ:
							    Resolution=(Resolution == resolve.top()); 
								resolve.pop();
								resolve.push(Resolution);break;	
						case NE:
							    Resolution=(Resolution != resolve.top()); 
								resolve.pop();
								resolve.push(Resolution);break;	
								
						case SUM:							    
								Resolution=resolve.top()+Resolution;
								resolve.pop();
								resolve.push(Resolution);break;						
						case SUB:							    
								Resolution=resolve.top()-Resolution;
								resolve.pop();
								resolve.push(Resolution);break;		
						case MUL:							    
								Resolution=resolve.top()*Resolution;
								resolve.pop();
								resolve.push(Resolution);break;		
						case DIV:							   
								if (((int)Resolution)==0)
									throw (ParseException(DIVISION_BY_ZERO,this));
								Resolution=resolve.top()/Resolution;
								resolve.pop();
								resolve.push(Resolution);break;		
						case NEG:
								Resolution=!((bool)(resolve.top()));
								resolve.push(Resolution);break;	
						default:
								throw ParseException(POLAND_REVERSE_ERROR,this);
							break;
						}
		}
	}
	
	return Resolution;
}

//-------------------------------------------------------------------
//  GSCRIPT::LookUpMap()
/*  busca en las variables un mapa con el nombre name y devuelve su subvalor sub
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2010
//	
//	@return 			VAR : resultado del scan
*/
//------------------------------------------------------------------
GSCRIPT::VAR* GSCRIPT::LookUpMap(char* name,char * sub)
{
	VAR*v;

	std::map<std::string,VAR*> * pmap=0;
	void* l=0;

	if (name[0]!='@')//no es indirect
	{
	VAR * m=LookUpVariable(name);

	if (m->value[0]!='@')
	{
		pmap=new std::map<std::string,VAR*>();

		if (pmap==0)
			throw ParseException(NO_MEMORY,this);

		sprintf(m->value,"@%p",pmap);
		this->ReferenceCountLocatedMemory[m->value]=1;
	}
	 sscanf(m->value,"@%p",&l);
	}
	else
		sscanf(name,"@%p",&l);

		pmap=(std::map<std::string,VAR*> *)l;
	
		if (pmap->find(std::string(sub))==pmap->end())
			(*pmap)[std::string(sub)]=new VAR("0",NONE);

	v=((*pmap)[std::string(sub)]);
	sprintf(v->name,sub);
	return v;
}

//-------------------------------------------------------------------
//  GSCRIPT::Atom()
/*  devuelve el valor atomico de la expresion
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2010
//	
//	@return 			VAR : resultado del scan
*/
//------------------------------------------------------------------
GSCRIPT::VAR GSCRIPT::Atom()
{
VAR v;

SCOPE s;

		if (token.value[0]==';') //expresion vacia
			throw ParseException(NO_EXPRESSION,this);
		else
			if (token.value[0]=='(') //group of expression
			{
				//this->PC=token.PC;
				SCOPE * scope=new SCOPE();
				ScopePush(scope);
				ParsePolish();
				v=EvaluatePolish();
				ScopePop();
				delete(scope);
				GetToken();
				if (token.value[0]!=')') 
					throw ParseException(PARENTHESIS_EXPECTED,this);
				return v;
			}
		else
		if (token.token_type==CONSTANT)
		{
			v.v_type=CHAR;
			strcpy(v.value,token.value);
		}
		else
		if (token.token_type==BOOLEAN)
		{
			v.v_type=BOOL;
			strcpy(v.value,token.value);
		}
		else
		if (token.token_type==NUMBER)
		{
			v.v_type=INTEGER;
			if (strchr(token.value,'.')>0) //es float
				v.v_type=FLOAT;
			strcpy(v.value,token.value);
		}
		else
		if (token.token_type==LITERAL)
		{
			v.v_type=STRING;
			strcpy(v.value,token.value);
		}
		else
		if (token.token_type==CASTING)
		{
			char typ[25];
			strcpy(typ,token.value);
			if (!this->LookUpDatatype(typ))
				throw ParseException(TYPE_EXPECTED,this);			
			GetToken();
			VAR v=Atom();
			v.v_type=this->DataTypes[typ];
			return v;
		}
		else
		if (token.token_type==IDENTIFIER)
		{
			if (LookUpFunction	(token.value))
				return v=CallFunction(token.value);					
			
			VAR *var=LookUpVariable(token.value);
			char * s=var->name;	
								
			GetToken();
			if (token.value[0]=='[')
			do
			{			
				if (var->v_type!=MAP)
				throw ParseException(NOT_A_MAP,this);

				GetToken();
				std::string st(s);

				VAR a=Atom();

				if (var->value[0]!='@')
				var=LookUpMap(var->name,a.value);
				else
				var=LookUpMap(var->value,a.value);

				GetToken(); //"]"

				if (token.value[0]!=']') 
					throw ParseException(NOT_A_MAP,this);

				GetToken(); 
			}
			while(token.value[0]=='[');
			
			if (token.token_class==INC) 
			{
				ProceedAssignment(var,++*var);
				return *LookUpVariable(var->name);
			}
			else
			if (token.token_class==DEC) 
			{
				ProceedAssignment(var,--*var);
				return *LookUpVariable(var->name);
			}	
			else
			if (token.token_class==ASSIGNMENT) //es una assignación
			{				
				ParseStack.push(token);
				ParsePolish();
				v=EvaluatePolish();
				ParseStack.pop();				
				ProceedAssignment(var,v);
				return v;
			}
			else
			{				
				this->PC=token.PC;
				v=*var;
				return v;
			}
		}
		else
			throw ParseException(EXPECTED_IDENTIFIER,this);

return v;
}
//-------------------------------------------------------------------
//  GSCRIPT::ProceedAssignment()
/*  Asigna un valor a una variable
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			8/11/2010
//	
//	@param				char * name : nombre de la variable
						VAR v		: valor
*/
//------------------------------------------------------------------
void GSCRIPT::ProceedAssignment(char *vname,VAR v)
{
	SCOPE * sc;

	size_t n=GetScopeLevel();

	for (int e=(int)(n-1);e>=0;e--)
	{
		sc=(this->ScopeStack[e]);
		if (sc->LocalVariables.find(vname)!=sc->LocalVariables.end())
		{
			VAR x=sc->LocalVariables[vname];
			
			if (sc->LocalVariables[vname].v_type==NONE)
				sc->LocalVariables[vname].v_type=v.v_type;

			if (sc->LocalVariables[vname].v_type!=v.v_type)
				throw (ParseException(INCOMPATIBLE_TYPES,this));

			if (v.value[0]=='@')
				GSCRIPT::ReferenceCountLocatedMemory[v.value]=GSCRIPT::ReferenceCountLocatedMemory[v.value]+1;

			strcpy(sc->LocalVariables[vname].value,v.value);	
			return;
		}
	}

	throw(ParseException(NOT_A_VARIABLE,this));
	
}

//-------------------------------------------------------------------
//  GSCRIPT::ProceedAssignment()
/*  Asigna un valor a una variable
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			8/11/2010
//	
//	@param				VAR a		: variable receptora
						VAR v		: valor
*/
//------------------------------------------------------------------
void GSCRIPT::ProceedAssignment(VAR *a,VAR v)
{
	if (a->v_type==NONE)
			a->v_type=v.v_type;

	if (a->v_type!=v.v_type)
			throw (ParseException(INCOMPATIBLE_TYPES,this));	

	if (v.value[0]=='@')
			GSCRIPT::ReferenceCountLocatedMemory[v.value]=GSCRIPT::ReferenceCountLocatedMemory[v.value]+1;
	
	strcpy(a->value,v.value);	
}


//-------------------------------------------------------------------
//  GSCRIPT::Show()
/*  Muestra datos de Script
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			1/12/2010
*/
//------------------------------------------------------------------
void GSCRIPT::Show()
{
	GDebug("GlobalVariables:");
	GDebug("\r\n");

	for (std::map	<std::string , VAR		>::iterator it=this->GetGlobalScope()->LocalVariables.begin();
		 it!=this->GetGlobalScope()->LocalVariables.end();
		 ++it)
		GDebug("\t\tVar : "+std::string(((*it).second).name)+"\t\t\tValue:"+std::string(((*it).second).value));
	

	GDebug("GlobalFunctions:");

	for (std::map	<std::string , FUNC	*	>::iterator it=this->FunctionTable.begin();
		 it!=this->FunctionTable.end();
		 ++it)
		GDebug("\t\tFunction : "+(((*it).second)->name));
	
}

//-------------------------------------------------------------------
//  GSCRIPT::ProceedPersistentData()
/*  Ejecuta el script entrando en el entrypoint
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/11/2010
//	
//	@return 			VAR  : resultado del script
*/
//------------------------------------------------------------------
void GSCRIPT::ProceedPersistentData()
{

	for (
		std::map<std::string, std::map < std::string, std::string> * >::iterator it=this->LinkMaps.begin();
		it!=this->LinkMaps.end();
		it++
		)
	{
		std::string name=(*it).first; //tomamos el nombre ficticio del map
		std::map < std::string, std::string> * Container=(*it).second;

		std::map<std::string,VAR*> * pmap=0;

		VAR * m=LookUpVariable((char*)name.c_str());
		if (m->value[0]!='@')		
			throw ParseException(NOT_A_MAP,this);

		sscanf(m->value,"@%p",&pmap);
		//----------- tenemos el mapa en la variable
			
		(*Container).clear(); //limpiamos el original

		for (
			std::map<std::string,VAR*>::iterator sub=(*pmap).begin();
			sub!=(*pmap).end();
			sub++
			)
			{
				(*Container)[((*sub).first)]=((*sub).second)->value;
			}

	}

	//---------------- clear non-persistent function scopes and reference counts
	for (
		std::map<std::string, FUNC *>::iterator it=this->FunctionTable.begin();
		it!=this->FunctionTable.end();
		it++
		)
		{
			delete(((*it).second)->Scope);
			delete((*it).second);
		}
		
/*	for (
		std::map<std::string, int>::iterator it=this->ReferenceCountLocatedMemory.begin();
		it!=this->ReferenceCountLocatedMemory.end();
		it++
		)
		{			
			std::map<std::string,VAR*> * pmap=0;
			if (sscanf((*it).first.c_str(),"@%p",&pmap)==1)
			{
				pmap->clear();

			}
		}
	for (
		std::map<std::string, int>::iterator it=this->ReferenceCountLocatedMemory.begin();
		it!=this->ReferenceCountLocatedMemory.end();
		it++
		)
		{			
			std::map<std::string,VAR*> * pmap=0;
			if (sscanf((*it).first.c_str(),"@%p",&pmap)==1)
			{
				delete(pmap);
			}
		}

	*/
	this->ReferenceCountLocatedMemory.clear();
	this->LinkMaps.clear();

		
}
