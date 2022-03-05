#include "randArr.h"

void randArray(int a[])
{
	int n = NodeCountNum;
	int i, j, T = 1000, tmp;
	srand(unsigned(time(NULL)));
	while (T--)
	{
		i = rand() % n;
		j = rand() % n;
		tmp = a[i];
		a[i] = a[j];
		a[j] = tmp;
	}
	//for (i = 0; i < n; i++)printf("%d ", a[i]);

}
