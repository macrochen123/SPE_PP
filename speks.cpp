#include "speks.h"

void SPEKS_KeyGen(G1& pk_r, G1& pk_s, Big a, Big b, G1& P)
{
	pk_r = pfc.mult(P, a);
	pk_s = pfc.mult(P, b);
}

void SPEKS_PEKS(G1& c1, G1& c2, GT& c3, G1 pk_r, G1 pk_s, 
				Big sk_s, Big w, Big r, G1& P, GT GTPK, G1 Q)
{
	/************************/
	Big Btmp1, Btmp2;
	Big order = pfc.order();
	G1 tmp1, tmp2;
	GT GtTmp1;
	/************************/
	c1 = pfc.mult(P, r);
	c2 = pfc.mult(Q, r);//Q = kP
	////h(w)
	pfc.start_hash();
	pfc.add_to_hash(w);
	Btmp1 = pfc.finish_hash_to_group();
	///e(PK_s, PK_r)^(r*h(w))
	Btmp2 = modmult(r, Btmp1, order);
	c3 = pfc.power(GTPK, Btmp2);
}
void SPEKS_PEKS_II(SPEKS_Cipher& C, G1 pk_r, G1 pk_s, 
				   Big sk_s, Big w, Big r, G1& P, GT GTPK, G1 Q)
{
	/************************/
	Big Btmp1, Btmp2;
	Big order = pfc.order();
	G1 tmp1, tmp2;
	GT GtTmp1;
    /************************/
	C.C1 = pfc.mult(P, r);
	C.C2 = pfc.mult(Q, r);
	pfc.start_hash();
	pfc.add_to_hash(w);
	Btmp1 = pfc.finish_hash_to_group(); 
	Btmp2 = modmult(r, Btmp1, order);
	C.C3 = pfc.power(GTPK, Btmp2);
}
void SPEKS_Trapdoor(G1& t1, G1& t2, G1 pk_r, G1 pk_s, 
					Big sk_r, Big w, Big r, G1& P, G1 Q)
{
	/********************/
	Big Btmp1, Btmp2;
	Big order = pfc.order();
	G1 G1Tmp1, G1Tmp2;
	/********************/
	t1 = pfc.mult(P, r);
	G1Tmp2 = pfc.mult(Q, r);//rQ
	pfc.start_hash();
	pfc.add_to_hash(w);
	Btmp1 = pfc.finish_hash_to_group(); 
	Btmp2 = modmult(Btmp1, sk_r, order);
	G1Tmp1 = pfc.mult(pk_s, Btmp2);//h(w)sk_rPK_s
	t2 = G1Tmp1 +G1Tmp2;
}

BOOL SPEKS_Test(G1 t1, G1 t2, G1 c1, G1 c2, GT c3, G1& P)
{
	/************************/
	G1 tmp1, tmp2, tmp3;
	G1 ctmp1, ctmp2;
	GT GtE1, GtE2, GtE3;
	/************************/
	GtE1 = pfc.pairing(t1, c2);//E1
	GtE2 = pfc.pairing(c1, t2);//E2
	GtE3 = GtE2/GtE1;//E3
	if (GtE3 == c3)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL SPEKS_Test_II(G1 t1, G1 t2, SPEKS_Cipher C, G1& P)
{
	/*******************/
	G1 tmp1, tmp2, tmp3;
	G1 ctmp1, ctmp2;
	GT GtE1, GtE2, GtE3;
	/*******************/
	GtE1 = pfc.pairing(t1, C.C2);
	GtE2 = pfc.pairing(C.C1, t2);
	GtE3 = GtE2/GtE1;
	if (GtE3 == C.C3)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void SPEKS()
{
	/*************************************/
	G1 P;
	G1 pk_r, pk_s;
	Big sk_r, sk_s;
	Big w1, w2;
	G1 c1, c2;
	GT c3, GTPK;
	G1 t1, t2;
	Big r1, r2;
	Big key;
	G1 G1Tmp, Q;
	////////////////////////////
	LARGE_INTEGER freq;  
	LARGE_INTEGER start_t, stop_t;  
	double exe_time=0;  
	int counter = 1000;
	int i, j, number;
	/*************************************/
	QueryPerformanceFrequency(&freq); 
	/*KeyGen*/
	pfc.random(P);
	pfc.random(sk_r);
	pfc.random(sk_s);
	SPEKS_KeyGen(pk_r, pk_s, sk_r, sk_s, P);
	///////e(PK_r, PK_s)///////////
	GTPK = pfc.pairing(pk_r, pk_s);
	///Q = kP//
	G1Tmp = pfc.mult(pk_s, sk_r);
	pfc.start_hash();
	pfc.add_to_hash(G1Tmp);
	key = pfc.finish_hash_to_group();
	Q = pfc.mult(P, key);
	///////////////////
	number = 0;
	for(j = 0; j < 10; j++)
	{
		number = number + 50;
		exe_time = 0;
		for(i = 0; i < counter; i++)
		{
			pfc.random(r1);
			pfc.random(w1);
			QueryPerformanceCounter(&start_t); 
			SPEKS_PEKS(c1, c2, c3, pk_r, pk_s, sk_s, w1, r1, P, GTPK, Q);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
		}
		cout << "The performance of SPEKS_PEKS(number = "<<number<<") = " << (exe_time/counter)*number<< " ms" << endl;

		exe_time = 0;
		for(i = 0; i < counter; i++)
		{
			//pfc.random(w2);
			w2 = w1;
			pfc.random(r2);
			QueryPerformanceCounter(&start_t); 
			SPEKS_Trapdoor(t1, t2, pk_r, pk_s, sk_r, w2, r2, P, Q);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
		}
		cout << "The performance of SPEKS_Trapdoor(number = "<<number<<") = " << (exe_time/counter)*number<< " ms" << endl;

		exe_time = 0;
		for(i = 0; i < counter; i++)
		{
			QueryPerformanceCounter(&start_t); 
			SPEKS_Test(t1, t2, c1, c2, c3, P);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
		}
		cout << "The performance of SPEKS_Test(number = "<<number<<") = " << (exe_time/counter)*number<< " ms" << endl;
		cout<<"**************************"<<endl;
	}

}

void SPEKS_Test()
{	
	///////////////////////////////
	miracl *mip = get_mip();
	char* filename = "data.txt";
	int length = 2000;
	int i, j;
	int counter = 20;
	int index;
	int flag = 0;
	G1 P;
	G1 pk_r, pk_s;
	G1 t1[2000], t2[2000];
	G1 G1Tmp,Q;
	GT GTPK;
	Big sk_r, sk_s, key;
	Big W[2000];
	Big r[2000];
	SPEKS_Cipher C[2000];
	Big w;
	Big r2;
	//////////////
	LARGE_INTEGER freq;  
	LARGE_INTEGER start_t, stop_t;  
	double exe_time=0;  
	//////////////////////////
	QueryPerformanceFrequency(&freq); 
	mip->IOBASE = 256;
	Read_Data(W, filename, length);
	mip->IOBASE = 16;
	pfc.random(P);
	pfc.random(sk_r);
	pfc.random(sk_s);
	SPEKS_KeyGen(pk_r, pk_s, sk_r, sk_s, P);
	////
	GTPK = pfc.pairing(pk_r, pk_s);
	G1Tmp = pfc.mult(pk_s, sk_r);
	pfc.start_hash();
	pfc.add_to_hash(G1Tmp);
	key = pfc.finish_hash_to_group();
	Q = pfc.mult(P, key);
	///////////////
	for (i = 0; i < length; i++)
	{
		cout<<W[i]<<endl;
	}
	exe_time = 0;
	for (j = 0; j < counter; j++)
	{
		for (i  =0; i < length; i++)
		{
			pfc.random(r[i]);
			QueryPerformanceCounter(&start_t);
			SPEKS_PEKS_II(C[i], pk_r, pk_s, sk_s, W[i], r[i], P, GTPK, Q);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart; 
		}
	}
 	cout << "The performance of SPEKS_PEKS= " << exe_time/counter << " ms" << endl;

	exe_time = 0;
	for (j = 0; j < counter; j++)
	{
		for (i  =0; i < length; i++)
		{
			pfc.random(r2);
			QueryPerformanceCounter(&start_t);
			SPEKS_Trapdoor(t1[i], t2[i], pk_r, pk_s, sk_r, W[i], r2, P, Q);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart; 
		}
	}
	cout << "The performance of SPEKS_Trapdoor= " << exe_time/counter << " ms" << endl;
	exe_time = 0;
	for (j = 0; j < counter; j++)
	{
		index = rand() % length;
		for (i = 0; i < length; i++)
		{
			QueryPerformanceCounter(&start_t);
			flag = SPEKS_Test_II(t1[index], t2[index], C[i], P);
			QueryPerformanceCounter(&stop_t);  
			exe_time += 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart; 
			if (flag)
			{
				break;
			}
		}	
	}
	cout << "The performance of SPEKS_Test= " << exe_time/counter << " ms" << endl;

}
////////////////////////////////////
void Read_Data(Big* KW, char* filename, int length)
{
	int i = 0;
	ifstream reader;
	char keywordspace[5000][50];
	reader.open(filename, ios::ate);
	reader.seekg(0, ios::beg);
	for(i = 0; i < length; i++)
	{
		reader.getline(keywordspace[i], 50);
		cout<< keywordspace[i]<< endl;
		KW[i] = (Big)keywordspace[i];
	}
}
