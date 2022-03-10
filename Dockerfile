FROM gcc:latest
#FROM ubuntu:latest

COPY . /usr/src/myapp
WORKDIR /usr/src/myapp

#RUN apt update
#RUN apt install g++ -y
RUN g++ -o myapp main.cpp

EXPOSE 8001

CMD ["./myapp"]

LABEL Name=linuxserver Version=0.0.1