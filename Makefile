CC= g++
COMPILE = $(CC) -c -Wall
LINK = $(CC)

OBJUBI= ubicacio_driver.o ubicacio.o
OBJCONT= contenidor_driver.o contenidor.o
OBJCAT= cataleg_driver.o
OBJTERM= terminal_driver.o terminal.o ubicacio.o contenidor.o
OBJGEST = driver_gestterm.o terminal.o ubicacio.o contenidor.o

ubi : ubicacio_driver.exe
cont: contenidor_driver.exe
cat: cataleg_driver.exe
term: terminal_driver.exe
gest: driver_gestterm.exe
clean:
	rm *.o;
	rm *.exe;
	rm *.out;
	@echo "Fitxers esborrats\n"

#-------------------------------------------------------
# UBICACIO

ubicacio_driver.exe : $(OBJUBI)
	$(LINK) -o ubicacio_driver.exe $(OBJUBI) -lesin
ubicacio_driver.o : ubicacio_driver.cpp
	$(COMPILE) ubicacio_driver.cpp
ubicacio.o : ubicacio.cpp
	$(COMPILE) ubicacio.cpp -lesin
#-------------------------------------------------------
# CONTENIDOR

contenidor_driver.exe : $(OBJCONT)
	$(LINK) -o contenidor_driver.exe $(OBJCONT) -lesin
contenidor_driver.o : contenidor_driver.cpp
	$(COMPILE) contenidor_driver.cpp
contenidor.o : contenidor.cpp
	$(COMPILE) contenidor.cpp -lesin

#-------------------------------------------------------
# CATALEG

cataleg_driver.exe : $(OBJCAT)
	$(LINK) -o cataleg_driver.exe $(OBJCAT) -lesin

cataleg_driver.o : cataleg_driver.cpp cataleg.hpp cataleg.rep cataleg.t
	$(COMPILE) cataleg_driver.cpp

#-------------------------------------------------------
# TERMINAL

terminal_driver.exe : $(OBJTERM)
	$(LINK) -o terminal_driver.exe $(OBJTERM) -lesin

terminal_driver.o : terminal_driver.cpp terminal.hpp
	$(COMPILE) terminal_driver.cpp

terminal.o : terminal.cpp terminal.hpp terminal.rep
	$(COMPILE) terminal.cpp

#-------------------------------------------------------
# GESTTERM

driver_gestterm.exe : $(OBJGEST)
	$(LINK) -o driver_gestterm.exe $(OBJGEST) -lesin

driver_gestterm.o : driver_gestterm.cpp terminal.hpp terminal.rep cataleg.hpp cataleg.rep cataleg.t ubicacio.hpp contenidor.hpp gestterm.err
	$(COMPILE) driver_gestterm.cpp
