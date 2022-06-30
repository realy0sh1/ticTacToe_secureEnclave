/*
 * Copyright (C) 2011-2021 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "Enclave.h"
#include "Enclave_t.h" /* print_string */
#include <stdarg.h>
#include <stdio.h> /* vsnprintf */
#include <string.h>

// is ascii for 1...9
static char gF[9] = {49,50,51,52,53,54,55,56,57};
static char placement = 88; // 88 (player 1), 79 (player 2)
/* 
 * printf: 
 *   Invokes OCALL to display the enclave buffer to the terminal.
 */
int printf(const char* fmt, ...)
{
	char buf[BUFSIZ] = { '\0' };
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, BUFSIZ, fmt, ap);
	va_end(ap);
	ocall_print_string(buf);
	return (int)strnlen(buf, BUFSIZ - 1) + 1;
}

int nextGameMove(int a){
	// 79 is "O" and 88 is "X" in ascii
	// check for valid input range (attacker could input false input); then check if game move is possible (short circuit behaviour)
	if(a < 1 || a > 9 || gF[a-1] == 79 || gF[a-1] == 88){
		// failure
		//print unchangedGame Field
		printf("\n \t \t \t %c|%c|%c\n \t \t \t -----\n \t \t \t %c|%c|%c\n \t \t \t -----\n \t \t \t %c|%c|%c\n",gF[6],gF[7],gF[8],gF[3],gF[4],gF[5],gF[0],gF[1],gF[2]);
		return 1;
	}else{
		// set game field
		gF[a-1] = placement;
		//print Game Field
		printf("\n \t \t \t %c|%c|%c\n \t \t \t -----\n \t \t \t %c|%c|%c\n \t \t \t -----\n \t \t \t %c|%c|%c\n",gF[6],gF[7],gF[8],gF[3],gF[4],gF[5],gF[0],gF[1],gF[2]);
		// change turns
		placement = (placement == 88)?79:88;
		return 0;
	}
}

int isGameFinished(){
	// check all combinations for winning
	// check if player 1 (88) won
	if(     (gF[0]==88 &&gF[3]==88&&gF[6]==88)||
		(gF[1]==88 &&gF[4]==88&&gF[7]==88)||
		(gF[2]==88 &&gF[5]==88&&gF[8]==88)||
		(gF[0]==88 &&gF[1]==88&&gF[2]==88)||
		(gF[3]==88 &&gF[4]==88&&gF[5]==88)||
		(gF[6]==88 &&gF[7]==88&&gF[8]==88)||
		(gF[0]==88 &&gF[4]==88&&gF[8]==88)||
		(gF[2]==88 &&gF[4]==88&&gF[6]==88)){
			//player 1 won
			printf("\n \t \t   !!!Player 1 won!!!\n\n");
			return 0;
	}
	// check if player 2 (79) won
	if(     (gF[0]==79 &&gF[3]==79&&gF[6]==79)||
		(gF[1]==79 &&gF[4]==79&&gF[7]==79)||
		(gF[2]==79 &&gF[5]==79&&gF[8]==79)||
		(gF[0]==79 &&gF[1]==79&&gF[2]==79)||
		(gF[3]==79 &&gF[4]==79&&gF[5]==79)||
		(gF[6]==79 &&gF[7]==79&&gF[8]==79)||
		(gF[0]==79 &&gF[4]==79&&gF[8]==79)||
		(gF[2]==79 &&gF[4]==79&&gF[6]==79)){
			//player 2 won
			printf("\n \t \t   !!!Player 2 won!!!\n\n");
			return 0;
	}
	// check if no more moves possible
	if(gF[0]!=49&&gF[1]!=50&&gF[2]!=51&&gF[3]!=52&&gF[4]!=53&&gF[5]!=54&&gF[6]!=55&&gF[7]!=56&&gF[8]!=57){
			//draw
			printf("\n \t \t      it's a draw\n\n");
			return 0;
	}
	
	// 1 means game is still running
	return 1;
}
