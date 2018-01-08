#include "PEKS.h"

void PEKS_KeyGen(G1& PK_r, Big sk_r, G1 P)
{
	PK_r = pfc.mult(P, sk_r);
}

void PEKS_PEKS(G1& C1, Big& BC2, G1 PK_r, G1 P, Big w, Big r)
{
	G1 tmp1, tmp2;
	GT GtTmp;
	C1 = pfc.mult(P, r);

	tmp1 = pfc.Big_To_map(w, *pfc.cof);
	tmp2 = pfc.mult(PK_r, r);
	GtTmp = pfc.pairing(tmp1, tmp2);

	pfc.start_hash();
	pfc.add_to_hash(GtTmp);
	BC2 = pfc.finish_hash_to_group();
}

void PEKS_Trapdoor(G1& T, Big w, Big sk_r)
{
	G1 tmp;
	tmp = pfc.Big_To_map(w, *pfc.cof);
	T = pfc.mult(tmp, sk_r);
}

BOOL PEKS_Test(G1 C1, Big BC2, G1 T)
{
	GT GtTmp;
	Big BT;
	GtTmp = pfc.pairing(T, C1);
	pfc.start_hash();
	pfc.add_to_hash(GtTmp);
	BT = pfc.finish_hash_to_group();
	if (BT == BC2)
	{
		return TRUE;
	}
	return FALSE;
}

void PEKS()
{
	G1 PK_r, P;
	Big sk_r;
	G1 C1;
	Big BC2;
	Big r;
	Big w1, w2;
	G1 T;
	LARGE_INTEGER freq;  
	LARGE_INTEGER start_t, stop_t;  
	double exe_time=0;  
	int counter = 1000;
	int number;
	int i, j;
	QueryPerformanceFrequency(&freq); 
	pfc.random(P);
	pfc.random(sk_r);
	pfc.random(r);
	///////////密钥产生//////////
	PEKS_KeyGen(PK_r, sk_r, P);
	///////////加密产生//////////
	number = 0;
	for (j = 0; j < 10; j++)
	{
		number = number + 50;
		exe_time = 0;
		for(i = 0; i < counter; i++)
		{
			pfc.random(w1);
			QueryPerformanceCounter(&start_t); 
			PEKS_PEKS(C1, BC2, PK_r, P, w1, r);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
		}
		cout << "The performance of PEKS_PEKS (number = "<<number<<") = " << (exe_time/counter)*number<< " ms" << endl;
		///////////陷门产生//////////
		exe_time = 0;
		for(i = 0; i < counter; i++)
		{
			pfc.random(w2);
			QueryPerformanceCounter(&start_t); 
			PEKS_Trapdoor(T, w2, sk_r);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
		}
		cout << "The performance of PEKS_Trapdoor(number = "<<number<<") = " << (exe_time/counter)*number<< " ms" << endl;
		///////////测试产生//////////
		exe_time = 0;
		for(i = 0; i < counter; i++)
		{
			QueryPerformanceCounter(&start_t); 
			PEKS_Test(C1, BC2, T);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
		}
	   cout << "The performance of PEKS_Test (number = "<<number<<") = " << (exe_time/counter)*number<< " ms" << endl;
	   cout<<"********************************"<<endl;
	}
	
}