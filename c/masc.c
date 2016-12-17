/*
NAME: MASC ("MAthematical SCript")
Interprets a simple "script" to perform, for now only, grouptheoretical operations on permutations.
Syntax: (N_{} are integer long numbers, where {} is some kind of index).
"[N_0 N_1 N_2 ... N_k]" <=> A permutation which maps "0 -> N_0", "1 -> N_1", ..., "k -> N_k"
"(N_0 N_1 N_2 ... N_k)" <=> A cycle which maps "N_0 -> N_1", "N_1 -> N_2", ..., "N_(k-1) -> N_k"
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINEBUFFER_SIZE 1024
#define STATE_SIZE 512
#define R_CYCLE_SIZE 512

char linebuffer[LINEBUFFER_SIZE]; // Stores the current line
char subbuffer[LINEBUFFER_SIZE]; // Stores part of a line to be processed further
int charsread; // Stores how many characters were read (and how many are stored in linebuffer)
int charsinsb; // Stores how many characters are in subbuffer
long curr_perm_size = 0;

FILE * instream; // The filestream from which to read. Default is stdin.

long state[STATE_SIZE]; // The current state on which all further permutations get applied.
long r_perm[STATE_SIZE]; // The permutation register. For temporary storage of permutations.
long r_cycle[R_CYCLE_SIZE]; // The cycle register. For temporary storage of transpositions.

//Math functions
long invnum(long * p, const long P_SIZE);
void ppcomp(long * p0, long * p1, const long P_SIZE);
void cpcomp(long * p, long * c, const long P_SIZE, const long C_SIZE);

//Util functions
long strlisttonums(char * str, long * res, const char delim, const long STR_SIZE, const long RES_SIZE);
void dumplongarray(long * a, const int A_SIZE);

int main(int argc, char *argv[]) {
	if(argc > 1)
	{
		if(strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0)
		{
			printf("Usage:\n masc [FILE]\n (FILE = file to read from. If none give, reads from stdin)\n");
			return 0;
		}
		if((instream = fopen(argv[1],"r")) == NULL)
		{
			fprintf(stderr,"ERROR: Unable to read file. Defaulting to stdin.\n");
			instream = stdin;
		}
	}
	else
	{
		instream = stdin;
	}

	linebuffer[0] = '\0';
	subbuffer[0] = '\0';
	
	//Set state to id (1->1, 2->2, 3->3, ...)
	for(long i = 0;i < STATE_SIZE; i++)
	{
		state[i] = i;
	}
	
	//Read line by line.
	while(fgets(linebuffer, LINEBUFFER_SIZE, instream) != NULL)
	{
		charsread = strlen(linebuffer);

		//Look for beginning of a permutation
		for(int i = 0; i < charsread; i++)
		{
			if(linebuffer[i] == '[')
			{
				//Look for end of the permutation
				for(int j = i+1; j < charsread; j++)
				{
					if(linebuffer[j] == ']')
					{
						memcpy(subbuffer,linebuffer+i+1,j-i-1);  // Copy everything between the '[' and ']' to the subbuffer
						subbuffer[j-i] = '\0';   // Append a '\0'
						charsinsb = j-i;   // Save how many characters are in the subbuffer
						curr_perm_size = strlisttonums(subbuffer,r_perm,' ',charsinsb,STATE_SIZE);   // Convert "N1 N2 N3 N4 ..." to [N1, N2, N3, N4, ...]
						ppcomp(state,r_perm,STATE_SIZE);   // Apply our read in permutation to the current state
						i = j+1;   // Continue with first character following the ']'
						break;
					}
				}
			}
			if(linebuffer[i] == '(')
			{
				for(int j = i+1; j < charsread; j++)
				{
					if(linebuffer[j] == ')')
					{
						memcpy(subbuffer,linebuffer+i+1,j-i-1);   // Copy everything between the '[' and ']' to the subbuffer
						subbuffer[j-i-1] = '\0';   // Append a '\0'
						charsinsb = j-i;   // Save how many characters are in the subbuffer
						strlisttonums(subbuffer,r_cycle,' ',charsinsb,R_CYCLE_SIZE);   // Convert "N1 N2 N3 N4 ..." to [N1, N2, N3, N4, ...]
						cpcomp(state,r_cycle,STATE_SIZE,R_CYCLE_SIZE);
						i = j+1;   // Continue with first character following the ']'
					}
				}
			}
		}
		
		dumplongarray(state, curr_perm_size);
	}
}


//Math functions
//--------------

// Calculates the inversion number of the permutation p. Result is returned.
long invnum(long * p, const long P_SIZE)
{
	long result = 0;
	for(long i = 0; i < P_SIZE; i++)
	{
		for(long j = i+1; j < P_SIZE; j++)
		{
			if(p[j] < p[i])
			{
				result++;
			}
		}
	}
	return result;
}

// Forms the composition of two given permutations. The order is "p1 o p0" / "p1(p0(.))"). Result stored in p0.
void ppcomp(long * p0, long * p1, const long P_SIZE)
{
	for(long i = 0; i < P_SIZE; i++)
	{
		p0[i] = p1[p0[i]];
	}
}

// Forms the composition of a permutation and a cycle. The order is ( "c o p" / "c(p(.))" ). Result stored in p.
// NOTES:
//  I'd guess this could be done more efficient. Would be nice if I didn't have to loop through c for ever element in p.
void cpcomp(long * p, long * c, const long P_SIZE,const long C_SIZE)
{
	for(long i = 0; i < P_SIZE; i++)
	{
		//Go from c[0] to c[C_SIZE-2]. c[C_SIZE-1] maps to c[0], so it gets it's special case.
		for(long j = 0; j < C_SIZE-1; j++)
		{
			if(p[i] == c[j])
			{
					p[i] = c[j+1];
			}
		}
		if(p[i] == c[C_SIZE-1])
		{
			p[i] = c[0];
		}
	}
}

//Util functions
//--------------

// splits the sting str on delim, converts the resulting substrings to long and stores them in res.
// Returns how many numbers it got.
long strlisttonums(char * str, long * res, const char delim, const long STR_SIZE, const long RES_SIZE)
{
	char * endp;
	long resCount = 0;
	for(char * p = str; p < str+STR_SIZE; p++)
	{
		if(*p == delim && *(p+1) != delim)
		{
			if(resCount+1 < RES_SIZE)
			{
				res[resCount] = strtol(p+1,&endp,10);
				resCount++;
				p = endp;
			}
			else
			{
				break;
			}
		}
	}
	return resCount;
}

void dumplongarray(long * a, const int A_SIZE)
{
	printf("[%ld", a[0]);
	for(unsigned int i = 1; i < A_SIZE; i++)
	{
		printf(", %ld");
	}
	printf("]\n");

}
