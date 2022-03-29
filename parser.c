#include <stdio.h>

// Lexer output
enum token {
	empty_token,
	identifier_token,
	if_token,
	while_token,
	int_literal_token,
	l_paren_token,
	r_paren_token,
	l_brace_token,
	r_brace_token,
	assignment_token,
	equals_token,
	divide_token,
	star_token,
	plus_token,
	minus_token,
	semicolon_token,
};

const char* token_name(enum token token) {
	static const char* names[] = {
		"empty_token",
		"identifier_token",
		"if_token",
		"while_token",
		"int_literal_token",
		"l_paren_token",
		"r_paren_token",
		"l_brace_token",
		"r_brace_token",
		"assignment_token",
		"equals_token",
		"divide_token",
		"star_token",
		"plus_token",
		"minus_token",
		"semicolon_token",
	};
	if (token >= empty_token && token <= semicolon_token) {
		return names[token];
	} else {
		return "unknown!";
	}
}

const char* pretty_token_name(enum token token) {
	static const char* names[] = {
		"",
		"identifier_token",
		"if",
		"while",
		"int_literal_token",
		"(",
		")",
		"{",
		"}",
		"=",
		"==",
		"/",
		"*",
		"+",
		"-",
		";",
	};
	if (token >= empty_token && token <= semicolon_token) {
		return names[token];
	} else {
		return "unknown!";
	}
}

// States of lexer finite state machine
enum state {
	start_state,
	identifier_state,
	i_state,
	if_state,
	w_state,
	wh_state,
	whi_state,
	whil_state,
	while_state,
	int_literal_state,
	l_paren_state,
	r_paren_state,
	l_brace_state,
	r_brace_state,
	semicolon_state,
	equals_state,
	equals_equals_state,
	forward_slash_state,
	star_state,
	plus_state,
	minus_state,
	comment_state,
	error_state,
	end_state // Must actually be final enum as a boundary. 
};

const char* state_name(enum state state) {
	static const char* names[] = {
		"start_state        ",
		"identifier_state   ",
		"i_state            ",
		"if_state           ",
		"w_state            ",
		"wh_state           ",
		"whi_state          ",
		"whil_state         ",
		"while_state        ",
		"int_literal_state  ",
		"l_paren_state      ",
		"r_paren_state      ",
		"l_brace_state      ",
		"r_brace_state      ",
		"semicolon_state    ",
		"equals_state       ",
		"equals_equals_state",
		"forward_slash_state",
		"star_state         ",
		"plus_state         ",
		"minus_state        ",
		"comment_state      ",
		"error_state        ",
		"end_state          "
	};
	if (state >= start_state && state <= end_state) {
		return names[state];
	} else {
		return "unknown!";
	}
}

char is_digit(char input) {
	return '0' <= input && input <= '9';
}

char is_alpha(char input) {
	return ('a' <= input && input <= 'z') || ('A' <= input && input <= 'Z');
}

char is_whitespace(char input) {
	return input == ' ' || input == 10 || input == 9;
}

struct transition {
	enum state next_state;
	enum token output;
};

struct transition next_from_start(char input) {
	struct transition t;
	t.next_state = error_state;
	t.output = empty_token;
	if (input == 'i') {
		t.next_state = i_state;
	} else if (input == 'w') {
		t.next_state = w_state;
	} else if (is_alpha(input) || input == '_') {
		t.next_state = identifier_state;
	} else if (is_digit(input)) {
		t.next_state = int_literal_state;
	} else if (input == '(') {
		t.next_state = l_paren_state;
	} else if (input == ')') {
		t.next_state = r_paren_state;
	} else if (input == '{') {
		t.next_state = l_brace_state;
	} else if (input == '}') {
		t.next_state = r_brace_state;
	} else if (input == ';') {
		t.next_state = semicolon_state;
	} else if (input == '=') {
		t.next_state = equals_state;
	} else if (input == '/') {
		t.next_state = forward_slash_state;
	} else if (input == '*') {
		t.next_state = star_state;
	} else if (input == '+') {
		t.next_state = plus_state;
	} else if (input == '-') {
		t.next_state = minus_state;
	} else if (is_whitespace(input)) {
		t.next_state = start_state;
	} else if (input == EOF) {
		t.next_state = end_state;
	}
	return t;
}

struct transition next_from_identifier(char input) {
	struct transition t;
	t.next_state = error_state;
	t.output = empty_token;
	if (is_digit(input) || is_alpha(input) || input == '_') {
		t.next_state = identifier_state;
	} else {
		t = next_from_start(input);
		t.output = identifier_token;
	}
	return t;
}

struct transition next_from_i(char input) {
	struct transition t;
	t.next_state = error_state;
	t.output = empty_token;
	if (input == 'f') {
		t.next_state = if_state;
	} else if (is_digit(input) || is_alpha(input) || input == '_') {
		t.next_state = identifier_state;
	} else {
		t = next_from_start(input);
		t.output = identifier_token;
	}
	return t;
}

struct transition next_from_if(char input) {
	struct transition t;
	t.next_state = error_state;
	t.output = empty_token;
	if (is_alpha(input) || is_digit(input) || input == '_') {
		t.next_state = identifier_state;
	} else {
		t = next_from_start(input);
		t.output = if_token;
	}
	return t;
}

struct transition next_from_int_literal(char input) {
	struct transition t;
	t.next_state = error_state;
	t.output = empty_token;
	if (is_digit(input)) {
		t.next_state = int_literal_state;
	} else {
		t = next_from_start(input);
		t.output = int_literal_token;
	}
	return t;
}

struct transition next_from_forward_slash_state(char input) {
	struct transition t;
	t.next_state = error_state;
	t.output = empty_token;
	if (input == '/') {
		t.next_state = comment_state;
	} else {
		t = next_from_start(input);
		t.output = divide_token;
	}
	return t;
}

struct transition next_from_comment_state(char input) {
	struct transition t;
	t.next_state = comment_state;
	t.output = empty_token;
	if (input == 10) {
		t.next_state = start_state; 
	}
	return t;
}

struct transition next_from(enum token output, char input) {
	//printf("calling next_from(%d, %d)\n", output, input);
	struct transition t;
	t = next_from_start(input);
	t.output = output;
	return t;
}

struct transition next(enum state current_state, char input) {
	
	struct transition t;
	int i;
	t.next_state = error_state;
	t.output = empty_token;
	switch (current_state) {
		case start_state:
			return next_from_start(input);
		case identifier_state:
			return next_from_identifier(input);
		case i_state:
			return next_from_i(input);
		case if_state:
			return next_from_if(input);
		case w_state:
		case wh_state:
		case whi_state:
		case whil_state:
			i = current_state - w_state;
			if ("while"[i+1] == input) {
				t.next_state = current_state + 1;
				return t;
			} else {
				return next_from_identifier(input);
			}
		case while_state:
			t = next_from_start(input);
			t.output = while_token;
			return t;
		case int_literal_state:
			return next_from_int_literal(input);
		case l_paren_state:
			return next_from(l_paren_token, input);
		case r_paren_state:
			return next_from(r_paren_token, input);
		case l_brace_state:
			return next_from(l_brace_token, input);
		case r_brace_state:
			return next_from(r_brace_token, input);
		case semicolon_state:
			return next_from(semicolon_token, input);
		case equals_state:
			if (input == '=') {
				t.next_state = equals_equals_state;
				return t;
			} else {
				return next_from(assignment_token, input);
			}
		case equals_equals_state:
			return next_from(equals_token, input);
		case forward_slash_state:
			return next_from_forward_slash_state(input);
		case star_state:
			return next_from(star_token, input);
		case plus_state:
			return next_from(plus_token, input);
		case minus_state:
			return next_from(minus_token, input);
		case comment_state:
			return next_from_comment_state(input);
	}
	return t;
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
		
		printf("%s\t",   state_name(state));
		printf("'%c'\t", is_whitespace(c) || c == EOF ? ' ' : c);
		printf("%s\t",   state_name(t.next_state));
		printf("%s\n",   pretty_token_name(t.output));
		
		state = t.next_state;
	}
	
	if (state == end_state) {
		printf("done!\n", state);
	} else if (state == error_state) {
		printf("error!\n", state);
	} else {
		printf("impossible!\n", state);
	}
}
                                                                               