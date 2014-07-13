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
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>

using namespace std;


class BitArray
{
    public:

        BitArray(int length)
        {
            //allocates bit array through int array
            assert(length > 0);
            int array_length = (length / (sizeof(int) * 8));
            if( (length % (sizeof(int) * 8)) != 0 )
            {
                array_length ++;
            }
            cout << "allocated " << array_length << " bytes"<< endl;
            size = length;
            bits = (int*) calloc(array_length, sizeof(int));
        }

        ~BitArray()
        {
            free(bits);
//             free(size);
        }

        void set_bit(int position)
        {
            /* bit gets value of 1 at given position
             * the bit array is initially all zeros (calloc)
             */
            assert(0 <= position < size);
            bits[position/(sizeof(int)*8)] |= 1 << (position % (sizeof(int)*8));
        }


        void clear_bit(int position)
        {
            /* bit gets value of 0 at given position
             * the bit array is initially all zeros (calloc)
             */
            assert(0 <= position < size);
            bits[position/(sizeof(int)*8)] &= ~(1 << (position %(sizeof(int)*8)));
        }


        int get_bit(int position)
        {
            assert(0 <= position < size);
            return ( ((bits)[position/(sizeof(int)*8)] & (1 << (position % (sizeof(int)*8))))  != 0);
        }


//     private:
        int size;
        int* bits;
};


class BinaryNode
{
    public:
        BinaryNode()
        {
            char symbol = ' ';
            BinaryNode* children[] = {NULL, NULL};
        }

        ~BinaryNode() {}

        BinaryNode* get_child(char index)
        {
            BinaryNode* child;
            index == '0' ? child = children[0] : child = children[1];
            return child;
        }

        void new_child(char index)
        {
            index == '0' ? children[0] = new BinaryNode() : children[1] = new BinaryNode();
        }

        void set_symbol(char sym)
        {
            symbol = sym;
        }

        char get_symbol()
        {
            return symbol;
        }

    private:
        char symbol;
        BinaryNode* children[];
};


class BinaryTrie
{   /*trie data structure*/
    public:
        BinaryTrie():
            root(new BinaryNode()) { }

        ~BinaryTrie(){ }

        void add_code(const char* binary_code, char sym)
        {
            BinaryNode* position = root;
            for (const char* c = binary_code; *c != '\0'; c++)
            {
                if(position->get_child(*c) == NULL)
                {
                    position->new_child(*c);
                }
                position = position->get_child(*c);
            }
            position->set_symbol(sym);
        }


        string decode(char* message)
        {
            BinaryNode* position = root;
            vector<char> decoded_message = vector<char>();
            for(const char* c = message; *c != '\0'; c++)
            {
                if(position->get_child(*c) == NULL)
                {
                    decoded_message.push_back(position->get_symbol());
                    position = root;
                    c--;
                }
                else
                {
                    position = position->get_child(*c);
                }
            }
            decoded_message.push_back(position->get_symbol());
            string result (decoded_message.begin(), decoded_message.end());
            return result;
        }

    private:
        BinaryNode* root;
};


extern "C"
{
    /*
     * BIT ARRAY STRUCTURE
     * contiguous array of int
     */
    BitArray* BitArray_new(int size){ return new BitArray(size); }
    BitArray* BitArray_free(BitArray* array){ array->~BitArray();}
    void BitArray_clear_bit(BitArray* array, int position){ array->clear_bit(position); }
    void BitArray_set_bit(BitArray* array, int position){ array->set_bit(position); }
    int  BitArray_get_bit(BitArray* array, int position){ array->get_bit(position); }

    /*
     * BINARY TRIE STRUCTURE
     * each node of the trie has a maximum of 2 child
     */
    BinaryTrie* BinaryTrie_new(){ return new BinaryTrie(); }
    void BinaryTrie_add_code(BinaryTrie* trie, const char* binary_code, char sym){ trie->add_code(binary_code, sym);}
    void BinaryTrie_decode(BinaryTrie* trie, char* message){ trie->decode(message); }

}


int main()
{
    // Test program
    
    BinaryTrie* trie = new BinaryTrie();
    trie->add_code("11", 'a');
    trie->add_code("00", 'b');
    BinaryTrie_add_code(trie, "1001", 'c');
    trie->add_code("011", 'd');
    cout << "-----------------" << endl;
    cout << trie->decode("11") << endl;
    cout << trie->decode("00") << endl;
    cout << trie->decode("1001") << endl;
    cout << "-----------------" << endl;
    cout << trie->decode("1100") <<endl;
    cout << trie->decode("11001001011")<<endl;
    
    
//     delete(bit);
}