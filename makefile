name := gamma
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all 
all: lib bin

#CPPFLAGS+=-Wno-overloaded-virtual -Wno-unused

include $(G4INSTALL)/config/binmake.gmk


