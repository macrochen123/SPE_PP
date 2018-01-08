#include "paeks.h"

void PAEKS_KeyGen(G1& pk_s, G1& pk_r, Big sk_s, Big sk_r, G1& P)
{
	pk_s = pfc.mult(P, sk_s);
	pk_r = pfc.mult(P, sk_r);
}
/////////////
void PAEKS_PEKS(G1& c1, G1& c2, G1 pk_r, Big sk_s, 
				Big w, Big r1, G1& P)
{
	/**************/
	G1 tmp1, tmp2;
	/**************/
	//H(w)
	tmp1 = pfc.Big_To_map(w, *pfc.cof);
	tmp1 = pfc.mult(tmp1, sk_s);
	tmp2 = pfc.mult(P, r1);
	c1 = tmp1 + tmp2;
	c2 = pfc.mult(pk_r, r1);
}
void PAEKS_Trapdoor(GT& t, G1 pk_s, Big sk_r, Big w, G1& P)
{
	G1 tmp1;
	tmp1 = pfc.Big_To_map(w, *pfc.cof);
	tmp1 = pfc.mult(tmp1, sk_r);
	t = pfc.pairing(tmp1, pk_s);
}
BOOL PAEKS_Test(GT t, G1 c1, G1 c2, G1 pk_s, G1 pk_r, G1& P)
{
	GT tmp1, tmp2, tmp3;
	tmp1 = pfc.pairing(c2, P);
	tmp2 = pfc.pairing(c1, pk_r);
	tmp3 = t*tmp1;
	if (tmp3 == tmp2)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}


}
void PAEKS()
{
	G1 P;
	G1 pk_s, pk_r;
	G1 c1, c2;
	Big sk_s, sk_r;
	Big w1,w2;
	Big r1;
	GT t;
	LARGE_INTEGER freq;  
	LARGE_INTEGER start_t, stop_t;  
	double exe_time=0;  
	int counter = 1000;
	int i, j, number;
	QueryPerformanceFrequency(&freq); 
	///////KeyGen///////
	pfc.random(P);
	pfc.random(sk_s);
	pfc.random(sk_r);
	PAEKS_KeyGen(pk_s, pk_r, sk_s, sk_r, P);
	/////////////////////////
	number = 0;
	for(j = 0; j < 10; j++)
	{
		number = number + 50;
		exe_time = 0;
		for(i = 0; i < counter; i++)
		{
			pfc.random(w1);
			pfc.random(r1);
			QueryPerformanceCounter(&start_t); 
			PAEKS_PEKS(c1, c2, pk_r, sk_s, w1, r1, P);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
		}
		cout << "The performance of PAEKS_PEKS(number = "<<number<<") = " << (exe_time/counter)*number<< " ms" << endl;

		exe_time = 0;
		for(i = 0; i < counter; i++)
		{
			pfc.random(w2);
			QueryPerformanceCounter(&start_t); 
			PAEKS_Trapdoor(t, pk_s, sk_r, w2, P);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
		}
		cout << "The performance of PAEKS_Trapdoor(number = "<<number<<") = " << (exe_time/counter)*number<< " ms" << endl;

		exe_time = 0;
		for(i = 0; i < counter; i++)
		{
			QueryPerformanceCounter(&start_t); 
			PAEKS_Test(t, c1, c2, pk_s, pk_r, P);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
		}
		cout << "The performance of PAEKS_Test(number = "<<number<<") = " << (exe_time/counter)*number<< " ms" << endl;
		cout<<"*********************"<<endl;
	}

}