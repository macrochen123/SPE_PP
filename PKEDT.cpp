#include "PKEDT.h"

void G1_To_G1_Map(G1& des, G1 source, G1 P)
{
	Big tmp;
	pfc.start_hash();
	pfc.add_to_hash(source);
	tmp = pfc.finish_hash_to_group();
	des = pfc.mult(P, tmp);
}

void PKEDT_KeyGen(G1& PK_S1, G1& PK_S2, Big sk_s, G1& PK_R1,
				  G1& PK_R2, Big sk_r, G1 P, G1 U1, G1 U2)
{
	Big inv_sk;
	Big order;
	order = pfc.order();
	inv_sk = inverse(sk_s, order);
	PK_S1 = pfc.mult(P, sk_s);
	PK_S2 = pfc.mult(U1, inv_sk);
	//////////////////////
	PK_R1 = pfc.mult(P, sk_r);
	PK_R2 = pfc.mult(U2, sk_r);
}
/////////////////
void PKEDT_PEKS(G1& C1, Big& BC2, G1 PK_S1, G1 PK_S2, 
				G1 PK_R1, G1 PK_R2, Big w, Big r)
{
	G1 tmp1, tmp2;
	GT GtTmp;
	C1 = pfc.mult(PK_R1, r);
	///////////
	tmp1 = pfc.Big_To_map(w, *pfc.cof);
	tmp2 = pfc.mult(tmp1, r);
	GtTmp = pfc.pairing(PK_S1, tmp2);//e(pk_s1, h(w));
	pfc.start_hash();
	pfc.add_to_hash(GtTmp);
	BC2 = pfc.finish_hash_to_group();
}

void PKEDT_Trapdoor(G1& T1, G1& T2, G1 PK_S1, Big sk_r,
					Big w, Big r, G1 P)
{
	Big inv_sk;
	Big order = pfc.order();
	G1 tmp1, tmp2, tmp3;
	inv_sk = inverse(sk_r, order);
	T1 = pfc.mult(P, r);
	////////////////
	tmp1 = pfc.Big_To_map(w, *pfc.cof);
	tmp2 = pfc.mult(tmp1, inv_sk);
	tmp3 = pfc.mult(PK_S1, r);
	G1_To_G1_Map(tmp3, tmp3, P);
	T2 = tmp2 + tmp3;
}

BOOL PKEDT_Test(G1 T1, G1 T2, G1 C1, Big BC2, Big sk_s, G1 P)
{
	G1 tmp1, tmp2;
	GT GtTmp;
	Big BT;
	tmp1 = pfc.mult(T1, sk_s);
	G1_To_G1_Map(tmp1, tmp1, P);
	tmp2 = T2 + (-tmp1);
	tmp2 = pfc.mult(tmp2, sk_s);

	GtTmp = pfc.pairing(C1, tmp2);

	pfc.start_hash();
	pfc.add_to_hash(GtTmp);
	BT = pfc.finish_hash_to_group();
	if (BT == BC2)
	{
		return TRUE;
	}
	return FALSE;
}

void PKEDT()
{
	G1 PK_S1, PK_S2;
	G1 PK_R1, PK_R2;
	Big sk_s, sk_r;
	G1 P, U1, U2;
	G1 C1;
	Big BC2;
	Big w1,w2;
	Big r1, r2;
	G1 T1, T2;
	LARGE_INTEGER freq;  
	LARGE_INTEGER start_t, stop_t;  
	double exe_time=0;  
	int counter = 1000;
	int i,j,number;
	QueryPerformanceFrequency(&freq); 
	pfc.random(sk_s);
	pfc.random(sk_r);
	pfc.random(P);
	pfc.random(U1);
	pfc.random(U2);
	pfc.random(r1); 
	pfc.random(r2); 
	///////////密钥产生////////////
	PKEDT_KeyGen(PK_S1, PK_S2, sk_s, PK_R1, PK_R2, sk_r, P, U1, U2);
	///////////加密产生////////////
	number = 0;
	for (j = 0; j < 10; j++)
	{
		number = number + 50;
		exe_time = 0;
		for(i = 0; i < counter; i++)
		{
			pfc.random(w1);
			pfc.random(r1);
			QueryPerformanceCounter(&start_t); 
			PKEDT_PEKS(C1, BC2, PK_S1, PK_S2, PK_R1, PK_R2, w1, r1); 
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
		}
		cout << "The performance of PKEDT_PEKS (number = "<<number<<") = " <<(exe_time/counter)*number<< " ms" << endl;

		///////////陷门产生////////////
		exe_time = 0;
		for(i = 0; i < counter; i++)
		{
			pfc.random(w2);
			pfc.random(r2);
			QueryPerformanceCounter(&start_t); 
			PKEDT_Trapdoor(T1, T2, PK_S1, sk_r, w2, r2, P);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
		}
		cout << "The performance of PKEDT_Trapdoor(number = "<<number<<") = " <<(exe_time/counter)*number<< " ms" << endl;

		///////////测试产生////////////
		exe_time = 0;
		for(i = 0; i < counter; i++)
		{
			QueryPerformanceCounter(&start_t); 
			PKEDT_Test(T1, T2, C1, BC2, sk_s, P);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
		}
		cout << "The performance of PKEDT_Test(number = "<<number<<") = " <<(exe_time/counter)*number<< " ms" << endl;
		cout <<"************************************"<<endl;
	}

	
}