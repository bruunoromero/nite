module Chars = {
  let is_digit = char => {
    switch (char) {
    | '0'..'9' => true
    | _ => false
    };
  };

  let is_alpha = char => {
    switch (char) {
    | 'a'..'z'
    | 'A'..'Z' => true
    | _ => false
    };
  };

  let is_newline = char => {
    switch (char) {
    | '\n' => true
    | _ => false
    };
  };

  let is_whitespace = char => {
    switch (char) {
    | ' '
    | '\r'
    | '\t' => true
    | _ => false
    };
  };

  let is_escape = char => {
    switch (char) {
    | '\\' => true
    | _ => false
    };
  };
};

module Strings = {
  let last_char = s => s.[String.length(s) - 1];
};