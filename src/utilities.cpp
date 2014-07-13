/* binary code trie data structure
 *
 * COMPILATION INSTRUCTIONS
 * compiling with position independent code
 * gcc -c -Wall -Werror -fpic
 *
 *
 * creating a shared lib
 * gcc -shared -o
 *
 *
 *
 * c++ lib implementation of Trie data structure
 * with extern C declaration to allow dll
 * compilation instructions
 *
 * g++ -c -fPIC utilities.cpp -o utilities.o && g++ -shared -Wl,-soname,utilities.so -o utilities.so  utilities.o
*/

#include <cassert>
#include <vector>
#include <cstdlib>


using std::vector;


class BitArray {
    public:
        BitArray(int length) {
            /*
             * BitArray is a contiguous array of int allowing
             * bit manipulation through bitwise operations
             */
            assert(length > 0);
            int array_length = (length / (sizeof(int) * 8));
            if ( (length % (sizeof(int) * 8)) != 0 ) {
                array_length++;
            }
            size = length;
            bits = reinterpret_cast<int*>(calloc(array_length, sizeof(int)));
        }

        ~BitArray() {
            free(bits);
        }

        void set_bit(int position) {
            /* bit gets value of 1 at given position
             * the bit array is initially all zeros (calloc)
             */
            assert(0 <= position < size);
            bits[position/(sizeof(int)*8)] |=
                1 << (position % (sizeof(int)*8));
        }


        void clear_bit(int position) {
            /* bit gets value of 0 at given position
             * the bit array is initially all zeros (calloc)
             */
            assert(0 <= position < size);
            bits[position/(sizeof(int)*8)] &=
                ~(1 << (position %(sizeof(int)*8)));
        }


        int get_bit(int position) {
            assert(0 <= position < size);
            return ( ((bits)[position/(sizeof(int)*8)] &
                (1 << (position % (sizeof(int)*8))))  != 0);
        }

    private:
        int size;
        int* bits;
};


class Node {
    /*
     * Node object is used by the Trie structure
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


class Trie {
    /*
     * Trie is a customized trie, only usable for Huffman coding
     * because of the property of the code (no code is prefix of another)
     */

    private:
        Node* root;

    public:
        Trie() {
            root = new Node(' ');
        }

        ~Trie() { }

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
                (malloc(decoded_message.size() * sizeof(char)));
            for ( int i = 0; i < decoded_message.size(); i++ ) {
                result_string[i] = decoded_message[i];
            }
            return result_string;
        }
};




extern "C" {
    /*
     * BIT ARRAY STRUCTURE
     * contiguous array of int
     */
    BitArray* BitArray_new(int size) {
        return new BitArray(size);}
    BitArray* BitArray_free(BitArray* array) {
        array->~BitArray();}
    void BitArray_clear_bit(BitArray* array, int position) {
        array->clear_bit(position);}
    void BitArray_set_bit(BitArray* array, int position) {
        array->set_bit(position);}
    int  BitArray_get_bit(BitArray* array, int position) {
        array->get_bit(position);}

    /*
     * TRIE STRUCTURE
     * each node of the trie has a maximum of 2 child
     */
    Trie* Trie_new() { return new Trie(); }
    void Trie_add_code(Trie* trie, const char* binary_code, const char sym) {
        trie->add_code(binary_code, sym);
    }
    void Trie_decode(Trie* trie, const char* message) { trie->decode(message);}

}


int main() {
    // Test program
//     Trie* trie = Trie_new();
//     Trie_add_code(trie, "11", 'a');
//     Trie_add_code(trie, "00", 'b');
//     Trie_add_code(trie, "1001", 'c');
//     Trie_add_code(trie, "011", 'd');
//     Trie_add_code(trie, "101", 'e');
//     cout << "-----------------" << endl;
//     cout << trie->decode("11") << endl;
//     cout << trie->decode("00") << endl;
//     cout << trie->decode("1001") << endl;
//     cout << "-----------------" << endl;
//     cout << trie->decode("1100") <<endl;
//     cout << trie->decode("10011001100111001001011")<<endl;
}
