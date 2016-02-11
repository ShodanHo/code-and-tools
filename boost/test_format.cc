/*************************************************************************/
/*                                                                       */
/* Project: Boost.                                                       */
/*                                                                       */
/* Description: Test of boost module.                                    */
/*                                                                       */
/* Date: December 24, 2005.                                              */
/*                                                                       */
/*************************************************************************/

/*
 * $Id$
 *
 * $Log$
 */

#include <iostream>
#include <popt.h>
#include <boost/format.hpp>
#include <vector>

#define STR(x) #x << '=' << x << ' '

using namespace std;

static int port = 0;

enum {
  OPTION_HELP = 1,
  OPTION_PORT
};

struct poptOption options[] = {
  {"help", 'h', POPT_ARG_NONE, 0, OPTION_HELP,
   "print this message", "argDescrip" },
  {"port", 'p', POPT_ARG_INT, &port, OPTION_PORT,
   "specifies the port to serve on", "argDescrip" },
  { 0, 0, 0, 0, 0, 0, 0 }
};

#if 0
static void
printUsage(char **argv)
{
  cout << "Usage: " << argv[0]
       << " [{-h | --help}] {-p <port> | --port <port>}" << endl;
}
#endif

static void parseCommandLine(int argc, char **argv)
{
  poptContext context =
    poptGetContext(0, argc, (const char **)argv, options, 0);

  while (1) {
    int rc = poptGetNextOpt(context);

    if (rc > 0) {
      switch (rc) {
      case OPTION_HELP:
	poptPrintHelp(context, stdout, 0);
	exit(0);
      case OPTION_PORT:
	break;
      default:
	cerr << "Unknown switch" << endl;
	exit(-1);
      }
    } else if (rc < -1) {
      cerr << "Unknown parameter" << endl;
      exit(-1);
    } else {
      break;
    }
  }
}

int
main(int argc, char **argv)
{
  cout << __FILE__ << " start\n";

  parseCommandLine(argc, argv);

  using boost::format;
  using boost::io::group;
  cout << format("%|1$1| %|2$3|") % "Hello" % 3 << std::endl;

  cout << format() ;
  cout << __FILE__ << " end\n";

  return 0;
}

// End of file
