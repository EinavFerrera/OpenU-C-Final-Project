#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "structures.h"
#include "binaryBuild.h"
#include "firstPass.h"
#include "registerTable.h"
char digitToBase64(char *digit)
{
    int number;
    number = (int)strtol(digit, NULL, 2);
    if (number >= 0 && number <= 25) /* number between A(0 - 65) and Z(25 - 90)*/
        return number + 65;
    if (number >= 26 && number <= 51) /* number between a(26 - 97) and z(51 - 122)*/
        return number + 71;
    if (number >= 52 && number <= 61) /* number between 0(52 - 48) and 9(61 - 57)*/
        return number - 4;
    if (number == 62) /* return + */
        return 43;
    if (number == 63) /* return / */
        return 47;
  return -1;
}

void binaryCode(FILE *obFile, gNode rowData, gNode labels, int IC)
{

    writeCODE(obFile, rowData, labels);
    DATAparts(obFile, rowData);
    printf("\tob file was created successfully\n");
}

void writeCODE(FILE *obFile, gNode rowData, gNode labels)
{
    /**
     * definition of bytes in the general binary code
     * @param opDest define the destination operand type (bytes 2-3)
     * @param opSrc define the source operand type (bytes 4-5)
     * @param opCode define the opcode of command (bytes 6-9)
     * @param param2 define the second param type (bytes 10-11)
     * @param param1 define the first param type (bytes 12-13)
     * */
    int opCode, opSrc, opDest, opType1, opType2, final;

    if (rowData == NULL)
    {
        return;
    }
    writeCODE(obFile, (gNode)getNext(rowData), labels);

    if (getType(rowData) != DATA)
    {
	opType1 = getOpType(rowData, 1);
        opType2 = getOpType(rowData, 2);

        if (opType1 == DIRECT_REG)
            opType1 = 5; /*101*/
        else if (opType1 == DIRECT)
            opType1 = 3; /*011*/
        else if (opType1 == IMMEDIATE)
            opType1 = 1; /*001*/
        else if (opType1 == NO_ADDRESS)
            opType1 = 0; /*000*/

        if (opType2 == DIRECT_REG)
            opType2 = 5;
        else if (opType2 == DIRECT)
            opType2 = 3;
        else if ((opType2 == IMMEDIATE))
            opType2 = 1;
        else if (opType2 == NO_ADDRESS)
            opType2 = 0;

        if (getNumOfOps(rowData) == 1)
        {
            opSrc = 0;
            opDest = opType1;
        }
        else
        {
            opSrc = opType1;
            opDest = opType2;
        }

        opCode = getCommand(rowData);
        /*********************************************************************************************/
        /*
        index:
        0   1  2  3  4  5  6  7  8  9  10  11
        11 10  9  8  7  6  5  4  3  2  1   0
        [src   ]  [opcode  ]  [dest ]  [ARE]
        */

        opDest = opDest << 2;
        opSrc = opSrc << 9;
        opCode = opCode << 5;
        final = opSrc + opCode + opDest;
        writeBinary(final, obFile);
        writeBinaryParts(obFile, rowData, labels);
    }
}

void writeBinaryParts(FILE *obFile, gNode row, gNode labels)
{

    /*
    DIRECT REG (5)
    11 10 9 8 7 6 5 4 3 2  1  0
    [src     ]  [dest   ] [ARE]

    DIRECT(3)
    11 10 9 8 7 6 5 4 3 2  1  0
    [label adress       ] [ARE]

    IMMIDIATE (1)
    11 10 9 8 7 6 5 4 3 2  1  0
    [immidiate         ] [ARE]
    */
    int i = getNumOfOps(row);
    int count = 1;
    int op1, op2, ARE;
    if (getType(row) == JUMP)
    {
        ARE = getAREOfLabel(row, labels, 1);
        if (getOpType(row, 1) == DIRECT) /*if label*/
        {
            op1 = getAddressOfLabel(row, labels, 1) << 2;
            op1 = op1 + ARE;
        }

        else if (getOpType(row, 1) == DIRECT_REG) /*if reg*/
        {
            op1 = getOp(row, 1) << 2;
            op1 = op1 + ARE;
        }
    }
    else
    {
        if (getOpType(row, 1) == DIRECT)
        {
            ARE = getAREOfLabel(row, labels, 1);
            if (ARE == 1)
                op1 = 1;
            else
            {
                op1 = getAddressOfLabel(row, labels, 1) << 2;
                op1 = op1 + ARE;
            }
        }
        if (getOpType(row, 2) == DIRECT)
        {
            ARE = getAREOfLabel(row, labels, 2);
            if (ARE == 1)
                op2 = 1;
            else
            {
            op2 = getAddressOfLabel(row, labels, 2) << 2;
            op2 = op2 + ARE;
	    }
        }

        if (getOpType(row, 1) == IMMEDIATE)
        {
            op1 = atoi(getLabel1(row));
            op1 = op1 << 2;
        }
        if (getOpType(row, 2) == IMMEDIATE)
        {
            op2 = atoi(getLabel2(row));
            op2 = op2 << 2;
        }

        if (getOpType(row, 1) == DIRECT_REG)
        {

            if (i == 1) /*if only 1 operand- the op is in dest place*/
            	op1 = getOp(row, 1) << 2;
            else
            	op1 = getOp(row, 1) << 7;
        }
        if (getOpType(row, 2) == DIRECT_REG)
        {
            op2 = getOp(row, 2) << 2;
        }

        if (getOpType(row, 1) == DIRECT_REG && getOpType(row, 2) == DIRECT_REG) /*registers are at source and destination*/
        {
            i--;
            op1 = op1 + op2;
        }
    }
    while (count <= i)
    {
        if (count == 1)
        {
            writeBinary(op1, obFile);
        }
        else if (count == 2)
        {
            writeBinary(op2, obFile);
        }
        count++;
    }
}

void DATAparts(FILE *obFile, gNode rowData)
{

    char *expression;

    if (rowData == NULL)
    {
        return;
    }
    DATAparts(obFile, (gNode)getNext(rowData));

    if (getType(rowData) == DATA)
    {
        expression = getName(rowData);
        if ((*expression) != '"') /*data*/
        {
            writeData(obFile, rowData);
        }
        else /*String*/
        {
            writeString(obFile, rowData);
        }
    }
}

void writeData(FILE *obFile, gNode rowData)
{
    int num;
    int count = 0;
    char *expression, *originExp;
    expression = getName(rowData);
    originExp = expression;
    while (true)
    {
        if (*expression == ',')
        {
            num = atoi(originExp);
            writeBinary(num, obFile);
            originExp = expression + 1;
            count++;
        }
        if (endOfLine(expression) == EOL)
        {
            num = atoi(originExp);
            writeBinary(num, obFile);
            originExp = expression + 1;
            count++;
            break;
        }
        expression += 1;
    }
}

void writeString(FILE *obFile, gNode rowData)
{
    int num;
    int count = 0;
    char *expression;
    expression = getName(rowData);
    expression += 1;
    while (*expression != '"')
    {
        num = *expression;
        writeBinary(num, obFile);
        expression += 1;
        count++;
    }
    writeBinary(0, obFile); /* writting '\0' at the end*/
}

void writeBinary(int num, FILE *obFile)
{
    char base64[13] = "";
    char Astr[7], Bstr[7];
    char A, B;
    int i = 0;
    int numToCompare = 2048; /*2^11*/
    while (i < 12)
    {
        if (num & numToCompare)
        {
            strcat(base64, "1");
        }
        else
        {
            strcat(base64, "0");
        }
        i++;
        numToCompare = numToCompare >> 1;
    }
    base64[12] = '\0';
    strncpy(Astr, base64, 6);
    Astr[6] = '\0';
    strncpy(Bstr, base64 + 6, 6);
    Bstr[6] = '\0';
    A = digitToBase64(Astr); /* bits 11,10,9,8,7,6 ->base 64*/
    B = digitToBase64(Bstr);  /*bits  5,4,3,2,1,0 ->base 64*/
    fputc(A, obFile);
    fputc(B, obFile);
    fputs("\n", obFile);
}

int getAddressOfLabel(gNode row, gNode labels, int index)
{
    if (index == 1)
    {
        if (search(&labels, getLabel1(row)) == NULL)
            return 0;
        else
            return getAddress(search(&labels, getLabel1(row))) + 100;
    }
    else if (index == 2)
    {
        if (search(&labels, getLabel2(row)) == NULL)
            return 0;
        else
            return getAddress(search(&labels, getLabel2(row))) + 100;
    }
    else if (index == 3)
    {
        if (search(&labels, getLabel3(row)) == NULL)
            return 0;
        else
            return getAddress(search(&labels, getLabel3(row))) + 100;
    }
        return -1;
}

int getAREOfLabel(gNode row, gNode labels, int index)
{
    if (index == 1)
    {
        if (search(&labels, getLabel1(row)) == NULL)
            return 0;
        else
            return getARE(search(&labels, getLabel1(row)));
    }
    else if (index == 2)
    {
        if (search(&labels, getLabel2(row)) == NULL)
            return 0;
        else
            return getARE(search(&labels, getLabel2(row)));
    }
    else if (index == 3)
    {
        if (search(&labels, getLabel3(row)) == NULL)
            return 0;
        else
            return getARE(search(&labels, getLabel3(row)));
    }
    return -1;
}
