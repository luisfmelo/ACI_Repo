#
# Makefile
#
# ACI
# 1st Assignment
#

all: client server

client: clientModbus.c ApplicationLayer.c ApplicationLayer.h SessionLayer.c SessionLayer.h ModbusDataModel.c ModbusDataModel.h aux.c aux.h
	gcc -ggdb3 -O0 -std=c11 -Wall -Werror -D_BSD_SOURCE -o client clientModbus.c ApplicationLayer.c SessionLayer.c ModbusDataModel.c aux.c

server: serverModbus.c ApplicationLayer.c ApplicationLayer.h SessionLayer.c SessionLayer.h ModbusDataModel.c ModbusDataModel.h aux.c aux.h
	gcc -ggdb3 -O0 -std=c11 -Wall -Werror -D_BSD_SOURCE -o server serverModbus.c ApplicationLayer.c SessionLayer.c ModbusDataModel.c aux.c

clean:
	rm -f *.o a.out core client server
