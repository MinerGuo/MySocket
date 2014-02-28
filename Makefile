#VPATH=./:lib:lib/include
vpath %.h  lib lib/include ./
vpath %.c  lib lib/include ./
cc = gcc
CCFLAGS = -Werror  -g -D_GNU_SOURCE
o_005tcpclient = 005tcpcli.o 
o_005tcpclient2 = 005tcpcli2.o 
o_006tcpclient1 = 006tcpcli1.o 
o_006tcpclient2 = 006tcpcli2.o 
o_005tcpserver = 005tcpser.o 
o_005tcpserver2 = 005tcpser2.o 
o_005tcpserver3 = 005tcpser3.o 
o_006tcpserver1 = 006tcpser1.o 
o_006tcpserver2 = 006tcpser2.o 
o_007tcpcheckopt = 007tcpcheckopt.o 
o_008udpsrv1 = 008udpsrv1.o 
o_008udpcli1 = 008udpcli1.o 
o_comlib = comlib.o 
e_005tcpclient = exe_005tcpclient
e_005tcpclient2 = exe_005tcpclient2
e_006tcpclient1 = exe_006tcpclient1
e_006tcpclient2 = exe_006tcpclient2
e_005tcpserver = exe_005tcpserver
e_005tcpserver2 = exe_005tcpserver2
e_005tcpserver3 = exe_005tcpserver3
e_006tcpserver1 = exe_006tcpserver1
e_006tcpserver2 = exe_006tcpserver2
e_007tcpcheckopt = exe_007tcpcheckopt
e_008udpsrv1 = exe_008udpsrv1
e_008udpcli1 = exe_008udpcli1


all : $(e_005tcpclient) $(e_005tcpclient2) $(e_006tcpclient1)\
 $(e_006tcpclient2) $(e_005tcpserver) $(e_005tcpserver2)  \
 $(e_005tcpserver3)  $(e_006tcpserver1) $(e_006tcpserver2) \
 $(e_007tcpcheckopt)  $(e_008udpsrv1) $(e_008udpcli1) 

#$<表示依赖项 $@表示目标项

$(o_comlib)  : unp.c unp.h read.h readbuf.h
	cc  $(CCFLAGS) -c $< -o $@
	
$(o_005tcpclient) : 00005tcpcli01.c  $(o_comlib) 
	cc  $(CCFLAGS) -c $< -o $@

$(e_005tcpclient) : ${o_005tcpclient} $(o_comlib) 
	cc	$(CCFLAGS)  -o $@  $(o_005tcpclient) $(o_comlib) 
	
$(o_006tcpclient1) : 00006tcpcli01.c  $(o_comlib) 
	cc  $(CCFLAGS) -c $< -o $@

$(e_006tcpclient1) : ${o_006tcpclient1} $(o_comlib) 
	cc	$(CCFLAGS)  -o $@  $(o_006tcpclient1) $(o_comlib) 
	
$(o_006tcpclient2) : 00006tcpcli02.c  $(o_comlib) 
	cc  $(CCFLAGS) -c $< -o $@

$(e_006tcpclient2) : ${o_006tcpclient2} $(o_comlib) 
	cc	$(CCFLAGS)  -o $@  $(o_006tcpclient2) $(o_comlib) 
	
$(o_005tcpclient2) : 00005tcpcli02.c  $(o_comlib) 
	cc  $(CCFLAGS) -c $< -o $@

$(e_005tcpclient2) : ${o_005tcpclient2} $(o_comlib) 
	cc	$(CCFLAGS)  -o $@  $(o_005tcpclient2) $(o_comlib) 
	
$(o_005tcpserver) : 00005tcpsrv01.c $(o_comlib) 
	cc  $(CCFLAGS) -c $< -o $@

$(e_005tcpserver) : ${o_005tcpserver}  $(o_comlib) 
	cc	$(CCFLAGS) -o $@  $(o_005tcpserver)  $(o_comlib) 
	
$(o_005tcpserver2) : 00005tcpsrv02.c $(o_comlib) 
	cc  $(CCFLAGS) -c $< -o $@

$(e_005tcpserver2) : ${o_005tcpserver2}  $(o_comlib) 
	cc	$(CCFLAGS) -o $@  $(o_005tcpserver2)  $(o_comlib) 
	
$(o_005tcpserver3) : 00005tcpsrv03.c $(o_comlib) 
	cc  $(CCFLAGS) -c $< -o $@

$(e_005tcpserver3) : ${o_005tcpserver3}  $(o_comlib) 
	cc	$(CCFLAGS) -o $@  $(o_005tcpserver3)  $(o_comlib) 
	
$(o_006tcpserver1) : 00006tcpsrv01.c $(o_comlib) 
	cc  $(CCFLAGS) -c $< -o $@

$(e_006tcpserver1) : ${o_006tcpserver1}  $(o_comlib) 
	cc	$(CCFLAGS) -o $@  $(o_006tcpserver1)  $(o_comlib) 
	
	
$(o_006tcpserver2) : 00006tcpsrv02.c $(o_comlib) 
	cc  $(CCFLAGS) -c $< -o $@

$(e_006tcpserver2) : ${o_006tcpserver2}  $(o_comlib) 
	cc	$(CCFLAGS) -o $@  $(o_006tcpserver2)  $(o_comlib) 

$(o_007tcpcheckopt) : 00007tcpcheckopt.c $(o_comlib) 
	cc  $(CCFLAGS) -c $< -o $@

$(e_007tcpcheckopt) : ${o_007tcpcheckopt}  $(o_comlib) 
	cc	$(CCFLAGS) -o $@  $(o_007tcpcheckopt)  $(o_comlib) 

$(o_008udpsrv1) : 00008udpsrv01.c $(o_comlib) 
	cc  $(CCFLAGS) -c $< -o $@

$(e_008udpsrv1) : ${o_008udpsrv1}  $(o_comlib) 
	cc	$(CCFLAGS) -o $@  $(o_008udpsrv1)  $(o_comlib) 
	
	
$(o_008udpcli1) : 00008udpcli01.c $(o_comlib) 
	cc  $(CCFLAGS) -c $< -o $@

$(e_008udpcli1) : ${o_008udpcli1}  $(o_comlib) 
	cc	$(CCFLAGS) -o $@  $(o_008udpcli1)  $(o_comlib) 

clean :
	rm $(o_005tcpclient) $(o_005tcpclient2) $(o_006tcpclient1)  \
	$(o_006tcpclient2) $(o_005tcpserver) $(o_006tcpserver1) $(o_006tcpserver2) $(o_005tcpserver2) \
	$(o_005tcpserver3) $(o_comlib)  $(e_005tcpclient) $(e_005tcpclient2) \
	$(e_006tcpclient1) $(e_006tcpclient2)  $(e_005tcpserver)  $(e_005tcpserver2) \
	 $(e_006tcpserver1) $(e_006tcpserver2)  $(e_007tcpcheckopt) \
	 $(o_008udpsrv1) $(e_008udpsrv1)  $(o_008udpcli1) $(e_008udpcli1)