#include "app.h"
ST_cardData_t cardData;
ST_terminalData_t terminalData;
ST_transaction_t transactionData;
EN_terminalError_t terminalReturn;
EN_serverError_t serverReturn;
EN_cardError_t cardReturn;


void main()
{
	appStart();
};

void appStart(void)
{
	
	printf("Reading Data Base Acounts ... \n");
	getDataBaseAcounts();
	printf("Data Base Acounts Succesfully loaded.");


	printf("\n\n================================================\n\n");

	
	cardReturn = getCardHolderName(&cardData);
	while (cardReturn == WRONG_NAME)
	{
		printf("\nYou Entered Wrong Name.\n");
		printf("Please Re Enter Your Name :\n");
		cardReturn = getCardHolderName(&cardData);
	}

	cardReturn = getCardExpiryDate(&cardData);
	while (cardReturn == WRONG_EXP_DATE)
	{
		printf("\nYou Entered Wrong EXP Date.\n");
		printf("Please Re Enter Your EXP Date :\n");
		cardReturn = getCardExpiryDate(&cardData);
	}


	cardReturn = getCardPAN(&cardData);
	while (cardReturn == WRONG_PAN)
	{
		printf("\nYou Entered Wrong PAN Number.\n");
		printf("Please Re Enter Your PAN Number :\n");
		cardReturn = getCardPAN(&cardData);
	}

	printf("\n================================================\n\n");


	terminalReturn = getTransactionDate(&terminalData);
	while (terminalReturn == WRONG_DATE)
	{
		printf("\nWrong Transaction Date.\n");
		printf("Again ...\n");
		terminalReturn = getTransactionDate(&cardData);
	}

	terminalReturn = isCardExpired(cardData, terminalData);
	if (terminalReturn == EXPIRED_CARD)
	{
		printf("Sorry. \nYour Card Expired !\n");
		return 0;
	}


	terminalReturn = setMaxAmount(&terminalData);
	while (terminalReturn == INVALID_AMOUNT)
	{
		printf("\nInvalid Amount.\n");
		printf("Again ...\n");
		terminalReturn = setMaxAmount(&terminalData);
	}


	terminalReturn = getTransactionAmount(&terminalData);
	while (terminalReturn == INVALID_AMOUNT)
	{
		printf("\nInvalid Amount.\n");
		printf("Again ...\n");
		terminalReturn = getTransactionAmount(&terminalData);
	}


	terminalReturn = isBelowMaxAmount(&terminalData);
	if (terminalReturn == EXCEED_MAX_AMOUNT)
	{
		printf("\n\tDeclined \nExceed Max Balance Aount\n");
		return;
	}


	transactionData.cardHolderData = cardData;
	transactionData.terminalData = terminalData;

	serverReturn = recieveTransactionData(&transactionData);
	if (serverReturn == DECLINED_STOLEN_CARD)
	{
		printf("\nDECLINED\nSTOLEN CARD\nINVALID ACCOUNT\n");
	}

	else if (serverReturn == DECLINED_INSUFFECIENT_FUND)
	{
		printf("\nDECLINED\nINSUFFECIENT FUND\n");
	}
	else if (serverReturn == APPROVED)
	{
		printf("\n================================================");
		printf("\n\nTRANSACTION APPROVED\n");
		printf("Updating Data Base Accounts ...\n");
		updateDataBaseAcounts();
		printf("Saving Transaction ...\n");
	}
	printf("\n================================================\n");
	

}

