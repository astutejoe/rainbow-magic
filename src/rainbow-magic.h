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

#ifndef RAINBOW_MAGIC_H_
#define RAINBOW_MAGIC_H_

	#include <stdlib.h>

	/**
	 * Wordsize in the dictionary.
	 */
	#define WORDSIZE (8 + 1)

	/**
	 * @brief Number of words in the dictionary.
	 */
	#define NWORDS 10000000

	/* Forward function definitions. */
	extern void sha1(unsigned char* , unsigned int, unsigned char*);
	extern void hmac(unsigned char*, unsigned char, unsigned char*, unsigned char, unsigned char*);
	extern void *smalloc(size_t);
	extern unsigned char *pbkdf2(unsigned char *, unsigned char);
	
	/* Forward variable defintions. */
	extern const char *salt;
	extern unsigned saltlen;

#endif /* RAINBOW_MAGIC_H_ */
