#VPATH=./:IO3:IO3/include:Signal10:Signal10/include:daemon13
vpath %.h IO3/include Signal10/include 
vpath %.c IO3 Signal10 daemon13
cc = gcc
CCFLAGS = -Werror 
all_objects1 = iscanseek.o 
all_objects2 = holefile.o 
all_objects3 = dopread.o 
all_objects4 = catchsignal.o
all_objects5 = unrein.o 
all_objects6 = unworksigchld.o 
all_objects7 = sigprocmask1.o 
all_objects8 = sigprocmask2.o 
all_objects9 = sigprocmask3.o 
all_objects10 = sigprocmask.o 
all_objects11 = parentChildWrite.o 
all_objects12 = mutexAndCondition.o 
all_objects13 = restartedok.o 
all_objects14 = rule.o 





all : iscanseek holefile dopread catchsignal unrein unworksigchld sigprocmask1 sigprocmask2 sigprocmask3 \
 parentChildWrite mutexAndCondition restartedok rule

rule.o : rule.c
	cc  $(CCFLAGS) -c $< 

rule : ${all_objects14} 
	cc	$(CCFLAGS) -o $@  $(all_objects14) 


restartedok.o : restartedok.c
	cc  $(CCFLAGS) -c $< 

restartedok : ${all_objects13} 
	cc	$(CCFLAGS) -o $@  $(all_objects13) -lpthread 
	
	
mutexAndCondition.o : mutexAndCondition.c
	cc  $(CCFLAGS) -c $< 

mutexAndCondition : ${all_objects12} 
	cc	$(CCFLAGS) -o $@  $(all_objects12) -lpthread 
	
parentChildWrite.o : parentChildWrite.c
	cc  $(CCFLAGS) -c $< 

parentChildWrite : ${all_objects11} 
	cc	$(CCFLAGS) -o $@  $(all_objects11)


sigprocmask.o : sigprocmask.c  sigprocmask.h
	cc  $(CCFLAGS) -c $< 

sigprocmask2.o : sigprocmask2.c 
	cc  $(CCFLAGS) -c $< 
sigprocmask2 : ${all_objects8} 
	cc	$(CCFLAGS) -o $@  $(all_objects8)
	

sigprocmask3.o : sigprocmask3.c sigprocmask.h
	cc  $(CCFLAGS) -c $<
	
sigprocmask3 : $(all_objects9) $(all_objects10) 
	cc  $(CCFLAGS) -o $@  $(all_objects9) $(all_objects10) 
	
iscanseek : $(all_objects1)
	cc  $(CCFLAGS) -o $@  $(all_objects1)

iscanseek.o : iscanseek.c iscanseek.h 
	cc  $(CCFLAGS) -c $< 

holefile : $(all_objects2)
	cc  $(CCFLAGS) -o $@  $(all_objects2)

holefile.o : holefile.c holefile.h 
	cc  $(CCFLAGS) -c $< 
	
dopread : $(all_objects3)
	cc  $(CCFLAGS) -o $@  $(all_objects3)

dopread.o : dopread.c dopread.h 
	cc  $(CCFLAGS) -c $< 

catchsignal: $(all_objects4)
	cc  $(CCFLAGS) -o $@  $(all_objects4)

catchsignal.o : catch.c catch.h 
	cc  $(CCFLAGS) -c $< -o $(all_objects4)


unrein: $(all_objects5)
	cc  $(CCFLAGS) -o $@  $(all_objects5)

unrein.o : unrein.c unrein.h 
	cc  $(CCFLAGS) -c $< -o $(all_objects5)
	
unworksigchld: $(all_objects6)
	cc  $(CCFLAGS) -o $@  $(all_objects6)
	
unworksigchld.o : unworksigchld.c
	cc  $(CCFLAGS) -c $< -o $(all_objects6)
	
sigprocmask1: $(all_objects7) $(all_objects10) 
	cc  $(CCFLAGS) -o $@  $(all_objects7) $(all_objects10) 
	
sigprocmask1.o : sigprocmask1.c sigprocmask.h
	cc  $(CCFLAGS) -c $< -o $(all_objects7)


clean :
	rm $(all_objects1) $(all_objects2) $(all_objects3)\
	 $(all_objects4) $(all_objects5) $(all_objects6) $(all_objects7)\
	 $(all_objects8)  $(all_objects9) $(all_objects10) $(all_objects11) \
	 $(all_objects12) $(all_objects13) $(all_objects14) 