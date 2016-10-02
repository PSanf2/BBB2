#!/bin/bash

./bash/clean.sh

echo "Building RobotWalk main..."

./bash/compile_BBIO_Lite.sh

g++ -o obj/IR_LED.o -c src/RobotWalk/IR_LED.cpp
g++ -o obj/IR_Sensor.o -c src/RobotWalk/IR_Sensor.cpp
g++ -o obj/Servo.o -c src/RobotWalk/Servo.cpp

g++ -o obj/RobotWalk_main.o -c src/RobotWalk/RobotWalk_main.cpp

g++ -o bin/RobotWalk obj/* -lpthread

echo "Done Building RobotWalk main"
