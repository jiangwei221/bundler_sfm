FROM ubuntu:16.04

RUN apt-get update

RUN apt-get install -y cmake libgmm++-dev liblapack-dev libf2c2-dev unzip g++ vim wget git zlib1g-dev jhead libjpeg-dev

RUN cd /home && git clone https://github.com/jiangwei221/bundler_sfm.git

WORKDIR /home/bundler_sfm

RUN make

RUN cp /home/bundler_sfm/bin/libANN_char.so /usr/lib

RUN echo "export PATH=/home/bundler_sfm/bin:$PATH" >> ~/.bashrc 
