/* huffman encoding bit array data structure
 * based on
 * www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
 *
 *
 * COMPILATION INSTRUCTIONS
 * compiling with position independent code
 * gcc -c -Wall -Werror -fpic
 *
 * creating a shared lib
 * gcc -shared -o
 */


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define LINE_SIZE 50

int* calloc_bit_array(int num_bits)
{
  //allocates bit array through int array (memory access is efficient)
  assert(num_bits > 0);
  int array_length = (num_bits / (sizeof(int) * 8));

  if( (num_bits % (sizeof(int) * 8)) != 0 )
  {
    array_length ++;
  }

  int* bit_array = calloc(array_length, sizeof(int));

  return bit_array;
 }


void set_bit(int* bit_array, int position)
{
  /* bit gets value of 1 at given position
   * the bit array is initially all zeros (calloc)
   */
  assert(position >= 0);
  bit_array[position/(sizeof(int)*8)] |= 1 << (position % (sizeof(int)*8));
}


void clear_bit(int* bit_array, int position)
{
  /* bit gets value of 0 at given position
   * the bit array is initially all zeros (calloc)
   */
  assert(position >= 0);
  bit_array[position/(sizeof(int)*8)] &= ~(1 << (position %(sizeof(int)*8)));
}


int get_bit(int* bit_array, int position)
{
  assert(position >= 0);
  return ( (bit_array[position/(sizeof(int)*8)] & (1 << (position % (sizeof(int)*8))))  != 0);
}


// char* bit_array_to_char(int* bit_array, int size)
// {
//   int num_lines = size / LINE_SIZE;
//   if(size % LINE_SIZE != 0)
//   {
//     num_lines ++;
//   }
// 
//   char* result = (char*) calloc((num_lines * LINE_SIZE) + 1, sizeof(char));
//   printf("%s\n", result);
//   int i = 0,  offset = 0;
//   for(i = 0; i < size; i ++)
//   {
//     if(i % LINE_SIZE == 0)
//     {
//       result[i + offset] = '\n';
//       offset++;
//     }
//     int bit = get_bit(bit_array, i);
//     if(bit == 1)
//     {
//       result[i + offset] = '1';
//     }
//   }
//   result[i + offset + 1] = '\0';
//   return result;
// }
// 
// 
// void print_bit_array(int* bit_array, int size)
// {
//   int i;
//   for(i = 0; i< size; i++)
//   {
//     if( (i % 8 == 0) && (i % 72 != 0))
//     {
//       printf("    ");
//     }
//     if(i % 72 == 0)
//     {
//       printf("\n");
//     }
//     printf("%d", get_bit(bit_array, i));
//   }
//   printf("\n");
// }
// 


#define ARRAY_SIZE 42
int main()
{
  int* bit_array = calloc_bit_array(ARRAY_SIZE);
  print_bit_array(bit_array, ARRAY_SIZE);

  set_bit(bit_array, 12);
  print_bit_array(bit_array, ARRAY_SIZE);

  set_bit(bit_array, 12);
  print_bit_array(bit_array, ARRAY_SIZE);

  clear_bit(bit_array, 12);
  print_bit_array(bit_array, ARRAY_SIZE);
  return 0;

}
