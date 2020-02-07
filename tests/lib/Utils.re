open Nite.Utils;
open TestFramework;

describe("Nite.Utils.Chars", ({test}) => {
  open Chars;

  test("is_alpha should return true for letters", ({expect}) => {
    expect.bool(is_alpha('a')).toBe(true);
    expect.bool(is_alpha('c')).toBe(true);
    expect.bool(is_alpha('z')).toBe(true);
    expect.bool(is_alpha('A')).toBe(true);
    expect.bool(is_alpha('B')).toBe(true);
    expect.bool(is_alpha('Z')).toBe(true);
    expect.bool(is_alpha('0')).toBe(false);
    expect.bool(is_alpha('9')).toBe(false);
    expect.bool(is_alpha('_')).toBe(false);
    expect.bool(is_alpha('$')).toBe(false);
    expect.bool(is_alpha('.')).toBe(false);
  });

  test("is_digit should return true for digits", ({expect}) => {
    expect.bool(is_digit('0')).toBe(true);
    expect.bool(is_digit('7')).toBe(true);
    expect.bool(is_digit('4')).toBe(true);
    expect.bool(is_digit('.')).toBe(false);
    expect.bool(is_digit('a')).toBe(false);
    expect.bool(is_digit('A')).toBe(false);
    expect.bool(is_digit('_')).toBe(false);
  });

  test("is_newline should return true for newline char", ({expect}) => {
    expect.bool(is_newline('\n')).toBe(true);
    expect.bool(is_newline('\r')).toBe(false);
    expect.bool(is_newline('\t')).toBe(false);
    expect.bool(is_newline(' ')).toBe(false);
    expect.bool(is_newline('a')).toBe(false);
    expect.bool(is_newline('A')).toBe(false);
    expect.bool(is_newline('_')).toBe(false);
  });

  test("is_whitespace should return true for whitespace chars", ({expect}) => {
    expect.bool(is_whitespace(' ')).toBe(true);
    expect.bool(is_whitespace('\r')).toBe(true);
    expect.bool(is_whitespace('\t')).toBe(true);
    expect.bool(is_whitespace('\n')).toBe(false);
    expect.bool(is_whitespace('a')).toBe(false);
    expect.bool(is_whitespace('A')).toBe(false);
    expect.bool(is_whitespace('_')).toBe(false);
  });
});