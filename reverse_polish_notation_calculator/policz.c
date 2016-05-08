#include "stos.h"
#include "stos.c"

int main(void)
{
	stack_clear();
	/*TESTING TESTING...
	stack_push(1.0);
	stack_push(77.0);
	stack_push(-5.0);
	stack_push(42.42);
	stack_push(0.1);
	stack_push(7.0);
	stack_pop();
	stack_print();
	stack_push(7.0);
	printf("\n%f\n",stack_top());*/
	double przed_przec = 0.0, po_przec = 0.0;
	int po_przec_divisor = 0, negative = 0;
	int is_number = 0;
	char znak;
	while((znak = getchar()) != -1)
	{
		if(znak == '~')
		{
			is_number = 1;
			negative = 1;
		}
		else if((znak >= '0' && znak <= '9'))
		{
			is_number = 1;
			if(po_przec_divisor)
			{
				po_przec += (znak - '0') / (double)po_przec_divisor;
				po_przec_divisor *= 10;
			}
			else
			{
				przed_przec = przed_przec * 10 + (znak - '0');
			}
		}
		else if(is_number && (znak == '.' || znak == ','))
		{
			po_przec_divisor = 10;
		}
		else if(znak == '+')
		{
			double first, second;
			first = stack_top();
			stack_pop();
			second = stack_top();
			stack_pop();
			second = second + first;
			stack_push(second);
		}
		else if(znak == '-')
		{
			double first, second;
			first = stack_top();
			stack_pop();
			second = stack_top();
			stack_pop();
			second = second - first;
			stack_push(second);
		}
		else if(znak == '*')
		{
			double first, second;
			first = stack_top();
			stack_pop();
			second = stack_top();
			stack_pop();
			second = second * first;
			stack_push(second);
		}
		else if(znak == '/')
		{
			double first, second;
			first = stack_top();
			stack_pop();
			second = stack_top();
			stack_pop();
			second = second / first;
			stack_push(second);
		}
		else if(znak == '^')
		{
			double first, second;
			first = stack_top();
			stack_pop();
			second = stack_top();
			stack_pop();
			second = pow(first, second);
			stack_push(second);
		}
		else
		{
			if(is_number) stack_push((przed_przec + po_przec) * (negative ? -1 : 1));
			is_number = negative = po_przec_divisor = 0;
			przed_przec = po_przec = 0.0;
		}
	}
	printf("\n");
	stack_print();
}