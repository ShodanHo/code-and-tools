#include <iostream>
#include "Lexer.h"
#include "term_control.h"
#include "error_handling.h"
#include "Postfix_Evaluator.h"
#include <map>
#include <cstdlib>

using namespace std;
typedef void (*cmd_t)(Lexer);
void bye(Lexer);
void eval_calc(Lexer);

/**
 * ----------------------------------------------------------------------------------
 * just print a prompt.
 * ----------------------------------------------------------------------------------
 */
void prompt()
{
  cout << term_cc(BLUE) << "> " << term_cc() << flush;
}

/**
 * exits program
 */
void bye(Lexer lexer) {
  if (lexer.has_more_token()) {
    error_return("Syntax error: use bye/exit/quit\n");
  } else {
    exit(0);
  }
}

/**
 * calculate expression.
 */
void eval_calc(Lexer lexer) {




}

int main()
{
  Lexer lexer; string line; Postfix_Evaluator pe;
  Token tok;
  map<string,cmd_t> cmd_map;
  cmd_map["bye"] = &bye;
  cmd_map["calc"] = &eval_calc;

  while (cin) {
    prompt(); getline(cin, line); lexer.set_input(line);
    if (!lexer.has_more_token()) continue;
    tok = lexer.next_token();
    if (tok.type != IDENT) { error_return("Syntax error\n"); continue; }
    
    if (cmd_map.find(tok.value) != cmd_map.end())
      cmd_map[tok.value](lexer);
    else
      error_return("Unknown command");
    
  }
  return 0;
}
