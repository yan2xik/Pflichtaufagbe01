// Pflichtaufagbe01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <math.h>

void printTaxTable(int maxIncome, int minIncome, int stepSize, int taxClassInt);
float calculateTax(int taxClass, int income);
void printTable(float* tableData, char* headerData, int row, int column);

struct taxBracket { //
	int upperLimit; //this is technically just the width of the bracket so if you want a bracket from 50000 to 75000 you need to set this to 25000 they also need to be ascending in value
	float rate; //rate not in percent
};
struct taxClass {
	int deductable;
};
//declare all valid to be used tax brackets, declare last with int_max //static is implied
struct taxBracket TAX_BRACKETS[] = { {.upperLimit = 12000, .rate = 0.15}, {.upperLimit = 28000-12000, .rate = 0.22},{.upperLimit = 50000-28000, .rate = 0.32}, {.upperLimit = _CRT_INT_MAX, .rate = 0.45} };
struct taxClass TAX_CLASSES[] = { {.deductable = 10000}, {.deductable = 20000} };

int main()
{
	int minIncome = -1;
	int maxIncome = -1;
	int stepSize = -1;
	int taxClassInt = -1;
	bool inputOK = 0;
	while (inputOK == 0) {
		printf_s("Geben sie das Mindesteinkommen ein: ");
		scanf_s("%d", &minIncome);
		printf_s("Geben sie das Maximaleinkommen ein: ");
		scanf_s("%d", &maxIncome);
		printf_s("Geben sie die Schrittweite ein: ");
		scanf_s("%d", &stepSize);
		printf_s("Geben sie die Steuerklasse ein: ");
		scanf_s("%d", &taxClassInt);

		if (minIncome == -1 || maxIncome == -1 || stepSize == -1 || taxClassInt == -1) {
			scanf_s("%*s"); //clears input buffer
			printf("Eine oder mehere Eingaben waren ungültig\n");
		}
		else if (minIncome > maxIncome || stepSize == 0 || taxClassInt < 1 || taxClassInt > 2) {
			printf("Eine oder mehere Eingaben liegt außerhalb des Gültigenberiches\n");
		}
		else {
			inputOK = 1;
		}
	}
	printTaxTable( maxIncome,  minIncome,  stepSize,  taxClassInt);

	return 1;
}

void printTaxTable(int maxIncome, int minIncome, int stepSize, int taxClassInt) {
	int rows = ceil(maxIncome * 1.0 - minIncome * 1.0) / (stepSize * 1.0) + 1;
	int columns = 3;
	int currentRow = 0;
	float* tableData = malloc(rows * columns * sizeof(float)); //allocate the space for the table
	if (tableData == NULL) { //check if pointer is null to avoid allocation issues
		printf_s("Pointer erstellen fehlgeschalgen, abort");
		exit(0);
	}
	for (int income = minIncome; income <= maxIncome; income += stepSize) {
		float ic = income * 1.0f; //convert to float so it can be added the "array"
		float tax = calculateTax(taxClassInt, income);
		float taxRate = 0;
		if (ic > 0.01) { //avoids division by 0
			taxRate = tax / ic * 100;
		}
		tableData[currentRow * columns] = ic;
		tableData[currentRow * columns + 1] = tax;
		tableData[currentRow * columns + 2] = taxRate;
		currentRow++;
	}
	char* headerString = "Einkommen     Steuern  Steuersatz\n"; //each set of numbers has a width of 14
	printTable(tableData, headerString, rows, columns);
	free(tableData); //free allocted space
}


float calculateTax(int taxClass, int income) {
	float dueTaxes = 0.0f;

	income -= TAX_CLASSES[taxClass - 1].deductable; //get via array index, if theres a wrong number entered this will cause an access violation

	int currentTaxBracket = 0;
	while (income > 0) {
		if (income > TAX_BRACKETS[currentTaxBracket].upperLimit) {
			dueTaxes += TAX_BRACKETS[currentTaxBracket].upperLimit * TAX_BRACKETS[currentTaxBracket].rate; //takem max amount for bracket and advance to next
			income -= TAX_BRACKETS[currentTaxBracket].upperLimit; // do I risk an underflow? //subtract income, what is left in income is the amount that still needs to taxed
			currentTaxBracket++; //advance to next bracket
		}
		else { //last bracket
			dueTaxes += income * TAX_BRACKETS[currentTaxBracket].rate;
			income = -1; // if I subtract int_max from 1 I might risk an underflow otherwise I could've created a single subtract income line outside of the if clause
		}
	}
	return dueTaxes;
}

void printTable(float* tableData, char* headerData, int row, int column) { //float has to be filled in the same way its printed row by row
	printf(headerData);
	for (int i = 0; i < row; i++) {
		for (int a = 0; a < column; a++) {
			printf_s("%9.1f   ", tableData[i * column + a]); //prints each number with a width of 9 and 1 number past the decimal and 4 spaces. there a leading spaces not 0s
		}
		printf_s("\n"); //start printing next row
	}
}

