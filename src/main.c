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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Network SSID.
 */
char *ssid = "test";

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
	};           */
	
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
					ssid = arg;
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
}

int main(int argc, char **argv)
{
	char *output;
  
	while (!feof(stdin))
	{
		char password[9];
		
		fgets(password, sizeof(password), stdin);
		output = PBKDF2(pass, 6);
		
		free(output);

	}
	
	return (EXIT_FAILURE);
}
