/*
 * Huffman trie data structure
 *
 *
 * COMPILATION INSTRUCTIONS (on linux)
 * if on mac, figure out how to make a dll...
 * g++ -c -fPIC huffman_trie.cpp -o huffman_trie.o && g++ -shared -Wl,-soname,lib_huffman_trie.so -o lib_huffman_trie.so  huffman_trie.o
 *
 */


#include <cassert>
#include <vector>
#include <cstdlib>

#include <iostream>


using std::vector;


class Node {
    /*
     * Node object is used by the HuffmanTrie structure
     */

    public:
        char code;
        char symbol;
        vector<Node*> children;

        Node(char c, char sym=' ') {
            code = c;
            symbol = sym;
            children = vector<Node*>();
        }

        ~Node() { }


        Node* find_child(char c) {
            for ( int i = 0; i < children.size(); i++ ) {
                if (children[i]->code == c) {
                    return children[i];
                }
            }
            return NULL;
        }
};


class HuffmanTrie {
    /*
     * Trie is a customized trie, only usable for Huffman coding
     * because of the property of the code (no code is prefix of another)
     */

    private:
        Node* root;

    public:
        HuffmanTrie() {
            root = new Node(' ');
        }

        ~HuffmanTrie() { }

        void add_code(const char* code, const char symbol) {
            Node* position = root;
            for (const char* c = code; *c != '\0'; c++) {
                Node* child = position->find_child(*c);
                if ( child != NULL ) {
                    position = child;
                } else {
                    Node* tmp = new Node((*c));
                    position->children.push_back(tmp);
                    position = tmp;
                }
            }
            position->symbol = symbol;
        }

        char* decode(const char* message) {
            Node* position = root;
            vector<char> decoded_message = vector<char>();
            for ( const char* c = message; *c != '\0'; c++ ) {
                if (position->find_child(*c) == NULL) {
                    decoded_message.push_back(position->symbol);
                    position = root;
                    c--;
                } else {
                    position = position->find_child(*c);
                }
            }
            decoded_message.push_back(position->symbol);
            char* result_string = reinterpret_cast<char*>
                (malloc((decoded_message.size()+1) * sizeof(char)));
            for ( int i = 0; i < decoded_message.size(); i++ ) {
                result_string[i] = decoded_message[i];
            }
            result_string[decoded_message.size()] = '\0';
            return result_string;
        }
};

int main()
{
    HuffmanTrie* a = new HuffmanTrie();
    a->add_code("00", 'a');
    a->add_code("11", 'b');
    a->add_code("1001", 'c');
    std::cout << a->decode("00111001") << std::endl;
}

extern "C" {

    HuffmanTrie* _new() { return new HuffmanTrie(); }

    void _add_code(HuffmanTrie* trie, const char* binary_code, const char sym) {
        trie->add_code(binary_code, sym); }

    void _decode(HuffmanTrie* trie, const char* message) { trie->decode(message);}

}