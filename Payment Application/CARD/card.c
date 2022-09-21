#include "card.h"


/*

	Ask for cardholder's name and store it into card data
	Card holder name is 24 characters string max and 20 min
	if cardholder name is null, less than 20 cahracters or more than 24 will return
	WRONG_NAME error, else return OK

*/
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	char name[25];
	for (uint8_t i = 0 ; i < 25 ; i++)
	{
		name[i] = '\0';
	}

	printf("Please Enter Your Name [20-24] characters : ");	
	char ch;
	uint8_t i = 0;
	while ((ch = getchar()) != '\n')
	{
		name[i] = ch;
		i++;
	}

	int len = strlen(name);
	if (!name) return WRONG_NAME;
	if (len > 24 || len < 20)
	{
		return WRONG_NAME;
	}
	else
	{
		for (uint8_t i = 0; i <= len; i++)
		{
			cardData->cardHolderName[i] = name[i];
		}		
		return cardOk;
	}
}



/*

	will ask for the card expiry dateand store it in card data.
	Card expiry date is 5 characters string in the format "MM/YY", e.g "05/25".
	If the card expiry date is NULL, less or more than 5 characters, or has the wrong format will return WRONG_EXP_DATE error, else return OK.

*/
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{

	char expDate[100];
	uint8_t cardMonth, cardYear;
	for (uint8_t i = 0; i < 100; i++)
	{
		expDate[i] = '\0';
	}

	printf("Please Enter Your Card Expiry Date in [MM/YY] Formate : ");
	fseek(stdin, 0, SEEK_END);
	scanf("%s", expDate);

	if (!expDate)
	{
		return WRONG_EXP_DATE;
	}
	else if (expDate[6] != '\0' || expDate[3] == '\0')//  11/24    124
	{
		return WRONG_EXP_DATE;
	}
	else if (expDate[2] != '/')//   1114
	{
		return WRONG_EXP_DATE;
	}

	cardMonth = (expDate[0] - '0') * 10 + (expDate[1] - '0');
	cardYear  = (expDate[3] - '0') * 10 + (expDate[4] - '0');

	if ((cardMonth > 12 || cardMonth < 1) || (cardYear < 0 || cardYear > 99))
	{
		return WRONG_EXP_DATE;
	}

	for (uint8_t i = 0 ; i < 6 ; i++)
	{
		cardData->cardExpirationDate[i] = expDate[i];
	}

	return cardOk;
}


/*

	This function will ask for the card's Primary Account Number and store it in card data.
	PAN is 20 characters alphanumeric only string 19 character max, and 16 character min.
	If the PAN is NULL, less than 16 or more than 19 characters, will return WRONG_PAN error, else return OK.

	Give PAN that is a Luhn number, Luhn number generator, and checker

*/

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	char panNumber[30];
	for (uint8_t i = 0; i < 30; i++)
	{
		panNumber[i] = '\0';
	}

	printf("Please Enter Your PAN Number [16-19] characters : ");

	char ch;
	uint8_t i = 0;
	fseek(stdin, 0, SEEK_END);
	while ((ch = getchar()) != '\n')
	{
		panNumber[i] = ch;
		i++;
	}

	int len = strlen(panNumber);
	if (!panNumber) return WRONG_PAN;
	if (len > 19 || len < 16)
	{
		return WRONG_PAN;
	}
	else
	{
			for (uint8_t i = 0; i <= len; i++)
			{
				cardData->primaryAccountNumber[i] = panNumber[i];
			}
			return cardOk;
	}	
}


