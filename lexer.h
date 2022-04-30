
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

const char* token_name(enum token token);
const char* pretty_token_name(enum token token);

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

const char* state_name(enum state state);

struct transition {
	enum state next_state;
	enum token output;
};

struct transition next(enum state current_state, char input);

char is_whitespace(char input);