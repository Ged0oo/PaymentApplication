#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

/* ---------- SECTION INCLUDES ---------- */

#include "../TERMINAL/terminal.h"
#include "../CARD/card.h"
#include "../APPLICATION/std_types.h"


/* ---------- SECTION DATATYPE DECLARATION ---------- */

typedef enum EN_transState_t
{
	APPROVED,
	DECLINED_INSUFFECIENT_FUND,
	DECLINED_STOLEN_CARD,
	INTERNAL_SERVER_ERROR
}EN_transState_t;

typedef enum EN_serverError_t
{
	serverOk,
	SAVING_FAILED,
	TRANSACTION_NOT_FOUND,
	ACCOUNT_NOT_FOUND,
	LOW_BALANCE,
	BLOCKED_ACCOUNT,
	RUNNING_ACCOUNT
}EN_serverError_t;


typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transState_t transState;
	uint32_t transactionSequenceNumber;
}ST_transaction_t;


typedef struct ST_accountsDB_t
{
	float balance;
	uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;

typedef struct ST_arrayDB_t
{
	float balance;
	EN_cardState_t state;
	uint8_t primaryAccountNumber[12];
}ST_arrayDB_t;


/* ---------- SECTION FUNCTIONS PROTOTYPES ---------- */

EN_transState_t recieveTransactionData(ST_transaction_t *transData);
EN_serverError_t isValidAccount(ST_cardData_t* cardData);
EN_serverError_t isBlockedAccount(ST_cardData_t* cardData);
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData);
EN_serverError_t saveTransaction(ST_transaction_t* transData);
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData);
void getDataBaseAcounts(void);
void updateDataBaseAcounts(void);



#endif // SERVER_H_INCLUDED
