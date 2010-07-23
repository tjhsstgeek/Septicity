C:=component.c panel.c button.c render.c event.c text.c checkbox.c
EXTENSION:=a
OBJECTS:=component.o panel.o button.o render.o event.o text.o checkbox.o

all:
	make -s -C . quiet
quiet:
	$(foreach FILE,$(C),echo "CC "$(FILE)"" && gcc -O3 -c -Wall -g -I/usr/include/freetype2/ $(FILE) && ) ar rcs libgui.$(EXTENSION) $(OBJECTS)
	
