#include <stdio.h>
#include <stdlib.h>      /* Para el uso de funcion atof() */

#define MAXOP 100        /*  El operando solo puede tener expresarse en un maximo de 100 caracteres */
#define NUMBER '0'       /*  Valor que funciona como señal de que un numero fue leido */
#define END	1		     /*  Este valor indica al control del programa que una operacion ingresada ya fue realizada */
#define IN 0			 /*  Este valor indica al control del programa que una operacion matematica esta siendo procesada */
int getop(char []);
void push(double);
double pop(void);

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
			"como se muestra en el ejemplo anterior.\n"
			"Despues de ingresar la operacion, la herramienta imprime el resultado del calculo realizado.\n\n"
			"Considerar que la herramienta solo puede realizar las siguientes operaciones:\n\nOperador\tOperacion\n"
			"+\t\tSuma\n-\t\tResta\n*\t\tMultiplicacion\n/\t\tDivision\nCualquier otra operador ingresado provoca un resultado erroneo.\n"
			"Para salir de la herramienta, ingresar un caracter distinto a la letra \"S\".\n\n"
			"--COMIENZA EL PROGRAMA--\n");
	do
	{
		state = IN;
		printf("Ingresa la operacion a realizar:");
		while (state == IN)
		{
			type = getop(s);
			printf("%d\n", type); 
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
					else
						printf("error: el divisor es cero.\n");
					break;
				case '\n':
					printf ("El resultado es:%.8g\n", pop());
					state = END;
					break;
				default:
					printf("error: se introdujo comando desconocido:%s\n", s);
					break;
			}
		}
		printf("Para ingresar nuevamente la herramienta de operaciones matematicas, ingresar la letra \"S\":");
		while ((type = getchar()) != '\n')
			state = type;
	}
	while (state == 'S');
	return 0;
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
	int i, c;
	
	while ((s[0] = c = getch()) == ' ' || c == '\t')
		;
	s[1] = '\0';
	if (!isdigit(c) && c != '.')
		return c;        /* no es un numero */
	i = 0;
	if (isdigit(c))      /* prueba requerida para recolectar la parte entera del numero en la entrada */
		while (isdigit(s[++i] = c = getch()))
			;
	if (c == '.')		 /* prueba requerida para recolectar la parte decimal del numero en la entrada */
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

		
			
