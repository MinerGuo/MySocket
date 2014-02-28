#VPATH=./:include
vpath %.h include
cc = gcc
CCFLAGS = -Werror
all_objects1 = main.o fork.o
all_objects2 = main2.o fork2.o

all : fork1 fork2

fork1 : $(all_objects1)
	cc  $(CCFLAGS) -o $@  $(all_objects1)
fork2 : $(all_objects2)
	cc  $(CCFLAGS) -o $@  $(all_objects2)
	
main.o : main.c 
	cc  $(CCFLAGS) -c $< 

fork.o : fork.c fork.h
	cc  $(CCFLAGS) -c $<

main2.o :  main2.c 
	cc  $(CCFLAGS) -c $<

fork2.o : fork2.c fork2.h
	cc  $(CCFLAGS) -c $<


clean :
	rm $(all_objects1) $(all_objects2)
	
# $@ 表示目标项； $<表示依赖项
