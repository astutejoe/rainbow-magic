/*
 * Copyright (C) 2015 Gabriel G. Garcia <gabriel.antunes.garcia@gmail.com>
 *                    Pedro H. Penna <pedrohenriquepenna@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
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

extern char* PBKDF2(unsigned char *, unsigned char);

/**
 * Wordsize in the dictionary.
 */
#define WORDSIZE (8 + 1)

/**
 * @brief Number of words in the dictionary.
 */
#define NWORDS 100000000

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

/**
 * @brief Dictionary.
 */
struct
{
	char *words;
	unsigned nwords;
	unsigned wordsize;
} dictionary;

/**
 * @brief Reads dictionary to memory.
 */
static void dictionary_create(FILE *file)
{
	unsigned i;
	
	dictionary.words = malloc(NWORDS*WORDSIZE);
	assert(dictionary.words != NULL);
	
	/* Read dictionary. */
	i = 0;
	while (1)
	{
		if (fgets(&dictionary.words[i + WORDSIZE], WORDSIZE, stdin) == NULL)
			break;
		i++;
	}
}

/**
 * @brief Destroys dictionary.
 */
static void dictionary_destroy(void)
{
	free(dictionary.words);
}

int main(int argc, const char **argv)
{
	readargs(argc, argv);
	
	dictionary_create(stdin);
  
	/* Create rainbow table. */
	for (unsigned i = 0; i < dictionary.nwords; i++)
	{
		char *digest;
		
		digest = PBKDF2(&dictionary.words[i + WORDSIZE], WORDSIZE - 1);
		
		fprintf(stderr, "%s %s\n", &dictionary.words[i + WORDSIZE], digest);
		
		free(digest);
	}
	
	dictionary_destroy();
	
	return (EXIT_FAILURE);
}
