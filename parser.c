#include <stdio.h>

#include "lexer.h"

void traceLexerTransition(enum state s, char c, struct transition t) {
	printf("%s\t",   state_name(s));
	printf("'%c'\t", is_whitespace(c) || c == EOF ? ' ' : c);
	printf("%s\t",   state_name(t.next_state));
	printf("%s\n",   pretty_token_name(t.output));
}

int main(int argc, char **argv) 
{
	enum state state = start_state;
	struct transition t;
	t.next_state = error_state;
	t.output = empty_token;
	int c;
	while (!(state == end_state || state == error_state)) {
		c = getchar();
		t = next(state, c);
		traceLexerTransition(state, c, t);

		
		state = t.next_state;
	}
	
	return state != end_state;
}
