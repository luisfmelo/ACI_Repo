SRC =	./ServerClientTCP/server.c	\
	./Modbus/serverModbus.c		\
	./Modbus/modbusAPDU.c		\
	./Modbus/modbusTCP_Session.c	\
	./Modbus/tcp.c

INCLUDE = -I./Modbus

server: $(SRC)
	cc $(SRC) -I $(INCLUDE) -o server

server.o: $(INCLUDE)
