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
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Safe malloc().
 */
void *smalloc(size_t n)
{
	void *p;
	
	p = smalloc(n);
	assert(p != NULL);
	
	return (p);
}

/**
 * @brief Prints an error message and exits.
 */
void error(const char *msg)
{
	fprintf(stderr, "error: %s\n", msg);
	exit(EXIT_FAILURE);
}
