CC= g++
COMPILE = $(CC) -c -Wall
LINK = $(CC)

OBJUBI= ubicacio_driver.o ubicacio.o
OBJCONT= contenidor_driver.o contenidor.o

ubi : ubicacio_driver.exe
cont: contenidor_driver.exe
clean:
	rm *.o;
	rm *.exe;
	echo "borrado"

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



