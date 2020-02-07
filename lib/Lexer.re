module State = {
  type t = {
    line: int,
    index: int,
    column: int,
    source: string,
  };

  let make = source => {index: 0, source, line: 0, column: 0};

  let can_read = state => String.length(state.source) > state.index;

  let current_char = state => {
    can_read(state) ? Some(state.source.[state.index]) : None;
  };

  let advance = state => {
    let new_state = {...state, index: state.index + 1};

    switch (current_char(state)) {
    | None => new_state
    | Some(char) =>
      if (Utils.Chars.is_newline(char)) {
        {...new_state, line: new_state.line + 1, column: 0};
      } else {
        {...new_state, column: new_state.column + 1};
      }
    };
  };

  let peek = state => {
    state |> advance |> current_char;
  };
};

module Token = {
  type t =
    | EOF
    | WS(int)
    | NL(int)
    | Int(int)
    | Id(string)
    | Float(float)
    | String(string);

  type position = {
    line: int,
    column: int,
  };

  type location = {
    start: position,
    finish: position,
  };

  let make_position = (~line, ~column) => {line, column};

  let make_location = (~start, ~finish) => {start, finish};

  let make_number = str =>
    if (String.contains(str, '.')) {
      Float(float_of_string(str));
    } else {
      Int(int_of_string(str));
    };

  let make_string = str =>
    String(String.sub(str, 1, String.length(str) - 2));
};

/* LEXER UTILS */

let is_valid_id_char = (char, acc) => {
  Utils.Chars.is_alpha(char);
};

let is_valid_number_char = (char, acc) =>
  Utils.Chars.is_digit(char) || char == '.' && !String.contains(acc, '.');

let is_valid_string_char = (char, acc) =>
  if (String.length(acc) == 0) {
    char == '"';
  } else if (String.length(acc) > 1) {
    let last_char = acc.[String.length(acc) - 1];
    let before_last_char = acc.[String.length(acc) - 2];

    !(last_char == '"' && before_last_char != '\\');
  } else {
    true;
  };

let tracking_location = (fn: State.t => (Token.t, State.t), state: State.t) => {
  let (token, new_state) = fn(state);

  let position =
    Token.make_location(
      Token.make_position(state.line, state.column),
      Token.make_position(new_state.line, new_state.column),
    );

  (position, token, new_state);
};

/* LEXERS */

let rec tokenize_ws_2 = (state, ~acc) =>
  switch (State.current_char(state)) {
  | None => (Token.WS(acc), state)
  | Some(char) =>
    if (Utils.Chars.is_whitespace(char)) {
      tokenize_ws_2(State.advance(state), acc + 1);
    } else {
      (Token.WS(acc), state);
    }
  };

let rec tokenize_nl_2 = (state, ~acc) =>
  switch (State.current_char(state)) {
  | None => (Token.NL(acc), state)
  | Some(char) =>
    if (Utils.Chars.is_newline(char)) {
      tokenize_nl_2(State.advance(state), acc + 1);
    } else {
      (Token.NL(acc), state);
    }
  };

let rec tokenize_id_2 = (state, ~acc) =>
  switch (State.current_char(state)) {
  | None => (Token.Id(acc), state)
  | Some(char) =>
    if (is_valid_id_char(char, acc)) {
      tokenize_id_2(State.advance(state), acc ++ String.make(1, char));
    } else {
      (Token.Id(acc), state);
    }
  };

let rec tokenize_number_2 = (state, ~acc) =>
  switch (State.current_char(state)) {
  | None => (Token.make_number(acc), state)
  | Some(char) =>
    if (is_valid_number_char(char, acc)) {
      tokenize_number_2(State.advance(state), acc ++ String.make(1, char));
    } else {
      (Token.make_number(acc), state);
    }
  };

let rec tokenize_string_2 = (state, ~acc) =>
  switch (State.current_char(state)) {
  | None => (Token.make_string(acc), state)
  | Some(char) =>
    if (is_valid_string_char(char, acc)) {
      tokenize_string_2(State.advance(state), acc ++ String.make(1, char));
    } else {
      (Token.make_string(acc), state);
    }
  };

/* TRACKING LEXERS */

let tokenize_ws = tracking_location(tokenize_ws_2(~acc=0));

let tokenize_nl = tracking_location(tokenize_nl_2(~acc=0));

let tokenize_id = tracking_location(tokenize_id_2(~acc=""));

let tokenize_number = tracking_location(tokenize_number_2(~acc=""));

/* MAIN */

let end_accumulator = acc => {
  let eof_token_data = (
    Token.make_location(
      Token.make_position(0, 0),
      Token.make_position(0, 0),
    ),
    Token.EOF,
  );

  List.append(acc, [eof_token_data]);
};

let tokenize_from_char = (char, state) =>
  if (Utils.Chars.is_digit(char)) {
    tokenize_number(state);
  } else if (Utils.Chars.is_alpha(char)) {
    tokenize_id(state);
  } else {
    (
      Token.make_location(
        Token.make_position(0, 0),
        Token.make_position(0, 0),
      ),
      Token.EOF,
      state,
    );
  };

let rec tokenize_2 = (state, acc) => {
  switch (State.current_char(state)) {
  | None => end_accumulator(acc)
  | Some(char) =>
    let (location, token, new_state) = tokenize_from_char(char, state);
    tokenize_2(new_state, List.append(acc, [(location, token)]));
  };
};

let tokenize = source => tokenize_2(State.make(source), []);