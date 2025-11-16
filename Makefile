CC= g++
INCL=/home/valentina/lib
COMPILE = $(CC) -c -Wall -I $(INCL)
LINK = $(CC)

OBJS= a.o ubicacio.o
#OBJS= prog.o terminal.o cataleg.o contenidor.o ubicacio.o

a.exe : $(OBJS)
	$(LINK) -o a.exe $(OBJS) -lesin

a.o : a.cpp
	$(COMPILE) a.cpp



# prog.exe : $(OBJS) $(LIBS)/
# 	$(LINK) -o prog.exe $(OBJS) -lesin

#prog.o : prog.cpp
#	$(COMPILE) prog.cpp -lesin

# terminal.o : terminal.cpp
# 	$(COMPILE) terminal.cpp -lesin


# cataleg.o : cataleg.cpp
# 	$(COMPILE) cataleg.cpp -lesin



ubicacio.o : ubicacio.cpp
	$(COMPILE) ubicacio.cpp

#contenidor.o : contenidor.cpp
#	$(COMPILE) contenidor.cpp -lesin





clean:
	rm *.o;
	rm *.exe;
	echo "borrado"
