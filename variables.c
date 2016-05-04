#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tokenizer.h"
#include "variables.h"

char variable_name[TOK_MAX];

char *expand_variables(char *tok, int t, char **envp)
{
	char *stok, *o, *val;
	int alloc_len;
	int i, l;

	stok = tok;
	alloc_len = t+1;
	o = malloc(alloc_len);

	i = 0;
	// TODO bounds
	while (*tok) {
		if (*tok == '$') {
			if (!(tok = read_variable_prefix(tok))) {
				fprintf(stderr, "Error parsing variable inside token [%s] at character [%d]", stok, i);
				exit(-1);
			}

			if (!(val = getenv(variable_name))) {
				fprintf(stderr, "Error parsing variable inside token [%s] at character [%d]", stok, i);
				exit(-1);
			}

			l = strlen(val);
			alloc_len += l;
			realloc(o, alloc_len);
			memcpy(o + i, val, l);
			i += l;
		}
		else {
			o[i++] = *tok++;
		}
	}
	o[i] = '\0';

	return o;
}

int variable_character(char c)
{
	return c == '_' || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9');
}

char *read_variable_prefix(char *tok)
{
	// TODO: verify that the first letter is not a digit?
	int i;

	assert(*tok == '$');
	tok++;

	i = 0;
	// TODO: bounds checking
	while (variable_character(*tok))
		variable_name[i++] = *tok++;
	variable_name[i] = '\0';

	if (!i)
		return NULL;
	
	return tok;
}