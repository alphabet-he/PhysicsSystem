#include "RandomNum.h"
#include <iostream>

RandomNum::RandomNum()
{
	//size = 4;
	//used = 0;
	//numPool = (unsigned long long*)malloc(size * sizeof(unsigned long long));
	//for (int i = 0; i < size; i++)
	//{
	//	int* p;
	//	p = (int*)malloc(sizeof(int)); // (definition is lying under the code)
	//	unsigned long long l = (unsigned long long)p;
	//	printf("%llu\n", l);
	//	int rnd = l % 63;
	//	printf("%i\n", rnd);
	//}
}

unsigned long long RandomNum::getRnd()
{
	//__int64 ret = -1;
	//if (used < size) {
	//	ret = numPool[used];
	//}
	//else {
	//	numPool = (unsigned long long*)realloc(numPool, (size + 1) * sizeof(__int64));
	//	int* p;
	//	p = (int*)malloc(sizeof(int));
	//	ret = (__int64)p;
	//	numPool[used] = (__int64)p;
	//	size++;
	//}
	//used++;
	//return ret;

	int* p;
	p = (int*)malloc(sizeof(int)); // (definition is lying under the code)
	unsigned long long l = (unsigned long long)p;
	return l;
}

void RandomNum::releaseMem()
{
	//for (int i = 0; i < size; i++) {
	//	free((int*)numPool[i]);
	//}
	//free(numPool);
}


