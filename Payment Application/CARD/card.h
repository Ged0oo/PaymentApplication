#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED


/* ---------- SECTION INCLUDES ---------- */

#include "../APPLICATION/std_types.h"
#include <string.h>

/* ---------- SECTION DATATYPE DECLARATION ---------- */


typedef struct ST_cardData_t
{
	uint8_t cardHolderName[25];
	uint8_t primaryAccountNumber[20];
	uint8_t cardExpirationDate[6];
}ST_cardData_t;


typedef enum EN_cardError_t
{
	cardOk = 0, 
	WRONG_NAME, 
	WRONG_EXP_DATE, 
	WRONG_PAN
}EN_cardError_t;

typedef enum EN_cardState_t
{
	BLOCKED = 0,
	RUNNING
}EN_cardState_t;


/* ---------- SECTION FUNCTIONS PROTOTYPES ---------- */
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData);
EN_cardError_t getCardPAN(ST_cardData_t* cardData);
EN_cardError_t getCardHolderName(ST_cardData_t* cardData);
int LuhnNumberChecker(uint8_t* cardPan);
uint8_t* LuhnNumberGenerator();

#endif // CARD_H_INCLUDED
