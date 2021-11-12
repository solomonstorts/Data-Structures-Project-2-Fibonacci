//
//  Fibonacci.c
//
//  Programming Assignment #2: Huge Fibonacci
//  Spring 2021
//  Data Structures
//  Created by Solomon Storts
//  Student ID #7625
//
//  In this programming assignment, you will implement a Fibonacci function that avoids repetitive
//  computation by computing the sequence linearly from the bottom up: F(0) through F(n).
//  You will also overcome the limitations of C’s 32-bit integers by storing very large integers
//  in arrays of individual digits.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Fibonacci.h"
#include "limits.h"


//  HELPER FUNCTIONS

//  Used for parseString, this function uses a switch comparison in order to convert a character number ('0' - '9') to an integer.
int charToInt(char number) {
    switch (number) {
        case '0':
            return 0;
            break;
        case '1':
            return 1;
            break;
        case '2':
            return 2;
            break;
        case '3':
            return 3;
            break;
        case '4':
            return 4;
            break;
        case '5':
            return 5;
            break;
        case '6':
            return 6;
            break;
        case '7':
            return 7;
            break;
        case '8':
            return 8;
            break;
        case '9':
            return 9;
            break;
        default:
            break;
    }
    return 0;
}


//  REQUIRED FUNCTIONS

//  Return a pointer to a new, dynamically allocated HugeInteger struct that contains the result of adding the huge integers represented by p and q.
HugeInteger *hugeAdd(HugeInteger *p, HugeInteger *q) {
    
    //If a NULL pointer is passed to this fuction, return NULL
    if(p == NULL || q == NULL) {
        return NULL;
    }
    
    //Create HugeInteger to store result
    HugeInteger *result = malloc(sizeof(HugeInteger));
    //Ensure malloc() succeeded
    if (result == NULL) {
        return NULL;
    }
    
    //determine which array is largest, and set length of result to that + 1
    //the +1 is for cases where we have an extra carry
    if(p->length > q->length) {
        result->length = p->length + 1;
    } else {
        result->length = q->length + 1;
    }
    
    //Allocate space for result array
    result->digits = malloc(sizeof(int) * result->length);
    //Ensure malloc() succeeded
    if (result->digits == NULL) {
        return NULL;
    }
    
    int sum = 0; //holds sum of p's digit and q's digit
    int carryToAdd = 0; //holds the carry that needs to be added THIS ITERATION
    int carryHold = 0; //holds the carry that needs to be added NEXT ITERATION
    
    for(int i = 0; i < result->length; i++) {

        carryToAdd = carryHold;
        carryHold = 0;
        
        //case where both digits are empty, we simply add any carry left
        if (i >= p->length && i >= q->length) {
            result->digits[i] = carryToAdd;
        
        //cases where one number has less digits than the other, so we simply copy the digit from the larger number (+ any carry)
        } else if (i >= p->length) {
            result->digits[i] = q->digits[i] + carryToAdd;
        } else if (i >= q->length){
            result->digits[i] = p->digits[i] + carryToAdd;
        
        //case where there are two digits to add
        } else {
            sum = p->digits[i] + q->digits[i] + carryToAdd;
            if(sum > 9) { //if sum > 9, there is a carry to add
                carryHold = sum / 10;
                sum %= 10;
            }
            result->digits[i] = sum;
        }
    }
    
    //if there was no final carry, we can remove extra overhead
    if(carryToAdd == 0) {
        result->length--;
    }
    
    return result;
}

//  Destroy any and all dynamically allocated memory associated with p.
HugeInteger *hugeDestroyer(HugeInteger *p) {
    
    //if p is NULL, sunply return NULL (there's nothing to free)
    if(p == NULL || p->digits == NULL) {
        return NULL;
    }
    
    free(p->digits);
    free(p);
    
    return NULL;
}

//  Convert a number from string format to HugeInteger format.
HugeInteger *parseString(char *str) {
    
    //if str is null, return null
    if(str == NULL) {
        return NULL;
    }
    
    //create and allocate space for result
    HugeInteger *result = malloc(sizeof(HugeInteger));
    //Ensure malloc() succeeded
    if (result == NULL) {
        return NULL;
    }

    int strLength = strlen(str); //holds the length of str
    
    if(strLength == 0) { //the case where the empty string ("") is passed
        //allocate space for digits
        result->digits = malloc(sizeof(int));
        //Ensure malloc() succeeded
        if (result->digits == NULL) {
            return NULL;
        }
        
        //if empty string is passed, it is treated as zero
        result->digits[0] = 0;
        result->length = 1;
        
    } else { //the case where the empty string isn't passed
        //allocate space for digits
        result->digits = malloc(sizeof(int) * strLength);
        //Ensure malloc() succeeded
        if (result->digits == NULL) {
            return NULL;
        }
        
        result->length = strLength;
        for(int i = 0; i < strLength; i++) {
            result->digits[i] = charToInt(str[strLength - (i + 1)]); //copy number from str into digits using charToInt() function
        }
    }
    
    return result;
}

//  Convert the unsigned integer n to HugeInteger format.
HugeInteger *parseInt(unsigned int n) {
    
    //create and allocate space for result
    HugeInteger *result = malloc(sizeof(HugeInteger));
    //Ensure malloc() succeeded
    if (result == NULL) {
        return NULL;
    }
    
    //determine number of digits needed
    int numberOfDigits = 0;
    if(n == 0) {
        numberOfDigits = 1;
    } else {
        for(unsigned int i = n; i > 0; i /= 10) {
            numberOfDigits++;
        }
    }
    
    //allocate space for digits
    result->digits = malloc(sizeof(int) * numberOfDigits);
    //Ensure malloc() succeeded
    if (result->digits == NULL) {
        return NULL;
    }
    result->length = numberOfDigits;
        
    for(int i = 0; i < numberOfDigits; i++) {
        result->digits[i] = n % 10; //copy number from n into digits
        n /= 10;
    }
    
    return result;
}

//  Convert the integer represented by p to a dynamically allocated unsigned int, and return a pointer to that value.
unsigned int *toUnsignedInt(HugeInteger *p) {
    
    //if p is null, return null
    if(p == NULL || p->digits == NULL) {
        return NULL;
    }
    
    //allocate space for result
    unsigned int *result = malloc(sizeof(unsigned int));
    //Ensure malloc() succeeded
    if (result == NULL) {
        return NULL;
    }
    
    //the for loop gets each member of p->digits, and adds it the resulting unsigned int
    //after the first number, we have to multiply by a power of 10 to add to the sum
    for(int i = 0; i < p->length; i++) {
        if(UINT_MAX - (p->digits[i] * pow(10, i)) >= *result) { //test if this iteration will exceed UINT_MAX
            if(i > 0) { //if it's not the first number, we have to multiply by a power of 10
                *result = *result + (p->digits[i] * pow(10, i));
            } else {
                *result = *result + p->digits[i];
            }
        } else {
            return NULL; //if it exceeds UINT_MAX, return NULL
        }
    }
    return result;
}

//  Implement an iterative solution that runs in O(nk) time and returns a pointer to a HugeInteger struct that contains F(n).
HugeInteger *fib(int n) {
    
    //if n is 0 or 1, return 0 or 1 for increased simplicity in for loop
    if(n == 0) {
        return parseInt(0);
    }
    if(n == 1) {
        return parseInt(1);
    }
    
    //allocate space for array fibStorage
    //fibStorage contains three HugeIntegers used for calculating fibonacci
    HugeInteger **fibStorage = malloc(sizeof(HugeInteger) * 3);
    //Ensure malloc() succeeded
    if (fibStorage == NULL) {
        return NULL;
    }
    
    
    //allocate space for fibStorage[0]
    fibStorage[0] = malloc(sizeof(HugeInteger));
    //Ensure malloc() succeeded
    if (fibStorage[0] == NULL) {
        return NULL;
    }
    //allocate space for fibStorage[0]->digits
    fibStorage[0]->digits = malloc(sizeof(int) * UINT_MAX);
    //Ensure malloc() succeeded
    if (fibStorage[0]->digits == NULL) {
        return NULL;
    }
    fibStorage[0] = parseInt(0); //initialize fibStorage[0] to 0
    
    //allocate space for fibStorage[1]
    fibStorage[1] = malloc(sizeof(HugeInteger));
    //Ensure malloc() succeeded
    if (fibStorage[1] == NULL) {
        return NULL;
    }
    //allocate space for fibStorage[1]->digits
    fibStorage[1]->digits = malloc(sizeof(int) * UINT_MAX);
    //Ensure malloc() succeeded
    if (fibStorage[1]->digits == NULL) {
        return NULL;
    }
    fibStorage[1] = parseInt(1); //initialize fibStorage[1] to 1
    
    //allocate space for fibStorage[2]
    fibStorage[2] = malloc(sizeof(HugeInteger));
    //Ensure malloc() succeeded
    if (fibStorage[2] == NULL) {
        return NULL;
    }
    //allocate space for fibStorage[2]->digits
    fibStorage[2]->digits = malloc(sizeof(int) * UINT_MAX);
    //Ensure malloc() succeeded
    if (fibStorage[2]->digits == NULL) {
        return NULL;
    }
    fibStorage[2] = parseInt(1); //initialize fibStorage[2] to 1
    
    
    //for loop where fib calculation is done
    //fibStorage[0] holds fib(n - 2)
    //fibStorage[1] holds fib(n - 1)
    //fibStorage[2] holds fib(n-2) + fib(n - 1)
    //the runtime is O(nk), because we make n calls to hugeAdd (O(k)).
    for(int i = 2; i <= n; i++) {
        fibStorage[2] = hugeAdd(fibStorage[0], fibStorage[1]); //perform fib addition using hugeAdd (O(k))
        fibStorage[0] = fibStorage[1]; //update fibStorage[0]
        fibStorage[1] = fibStorage[2]; //update fibStorage[1]
    }
    
    return fibStorage[2]; //return result
}

//  A double indicating how difficult I found this assignment on a scale of 1.0 (ridiculously easy) through 5.0 (insanely difficult).
double difficultyRating(void) {
    return 4;
}

//  An estimate (greater than zero) of the number of hours you spent on this assignment.
double hoursSpent(void) {
    return 7.0;
}
