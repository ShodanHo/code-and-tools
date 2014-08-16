#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

using namespace std;

#define HERE() __FILE__ << '(' << __LINE__ << "):"

int main()
{
  char *cmd1[] = { "/bin/ls", "-l", "/", 0 }; 
  char *cmd2[] = { "/usr/bin/wc", "-w", "/", 0 };
	
	int pipefd[2], rs;
	
	rs = pipe(pipefd);
	if (rs < 0) 
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	
	if(rs == 0) 
	{	// Child process
		// close write end of pipe
		close(pipefd[1]);
		// close standard input
		close(0);
		// duplicate read end of pipe
		// into standard input
		dup(pipefd[0]);
		// close read end of pipe
		close(pipefd[0]);
		// run wc
		std::cout << HERE() << std::endl;
		int pid = fork();
		if (pid==0) {
		  rs = execvp(cmd1[0], cmd1);
		}
		std::cout << HERE() << std::endl;
		if (rs < 0) 
		{
			perror("execl");
			exit(EXIT_FAILURE);
		}
	}
	else 
	{ 	// Parent process
		// close read end of pipe,
		// keep write end open
		close(pipefd[0]);
		// close standard output
		close(1);
		// duplicate write end of pipe
		// into standard output
		dup(pipefd[1]);
		// close write end of pipe
		close(pipefd[1]);
		// run ls
		std::cout << HERE() << std::endl;
		rs = execvp(cmd2[0], cmd2);
		std::cout << HERE() << std::endl;
		if(rs < 0) 
		{
			perror("execl");
			exit(EXIT_FAILURE);
		}
	}
	std::cout << "hello world" << std::endl;
	return 0;
}
