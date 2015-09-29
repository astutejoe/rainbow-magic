/*
 * Copyright (C) 2015 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 *                    Gabriel G. Garcia <gabriel.antunes.garcia@gmail.com>
 * 
 * Rainbow Magic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Rainbow Magic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */ 

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#include "rainbow-magic.h"

/*============================================================================*
 *                               Dictionary                                   *
 *============================================================================*/

/**
 * @brief Dictionary.
 */
struct
{
	char *words;       /**< Words.           */
	unsigned nwords;   /**< Number of words. */
	unsigned wordsize; /**< Wordsize.        */
} dictionary;

/**
 * @brief Reads dictionary to memory.
 */
static void dictionary_create(FILE *file)
{
	/* Initialize dictionary. */
	dictionary.words = smalloc(NWORDS*WORDSIZE);
	dictionary.nwords = 0;
	dictionary.wordsize = WORDSIZE;
	
	/* Read dictionary. */
	do
	{
		char ch;
		
		for (int i = 0; (ch = getc(file)) != '\n'; i++)
		{
			if (ch == EOF)
				return;
				
			dictionary.words[dictionary.nwords*WORDSIZE + i] = ch;
		}
		
		dictionary.nwords++;
	} while (1);
}

/**
 * @brief Destroys dictionary.
 */
static void dictionary_destroy(void)
{
	free(dictionary.words);
}

/*============================================================================*
 *                                  Main                                      *
 *============================================================================*/

/**
 * @brief Network SSID.
 */
const char *salt = "test";

/**
 * @brief Salt length.
 */
unsigned saltlen = 4;

/**
 * @brief Prints program usage and exits.
 */
static void usage(void)
{
	printf("Usage: rainbow-magic [options]\n");
	printf("Options:\n");
	printf("  --help\n");
	printf("  --ssid <name>\n");
	
	exit(EXIT_FAILURE);
}

/**
 * @brief Reads command line arguments.
 */
static void readargs(int argc, const char **argv)
{
	enum states{
		STATE_READ_ARG, /* Read argument. */
		STATE_SET_SSID  /* Set SSID name. */
	};
	
	unsigned state;
	
	/* Parse command line arguments. */
	state = STATE_READ_ARG;
	for (int i = 1; i < argc; i++)
	{
		const char *arg = argv[i];
		
		/* Set value. */
		if (state != STATE_READ_ARG)
		{
			switch (state)
			{
				case STATE_SET_SSID:
					salt = arg;
					state = STATE_READ_ARG;
					break;
			}
			
			continue;
		}
		
		/* Parse command. */
		if (!strcmp(arg, "--help"))
			usage();
		else if (!strcmp(arg, "--ssid"))
			state = STATE_SET_SSID;
	}
	
	saltlen = strlen(salt);
}

int print_hex(unsigned char *buf, int len)
{
    for(int i=0;i<len;i++)
        printf("%02x",buf[i]);
    printf("\n");

    return(0);
}

/*int main(int argc, const char **argv)
{
	char **digest;
	
	digest = smalloc(NWORDS*sizeof(char *));
	
	readargs(argc, argv);
	
	dictionary_create(stdin);
  
	#pragma omp parallel for
	for (unsigned i = 0; i < dictionary.nwords; i++)
	{
		digest[i] = pbkdf2(&dictionary.words[i*WORDSIZE], WORDSIZE - 1);	
#ifndef NDEBUG
		printf("%s ", &dictionary.words[i*WORDSIZE]);
		print_hex(digest[i], 32);
#endif
	}
	
	for (unsigned i = 0; i < dictionary.nwords; i++)
		free(digest[i]);
	dictionary_destroy();
	
	return (EXIT_FAILURE);
}*/

int main()
{
	unsigned char digest[20];
	sha1("gabriel", 7, digest);
	print_hex(digest, 20);
	return 0;
}