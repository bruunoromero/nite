open Nite.Lexer;
open TestFramework;

describe("Nite.Lexer.State", ({test}) => {
  open State;

  test("make should return state record", ({expect}) => {
    expect.equal(make(""), {source: "", index: 0, line: 0, column: 0});
    expect.equal(make("a"), {source: "a", index: 0, line: 0, column: 0});
    expect.equal(
      make("bcd"),
      {source: "bcd", index: 0, line: 0, column: 0},
    );
  });

  test(
    "can_read should return true as long as the index is less than the source length",
    ({expect}) => {
      expect.bool(can_read({source: "", index: 0, line: 0, column: 0})).toBe(
        false,
      );
      expect.bool(can_read({source: "a", index: 1, line: 0, column: 0})).toBe(
        false,
      );
      expect.bool(can_read({source: "abc", index: 3, line: 0, column: 0})).
        toBe(
        false,
      );

      expect.bool(can_read({source: "a", index: 0, line: 0, column: 0})).toBe(
        true,
      );
      expect.bool(can_read({source: "abc", index: 1, line: 0, column: 0})).
        toBe(
        true,
      );
      expect.bool(can_read({source: "def", index: 2, line: 0, column: 0})).
        toBe(
        true,
      );
      expect.bool(
        can_read({source: "abc def", index: 3, line: 0, column: 0}),
      ).
        toBe(
        true,
      );
    },
  );

  test(
    "advance should return a new state with the index incremented",
    ({expect}) => {
    expect.equal(
      advance({source: "", index: 0, line: 0, column: 0}),
      {source: "", index: 1, line: 0, column: 0},
    );

    expect.equal(
      advance({source: "abc", index: 1, line: 0, column: 0}),
      {source: "abc", index: 2, line: 0, column: 1},
    );

    expect.equal(
      advance({source: "abc", index: 1, line: 0, column: 1}),
      {source: "abc", index: 2, line: 0, column: 2},
    );

    expect.equal(
      advance({source: "abc def", index: 3, line: 0, column: 3}),
      {source: "abc def", index: 4, line: 0, column: 4},
    );

    expect.equal(
      advance({source: "abc\ndef", index: 3, line: 0, column: 3}),
      {source: "abc\ndef", index: 4, line: 1, column: 0},
    );
  });

  test(
    "current_char should return Some(char) if can_read or None", ({expect}) => {
    expect.option(current_char({source: "a", index: 0, line: 0, column: 0})).
      toBe(
      Some('a'),
    );
    expect.option(
      current_char({source: "abc", index: 1, line: 0, column: 0}),
    ).
      toBe(
      Some('b'),
    );
    expect.option(
      current_char({source: "abc", index: 2, line: 0, column: 0}),
    ).
      toBe(
      Some('c'),
    );
    expect.option(
      current_char({source: "abc def", index: 3, line: 0, column: 0}),
    ).
      toBe(
      Some(' '),
    );
    expect.option(
      current_char({source: "abc\ndef", index: 3, line: 0, column: 0}),
    ).
      toBe(
      Some('\n'),
    );

    expect.option(
      current_char({source: "def", index: 3, line: 0, column: 0}),
    ).
      toBe(
      None,
    );
    expect.option(
      current_char({source: "abc def", index: 7, line: 0, column: 0}),
    ).
      toBe(
      None,
    );
  });

  test("peek should return Some(char) if can_read or None", ({expect}) => {
    expect.option(peek({source: "a", index: 0, line: 0, column: 0})).toBe(
      None,
    );
    expect.option(peek({source: "abc", index: 0, line: 0, column: 0})).toBe(
      Some('b'),
    );
    expect.option(peek({source: "abc", index: 1, line: 0, column: 0})).toBe(
      Some('c'),
    );
    expect.option(peek({source: "abc def", index: 3, line: 0, column: 0})).
      toBe(
      Some('d'),
    );
    expect.option(peek({source: "abc\ndef", index: 2, line: 0, column: 0})).
      toBe(
      Some('\n'),
    );

    expect.option(peek({source: "abc def", index: 7, line: 0, column: 0})).
      toBe(
      None,
    );
  });
});

describe("Nite.Lexer.Token", ({test}) => {
  open Token;

  test(
    "make_number should take a string and returs a int or a float token",
    ({expect}) => {
    expect.equal(make_number("0"), Int(0));
    expect.equal(make_number("1"), Int(1));
    expect.equal(make_number("2"), Int(2));
    expect.equal(make_number(".2"), Float(0.2));
    expect.equal(make_number("0.0"), Float(0.0));
    expect.equal(make_number("1.1"), Float(1.1));
    expect.equal(make_number("1.7"), Float(1.7));
    expect.equal(make_number("2.2"), Float(2.2));
  });

  test(
    "make_string should take a string and returs a string token", ({expect}) => {
    expect.equal(make_string("\"\""), String(""));
    expect.equal(make_string("\"1\""), String("1"));
    expect.equal(make_string("\"a\""), String("a"));
    expect.equal(make_string("\"1234 abcd\""), String("1234 abcd"));
    expect.equal(make_string("\"abc\n\\\"def\""), String("abc\n\\\"def"));
  });

  test("make_position should make a position record", ({expect}) => {
    expect.equal(make_position(0, 0), {line: 0, column: 0});
    expect.equal(make_position(1, 1), {line: 1, column: 1});
    expect.equal(make_position(0, 1), {line: 0, column: 1});
    expect.equal(make_position(22, 37), {line: 22, column: 37});
    expect.equal(make_position(~line=0, ~column=1), {line: 0, column: 1});
    expect.equal(make_position(~column=0, ~line=1), {line: 1, column: 0});
  });

  test("make_location should make a location record", ({expect}) => {
    expect.equal(
      make_location({line: 0, column: 0}, {line: 0, column: 0}),
      {
        start: {
          line: 0,
          column: 0,
        },
        finish: {
          line: 0,
          column: 0,
        },
      },
    );

    expect.equal(
      make_location(
        ~start={line: 10, column: 37},
        ~finish={line: 22, column: 11},
      ),
      {
        start: {
          line: 10,
          column: 37,
        },
        finish: {
          line: 22,
          column: 11,
        },
      },
    );

    expect.equal(
      make_location(
        ~finish={line: 14, column: 80},
        ~start={line: 7, column: 12},
      ),
      {
        start: {
          line: 7,
          column: 12,
        },
        finish: {
          line: 14,
          column: 80,
        },
      },
    );
  });
});

describe("Nite.Lexer", ({test}) => {
  test(
    "end_accumulator should adds the eof token to the end of the tokens list",
    ({expect}) => {
    let token_location =
      Token.make_location(
        Token.make_position(0, 0),
        Token.make_position(0, 0),
      );

    let eof_token_data = (
      Token.make_location(
        Token.make_position(0, 0),
        Token.make_position(0, 0),
      ),
      Token.EOF,
    );

    let expect_data_1 = (token_location, Token.Id("a"));
    expect.equal(
      end_accumulator([expect_data_1]),
      [expect_data_1, eof_token_data],
    );

    let expect_data_2 = (token_location, Token.Id("b"));
    let expect_data_3 = (token_location, Token.Int(1));
    expect.equal(
      end_accumulator([expect_data_2, expect_data_3]),
      [expect_data_2, expect_data_3, eof_token_data],
    );

    let expect_data_4 = (token_location, Token.Float(1.1));
    let expect_data_5 = (token_location, Token.Int(1));
    let expect_data_6 = (token_location, Token.WS(1));
    expect.equal(
      end_accumulator([expect_data_4, expect_data_5, expect_data_6]),
      [expect_data_4, expect_data_5, expect_data_6, eof_token_data],
    );
  });

  test(
    "is_valid_number_char should take a char and an acc, and returns if the can be added to the acc for a valid number",
    ({expect}) => {
      expect.bool(is_valid_number_char('7', "")).toBe(true);
      expect.bool(is_valid_number_char('.', "")).toBe(true);
      expect.bool(is_valid_number_char('.', "1")).toBe(true);
      expect.bool(is_valid_number_char('.', "2")).toBe(true);
      expect.bool(is_valid_number_char('1', "1")).toBe(true);
      expect.bool(is_valid_number_char('3', "2")).toBe(true);

      expect.bool(is_valid_number_char('a', "2")).toBe(false);
      expect.bool(is_valid_number_char(' ', "3")).toBe(false);
      expect.bool(is_valid_number_char('.', "1.1")).toBe(false);
      expect.bool(is_valid_number_char('.', "2.5")).toBe(false);
    },
  );

  test(
    "is_valid_string_char should take a char and an acc, and returns if the can be added to the acc for a valid string",
    ({expect}) => {
      expect.bool(is_valid_string_char('"', "")).toBe(true);
      expect.bool(is_valid_string_char('b', "\"a")).toBe(true);
      expect.bool(is_valid_string_char('"', "\"abc")).toBe(true);
      expect.bool(is_valid_string_char('.', "\"\\a")).toBe(true);
      expect.bool(is_valid_string_char('\n', "\"\\a")).toBe(true);
      expect.bool(is_valid_string_char('.', "\"def\\\"")).toBe(true);

      expect.bool(is_valid_string_char('a', "\"\"")).toBe(false);
      expect.bool(is_valid_string_char('.', "\"abc\"")).toBe(false);
      expect.bool(is_valid_string_char(' ', "\"\\\"\"")).toBe(false);
    },
  );

  test(
    "tokenize_number_2 takes a state and acc, and return a tuple of the token and new state",
    ({expect}) => {
      let (t1, _) = tokenize_number_2(State.make("1.1"), "");
      expect.equal(t1, Float(1.1));

      let (t2, _) = tokenize_number_2(State.make("1.4"), "");
      expect.equal(t2, Float(1.4));

      let (t3, _) = tokenize_number_2(State.make(".7"), "");
      expect.equal(t3, Float(0.7));

      let (t4, _) = tokenize_number_2(State.make("3"), "");
      expect.equal(t4, Int(3));

      let (t5, _) = tokenize_number_2(State.make("0"), "");
      expect.equal(t5, Int(0));

      let (t6, _) = tokenize_number_2(State.make("01234"), "");
      expect.equal(t6, Int(1234));

      let (t7, _) = tokenize_number_2(State.make("123 abc"), "");
      expect.equal(t7, Int(123));

      let (t8, _) = tokenize_number_2(State.make("123.4 def"), "");
      expect.equal(t8, Float(123.4));

      let (t8, _) = tokenize_number_2(State.make("123.45 "), "");
      expect.equal(t8, Float(123.45));

      let (_, s1) = tokenize_number_2(State.make("0"), "");
      expect.int(s1.index).toBe(1);

      let (_, s2) = tokenize_number_2(State.make("0"), "");
      expect.int(s2.index).toBe(1);

      let (_, s3) = tokenize_number_2(State.make("01234"), "");
      expect.int(s3.index).toBe(5);

      let (_, s4) = tokenize_number_2(State.make("123 abc"), "");
      expect.int(s4.index).toBe(3);

      let (_, s5) = tokenize_number_2(State.make("123.4 def"), "");
      expect.int(s5.index).toBe(5);

      let (_, s6) = tokenize_number_2(State.make("123.45 "), "");
      expect.int(s6.index).toBe(6);
    },
  );

  test(
    "tokenize_ws_2 takes a state and acc, and return a tuple of the token and new state",
    ({expect}) => {
      let (t1, _) = tokenize_ws_2(State.make(" "), 0);
      expect.equal(t1, WS(1));

      let (t2, _) = tokenize_ws_2(State.make("  "), 0);
      expect.equal(t2, WS(2));

      let (t3, _) = tokenize_ws_2(State.make("   "), 0);
      expect.equal(t3, WS(3));

      let (t4, _) = tokenize_ws_2(State.make(" 3"), 0);
      expect.equal(t4, WS(1));

      let (t5, _) = tokenize_ws_2(State.make("  a  "), 0);
      expect.equal(t5, WS(2));

      let (t6, _) = tokenize_ws_2(State.make("  \n  "), 0);
      expect.equal(t6, WS(2));

      let (_, s1) = tokenize_ws_2(State.make(" "), 0);
      expect.int(s1.index).toBe(1);

      let (_, s2) = tokenize_ws_2(State.make("  "), 0);
      expect.int(s2.index).toBe(2);

      let (_, s3) = tokenize_ws_2(State.make("  1"), 0);
      expect.int(s3.index).toBe(2);

      let (_, s4) = tokenize_ws_2(State.make("   a   "), 0);
      expect.int(s4.index).toBe(3);
    },
  );

  test(
    "tokenize_nl_2 takes a state and acc, and return a tuple of the token and new state",
    ({expect}) => {
      let (t1, _) = tokenize_nl_2(State.make("\n"), 0);
      expect.equal(t1, NL(1));

      let (t2, _) = tokenize_nl_2(State.make("\n\n"), 0);
      expect.equal(t2, NL(2));

      let (t3, _) = tokenize_nl_2(State.make("\n\n\n"), 0);
      expect.equal(t3, NL(3));

      let (t4, _) = tokenize_nl_2(State.make("\n3"), 0);
      expect.equal(t4, NL(1));

      let (t5, _) = tokenize_nl_2(State.make("\n\n \n\n"), 0);
      expect.equal(t5, NL(2));

      let (_, s1) = tokenize_nl_2(State.make("\n"), 0);
      expect.int(s1.index).toBe(1);

      let (_, s2) = tokenize_nl_2(State.make("\n\n"), 0);
      expect.int(s2.index).toBe(2);

      let (_, s3) = tokenize_nl_2(State.make("\n\n1"), 0);
      expect.int(s3.index).toBe(2);

      let (_, s4) = tokenize_nl_2(State.make("\n\n\na\n\n\n"), 0);
      expect.int(s4.index).toBe(3);
    },
  );

  test(
    "tokenize_string_2 takes a state and acc, and return a tuple of the token and new state",
    ({expect}) => {
      let (t1, _) = tokenize_string_2(State.make("\"\""), "");
      expect.equal(t1, String(""));

      let (t2, _) = tokenize_string_2(State.make("\"1\""), "");
      expect.equal(t2, String("1"));

      let (t3, _) = tokenize_string_2(State.make("\"a\""), "");
      expect.equal(t3, String("a"));

      let (t4, _) = tokenize_string_2(State.make("\"1234 abcd\""), "");
      expect.equal(t4, String("1234 abcd"));

      let (t5, _) = tokenize_string_2(State.make("\"abc\n\\\"def\""), "");
      expect.equal(t5, String("abc\n\\\"def"));

      let (_, s1) = tokenize_string_2(State.make("\"\""), "");
      expect.int(s1.index).toBe(2);

      let (_, s2) = tokenize_string_2(State.make("\"1\""), "");
      expect.int(s2.index).toBe(3);

      let (_, s3) = tokenize_string_2(State.make("\"a\""), "");
      expect.int(s3.index).toBe(3);

      let (_, s4) = tokenize_string_2(State.make("\"abc\n\\\"def\""), "");
      expect.int(s4.index).toBe(11);
    },
  );
});