#pragma once
class RandomNum
{
public:
	unsigned long long* numPool;
	int used;
	int size;

	RandomNum();

	unsigned long long getRnd();
	void releaseMem();

};

