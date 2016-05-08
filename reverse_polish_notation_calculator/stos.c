
//jeśli sizeof int > sizeof double this code WILL FAIL.
#define stos_top (*((int*)(stos)))
#define stos_max 512
double stos[stos_max];

int stack_push(double wartosc)
{
	if(stos_top >= stos_max-1) return -1;
	stos_top++;
	stos[stos_top] = wartosc;
	return 0;	
}

int stack_pop()
{
	if(stos_top <= 0) return -1;
	stos_top--;
	return 0;
}

double stack_top()
{
	if(stos_top <= 0 || stos_top >= stos_max-1) return 0.0;
	return stos[stos_top];
}

int stack_isempty()
{
	if(stos_top <= 0) return 1;
	else return 0;
}

int stack_clear()
{
	stos_top = 0;
}

int stack_print()
{
	unsigned i;
	for(i = 1; i <= stos_top; ++i)
	{
		printf("%f ", stos[i]);
	}
}

#undef stos_top
#undef stos_max