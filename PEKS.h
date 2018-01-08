#ifndef	__HEADER_PEKS_H__
#define	__HEADER_PEKS_H__

//********* CHOOSE JUST ONE OF THESE **********
#define MR_PAIRING_SSP    // AES-80 or AES-128 security GF(p) curve
#define AES_SECURITY 80
//*********************************************
#include "pairing_1.h"
#include <ctime>
#include <windows.h>

extern PFC pfc;
static unsigned char P2[] = {0x01};

void PEKS_KeyGen(G1& PK_r, Big sk_r, G1 P);

void PEKS_PEKS(G1& C1, Big& BC2, G1 PK_r, G1 P, Big w, Big r);

void PEKS_Trapdoor(G1& T, Big w, Big sk_r);

BOOL PEKS_Test(G1 C1, Big BC2, G1 T);

void PEKS();

#endif