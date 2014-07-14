/*
 * Bit array data structure
 *
 *
 * COMPILATION INSTRUCTIONS (on linux)
 * if on mac, figure out how to make a dll...
 * g++ -c -fPIC bit_array.cpp -o bit_array.o && g++ -shared -Wl,-soname,lib_bit_array.so -o lib_bit_array.so  bit_array.o
 *
 */

#include <cstdlib>
#include <cassert>

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


extern "C" {
    /*
     * BIT ARRAY STRUCTURE
     * contiguous array of int
     */
    BitArray* _new(int size) {
        return new BitArray(size);}

    BitArray* _free(BitArray* array) {
        array->~BitArray();}

    void _clear_bit(BitArray* array, int position) {
        array->clear_bit(position);}

    void _set_bit(BitArray* array, int position) {
        array->set_bit(position);}

    int  _get_bit(BitArray* array, int position) {
        array->get_bit(position);}
}
