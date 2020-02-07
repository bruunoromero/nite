let () = {
  let tokens = Nite.Lexer.tokenize("1");
  let (location, token) = List.hd(tokens);

  switch (token) {
  | Nite.Lexer.Token.Int(value) => print_int(value)
  | _ => ()
  };
};