object_files/c_matrix.o : c/matrix.h c/matrix.c
	gcc -pthread -c c/matrix.c -o $@

executables/c_tmatmul.out : c/main.c object_files/c_matrix.o
	gcc -pthread c/main.c object_files/c_matrix.o -o $@

c : executables/c_tmatmul.out


object_files/cpp_matrix.o : cpp/matrix.hpp cpp/matrix.cpp
	g++ -pthread -c cpp/matrix.cpp -o $@

executables/cpp_tmatmul.out : cpp/main.cpp object_files/cpp_matrix.o
	g++ -pthread cpp/main.cpp object_files/cpp_matrix.o -o $@

cpp : executables/cpp_tmatmul.out

