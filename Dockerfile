FROM gcc:latest

WORKDIR /app

COPY lab7.h lab7.cpp Makefile ./

RUN make all

ENTRYPOINT ["./lab7"]