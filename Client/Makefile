C:=window.c init.c md3.c render.c network.c network_temp_fixes.c rts_wrap.c
CPLUSPLUS:=
EXTENSION:=a
OBJECTS:=window.o init.o md3.o render.o network.o network_temp_fixes.o rts_wrap.o

all:
	make -s -C . quiet
quiet:
	#gcc -O3 -c -Wall -g $(C) 
	#g++ -O3 -c -Wall -g $(CPLUSPLUS) 
	#gcc $(OBJECTS) -lSDL -lSDL_net -shared -o librtsclient.$(EXTENSION)
	$(foreach FILE,$(C),echo "CC "$(FILE)"" && gcc -O3 -c -Wall -g -I/usr/include/freetype2 -I/usr/include/python2.6 -fPIC $(FILE) && ) ar rcs librtsclient.$(EXTENSION) $(OBJECTS)
	gcc -O3 -Wall -g main.c -I/usr/include/freetype2 -L. -lrtsclient -lSDL -lSDL_net -lGL -lGLU -lIL -lfreetype
