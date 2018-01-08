#ifndef	__HEADER_PKEDT_H__
#define	__HEADER_PKEDT_H__

//********* CHOOSE JUST ONE OF THESE **********
#define MR_PAIRING_SSP    // AES-80 or AES-128 security GF(p) curve
#define AES_SECURITY 80
//*********************************************
#include "pairing_1.h"
#include <ctime>
#include <windows.h>

extern PFC pfc;
static unsigned char P[] = {0x01};

void G1_To_G1_Map(G1& des, G1 source, G1 P);

void PKEDT_KeyGen(G1& PK_S1, G1& PK_S2, Big sk_s, G1& PK_R1,
				  G1& PK_R2, Big sk_r, G1 P, G1 U1, G1 U2);

void PKEDT_PEKS(G1& C1, Big& BC2, G1 PK_S1, G1 PK_S2, 
				G1 PK_R1, G1 PK_R2, Big w, Big r); 

void PKEDT_Trapdoor(G1& T1, G1& T2, G1 PK_S1, Big sk_r,
					Big w, Big r, G1 P);

BOOL PKEDT_Test(G1 T1, G1 T2, G1 C1, Big BC2, Big sk_s, G1 P);

void PKEDT();

#endif