
// lexer.c
// Inspired on gcc 0.9.
// 2018 - Created by Fred Nora.

#include "gramcnf.h"


// ## current ##
// Usado pelo lexer pra saber qual lugar na lista 
// colocar o lexeme.
int current_keyword=0;
int current_identifier=0; 
int current_constant=0;
int current_string=0;
int current_separator=0; 
int current_special=0;

int eofno=0;

//
// Line support
//

// Current line number.
int lineno=0;
int lexer_number_of_lines=0;  // Total number of lines.
int lexer_firstline=0;
int lexer_lastline=0;


int lexer_token_count=0;
int number_of_tokens=0;  // Total number of tokens.

int lexer_code=0;
int current_token=0;  // The class of the curent token.

// When some element was found.
int directive_fould=0;
int type_found=0;
int modifier_found=0;
int qualifier_found=0;
int keyword_found=0;
int constant_type_found=0;
int constant_base_found=0;
int return_found=0;
int main_found=0;


//
// Return support
//

// Tipo de retorno da função.
int function_return_type=0;
// Tipo de retorno da função main.
int main_return_type=0;

// Índices na lista de tokens.
int return_index=0;   // Índice para a posição na lista onde está o retorno.
int next_index=0;     //índice do próximo token na lista de tokens.
int current_index=0;

// Flag para o tratamento da string dentro do asm inline.
// \" marcando início e fim de string.
int string_flag=0;

//tipo que foi encontrado.
int current_type=0;

//()
int parentheses_start=0;
int parentheses_end=0;
int parentheses_count=0;
//{}
int brace_start=0;
int brace_end=0;
int brace_count=0;


//
// -- Prototypes --------
//

static int __lexerInit(void);
static int __skip_white_space(void);

//---------------------------------------------------


//#### supensa ###
/*
int check_newline ()
{
    register int c;
    //register int token;	
	
	//obs
	//entramos aqui porque encontramos um '\n'

    while (1)
    {
	   //Entramos nessa função porque encontramos um '\n'.		
        lineno++;
		//printf(" [LF1] ");
		
		//pega mais um depois do '\n'
		c = getc (finput);
		
		//se o que segue o '\n' for um espaço, deixaremos o skip_white_space tratar o espaço.
		if (c == ' ' || c == '\t')
		{
			return (int) c;
		}
		
		//se for quanquer outra coisa também deixaremos o __skip_white_space tratar
		return (int) c;
        
		if ( c == '#' )
		{
            //Skip whitespace after the #.  
            while (1)
	        {
	            c = getc (finput);
	            if ( !(c == ' ' || c == '\t') )
	                break;
	        }
			
             
		     //If the # is the only nonwhite char on the line,
	         //just ignore it.  Check the new newline.  
		    
            if (c == '\n')
	            continue;


            //encontramos algum char válido após #.
			//Something follows the #; read a token.  
            ungetc (c, finput);
            //token = yylex ();		
		
		    //#bugbug:
			// ?? O que fizemos com esse token ??
			
		    //skip the rest of this line.  
            while ((c = getc (finput)) != '\n');		

		}else{
			 
		    // If no #, unread the character,
	        // except don't bother if it is whitespace.  		
			
			//se não é #, retorna ao encontrar espaço e
			//devolve se encontrar outra coisa. depois retorne também.
			
			//return (int) c;
			
	        //if (c == ' ' || c == '\t')
			//{
			//	return (int) c;
			//	
			//} else {
			//	
			//    ungetc ( c, finput );	
			//    return (int) -1;
			//};
		};
		
		
		
	};//while
	
};
*/


//------------------------------------------------
// Skipping white spaces.
static int __skip_white_space(void)
{
    register int c=0;
    register int inside=0;

begin:
    c = getc (finput);

// #debug
    //printf("%c ",c); 

    for (;;)
    {
        switch (c)
        {
            // ## spaces ##
            // Se encontramos um espaço, 
            // pegamos o próximo e saímos do switch 
            // para reentrarmos no for.
            case ' ':
            case '\t':
            case '\f':
            case '\r':
            case '\b':
                c = getc(finput);
                break;

            // ## new lines ##
            case '\n':
                lineno++;
                //próximo.
                c = getc(finput);
                break;

            // ## comments ##
            // '/' 
            // #importante: Isso pode ser a 
            // primeira barra do comentário ou uma divisão.
            case '/':
                c = getc(finput);
                
                //#### inicia um comentário de uma linha ####
                //Aqui encontramos a segunda barra de dias consecutivas.
                //single line comments.
                if ( c == '/' )
                {
                    while (1)
                    {
                        c = getc (finput);

                        //quando alinha acabar,
                        //apenas saímos do switch
                        //sairemos com '\n'
                        //??? e se chegarmos ao fim do arquivo ??? #todo
                        // Acho que isso só sai do while.
                        if (c == '\n'){
                            break;
                        }
                        // ?
                    };
                    //isso sai do switch
                    break;
                }

				//#### inicia um comentário de múltiplas linhas ####
				//#importante 
				//excluindo os casos acima, então significa que nossa barra não tinha nada a ver com comentário 
				//lembrando que a barra aparecei depois de um espaço em branco.
				//por enquanto vamos dizer que algo está errado com essa barra,
				//printf("__skip_white_space: todo: depois da barra / .");
				//exit(1);

                if (c == '*')
                {
                    c = getc (finput);

                    inside = 1; 

                    while (inside)
                    {
                        if (c == '*'){

                            //sequência
                            while (c == '*')
                                c = getc (finput);

                            // se logo em seguida da sequencia
                            // de astetiscos tiver uma barra.
                            if (c == '/')
                            {
                                //fim do comentário  
                                //sai do while ... com alguma coisa em c.
		                        inside = 0;

		                        //c = getc(finput);
							    //break; //sai do while.
								
								//begin: ??
								//Ao fim de um comentário /* ... */
								//podemos ter espaços tabs e talvez outros comentários.
		                        
                                goto begin;
                            }

						// se vamos pular mudar de linha dentro do comentário.
                        }else if (c == '\n'){

						    //precisamos contar.
                            lineno++;
							//printf(" [LF2] ");
                            c = getc (finput);
							  
					        //?? para onde vamos??
							//precisamos continuar no while até encontrarmos a barra /. ou o *.
								
                        }else if (c == EOF || c == '\0'){  

                            eofno++;
                            printf ("__skip_white_space: Unterminated comment in line %d \n", 
                                lineno );
                            exit(1);

						//default
                            
                        }else{
 
							//isso são letras do comentário.
							//continuaremos dentro do while(inside)
							//??#bugbug: mas até quando ??
                            //temos que contar ou confiar no EOF.
                            
                            c = getc(finput);
                        };
                    };
                }

				// aqui depois da barra não emcontramos nem o '*' nem o '/'
                // isso significa que estamos eliminando espaços dentro de uma expressão.
				// então vamos retornar a barra para que a rotina continue tratando a 
				// expressão.

                ungetc ( c, finput );	
				//return (int) '/';
                break;
                

            //#test 
            // ## ignorando diretivas do preprocessdor '#' ##
            /*
			case '#':
                while(1)
				{
					c = getc(finput);
					
					//quando acabar a linha,
					//apenas saímos do switch
					if( c == '\n' ){
						//não precisa contar, pois sairemos do switch e 
						//entraremos no switch novamente agora com \n que será contado na hora apropriada.
						//lineno++;
						//printf(" [LF3] ");
					    break;
					}
				};
                break;
            */
				

            default:
                return (int) c;

        };//switch

    }; // for
}


// -----------------------------------------
// yylex:
// Pega o próximo token.
int yylex(void)
{
    register int value=0;

    register int c=0;

    register char *p;

    register int c1=0;
    register int number_length=0;

again:

// Pega um char da stream de entrada.
    c = (int) __skip_white_space();

    switch (c)
    {

        // 0 or EOF. (-1).
        case EOF:
        case 0:
            //printf ("yylex: 0 or EOF\n");
            eofno++; 
            value = TOKENEOF;
            lexer_lastline = lineno;  // Last line?
            lexer_number_of_lines = lexer_lastline;
            return (int) (value);
            break;

        // [A~Z] [a~z] [_]
        case 'A': case 'B': case 'C': case 'D':
        case 'E': case 'F': case 'G': case 'H':
        case 'I': case 'J': case 'K': case 'L':
        case 'M': case 'N': case 'O': case 'P':
        case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X':
        case 'Y': case 'Z':
        case 'a': case 'b': case 'c': case 'd':
        case 'e': case 'f': case 'g': case 'h':
        case 'i': case 'j': case 'k': case 'l':
        case 'm': case 'n': case 'o': case 'p':
        case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x':
        case 'y': case 'z':
        case '_':

            // Address
            p = token_buffer;
            
            // clean
            memset( real_token_buffer, 0, MAXTOKEN );

            // #todo: 
            // Limite tamanho do buffer
            
            while (1)
            {
                // Put into the buffer and increment the buffer address.
                *p = c;
                p++;

                // Get next token from the file.
                c = getc(finput);

                // Not Alpha-numeric and not '_'.
                // Finalize the buffer if it is not an identifier.
                // Only // [A~Z] [a~z] [_] are accepted.
                // #todo: Maybe we can expand this set of chars accepted
                // as part of the identifier. Ex: '$'.

                if ( ( isalnum(c) == 0 ) &&  (c != '_') )
                {
                    *p = 0;  // Finalize
                    ungetc( c, finput );
                    goto we_have_an_identifier;
                }
            };

            // Temos um identificador.
            we_have_an_identifier:
            
            //#debug
            //printf ("real_token_buffer={%s}\n",real_token_buffer);

            // Vamos começar dizendo que somos um identificador.
            // Porem ...
            // O identificador pode ser um nome de função ou uma variável.
            // Mas vamos comparar com palavras reservadas.
            // Caso seja uma das palavras reservadas, então deixamos de ser um identificador.
            value = TOKENIDENTIFIER;

            // Reserved?
            // Determinamos que era um identificador,
            // mas vamos ver se ele é uma palavra reservada.
            // As palavras reservadas podem ser modificadores, tipos
            // ou palavras chave.

            // Modifiers

            if ( strncmp( real_token_buffer, "signed", 6 ) == 0 )
            {
                keyword_found  = KWSIGNED;
                value          = TOKENMODIFIER;
                modifier_found = MSIGNED;
            }
            if ( strncmp( real_token_buffer, "unsigned", 8 ) == 0 )
            {
                keyword_found  = KWUNSIGNED;
                value          = TOKENMODIFIER;
                modifier_found = MUNSIGNED;
            }
            if ( strncmp( real_token_buffer, "inline", 6 ) == 0 )
            {
                keyword_found  = KWINLINE;
                value          = TOKENMODIFIER;
                modifier_found = MINLINE;
            }
            if ( strncmp( real_token_buffer, "static", 6 ) == 0 )
            {
                keyword_found  = KWSTATIC;
                value          = TOKENMODIFIER;
                modifier_found = MSTATIC;
            }
            if ( strncmp( real_token_buffer, "volatile", 8 ) == 0  )
            {
                keyword_found  = KWVOLATILE;
                value          = TOKENMODIFIER;
                modifier_found = MVOLATILE;
            }

            // types

            if ( strncmp( real_token_buffer, "void", 4 ) == 0 )
            {
                keyword_found = KWVOID;
                value         = TOKENTYPE;
                type_found    = TVOID;
            }
            if ( strncmp( real_token_buffer, "char", 4 ) == 0 )
            {
                keyword_found = KWCHAR;
                value         = TOKENTYPE;
                type_found    = TCHAR;
            }
            if ( strncmp( real_token_buffer, "short", 5 ) == 0 )
            {
                keyword_found = KWSHORT;
                value         = TOKENTYPE;
                type_found    = TSHORT;
            }
            if ( strncmp( real_token_buffer, "int", 3 ) == 0 )
            {
                keyword_found = KWINT;
                value         = TOKENTYPE;
                type_found    = TINT;
            }
            if ( strncmp( real_token_buffer, "long", 4 ) == 0 )
            {
                keyword_found = KWLONG;
                value         = TOKENTYPE;
                type_found    = TLONG;
            }
            if ( strncmp( real_token_buffer, "meta", 4 ) == 0 )
            {
                keyword_found = KWMETA;
                value         = TOKENTYPE;
                type_found    = TMETA;
            }
            if ( strncmp( real_token_buffer, "def", 3 ) == 0 )
            {
                keyword_found = KWDEF;
                value         = TOKENTYPE;
                type_found    = TDEF;
            }
            if ( strncmp( real_token_buffer, "var", 3 ) == 0  )
            {
                keyword_found = KWVAR;
                value         = TOKENTYPE;
                type_found    = TVAR;
            }
            if ( strncmp( real_token_buffer, "let", 3 ) == 0  )
            {
                keyword_found = KWLET;
                value         = TOKENTYPE;
                type_found    = TLET;
            }

            // keywords

            if ( strncmp( real_token_buffer, "name", 4 ) == 0 )
            {
                keyword_found = KWNAME;
                value         = TOKENKEYWORD;
            }
            if ( strncmp( real_token_buffer, "content", 7 ) == 0 )
            {
                keyword_found = KWCONTENT;
                value         = TOKENKEYWORD;
            }
            if ( strncmp( real_token_buffer, "goto", 4 ) == 0 )
            {
                keyword_found = KWGOTO;
                value         = TOKENKEYWORD;
            }
            if ( strncmp( real_token_buffer, "return", 6 ) == 0 )
            {
                keyword_found = KWRETURN;
                value         = TOKENKEYWORD;
            }
            if ( strncmp( real_token_buffer, "switch", 6 ) == 0 )
            {
                keyword_found = KWSWITCH;
                value         = TOKENKEYWORD;
            }
            if ( strncmp( real_token_buffer, "case", 4 ) == 0 )
            {
                keyword_found = KWCASE;
                value         = TOKENKEYWORD;
            }
            if ( strncmp( real_token_buffer, "default", 7 ) == 0 )
            {
                keyword_found = KWDEFAULT;
                value         = TOKENKEYWORD;
            }
            if ( strncmp( real_token_buffer, "for", 3 ) == 0 )
            {
                keyword_found = KWFOR;
                value         = TOKENKEYWORD;
            }
            if ( strncmp( real_token_buffer, "continue", 8 ) == 0 )
            {
                keyword_found = KWCONTINUE;
                value         = TOKENKEYWORD;
            }
            if ( strncmp( real_token_buffer, "do", 2 ) == 0 )
            {
                keyword_found = KWDO;
                value         = TOKENKEYWORD;
            }
            if ( strncmp( real_token_buffer, "while", 5 ) == 0 )
            {
                keyword_found = KWWHILE;
                value         = TOKENKEYWORD;
            }
            if ( strncmp( real_token_buffer, "if", 2 ) == 0 )
            {
                keyword_found = KWIF;
                value         = TOKENKEYWORD;
            }
            if ( strncmp( real_token_buffer, "else", 4 ) == 0 )
            {
                keyword_found = KWELSE;
                value         = TOKENKEYWORD;
            }
            if ( strncmp( real_token_buffer, "union", 5 ) == 0 )
            {
                keyword_found = KWUNION;
                value         = TOKENKEYWORD;
            }
            if ( strncmp( real_token_buffer, "struct", 6 ) == 0 )
            {
                keyword_found = KWSTRUCT;
                value         = TOKENKEYWORD;
            }
            if ( strncmp( real_token_buffer, "enum", 4 ) == 0 )
            {
                keyword_found = KWENUM;
                value         = TOKENKEYWORD;
            }
            if ( strncmp( real_token_buffer, "sizeof", 6 ) == 0 )
            {
                keyword_found = KWSIZEOF;
                value         = TOKENKEYWORD;
            }

            //...

            break;

        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        case '8': case '9':
        //case '.':

            // Address
            p = token_buffer;

            if ( c == '0' ){
                // Coloca no buffer.
                *p = c;
                p++;
                c = getc(finput);

                if ( c == 'x' || c == 'X' )
                {
                    //base = 16;
                    //*p++ = c; //coloca o x.

                     *p = c;
                     p++;

                    while (1)
                    {
                        c = getc (finput);

                        // Se o próximo não for um digito hexadecimal. 
                        if ( isxdigit (c) == 0 )
                        {
                            *p = 0;
                             ungetc ( c, finput );
                            //fim
                            value = TOKENCONSTANT;
                            //constant_type_found = //#todo tem que contar. 
                            constant_base_found = CONSTANT_BASE_HEX;
                            goto constant_done;
                        }

                        //coloca se é hexa.
                        *p = c;
                        p++;
                    };
                }

                printf ("yylex: FAIL expected x in constant in line %", 
                    lineno );
                exit (1);

            }else{

                //base = 10.

                *p++ = c; 
                
                while (1)
                {
                     c = getc (finput);

					//se não é digito.
                    if ( isdigit( c ) == 0 )
                    {
						//fim
						*p = 0;
						ungetc(c, finput);
						value = TOKENCONSTANT;
						//constant_type_found = //#todo tem que contar. 
						constant_base_found = CONSTANT_BASE_DEC;
                        goto constant_done;
                    }

					//coloca o digito.
                    *p++ = c;
                };
            };

            constant_done:
            break;

        //String
        case '\"':
        {
            c = getc(finput);
            // Address
            p = token_buffer;
    
            //coloca no token_buffer.
            while (c != '\"')
            {
	            //if (c == '\\')
			    //{
		        //    c = readescape ();
		        //    if (c < 0)
		        //        goto skipnewline;
	        
			    //}else if (c == '\n')
			    //      {
		        //          lineno++;
	            //      }

	            //if (p == token_buffer + maxtoken)
	               // p = extend_token_buffer(p);
	                *p++ = c;

	            //skipnewline:
	                c = getc(finput);
	        };//while

            //finaliza a string
            *p++ = 0;

            //yylval.ttype = build_string (p - token_buffer, token_buffer);
            //TREE_TYPE (yylval.ttype) = char_array_type_node;

            // Avisa que é uma string
            // Ela vai estar no token_buffer.
            value = TOKENSTRING;
            break;
        };

        // Separators: (){}[],.;:?
        case '(':  case ')':
        case '{':  case '}':
        case '[':  case ']':
        case ',':
        case '.':
        case ';':
        case ':':
        case '?':
            p = token_buffer;
            *p++ = c;
            *p++ = 0;
            value = TOKENSEPARATOR;
            break;

        //usadas em expressões matemáticas, 
        //#todo: não mudar isso.
        //@todo: talvez se enviarmos esses chars para o buffer ajude no debug.
        case '+':  case '-':  case '*':  case '/':
        case '<':  case '>':
        case '&':
        case '|':
        case '%':
        case '^':
        case '!':
        case '=':
        {
            combine:

            switch (c)
            {
                // '+-*/'
                case '+':  lexer_code = PLUS_EXPR;       break;
                case '-':  lexer_code = MINUS_EXPR;      break;
                case '*':  lexer_code = MULT_EXPR;       break;
                case '/':  lexer_code = TRUNC_DIV_EXPR;  break;

                case '&':  lexer_code = BIT_AND_EXPR;     break;
                case '|':  lexer_code = BIT_IOR_EXPR;     break;
                case '%':  lexer_code = TRUNC_MOD_EXPR;   break;
                case '^':  lexer_code = BIT_XOR_EXPR;     break;

                // ?
                case LSHIFT:  lexer_code = LSHIFT_EXPR;  break;
                case RSHIFT:  lexer_code = RSHIFT_EXPR;  break;

                case '<':  lexer_code = LT_EXPR;  break;
                case '>':  lexer_code = GT_EXPR;  break;
            }

            c1 = getc (finput);

            if (c1 == '=')
            {
                switch (c)
                {
                    case '<':
                        value = ARITHCOMPARE;  //?
                        lexer_code = LE_EXPR; 
                        goto done;
                    case '>':
                        value = ARITHCOMPARE;  //?
                        lexer_code = GE_EXPR; 
                        goto done;
                    case '!':
                        value = EQCOMPARE;  //?
                        lexer_code = NE_EXPR; 
                        goto done;
                    case '=':
                        value = EQCOMPARE;  //?
                        lexer_code = EQ_EXPR; 
                        goto done;
                };

                // ?
                value = ASSIGN; 
                goto done;

            }else if (c == c1){

                switch (c)
                {
                    case '+':  value = PLUSPLUS;    goto done;
                    case '-':  value = MINUSMINUS;  goto done;
                    case '&':  value = ANDAND;      goto done;
                    case '|':  value = OROR;        goto done;
                    
                    case '<':  c = LSHIFT;  goto combine;
                    case '>':  c = RSHIFT;  goto combine;
                };

            }else if ((c == '-') && (c1 == '>')) {
                value = POINTSAT; 
                goto done; 
            };

            ungetc (c1, finput);

            if ((c == '<') || (c == '>'))
                value = ARITHCOMPARE;
            else value = c;
                goto done;
        };

        default:
            value = (int) c;
    }; //switch

done:
// Return the token.
    return (int) value;
}

/*
 * lexerInit:
 *     Inicializando o lexer.
 */
static int __lexerInit(void)
{
    register int i=0;
    //number_of_tokens = 0;
    //current_token = 0;
    //next_index = 0;

// Line support
// Arquivos de texto começa com a linha 1.

    lexer_firstline=1;
    lexer_lastline=1;
    lexer_number_of_lines=1;
    lineno = lexer_firstline;  // Current line.

    eofno = 0;  // eof++
    lexer_code = 0;
    maxtoken = MAXTOKEN;

// Clear buffer
    for ( i=0; i<MAXTOKEN; i++ )
    {
        //real_token_buffer[i] = (char) '\0';
        real_token_buffer[i] = 0;
    };

    token_buffer = real_token_buffer;
    //token_buffer = &real_token_buffer[0]; 
    //sprintf ( real_token_buffer, "uninitialized-token-string" );

    //...
    return 0;
}

int lexer_initialize(void)
{
    //printf ("parser_initialize:\n");
    return (int) __lexerInit();
}

/*
//check subsequent
int check_subseq ( int c, int a, int b )
{
	//extern getchar, peekc;

	if (!peekc)
		peekc = getchar();
	
	//se for diferente de c, retorna a.
	//se for igual a c, retorn b.
	
	if (peekc != c)
		return (a);
	
	peekc = 0;
	
	return (b);
};
*/

void error(char *msg)
{
    printf ("error: %s\n", msg );
}


//
// End
//



