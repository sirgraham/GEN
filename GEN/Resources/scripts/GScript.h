//------------------------------------------------------------------------------------------
//	GScript.H
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
	
#ifndef _G_REGISTER_H__
#define _G_REGISTER_H__
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <string>
#include <GBase.h>
#include <vector>
#include <stack>
#include <map>

#include <gerror.h>

#include <winsock2.h>
#include <windows.h>
#include <math.h>

#include <GDebug.h>

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class GSCRIPT : public GBase
{
public:
//----------------------------------------------------------- APUNTADORES
	//TO DO: desmembrar el codigo en un arbol de bloques de scope
			
	char * Program;
	char * PC;
	std::string file;


	enum PARSE_ERROR 
		{ 
			SYNTAX_ERROR, 
			NO_EXPRESSION, 
			EXPECTED_IDENTIFIER,
			NOT_A_VARIABLE, 
			DUPLICATE_VARIABLE, 
			DUPLICATE_FUNCTION, 
			SEMICOLON_EXPECTED,  
			UNBALANCED_BRACES, 
			FUNCTION_UNDEFINED, 
			TYPE_EXPECTED, 
			INVALID_RET, 
			PARENTHESIS_EXPECTED, 
			WHILE_EXPECTED, 
			QUOTE_EXPECTED, 
			DIVISION_BY_ZERO, 
			BRACE_EXPECTED, 
			COLON_EXPECTED,
			INCOMPATIBLE_TYPES,
			UNFLUSHABLE_TYPE,
			SHIFT_EXPECTED,
			SINGLE_QUOTE_EXPECTED,
			POLAND_REVERSE_ERROR,
			NO_ENTRYPOINT,
			MAP_SUBVALUE_ERROR,
			NOT_A_MAP,
			INCORRECT_ARGUMENTS,
			UNABLE_TO_LOCATE_MEMORY,
			UNRECOGNIBLE_TOKEN,
			NO_MEMORY,
			UNRELATED_BREAK
		};

		std::map<PARSE_ERROR,std::string> ErrorCodes;

		class ParseException 
		{ 
		public:
			// Exception class  
			PARSE_ERROR err; 

			char		Text[200];
			int			Line;
			int			Column;		
			bool		Continuable;

			public: 
				ParseException(PARSE_ERROR e, GSCRIPT * t); 

				PARSE_ERROR get_err() 
				{ 
					return err; 
				} 
		}; 
		
//------------------------------------------------------------ DATATYPES
	enum DATATYPES
	{
			UNDEF,
			NONE,
			CHAR, 
			STRING,
			INTEGER,
			LONG,
			FLOAT,
		DOUBLE,
		OBJECT,
		PROCEDURE,
			POLISH_OP, //for polish reverse operations
		MEMORY,
			BOOL,
		ARRAY,
		MAP,
		FLUX,
		DATA_END
	};

//------------------------------------------------------------ TOKEN TYPES
	enum TOKEN_TYPE
		{ 
		UNDEFTT, 
		DELIMITER, 
		IDENTIFIER, 
        NUMBER,
		BOOLEAN,
		KEYWORD,
		DATATYPE,
		ALPHA, 
		LITERAL, 
		CONSTANT,
		OPERAND,
		SUBSCRIPT,
		CASTING
		};

//------------------------------------------------------------ TOKEN CLASES
	enum TOKEN_CLASS 
	{ 
		UNDEFTOK, 
		ARG, 
		VARIABLE,
		VALUE,
		SWITCH, 
        CASE, 		
		DEFAULT,
		IF, 
		ELSE, 
		FOR, 
		DO, 
		WHILE, 
		BREAK,
		CONTINUE,
        RETURN, 
		STREAM, 
		INCLUDE,
		REQUIRE,
		IMPORT,
		EXTEND,		
		SYNTAX,
		COMMENT,
		BLOCK,
		SUM,
		SUB,
		MUL,
		DIV,
		INC,
		DEC,
		EQ,
		NE,
		GE,
		LE,
		LS,
		GT,
		NEG,
		MOD,
		EXP,
		ASSIGNMENT,
		END 
	};

//------------------------------------------------------------ VARIABLE PROTOTYPE
	class VAR		
	 { 		
	  public:
		char		name[20];
		DATATYPES	v_type;					// data type 
		char		value[100];				// value 
		int			op_deep;

		VAR& operator =(VAR v)
		{
			strncpy (this->name,v.name,20);			
			strncpy (this->value,v.value,100);
			this->v_type=v.v_type;
			return *this;
		};
		VAR& operator +(VAR v)
		{
			if (this->v_type==INTEGER)
			{
				int x=*this;
				x+=(int)(v);
				*this=x;
			}
			else
			if (this->v_type==FLOAT)
			{
				float x=*this;
				x+=(float)(v);	
				*this=x;
			}
			else
			if (this->v_type==STRING)
			{
				strcat(this->value,v.value);
			}
			
			return *this;
		};
		VAR& operator -(VAR v)
		{
			if (this->v_type==INTEGER)
			{
				int x=*this;
				x-=(int)(v);
				*this=x;
			}
			else
			if (this->v_type==FLOAT)
			{
				float x=*this;
				x-=(float)(v);	
				*this=x;
			}

			return *this;
		};

		VAR& operator *(VAR v)
		{
			if (this->v_type==INTEGER)
			{
				int x=*this;
				x*=(int)(v);
				*this=x;
			}
			else
			if (this->v_type==FLOAT)
			{
				float x=*this;
				x*=(float)(v);	
				*this=x;
			}
			else
			if (this->v_type==STRING)
			{
				if (v.v_type==INTEGER)
				{
					char buffer[100]="";
					for (int e=0;e<(int)v;e++)
						strcat(buffer,this->value);
					strcpy(this->value,buffer);
				}
			}			
			return *this;
		};
		VAR& operator /(VAR v)
		{
				float x=*this;
				x*=(float)(v);	
				if (this->v_type==INTEGER)
					*this=(int)floor(v);
				else
				*this=x;

			
			return *this;
		};
		VAR& operator =(char * b)
		{
			strcpy(this->value,b);
			return *this;
		}
		VAR& operator =(const char * b)
		{
			strcpy(this->value,b);
			return *this;
		}
		VAR& operator =(std::string b)
		{
			strcpy(this->value,b.c_str());
			return *this;
		}
		VAR& operator =(int b)
		{
			if (this->v_type==BOOL)
				b ? strcpy(this->value,"true") : strcpy(this->value,"false");
			else
				sprintf(this->value,"%d",b);
			return *this;
		};
		VAR& operator =(float b)
		{
			if (this->v_type==BOOL)
				b ? strcpy(this->value,"true") : strcpy(this->value,"false");
			else
				sprintf(this->value,"%f",b);
			return *this;
		};
		VAR& operator =(bool b)
		{
			this->v_type=BOOL;

			b ? strcpy(this->value,"true") : strcpy(this->value,"false");
			return *this;
		};



		VAR& operator ++()
		{
			if (this->v_type==INTEGER)
			{
				int x=(int)*this;
				x++;
				*this=x;
			}
			else
			if (this->v_type==FLOAT)
			{
				float x=(float)*this;
				x++;
				*this=(float)x;
			}

			return *this;
		};
		VAR& operator --()
		{
			if (this->v_type==INTEGER)
			{
				int x=(int)*this;
				x--;
				*this=x;
			}
			else
			if (this->v_type==FLOAT)
			{
				float x=(float)*this;
				x--;
				*this=x;
			}

			return *this;
		};
		bool operator <(VAR b)
		{
			return ((int)(*this))<(int)b;
		};
		bool operator >(VAR b)
		{
			return ((int)(*this))>(int)b;
		};

		bool operator >=(VAR b)
		{
			return ((int)(*this))>=(int)b;
		};

		bool operator <=(VAR b)
		{
			return ((int)(*this))<=(int)b;
		};
		bool operator ==(VAR b)
		{
			return (strcmp(this->value,b.value)==0);
		};
		bool operator !=(VAR b)
		{
			return (strcmp(this->value,b.value)!=0);
		};

		operator int()
		{			
			if (this->v_type==BOOL)
			{
				if (this->value[0]=='t') return 1;
				else return 0;
			}

		return (int)atoi(this->value);			
		}
 		operator float()
		{			
			if (this->v_type==BOOL)
			{
				if (this->value[0]=='t') return 1;
				else return 0;
			}

		return (float)atof(this->value);			
		}     
		operator bool()
		{			
		return ((int)(*this))==0 ? false : true;
		}
      
		VAR(int) : v_type(INTEGER),op_deep(2)   {strcpy(this->name	,"Number");	strcpy(this->value,"0");};
		VAR() : v_type(NONE),op_deep(2)			{strcpy(this->name	,"-var-");	strcpy(this->value,"0");};
		VAR(char * v, DATATYPES t) : v_type(t)	{strcpy(this->value	,v); };
		~VAR() 
		{ 
			
			char DebugSt[100];

			sprintf(DebugSt,"\r\nDestroying variable %s : %s [%d]",this->name,this->value,GSCRIPT::ReferenceCountLocatedMemory[this->value]);
			if (this->value[0]=='@')
			if (GSCRIPT::ReferenceCountLocatedMemory[this->value]<=1)
			{
				void* mem=0;
				sscanf(this->value,"@%p",&mem);
				std::map<std::string,VAR*>* pmap=0;
				pmap=(std::map<std::string,VAR*>*)mem;
				sprintf(this->value,"0");

					if (pmap==0)
					return;			

				for
				(
				std::map<std::string,VAR*>::iterator it=pmap->begin();
				it!=pmap->end();
				it++
				)
				  {
					delete((*it).second);
				  }

				delete ((void*)mem);
				mem=0;
			
				strcat(DebugSt," KILL");
			}
			
			GSCRIPT::ReferenceCountLocatedMemory[this->value]=GSCRIPT::ReferenceCountLocatedMemory[this->value]-1;

			//GDebug(s.c_str());
			OutputDebugStringA(DebugSt);

		};
 };
 static std::map<std::string, int> ReferenceCountLocatedMemory;

			VAR Zero;
//------------------------------------------------------------ PROTOTYPE SCOPE

	class SCOPE
	{
	public:
		size_t level;
		char * PC;
		std::map	<std::string , VAR		>	LocalVariables;
		std::vector	<VAR>						PolishStack;
		int	Parenthesis;

		SCOPE() : Parenthesis(0),PC(0),level(0) { LocalVariables.clear(); }
		~SCOPE()								{ LocalVariables.clear(); PC=0; }
	};

//------------------------------------------------------------ FUNCTION PROTOTYPE
	struct FUNC 
	{ 	
		std::string			name;
		DATATYPES			Ret;			// typo de token de devolucion
		char				*Loc;			// entrypoint de la funcion
		char				*FunctionStart;			
		char				*Body;
		SCOPE				*Scope;

		std::vector<VAR>	Arguments;

		FUNC() : Body(0) {}
		~FUNC()
		{
			if (Body!=0)
			delete(Body);
		}
	}; 



//------------------------------------------------------------ TOKEN PROTOTYPE
	class TOKEN
	{
	public:
	char*					PC;						//guardamos un apuntador al inicio del token en script
	char					value			[400];	// current token 
	TOKEN_TYPE				token_type;				// token type 
	TOKEN_CLASS				token_class;			// internal representation 
	SCOPE *					CurrentScope;
	};

//------------------------------------------------------------ KEYWORD PROTOTYPE
	class CKEYWORD 
	{ 
	public:
	TOKEN	tok; 
	int		Color;	

	CKEYWORD() {}
	CKEYWORD(std::string n, TOKEN_CLASS type, int c) { sprintf(tok.value,n.c_str()); tok.token_type=KEYWORD; tok.token_class=type; }
	};



//------------------------------------------------------------ MAPAS

	std::map	<std::string , CKEYWORD >	KeyWords;
	std::map	<std::string , DATATYPES>	DataTypes;
	std::map	<std::string , FUNC	*>		FunctionTable;

	std::map	<std::string,
				 std::map	<std::string,std::string>*> LinkMaps;


	

	SCOPE		GlobalScope;
	VAR			RetRegister;
	
	//un vector de SCOPES para hacer el backtrace de variables, teniendo el Global como Origen
	std::vector	<SCOPE*>					ScopeStack;
	std::vector	<FUNC *>					CallStack;

//------------------------------------------------------------ PARSE STACK
	std::stack	<TOKEN>						ParseStack;



//------------------------------------------------------------ STACK
	std::vector	<VAR>						LocalVariablesStack;		//es un stack pero tenemos que acceder a internos


//------------------------------------------------------------ CURRENT PARSE TOKEN
			TOKEN		token;
			

//------------------------------------------------------------ ARGUMENTOS DE PRECOMPILACION
			//std::string	CompilerArguments;
			std::string		EntryPoint;

//------------------------------------------------------------ GESTOR DE ERRORES CUSTOM
			GErrorHandler* ErrorHandler;


	

	virtual GSCRIPT::SCOPE*		GetScope				() { return (ScopeStack.back()); }
	virtual GSCRIPT::SCOPE*		GetGlobalScope			() { return &this->GlobalScope;}
	virtual size_t				GetScopeLevel			() { return ScopeStack.size();}

	virtual int					GetToken();
	virtual int					IsCommand				(std::string s) {return 0;};
	virtual int					IsDataType				(std::string s) {return 0;};
	virtual int					IsScopeDelimiter		(char *c);
	virtual int					IsDelimiter				(char *c);
	virtual int					IsComment				(char *c);
	virtual TOKEN				ReadComment				();
	virtual int					IdentifyAlphaToken		();

	virtual int					LookUpDatatype			(char * d);
	virtual int					LookUpCommand			(char * d);
	virtual VAR*				LookUpVariable			(char * d);
	virtual int					LookUpFunction			(char * d);
	virtual VAR*				LookUpMap				(char* name,char * sub);

	virtual void				LinkMap					(std::string,std::map<std::string,std::string> * map);
	virtual void				ProceedPersistentData	();

	virtual	void				ProcessKeyword			(char * key);
	virtual int					DefineFunction			(char * name);
	virtual int					DefineVariable			(char * pc,SCOPE*);


	virtual VAR					EvaluateExpression		();
	virtual VAR					Atom					();

	//Carga los nombres de las palabras claves
	virtual int					InitCommands			();
	virtual	int					InitDatatypes			();
	virtual int					InitErrorCodes			();

//	virtual int					MacroExpansion			();
	virtual int					PreScan					();
	virtual VAR					Execute					();
	virtual VAR					CallFunction			(std::string name);
	virtual VAR					ExecuteFunction			(FUNC *f);
	virtual void				ExecuteBlock			();
	virtual void				SkipBranch				(int Branch=0);

	virtual VAR					EvaluatePolish			();
	void						ParsePolish				();

	virtual SCOPE*				ScopeTop				()			 { return this->ScopeStack.back();}
	virtual void				ScopePush				(SCOPE * sc) { sc->level=this->GetScopeLevel()+1; sc->PC=this->PC; this->ScopeStack.push_back(sc); }
	virtual void				ScopePop				()			 { this->ScopeStack.pop_back(); }

	virtual TOKEN				ParseTop				()			{ return this->ParseStack.top();}
	virtual void				ParsePush				(TOKEN tok)	{ tok.CurrentScope=this->GetScope(); this->ParseStack.push(tok); }
	virtual void				ParsePop				()			{ this->ParseStack.pop();}

	virtual int					LoadProgramFromFile		(std::string fname);
//	virtual int					SetProgramFromMemory	(BYTE * program,int length);

	virtual char*				GetProgram				()	{	return this->Program;	}
	virtual char*				GetPC					()	{	return this->PC;		}

								GSCRIPT					();
	virtual						~GSCRIPT				();

	virtual	void				Show					();

	virtual void				ProceedAssignment		(char * n,	VAR v);
	virtual void				ProceedAssignment		(VAR * a,	VAR v);

	bool						BreakSignaled;
	
};




//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif