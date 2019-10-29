#include <sys/time.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SIZE 94
#define SYMBOL_LIMIT 32
#define NUMBER_LIMIT 10
#define LCASE_LIMIT 26
#define UCASE_LIMIT 26

int symbol, number, l_case, u_case;

// Checks if a given character is a symbol
int is_symbol(char ch) {
	if((ch>' ' && ch<'0') || (ch>'9' && ch<'A') ||
	   (ch>'Z' && ch<'a') || (ch>'z' && ch<'~'))
			return 1;
	else return 0;
}

// Generates a random symbol
char generate_symbol() {
	int op;
	
	srand(time(NULL));
	op=rand()%4;
	
	if(op==0) {
		return (char)33+rand()%15;
	}
	else if(op==1) {
		return (char)58+rand()%7;
	}
	else if(op==2) {
		return (char)91+rand()%6;
	}
	else {
		return (char)123+rand()%4;
	}
}

// Checks if a given character is a number
int is_number(char ch) {
	return (ch>'/' && ch<':') ? 1 : 0;
}

// Generates a random number
char generate_number() {
	srand(time(NULL));
	return (char)48+rand()%10;
}

// Checks if a given character is a lowercase letter
int is_lowercase(char ch) {
	return (ch>'`' && ch<'{') ? 1 : 0;
}

// Generates a random lowercase letter
char generate_lcase() {
	srand(time(NULL));
	return (char)97+rand()%26;
}

// Checks if a given character is an uppercase letter
int is_uppercase(char ch) {
	return (ch>'@' && ch<'[') ? 1 : 0;
}

// Generates a random uppercase letter
char generate_ucase() {
	srand(time(NULL));
	return (char)65+rand()%26;
}

// Gets an unused char
void get_unused_char(int *count, int type, int *next) {
	srand(time(NULL));
	do {
		// Generates a new random symbol
		if(type==0 && symbol<SYMBOL_LIMIT) {
			*(next)=(int)generate_symbol();
		}
		
		// Generates a new random lowercase letter
		else if(type==1 && l_case<LCASE_LIMIT) {
			*(next)=(int)generate_lcase();
		}
		
		// Generates a new random number
		else if(type==2 && number<NUMBER_LIMIT) {
			*(next)=(int)generate_number();
		}
		
		// Generates a new random uppercase letter
		else if(type==3 && u_case<UCASE_LIMIT) {
			*(next)=(int)generate_ucase();
		}
	} while(count[(int)*(next)-33]!=0);
	
	// Increments the occurrence of symbols
	if(type==0 && symbol<SYMBOL_LIMIT) {
		symbol++;
	}
	
	// Increments the occurrence of lowercase letters
	else if(type==1 && l_case<LCASE_LIMIT) {
		l_case++;
	}
	
	// Increments the occurrence of numbers 
	else if(type==2 && number<NUMBER_LIMIT) {
		number++;
	}
	
	// Increments the occurrence of uppercase letters
	else if(type==3 && u_case<UCASE_LIMIT) {
		u_case++;
	}
	
	return;
}

// Evaluates the password
void evaluate_pwd(char *pwd, int length) {
	printf("\n\nSymbols: %d\n"
		   "Lowercase characters: %d\n"
		   "Numbers: %d\n"
		   "Uppercase characters: %d\n\n", symbol, l_case, number, u_case);
	
	return;
}

// Scrambles the password
void scramble_pwd(char *pwd, int length, int *printed) {
	int idx, printed_chars=0;
	
	// Prints out the randomly generated password
	printf("\n\nYour randomly generated password is: ");
	
	srand(time(NULL));
	while(printed_chars<length) {	
		idx=rand()%length;
		
		if(printed[idx]==0) {
			printf("%c", pwd[idx]);
			printed[idx]=1;
			printed_chars++;
		}
	}
	printf("\n");
	
	return;
}

// Generates the password
void gen_pwd(int length, int repeated_chars, int *printed) {
	int i, aux, next, count[SIZE]={0};
	char *pwd=NULL;
	
	// Allocates memory for the string that contains the password
	pwd=(char *)malloc((length+1)*sizeof(char));
	
	// Sets everything to zero
	l_case=u_case=number=symbol=0;
	
	for(i=0; i<length; i++) {
		aux=-1;
		
		// Prints the progress of the generation of the password
		system("cls");
		printf("Progress: %d%%", (100*i)/length);
		
		// Generates a random symbol
		if((i%4==0 && repeated_chars) || (i%4==0 && symbol<SYMBOL_LIMIT)) {
			pwd[i]=generate_symbol();
			symbol++; aux=0;
		}
		
		// Generates a random lowercase letter
		else if((i%4==1 && repeated_chars) || (i%4==1 && l_case<LCASE_LIMIT)) {
			if(i>2) {
				do {
					pwd[i]=generate_lcase();
				} while(pwd[i]==pwd[i-2]+32);
			}
			else {
				pwd[i]=generate_lcase();
			}
			
			l_case++; aux=0;
		}
		
		// Generates a random number
		else if(i%4==2) {
			pwd[i]=generate_number();
			number++; aux=0;
		}
		
		// Generates a random uppercase letter
		else if((i%4==3 && repeated_chars) || (i%4==3 && u_case<=UCASE_LIMIT)) {
			do {
				pwd[i]=generate_ucase();
			} while(pwd[i]==pwd[i-2]-32);
			
			u_case++; aux=0;
		}
		
		// An occurrence of a new character has been added
		// to the count vector
		if(aux!=-1) {
			aux=(int)pwd[i]-33;
			count[aux]++;
		}
		
		// If the password length is greater than 94, then
		// repeated characters must be allowed
		if(i==94) {
			repeated_chars=1;
		}
		
		// Checks if a repeated character has been set to the password
		if(aux!=-1 && !repeated_chars && count[aux]>1 && number<NUMBER_LIMIT) {
			if(is_symbol(pwd[i])) {
				symbol--;
			}
			else if(is_number(pwd[i])) {
				number--;
			}
			else if(is_uppercase(pwd[i])) {
				u_case--;
			}
			else if(is_lowercase(pwd[i])) {
				l_case--;
			}
			
			get_unused_char(count, i%4, &next);
			pwd[i]=(char)next;
			count[aux]--;
			count[next-33]++;
		}
		
		// Evaluates the password
		// evaluate_pwd(pwd, length);
	}
	pwd[length]='\0';
	
	// Prints the progress of the generation of the password
	system("cls");
	printf("Progress: %d%%", (100*i)/length);
	
	// Scrambles the password
	scramble_pwd(pwd, length, printed);
	
	// Frees the previously allocated memory
	free(pwd);
	
	return;
}

// Generates a random password
int main(int argc, char *argv[]) {
	int *printed, len, repeated_chars=0;
	struct timeval t1, t2;
	unsigned long long diff;
	
	// Number of parameters must be 2
	if(argc!=2) {
		printf("Usage: %s [number of characters]\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	// Second parameter must be greater than zero
	if(atoi(argv[1])<1) {
		printf("Error: length must be an integer greater than zero!\n");
		return EXIT_FAILURE;
	}
	
	// Length of the password
	len=atoi(argv[1]);
	
	// Vector of printed characters (used for the scramble_pwd function)
	printed=(int *)calloc(len, sizeof(int));
	
	// Marks the start of time
	gettimeofday(&t1, NULL);
	
	// Function that generates the password
	gen_pwd(len, repeated_chars, printed);
	
	// Marks the end of time
	gettimeofday(&t2, NULL);
	
	// Gets the difference of timestamps
	diff=1000*(t2.tv_sec-t1.tv_sec)+(t2.tv_usec-t1.tv_usec)/1000;
	
	// Prints how long it took to generate the password
	printf("Elapsed time: %.3lf seconds\n", 1.0*diff/1000);
	
	// Frees the previously allocated memory
	free(printed);
	
	return EXIT_SUCCESS;
}