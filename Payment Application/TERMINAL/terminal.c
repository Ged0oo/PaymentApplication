#include "terminal.h"

/*

	This function will ask for the transaction data and store it in terminal data.
	Transaction date is 6 characters string in the format MM/YY, e.g 06/22.
	If the transaction date is NULL, less than 6 characters or wrong format will return WRONG_DATE error, else return OK.
	Optional:
	The function will read the current date from your computer and store it into terminal data with the mentioned size and format.

*/
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	uint8_t currentMonth, currentYear;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	currentYear = tm.tm_year - 100;//100 becouse tm_year returns the current year relative to 1900 on all POSIX-compliant platforms
	currentMonth = tm.tm_mon + 1;//tm_mon starting from 0 not 1 this is why we add 1 to it

	termData->transactionDate[0] = '0' + (currentMonth / 10);
	termData->transactionDate[1] = '0' + (currentMonth % 10);

	termData->transactionDate[2] = '/';

	termData->transactionDate[3] = '0' + (currentYear % 100)/10;
	termData->transactionDate[4] = '0' + (currentYear % 10);

	termData->transactionDate[5] = '\0';	
	return terminalOk;
}


/*

	This function compares the card expiry date with the transaction date.
	If the card expiration date is before the transaction date will return EXPIRED_CARD, else return OK.

*/
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
	uint8_t cardMonth, cardYear, systemMonth, systemYear;
	cardMonth = ((cardData.cardExpirationDate[0] - '0') * 10) + (cardData.cardExpirationDate[1] - '0');
	cardYear = ((cardData.cardExpirationDate[3] - '0') * 10) + (cardData.cardExpirationDate[4] - '0');

	systemMonth = ((termData.transactionDate[0] - '0') * 10) + (termData.transactionDate[1] - '0');
	systemYear = ((termData.transactionDate[3] - '0') * 10) + (termData.transactionDate[4] - '0');


	if (systemYear > cardYear)
	{
		return EXPIRED_CARD;
	}
	else if ((systemYear == cardYear) && (systemMonth > cardMonth))
	{
		return EXPIRED_CARD;
	}
	return terminalOk;
}


/*

	This function checks if the PAN is a Luhn number or not.
	If PAN is not a Luhn number will return INVALID_CARD, else return OK.

*/
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	char cardPan[20];
	uint8_t checkDigit, intialIndex = 0, digit = 0, sum = 0, len = 0, i=0;

	for (len ; cardData->primaryAccountNumber[len] != '\0' ; len++)
	{
		cardPan[len] = cardData->primaryAccountNumber[len];
	}

	cardPan[len] = '\0';
	checkDigit = cardPan[len - 1] - '0';
	intialIndex = len - 2;	

	//771581585647138968
	
	for (uint8_t i = intialIndex ; i != 0 ; i -= 2)
	{
		if (i <= -1)
		{
			break;
		}
		else
		{
			digit = cardPan[i] - '0';
			digit *= 2;

			if (digit < 10)
			{
				cardPan[i] = '0' + digit;
			}
			else
			{
				cardPan[i] = '0' + digit - 9;
			}
		}
	}

	if (i == 0)
	{
		digit = cardPan[i] - '0';
		digit *= 2;
		if (digit < 10) cardPan[i] = '0' + digit;
		else cardPan[i] = '0' + digit - 9;
	}

	for (uint8_t i = 0 ; i <= intialIndex ; i++)
	{
		sum += cardPan[i] - '0';
	}	

	if ((sum+checkDigit)%10)
	{
		return INVALID_CARD;
	}
	else
	{
		return terminalOk;
	}
}


/*

	This function asks for the transaction amount and saves it into terminal data.
	If the transaction amount is less than or equal to 0 will return INVALID_AMOUNT, else return OK.
	You should deliver a maximum 2min video to discuss your implementation and run different test cases on this function

*/
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	float amount = -1;
	printf("Enter the Transaction Amount : ");
	scanf("%f", &amount);

	if (amount <= 0)
	{
		return INVALID_AMOUNT;
	}
	else
	{
		termData->transAmount = amount;
	}

	return terminalOk;
}



/*

	This function compares the transaction amount with the terminal max amount.
	If the transaction amount is larger than the terminal max amount will return EXCEED_MAX_AMOUNT, else return OK.

*/
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > termData->maxTransAmount)
	{
		return EXCEED_MAX_AMOUNT;
	}
	else
	{
		return terminalOk;
	}
}


/*

	This function sets the maximum allowed amount into terminal data.
	Transaction max amount is a float number.
	If transaction max amount less than or equal to 0 will return INVALID_MAX_AMOUNT error, else return OK.

*/
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	if (MAX_TRANSACTION_AMOUNT <= 0)
	{
		return INVALID_MAX_AMOUNT;
	}
	else
	{
		termData->maxTransAmount = MAX_TRANSACTION_AMOUNT;
		return terminalOk;
	}
}
