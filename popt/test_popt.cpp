/*********************************************************** -*- C++ -*- */
/*                                                                       */
/* Title: Test popt argument parsing.                                    */
/*                                                                       */
/* Descirption: Put argument parsing package popt thru its paces.        */
/*                                                                       */
/* Syntax: ./test_popt [{-h|--help}]                                     */
/*                          [{-d|--debug} <debug level>]                 */
/*                                                                       */
/*************************************************************************/

#include <iostream>
#include <cstdlib>
#include <popt.h>

using namespace std;

enum {
  OPTION_HELP = 1,
  OPTION_STRING,
  OPTION_DEBUG_LEVEL_MAIN,
  OPTION_DEBUG_LEVEL_XYZ
};

static void
parse_command_line(int argc, char **argv, string &someStr,
                   unsigned &mainDebugLevel, unsigned &xyzDebugLevel)
{
  char *str = nullptr;
  unsigned main_debug_level = 0;
  unsigned xyz_debug_level = 0;

  struct poptOption options[] = {
      {"help", 'h', POPT_ARG_NONE, 0, OPTION_HELP, "print this message", 0 },
      {"string", 's', POPT_ARG_STRING, &str, OPTION_STRING, "enter a string", "some string" },
      {"debug", 'd', POPT_ARG_INT, &main_debug_level, OPTION_DEBUG_LEVEL_MAIN,
          "set the main debug level: 0(little) - 10(lots)", "main debug level" },
      {"dbgModuleXYZ", '\0', POPT_ARG_INT, &xyz_debug_level, OPTION_DEBUG_LEVEL_XYZ,
              "set the xyz debug level: 0(little) - 10(lots)", "xyz debug level" },
      POPT_AUTOHELP
      { 0, 0, 0, 0, 0, 0, 0 }
  };

  poptContext context;
  context = poptGetContext(0, argc, (const char **)argv, options, 0);

  while (1)
  {
    int rc = poptGetNextOpt(context);

    if (rc > 0) {
      switch (rc) {
      case OPTION_HELP:
        poptPrintHelp(context, stdout, 0);
        exit(0);
      case OPTION_STRING:
        someStr = str;
        break;
      case OPTION_DEBUG_LEVEL_MAIN:
        mainDebugLevel = main_debug_level;
        break;
      case OPTION_DEBUG_LEVEL_XYZ:
        xyzDebugLevel = xyz_debug_level;
        break;
      default:
        fprintf(stderr, "Unknown switch\n");
        exit(-1);
      }
    } else if (rc < -1) {
      fprintf(stderr, "Unknown parameter\n");
      exit(-1);
    } else {
      break;
    }
  }
}

int
main(int argc, char **argv)
{
  cout << "__FILENAME__" << ": start" << endl;

  std::string str;
  unsigned mainDebugLevel = 0;
  unsigned xyzDebugLevel = 0;
  parse_command_line(argc, argv, str, mainDebugLevel, xyzDebugLevel);

  cout << "str=" << str << '\n'
      << "mainDebugLevel=" << mainDebugLevel << '\n'
      << "xyzDebugLevel=" <<xyzDebugLevel << '\n';

  cout << "__FILENAME__" << ": stop " << endl;
}

// End of file
