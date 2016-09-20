#!/bin/bash

./bash/clean.sh

echo "Building Robot main..."

./bash/compile_BBIO_Lite.sh

g++ -o obj/Button.o -c src/Robot/Button.cpp
g++ -o obj/IR_LED.o -c src/Robot/IR_LED.cpp
g++ -o obj/IR_Sensor.o -c src/Robot/IR_Sensor.cpp
g++ -o obj/LED.o -c src/Robot/LED.cpp
g++ -o obj/LightSensor.o -c src/Robot/LightSensor.cpp
g++ -o obj/Servo.o -c src/Robot/Servo.cpp

g++ -o obj/Robot_main.o -c src/Robot/Robot_main.cpp

g++ -o bin/Robot obj/* -lpthread

echo "Done Building Robot main"
