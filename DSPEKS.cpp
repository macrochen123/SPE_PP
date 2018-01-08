#include "DSPEKS.h"

void DS_KeyGen(G1& PK_FS, G1& PK_BS, Big sk_fs1, Big sk_fs2, 
			   Big sk_bs1, Big sk_bs2, G1 P1, G1 P2)
{
	G1 tmp1, tmp2;
	tmp1 = pfc.mult(P1, sk_fs1);
	tmp2 = pfc.mult(P2, sk_fs2);
	PK_FS = tmp1 + tmp2;
    ///////////////////////////
	tmp1 = pfc.mult(P1, sk_bs1);
	tmp2 = pfc.mult(P2, sk_bs2);
	PK_BS = tmp1 + tmp2;
}

void DS_PEKS(G1& C1, G1& C2, G1& C3, G1 PK_FS, G1 PK_BS,
			 Big r, Big w, G1 P1, G1 P2)
{
	G1 HW;
	G1 tmp1, tmp2;
	C1 = pfc.mult(P1, r);
	C2 = pfc.mult(P2, r);
	tmp1 = pfc.mult(PK_FS, r);
	tmp2 = pfc.mult(PK_BS, r);
	HW = pfc.Big_To_map(w, *pfc.cof);
	C3 = tmp1 + tmp2 + HW;
}

void DS_Trapdoor(G1& T1, G1& T2, G1& T3, G1 PK_FS, G1 PK_BS,
				 Big r, Big w, G1 P1, G1 P2)
{
	G1 HW;
	G1 tmp1, tmp2;
	T1 = pfc.mult(P1, r);
	T2 = pfc.mult(P2, r);
	tmp1 = pfc.mult(PK_FS, r);
	tmp2 = pfc.mult(PK_BS, r);
	HW = pfc.Big_To_map(w, *pfc.cof);
	T3 = tmp1 + tmp2 + (-HW);
}

void DS_FS_Test(G1& IC1, G1& IC2, G1& IC3, G1 C1, G1 C2, G1 C3, 
				G1 T1, G1 T2, G1 T3, Big r, Big sk_fs1, Big sk_fs2)
{
	G1 tmp1, tmp2, tmp3;
	G1 t1, t2, t3;

	tmp1 = C1 + T1;
	tmp2 = C2 + T2;
	tmp3 = C3 + T3;

	t1 = pfc.mult(tmp1, sk_fs1);
	t2 = pfc.mult(tmp2, sk_fs2);
	t3 = t1 + t2;
	tmp3 = tmp3 + (-t3);
	
	IC1 = pfc.mult(tmp1, r);
	IC2 = pfc.mult(tmp2, r);
	IC3 = pfc.mult(tmp3, r);
}

BOOL DS_BS_Test(G1 IC1, G1 IC2, G1 IC3, Big sk_bs1, Big sk_bs2)
{
	G1 tmp1, tmp2, test;
	tmp1 = pfc.mult(IC1, sk_bs1);
	tmp2 = pfc.mult(IC2, sk_bs2);
	test = tmp1 + tmp2;
	if (test == IC3)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void DSPEKS()
{
	G1 P1, P2;
	G1 PK_FS, PK_BS;
	Big sk_fs1, sk_fs2, sk_bs1, sk_bs2;

	G1 C1, C2, C3;
	Big r1, r2, r3;
	Big w1, w2;

	G1 T1, T2, T3;
	G1 IC1, IC2, IC3;

	LARGE_INTEGER freq;  
	LARGE_INTEGER start_t, stop_t;  
	double exe_time=0;  
	int counter = 1000;
	int i, j, number;
	QueryPerformanceFrequency(&freq); 

	pfc.random(P1);
	pfc.random(P2);
	pfc.random(sk_fs1);
	pfc.random(sk_fs2);
	pfc.random(sk_bs1);
	pfc.random(sk_bs2);
	//////
	pfc.random(r1);
	pfc.random(r2);
	///////////密钥产生//////////
	DS_KeyGen(PK_FS, PK_BS, sk_fs1, sk_fs2, sk_bs1, sk_bs2, P1, P2);
	number= 0;
	exe_time = 0;
	for (j = 0; j < 10; j++)
	{
		number = number + 50;
		exe_time = 0;
		for(i = 0; i < counter; i++)
		{
			pfc.random(w1);
			pfc.random(r1);
			QueryPerformanceCounter(&start_t); 
			DS_PEKS(C1, C2, C3, PK_FS, PK_BS, r1, w1, P1, P2);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
		}
		cout << "The performance of DS_PEKS (number = "<<number<<") = " <<(exe_time/counter)*number<< " ms" << endl;

		///////////////陷门////////////////
		exe_time = 0;
		for(i = 0; i < counter; i++)
		{
			pfc.random(r2);
			QueryPerformanceCounter(&start_t); 
			DS_Trapdoor(T1, T2, T3, PK_FS, PK_BS, r2, w1, P1, P2);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
		}
		cout << "The performance of DS_Trapdoor= (number = "<<number<<") = " <<(exe_time/counter)*number<< " ms" << endl;
		///////////测试一////////////
		exe_time = 0;
		for(i = 0; i < counter; i++)
		{
			pfc.random(r3);
			QueryPerformanceCounter(&start_t); 
			DS_FS_Test(IC1, IC2, IC3, C1, C2, C3, T1, T2, T3, r3, sk_fs1, sk_fs2);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
		}
		cout << "The performance of DS_Test1 (number = "<<number<<") = " <<(exe_time/counter)*number<< " ms" << endl;

		///////////测试一////////////
		exe_time = 0;
		for(i = 0; i < counter; i++)
		{
			QueryPerformanceCounter(&start_t); 
			DS_BS_Test(IC1, IC2, IC3, sk_bs1, sk_bs2);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
		}
		cout << "The performance of DS_Test2 (number = "<<number<<") = " <<(exe_time/counter)*number<< " ms" << endl;
		cout<<"***************"<<endl;
	}
}