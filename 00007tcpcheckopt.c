#include <stdlib.h>
#include <stdio.h>
#include "lib/include/unp.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <strings.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <poll.h>
#include <netinet/tcp.h>
#include <sys/sysctl.h>
#include <linux/sysctl.h>

union val {
	int i_val;
	long l_val;
	struct linger linger_val;
	struct timeval timeval_val;
} val;

static char * sock_str_flag(union val *, int);
static char * sock_str_int(union val *, int);
static char * sock_str_linger(union val *, int);
static char * sock_str_timeval(union val *, int);

static char charstrres[128];

static char * sock_str_flag(union val * ptr, int len) {
	if (len != sizeof(int)) {
		snprintf(charstrres, sizeof(charstrres), "size (%d) not sizeof(int)", len);
	} else {
		snprintf(charstrres, sizeof(charstrres), "%s", (ptr->i_val == 0) ? "Off" : "On");
	}
	return charstrres;
}

static char * sock_str_int(union val * ptr, int len) {
	if (len != sizeof(long)) {
		snprintf(charstrres, sizeof(charstrres), "size (%d) not sizeof(long)", len);
	} else {
		snprintf(charstrres, sizeof(charstrres), "%ld", ptr->l_val);
	}
	return charstrres;
}

static char * sock_str_linger(union val * ptr, int len) {
	if (len != sizeof(struct linger)) {
		snprintf(charstrres, sizeof(charstrres), "size (%d) not sizeof(struct linger)", len);
	} else {
		snprintf(charstrres, sizeof(charstrres), "(onOrOff)%d--%d", ptr->linger_val.l_onoff, ptr->linger_val.l_linger);
	}
	return charstrres;
}

static char * sock_str_timeval(union val * ptr, int len) {
	if (len != sizeof(struct timeval)) {
		snprintf(charstrres, sizeof(charstrres), "size (%d) not sizeof(struct timeval)", len);
	} else {
		snprintf(charstrres, sizeof(charstrres), "%06d", ptr->timeval_val.tv_usec);
	}
	return charstrres;
}

struct sock_opts {
	const char * opt_str;
	int opt_leval;
	int opt_name;
	char * (*option_val_str)(union val *, int);
} sock_opts[] = { { "SO_BROADCAST", SOL_SOCKET, SO_BROADCAST, sock_str_flag }, { "SO_DEBUG", SOL_SOCKET, SO_DEBUG, sock_str_flag }, { "SO_DONTROUTE", SOL_SOCKET, SO_DONTROUTE, sock_str_flag },

{ "SO_ERROR", SOL_SOCKET, SO_ERROR, sock_str_int },

{ "SO_KEEPALIVE", SOL_SOCKET, SO_KEEPALIVE, sock_str_flag },

{ "SO_LINGER", SOL_SOCKET, SO_LINGER, sock_str_linger },

{ "SO_OOBINLINE", SOL_SOCKET, SO_OOBINLINE, sock_str_flag },

{ "SO_RCVBUF", SOL_SOCKET, SO_RCVBUF, sock_str_int },

{ "SO_SNDBUF", SOL_SOCKET, SO_SNDBUF, sock_str_int },

{ "SO_RCVLOWAT", SOL_SOCKET, SO_RCVLOWAT, sock_str_int },

{ "SO_SNDLOWAT", SOL_SOCKET, SO_SNDLOWAT, sock_str_int },

{ "SO_RCVTIMEO", SOL_SOCKET, SO_RCVTIMEO, sock_str_timeval },

{ "SO_SNDTIMEO", SOL_SOCKET, SO_SNDTIMEO, sock_str_timeval },

{ "SO_REUSEADDR", SOL_SOCKET, SO_REUSEADDR, sock_str_flag },

#ifdef SO_REUSEPORT
		{	"SO_REUSEPORT",SOL_SOCKET,SO_REUSEPORT,sock_str_flag},
#else
		{ "SO_REUSEPORT", 0, 0, NULL },
#endif

		{ "SO_TYPE", SOL_SOCKET, SO_TYPE, sock_str_int },
#ifdef SO_USELOOPBACK
		{	"SO_USELOOPBACK",SOL_SOCKET,SO_USELOOPBACK,sock_str_flag},
#endif

		{ "IP_TOS", IPPROTO_IP, IP_TOS, sock_str_int }, { "IP_TTL", IPPROTO_IP, IP_TTL, sock_str_int },

#ifdef IPV6_DONTFRAG
		{	"IPV6_DONTFRAG",IPPROTO_IPV6,IPV6_DONTFRAG,sock_str_flag},
#endif
		{ "IPV6_UNICAST_HOPS", IPPROTO_IPV6, IPV6_UNICAST_HOPS, sock_str_int }, { "IPV6_V6ONLY", IPPROTO_IPV6, IPV6_V6ONLY, sock_str_flag },

		{ "TCP_MAXSEG", IPPROTO_TCP, TCP_MAXSEG, sock_str_int }, { "TCP_NODELAY", IPPROTO_TCP, TCP_NODELAY, sock_str_flag },

#ifdef SCTP_AUTOCLOSE
		{	"SCTP_AUTOCLOSE",IPPROTO_SCTP,SCTP_AUTOCLOSE,sock_str_int},
#endif
#ifdef SCTP_MAXBURST
		{	"SCTP_MAXBURST",IPPROTO_SCTP,SCTP_MAXBURST,sock_str_int},
#endif
#ifdef SCTP_MAXSEG
		{	"SCTP_MAXSEG",IPPROTO_SCTP,SCTP_MAXSEG,sock_str_int},
#endif
#ifdef SCTP_NODELAY
		{	"SCTP_NODELAY",IPPROTO_SCTP,SCTP_NODELAY,sock_str_flag},
#endif
		{ NULL, 0, 0, NULL } };

int main(int argc, char * argv) {
	int fd;
	socklen_t len;

	struct sock_opts * ptr;
	for (ptr = sock_opts; ptr->opt_str != NULL ; ptr++) {
		printf("%s: ", ptr->opt_str);
		if (ptr->option_val_str == NULL ) {
			printf("undifined\n ");
		} else {
			switch (ptr->opt_leval) {
			case SOL_SOCKET:
			case IPPROTO_TCP:
			case IPPROTO_IP:
				fd = socket(AF_INET, SOCK_STREAM, 0);
				break;
#ifdef IPV6
				case IPPROTO_IPV6:
				fd=socket(AF_INET6,SOCK_STREAM,0);
				break;
#endif

#ifdef IPPROTO_SCTP
				case IPPROTO_IPV6:
				fd=socket(AF_INET,SOCK_SEQPACKET,IPPROTO_SCTP);
				break;
#endif
			default:
				err_quit("can't create socket for level %d", ptr->opt_leval);
			}

			len = sizeof(val);
			if (getsockopt(fd, ptr->opt_leval, ptr->opt_name, &val, &len) == -1) {
				err_log("getsockopt error!\n");
			} else {
				printf("default = %s \n", ptr->option_val_str(&val, len));
			}
		}
		close(fd);
	}

	return 0;
}

