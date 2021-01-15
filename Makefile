#!/bin/make
# Makefile

CXX = g++
CXX_FLAGS = -Wall
ALLEGRO_LIB = `pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_audio-5 allegro_ttf-5 allegro_acodec-5 --libs --cflags`

SOURCE = Sources/*.cpp
EXECS = BattleCity

all:
	$(CXX) -o $(EXECS) $(SOURCE) $(ALLEGRO_LIB) $(CXX_FLAGS)

clean:
	$(RM) $(EXECS)