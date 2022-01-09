CC       = gcc 
CFLAGS   = -O3 -Wall 
LIBS      = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL
OBJDIR   = ../mylib
OBJS     = $(OBJDIR)/initShader.o $(OBJDIR)/matlib.o

Project2: Project3.c $(OBJS)
	$(CC) -o Project3 Project3.c $(OBJS) $(CFLAGS) $(LIBS)
	
$(OBJDIR)/%.o: %.c %.h
	$(CC) -c @< -o $@ $(CFLAGS)

