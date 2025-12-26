FROM alpine:latest

WORKDIR /app

# Install g++, make, and other build tools
RUN apk add --no-cache build-base

COPY lab7.h lab7.cpp Makefile ./

RUN make all

ENTRYPOINT ["./lab7"]