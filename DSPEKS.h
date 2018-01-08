#ifndef	__HEADER_DSPEKS_H__
#define	__HEADER_DSPEKS_H__

//********* CHOOSE JUST ONE OF THESE **********
#define MR_PAIRING_SSP    // AES-80 or AES-128 security GF(p) curve
#define AES_SECURITY 80
//*********************************************
#include "pairing_1.h"
#include <ctime>
#include <windows.h>

extern PFC pfc;
static unsigned char P1[] = {0x01};

void DS_KeyGen(G1& PK_FS, G1& PK_BS, Big sk_fs1, Big sk_fs2, 
			   Big sk_bs1, Big sk_bs2, G1 P1, G1 P2);

void DS_PEKS(G1& C1, G1& C2, G1& C3, G1 PK_FS, G1 PK_BS, 
			 Big r, Big w, G1 P1, G1 P2);

void DS_Trapdoor(G1& T1, G1& T2, G1& T3, G1 PK_FS, G1 PK_BS,
				 Big r, Big w, G1 P1, G1 P2);

void DS_FS_Test(G1& IC1, G1& IC2, G1& IC3, G1 C1, G1 C2, G1 C3, 
				G1 T1, G1 T2, G1 T3, Big r, Big sk_fs1, Big sk_fs2);

BOOL DS_BS_Test(G1 IC1, G1 IC2, G1 IC3, Big sk_bs1, Big sk_bs2);

void DSPEKS();

#endif