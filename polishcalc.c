#include <stdio.h>
#include <stdlib.h>      /* Para el uso de funcion atof() */
#include <math.h>

#define MAXOP 100        /*  El operando solo puede tener expresarse en un maximo de 100 caracteres */
#define NUMBER '0'       /*  Valor que funciona como señal de que un numero fue leido */
#define END	1		     /*  Este valor indica al control del programa que una operacion ingresada ya fue realizada */
#define IN 0			 /*  Este valor indica al control del programa que una operacion matematica esta siendo procesada */
#define MAXVAR 6         /*  Numero de variables disponibles para el usuario */
int getop(char []);
void push(double);
double pop(void);
void inputclear(void);
void stclear(void);
int getvar(void);
/* reverse Polish calculator */

int main(void)
{
	int type, state, err;
	double op2, var[MAXVAR] = {0.0};
	char s[MAXOP];
	printf("El presente programa provee una herramienta para realizar operaciones matematicas en numeros enteros o con parte decimal.\n"
			"Para el uso correcto de esta herramienta, se requiere que el usuario ingrese las operaciones a realizar en notacion\n"
			"polaca inversa. A continuacion se muestra un ejemplo de una operacion en notacion polaca inversa:     8 2 + 4 *\n\n"
			"La operacion anterior es equivalente a la notacion infija:     (8 + 2) * 4\n\n"
			"Al ingresar una operacion es necesario separar los operandos de un operador con un espacio en blanco, como se muestra\n"
			"en el ejemplo anterior.\n\n"
			"Esta herramienta tiene la opcion de realizar operaciones con numeros con signo; para identificar un numero con signo\n"
			"es necesario que el signo anteceda al primer digito o punto decimal del mismo.\n\n"
			"Es posible almacenar un numero o el resultado de una operacion en una de las %d variables disponibles, a continuacion\n"
			"mostramos ejemplos de como almacenar distintos valores:\n-8.2 4.7 * 3.3 -10 + - > A\n-456.55 > B\n\n"
			"El operador \">\" indica que el ultimo numero ingresado o el producto de una operacion sera almacenado en alguna de las\n"
			"%d variables indicadas en el siguiente rango de letras mayusculas: A - F.\n"
			"Para usar el valor de una variable, sera necesario escribir la letra correspondiente en una operacion; el programa\n"
			"tomara el valor de la variable para el calculo.\n"
			"Despues de ingresar la operacion, la herramienta imprime el resultado del calculo realizado.\n\n"
			"Considerar que la herramienta solo puede realizar las siguientes operaciones:\n\nOperador\tOperacion\n"
			"+\t\tSuma\n-\t\tResta\n*\t\tMultiplicacion\n/\t\tDivision\n%%\t\tModulo\n^\t\tPotenciacion\n>\t\tAlmacenar dato en una variable\n"
			"Cualquier otro operador o comando ingresado invalida la operacion del usuario.\n\n"
			"Para salir de la herramienta, ingresar un caracter distinto a la letra \"S\".\n\n"
			"--COMIENZA EL PROGRAMA--\n", MAXVAR, MAXVAR);
	do
	{
		state = IN;
		err = 0;
		stclear();
		printf("Ingresa la operacion a realizar:");
		while (state == IN) {
			type = getop(s);
			/* printf("%d\t%c\n", type, (type != '\n') ? type : '-'); */
			switch (type) {
				case NUMBER:
					push(atof(s));
					break;
				case '+':
					push(pop() + pop());
					break;
				case '*':
					push(pop() * pop());
					break;	
				case '-':
					op2 = pop();
					push (pop() - op2);
					break;
				case '/':
					op2 = pop();
					if (op2 != 0.0)
						push(pop() / op2);
					else {
						printf("error: el divisor es cero.\n");
						err = 1;
					}
					break;
				case '%':
					op2 = pop();
					if (op2 != 0.0)
						push(fmod(pop(), op2));
					else {
						printf("error: en la operacion a %% n: n debe ser un numero diferente de 0.\n");
						err = 1;
					}
					break;	
				case '^':
					op2 = pop();
					push(pow(pop(), op2));
					break;
				case '>':
					type = getvar();
					/* En caso de leer una variable, devuelve un numero entero distinto a -1;
						-1 simboliza que no se tomo un valor valido que represente una variable */
					op2 = pop();
					if (type > 0) 
						var[type - 'A'] = op2;
					else
						printf("error: no es posible almacenar el valor %.8g; la variable ingresada no es valida.\n", op2);
					push(op2);
					break;
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F': 
					push(var[type - 'A']);
					break;
				case '\n':
					printf("El resultado es:%.8g\n", pop());
					state = END;
					break;
				default:
					printf("error: se introdujo comando desconocido:%s\n", s);
					err = 1;
					break;
			}
			if (err == 1) {
			/* En caso de que la herramienta se encuentre con un error en una operacion sera util
			ingresar a clear() para limpiar los operandos u operadores que hayan restado 
			en la linea de entrada */
				inputclear();
				stclear();
				state = END;
			}
		}
		printf("Para ingresar nuevamente la herramienta de operaciones matematicas, ingresar la letra \"S\":");
		type = getchar();
		/* En dado caso que el usuario haya ingresado mas de un caracter, utilizamos la funcion clear
		para limpiar esos caracteres indeseados en la entrada */
		if (type != '\n' && type != EOF)
			inputclear();
	}
	while (type == 'S');
	return 0;
}

/* inputclear: Limpia los caracteres en el flujo de entrada */
void inputclear(void)
{
	while(getchar() != '\n')
		;
}

#define MAXVAL 100       /* espacio maximo en la pila */
int sp = 0;			     /* indice del lugar disponible en la pila */
double val[MAXVAL];      /* pila con los operandos introducidos */

/* push: introducir el dato f en la pila: val */
void push(double f)
{
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: la pila esta llena, no es posible introducir el valor: %g\n", f);
}

/* pop: extraer el valor que se encuentre al inicio de la pila */
double pop(void)
{
	if (sp > 0)
		return val[--sp];
	else {
		printf("error: pila vacia\n");
		return 0.0;
	}
}

/* stclear: Se restablece el indice de pila a su valor inicial, provocando que los valores
	almacenados se eliminen al sobreescribir en los espacios ya ocupados */
void stclear(void)
{
	sp = 0;
}

#include <ctype.h>

int getch(void);
void ungetch(int);

/* getop: leer el siguiente operador u operando en la entrada */
int getop(char s[])
{
	int i, c, d;
	
	while ((s[0] = c = getch()) == ' ' || c == '\t')
		;
	s[1] = '\0';
	if (!isdigit(c) && c != '.' && c != '+' && c != '-')
		return c;        /* no es un numero y no es el signo que antecede a un numero */
	i = 0;
	/* Prueba para determinar si el primer caracter leido es un signo de un numero en la entrada o 
	si es un operador */
	if (c == '-' || c == '+') { 
		d = c;	
		if (isdigit(c = getch()) || c == '.')	
			s[++i] = c;
		else {		/* Se valida que es un operador de suma o resta; se devuelve al control */
			ungetch(c);
			return d;
		}
	}
	if (isdigit(c))   /* se prueba si el caracter en c es un digito de un numero */
		while (isdigit(s[++i] = c = getch())) 
			;
	if (c == '.')	 /* prueba requerida para recolectar la parte decimal del numero en la entrada */
		while (isdigit(s[++i] = c = getch()))
			;
	s[i] = '\0';
	ungetch(c);
	return NUMBER;
}

#define BUFSIZE 10			/* espacio maximo en el buffer de caracteres devueltos a la entrada */ 

char buf[BUFSIZE];			/* buffer para almacenar caracteres no deseados que se tomaron de la entrada */
int bufp = 0;				/* indice del lugar disponible en el buffer */

int getch(void)             /* tomar un caracter de la entrada */
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)			/* almacena los caracteres que se quieren devolver a la entrada */
{
	if (bufp >= BUFSIZE)
		printf("ungetch: el buffer esta lleno\n");
	else
		buf[bufp++] = c;
}

int getvar(void)
{
	int c;
	while ((c = getchar()) == ' ' || c == '\t')
		;
	if (isupper(c) && c >= 'A' && c <= 'F')
		return c;
	else if (c == '\n')
	/* En caso de que c sea una nueva linea, se tiene que guardar al arreglo que representa los 
	caracteres que se devuelven a la entrada, para que se imprima el valor al principio de la pila */
		ungetch(c);			
	return -1;
}