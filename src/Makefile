
all: huffman_trie bit_array

huffman_trie:
	g++ -c -fPIC huffman_trie.cpp -o lib/huffman_trie.o
	g++ -shared -Wl,-soname,lib_huffman_trie.so -o lib/lib_huffman_trie.so lib/huffman_trie.o

bit_array:
	g++ -c -fPIC bit_array.cpp -o lib/bit_array.o
	g++ -shared -Wl,-soname,lib_bit_array.so -o lib/lib_bit_array.so lib/bit_array.o


clean:
	rm lib/*
