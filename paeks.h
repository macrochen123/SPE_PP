#ifndef	__HEADER_PAEKS_H__
#define	__HEADER_PAEKS_H__

//********* CHOOSE JUST ONE OF THESE **********
#define MR_PAIRING_SSP    // AES-80 or AES-128 security GF(p) curve
#define AES_SECURITY 80
//*********************************************
#include "pairing_1.h"
#include <ctime>
#include <windows.h>

extern PFC pfc;

G1 hash_and_map(Big w, Big cof);
void PAEKS_KeyGen(G1& pk_s, G1& pk_r, Big sk_s, Big sk_r, G1& P);
void PAEKS_PEKS(G1& c1, G1& c2, G1 pk_r, Big sk_s, Big w, Big r1, G1& P);
void PAEKS_Trapdoor(GT& t, G1 pk_s, Big sk_r, Big w, Big r2, G1& P);
BOOL PAEKS_Test(GT t, G1 c1, G1 c2, G1 pk_s, G1 pk_r, G1& P);

void PAEKS();

#endif