#include <iostream>
#include "speks.h"
#include "paeks.h"
#include "PKEDT.h"
#include "DSPEKS.h"
#include "PEKS.h"

PFC pfc(AES_SECURITY);  // initialise pairing-friendly curve
int main()
{   
	time_t seed;
	time(&seed);
	irand((long)seed);
	/*************************************/
	LARGE_INTEGER freq;  
	LARGE_INTEGER start_t, stop_t;  
	double exe_time=0;  
	int counter = 1000;
	int i,j,number;
	QueryPerformanceFrequency(&freq); 

	GT gt;
	Big r;
	G1 g1;
	

	pfc.random(r);
	pfc.random(g1);
	gt = pfc.pairing(g1, g1);

	exe_time = 0;
	for(i = 0; i < counter; i++)
	{
		QueryPerformanceCounter(&start_t); 
		pfc.power(gt, r);
		QueryPerformanceCounter(&stop_t);  
		exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
	}
	cout << "The performance of = " <<(exe_time/counter)<< " ms" << endl;
	
	exe_time = 0;
	for(i = 0; i < counter; i++)
	{
		QueryPerformanceCounter(&start_t); 
		pfc.mult(g1, r);
		QueryPerformanceCounter(&stop_t);  
		exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
	}
	cout << "The performance of= " <<(exe_time/counter)<< " ms" << endl;

	//PKEDT();
	//PAEKS();
	//SPEKS();
	//SPEKS_Test();
	system("pause");
    return 0;
}
