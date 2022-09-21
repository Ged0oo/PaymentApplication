#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED


/* ---------- SECTION INCLUDES ---------- */

#include "../CARD/card.h"
#include "../APPLICATION/std_types.h"
#include<stdio.h>
#include <time.h>
#define MAX_TRANSACTION_AMOUNT			5000

/* ---------- SECTION DATATYPE DECLARATION ---------- */

typedef struct ST_terminalData_t
{
	float transAmount;
	float maxTransAmount;
	uint8_t transactionDate[6];
}ST_terminalData_t;


typedef enum EN_terminalError_t
{
	terminalOk,
	WRONG_DATE,
	EXPIRED_CARD,
	INVALID_CARD,
	INVALID_AMOUNT,
	EXCEED_MAX_AMOUNT,
	INVALID_MAX_AMOUNT
}EN_terminalError_t;


/* ---------- SECTION FUNCTIONS PROTOTYPES ---------- */

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData);
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData);

#endif // TERMINAL_H_INCLUDED
