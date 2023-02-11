#include "server.h"

uint8_t index = -1;
ST_accountsDB_t DataBaseAcounts[255];

  
/* 

Instead of a global array create a text file "Transactions DB.txt" where you will save all transactions and read if you need

*/


 
void getDataBaseAcounts(void)
{
	FILE* ptr;
	ptr = fopen("../DataBaseAcounts.txt", "r");

	if (!ptr)
	{
		printf("Error! \nCorrupted File.\n");
		return;
	}
	else
	{
		printf("DataBaseAcounts.txt\n");
	}

	uint8_t pan[20], i=0, j;
	float amount;

	while ( fscanf(ptr, "%f %s", &amount, pan) )
	{
		if (i > 0 && DataBaseAcounts[i - 1].balance == amount && DataBaseAcounts[i - 1].primaryAccountNumber[6] == pan[6])
		{
			break;
		}

		DataBaseAcounts[i].balance = amount;

		for (j = 0; j < 20; j++)
		{
			DataBaseAcounts[i].primaryAccountNumber[j] = pan[j];
		}

		i++;
	}
	fclose(ptr);
}



void updateDataBaseAcounts(void)
{
	FILE* ptr;
	ptr = fopen("../DataBaseAcounts.txt", "w");

	if (!ptr)
	{
		printf("Error! \nCorrupted File.\n");
		return;
	}
	else
	{
		printf("DataBaseAcounts.txt\n");
	}

	for (uint8_t i = 0 ; i < 255 ; i++)
	{
		if (DataBaseAcounts[i].primaryAccountNumber[0] == '\0')
		{
			break;
		}
		fprintf(ptr, "%f %s\n", DataBaseAcounts[i].balance, DataBaseAcounts[i].primaryAccountNumber);
	}
}



/*

	This function will take card data and validate these data.
	It checks if the PAN exists or not in the server's database.
	If the PAN doesn't exist will return DECLINED_STOLEN_CARD, else will return OK

*/
EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	//771581585647138968
	uint8_t *panNumber = cardData->primaryAccountNumber; 
	uint8_t flage = 0;//not found

	for (uint8_t i = 0 ; i < 255 ; i++)//loopo on data base content
	{
		if (DataBaseAcounts[i].primaryAccountNumber[0] != '\0')//check for the last of the file
		{
			uint8_t found = 1;//may be found
			for (uint8_t j = 0 ; j < 20 ; j++)
			{
				//771581585647138968
				//771581585647138968
				if (DataBaseAcounts[i].primaryAccountNumber[j] != panNumber[j])
				{
					found = 0;//exaxtly not found
					break;
				}
			}

			if (found)
			{
				flage = 1;
				index = i;
				break;
			}
		}

		else
		{
			break;
		}
	}


	if (!flage)
	{
		return ACCOUNT_NOT_FOUND;
	}
	else
	{
		return serverOk;
	}
}




/*

This function will take all transaction data into the transactions database.
It gives a sequence number to a transaction, this number is incremented once a transaction is processed into the server.
If saves any type of transaction, APPROVED or DECLINED, with the specific reason for declining/transaction state.
If transaction can't be saved will return SAVING_FAILED, else will return OK

*/
EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	FILE* ptr;
	ptr = fopen("../DataBaseInfo.txt", "r");

	if (!ptr)
	{
		printf("Error! \nCorrupted File.\n");
		return SAVING_FAILED;
	}

	
	uint32_t totalTransactions;
	fscanf(ptr, "%d", &totalTransactions);
	fclose(ptr);

	ptr = fopen("../DataBaseInfo.txt", "w");
	fprintf(ptr, "%d", totalTransactions + 1);
	fclose(ptr);

	transData->transactionSequenceNumber = totalTransactions + 1;
	ptr = fopen("../DataBaseTransactions.txt", "a");

	if (!ptr)
	{
		return SAVING_FAILED;
	}

	fprintf(ptr, "\n===========================================================================================\n");
	fprintf(ptr, "Transaction : \n");
	fprintf(ptr, "\tCard Holder Name : %s\n", transData->cardHolderData.cardHolderName);
	fprintf(ptr, "\tPAN : %s", transData->cardHolderData.primaryAccountNumber);
	fprintf(ptr, "\tEXP Date : %s\n", transData->cardHolderData.cardExpirationDate);
	fprintf(ptr, "\tAmount : %.2f", transData->terminalData.transAmount);
	fprintf(ptr, "\tMaximum Amount : %.2f", transData->terminalData.maxTransAmount);
	fprintf(ptr, "\tTransaction State : ");

	if (transData->transState == APPROVED)
	{
		fprintf(ptr, "APPROVED\n");
	}
	else if (transData->transState == DECLINED_INSUFFECIENT_FUND)
	{
		fprintf(ptr, "DECLINED_INSUFFECIENT_FUND\n");
	}
	else if (transData->transState == DECLINED_STOLEN_CARD)
	{
		fprintf(ptr, "DECLINED_STOLEN_CARD\n");
	}

	fprintf(ptr, "\tTransaction Sequence Number : %d\n\n", transData->transactionSequenceNumber);
	fprintf(ptr, "===========================================================================================\n");
	fclose(ptr);
	
	return serverOk;
}


/*

This function will take a transaction sequence number and search for this transaction in the database.
If the transaction can't be found will return TRANSACTION_NOT_FOUND, else will return OK and store the transaction in a structure

*/
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	return serverOk;
}



/*

	This function will take all transaction data and validate its data.
	It checks the account details and amount availability.
	If the account does not exist return DECLINED_STOLEN_CARD, if the amount is not available will return DECLINED_INSUFFECIENT_FUND, if a transaction can't be saved will return INTERNAL_SERVER_ERROR and will not save the transaction, else returns APPROVED.
	It will update the database with the new balance.

*/
EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	if (isValidAccount(&transData->cardHolderData) == ACCOUNT_NOT_FOUND)
	{
		transData->transState = DECLINED_STOLEN_CARD;
		return DECLINED_STOLEN_CARD;
	}

	if (isBlockedAccount(&transData->cardHolderData) == BLOCKED_ACCOUNT)
	{
		transData->transState = DECLINED_STOLEN_CARD;
		return DECLINED_STOLEN_CARD;
	}

	if (isAmountAvailable(&transData->terminalData) == LOW_BALANCE)
	{
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		return DECLINED_INSUFFECIENT_FUND;
	}
	
	transData->transState = APPROVED;
	DataBaseAcounts[index].balance -= transData->terminalData.transAmount;
	
	if (saveTransaction(transData) == SAVING_FAILED)
	{
		return INTERNAL_SERVER_ERROR;
	}
	
	return APPROVED;
}


/*

This function will take terminal data and validate these data.
It checks if the transaction's amount is available or not.
If the transaction amount is greater than the balance in the database will return LOW_BALANCE, else will return OK

*/
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{

	printf("\nTransaction Amount = %.2f\t\t", termData->transAmount);
	printf("Maximum Amount = %.2f\t\t", termData->maxTransAmount);
	printf("Available Amount = %.2f\n", DataBaseAcounts[index].balance);

	if ((DataBaseAcounts[index].balance) < (termData->transAmount))
	{
		return LOW_BALANCE;
	}
	else
	{
		return serverOk;
	}
}




EN_serverError_t isBlockedAccount(ST_cardData_t* cardData)
{

	uint8_t* panNumber = cardData->primaryAccountNumber;
	uint8_t* string[50];
	uint8_t flage = -1;//not found


	FILE* ptr;
	ptr = fopen("../BlockedAcounts.txt", "r");

	if (!ptr)
	{
		printf("Error! \nCorrupted File.\n");
		return SAVING_FAILED;
	}

	while (!feof(ptr))
	{
		fscanf(ptr, "%s", string);
		if ( !strcmp(string, panNumber) )
		{
			flage = 1;
			break;			
		}
		else
		{
			flage = 0;
		}	
	}

	if (flage)
	{
		return BLOCKED_ACCOUNT;
		
	}
	else
	{
		return RUNNING_ACCOUNT;
	}
}
