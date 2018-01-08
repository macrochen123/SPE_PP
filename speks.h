#ifndef	__HEADER_SPEKS_H__
#define	__HEADER_SPEKS_H__

//********* CHOOSE JUST ONE OF THESE **********
#define MR_PAIRING_SSP    // AES-80 or AES-128 security GF(p) curve
#define AES_SECURITY 80
//*********************************************
#include "pairing_1.h"
#include <ctime>
#include <windows.h>
#include <fstream>

extern PFC pfc;
typedef struct _CIPHER
{
	G1 C1, C2;
	GT C3;
}SPEKS_Cipher;

void SPEKS_KeyGen(G1& pk_r, G1& pk_s, Big a, Big b, G1& P);
void SPEKS_PEKS(G1& c1, G1& c2, GT& c3, G1 pk_r, G1 pk_s,  Big sk_s, Big w, Big r, G1& P, GT GTPK, G1 Q);
void SPEKS_Trapdoor(G1& t1, G1& t2, G1 pk_r, G1 pk_s, Big sk_r, Big w, Big r, G1& P, G1 Q);
BOOL SPEKS_Test(G1 t1, G1 t2, G1 c1, G1 c2, GT c3, G1& P);
void SPEKS();

void SPEKS_PEKS_II(SPEKS_Cipher& C, G1 pk_r, G1 pk_s,  Big sk_s, Big w, Big r, G1& P, GT GTPK, G1 Q);
BOOL SPEKS_Test_II(G1 t1, G1 t2, SPEKS_Cipher C, G1& P);
void SPEKS_Test();
void Read_Data(Big* KW, char* filename, int length);
#endif


