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

typedef struct
{
  int size;
  int* bits;
} BitArray;



BitArray* BitArray_new(int size)
{
  //allocates bit array through int array (memory access is efficient)
  assert(size > 0);
  int array_length = (size / (sizeof(int) * 8));

  if( (size % (sizeof(int) * 8)) != 0 )
  {
    array_length ++;
  }

  BitArray* result = malloc(sizeof(BitArray));
  result->size = size;
  result->bits = calloc(array_length, sizeof(int));
  return result;
 }


void set_bit(BitArray* array, int position)
{
  /* bit gets value of 1 at given position
   * the bit array is initially all zeros (calloc)
   */
  assert(0 <= position < array->size);
  (array->bits)[position/(sizeof(int)*8)] |= 1 << (position % (sizeof(int)*8));
}


void clear_bit(BitArray* array, int position)
{
  /* bit gets value of 0 at given position
   * the bit array is initially all zeros (calloc)
   */
  assert(0 <= position < array->size);
  (array->bits)[position/(sizeof(int)*8)] &= ~(1 << (position %(sizeof(int)*8)));
}


int get_bit(BitArray* array, int position)
{
  assert(0 <= position < array->size);
  return ( ((array->bits)[position/(sizeof(int)*8)] & (1 << (position % (sizeof(int)*8))))  != 0);
}


void free_bit_array(BitArray* array)
{
  free(array->bits);
  free(array);
}


void print_bit_array(BitArray* array)
{
  int i;
  for(i = 0; i < array->size; i++)
  {
    if( (i% 10 == 0) && (i % 60 != 0))
    {
      printf("  ");
    }
    if(i % 60 == 0)
    {
      printf("\n");
    }
    printf("%d", get_bit(array, i));
  }
  printf("\n");
}


char* bit_array_toString(BitArray* array)
{
  char* result = malloc(array->size * sizeof(char));
  int i;
  for(i = 0; i < array->size; i++)
  {
    if(get_bit(array, i) == 0)
    {
      result[i] = '0';
    }
    else
    {
      result[i] = '1';
    }
  }
  return result;
}


int main()
{
  BitArray* a = BitArray_new(64);
  print_bit_array(a);
  free_bit_array(a);
  return 0;

}
