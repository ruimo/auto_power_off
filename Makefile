all : flash

CH32V003FUN:=../ch32v003fun/ch32v003fun
TARGET:=auto_power_off

test : flash
	$(CH32V003FUN)/../minichlink/minichlink -X ECLK:1:0:0:1:63

include $(CH32V003FUN)/ch32v003fun.mk

flash : cv_flash
clean : cv_clean


