rm -f main
g++ -O2 -I. *.cpp -o main -Wno-deprecated-declarations -lcrypto -lssl -lsecp256k1
