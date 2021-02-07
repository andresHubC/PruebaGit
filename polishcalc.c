#include <stdio.h>
#include <stdlib.h>      /* Para el uso de funcion atof() */
#include <math.h>

#define MAXOP 100        /*  El operando solo puede tener expresarse en un maximo de 100 caracteres */
#define NUMBER '0'       /*  Valor que funciona como señal de que un numero fue leido */
#define END	1		     /*  Este valor indica al control del programa que una operacion ingresada ya fue realizada */
#define IN 0			 /*  Este valor indica al control del programa que una operacion matematica esta siendo procesada */
int getop(char []);
void push(double);
double pop(void);
void clear(void);
int err;
/* reverse Polish calculator */

int main(void)
{
	int type, state;
	double op2;
	char s[MAXOP];
	printf("El presente programa provee una herramienta para realizar operaciones matematicas en numeros enteros o con parte decimal.\n"
			"Para el uso correcto de esta herramienta, se requiere que el usuario ingrese las operaciones a realizar en notacion\n"
			"polaca inversa. A continuacion se muestra un ejemplo de una operacion en notacion polaca inversa:     8 2 + 4 *\n\n"
			"La operacion anterior es equivalente a la notacion infija:     (8 + 2) * 4\n\n"
			"Es necesario que al ingresar la operacion en la herramienta separe los operandos de un operador con un espacio en blanco,\n"
			"como se muestra en el ejemplo anterior.\n\n"
			"Esta herramienta tiene la opcion de realizar operaciones con numeros con signo; para identificar un numero con signo\n"
			"es necesario que el signo anteceda al primer digito o punto decimal del mismo.\n"
			"Despues de ingresar la operacion, la herramienta imprime el resultado del calculo realizado.\n\n"
			"Considerar que la herramienta solo puede realizar las siguientes operaciones:\n\nOperador\tOperacion\n"
			"+\t\tSuma\n-\t\tResta\n*\t\tMultiplicacion\n/\t\tDivision\n%%\t\tModulo\n"
			"Cualquier otro operador o comando ingresado invalida la operacion del usuario.\n\n"
			"Para salir de la herramienta, ingresar un caracter distinto a la letra \"S\".\n\n"
			"--COMIENZA EL PROGRAMA--\n");
	do
	{
		state = IN;
		err = 0;
		printf("Ingresa la operacion a realizar:");
		while (state == IN) {
			type = getop(s);
			printf("%d\t%c\n", type, type); 
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
				clear();
				state = END;
			}
		}
		printf("Para ingresar nuevamente la herramienta de operaciones matematicas, ingresar la letra \"S\":");
		type = getchar();
		/* En dado caso que el usuario haya ingresado mas de un caracter, utilizamos la funcion clear
		para limpiar esos caracteres indeseados en la entrada */
		if (type != '\n')
			clear();
	}
	while (type == 'S');
	return 0;
}

void clear(void)
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

#define BUFSIZE 100			/* espacio maximo en el buffer de caracteres devueltos a la entrada */ 

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