#! usr/bin/python3
"""a method for the construction of minimum-redundancy codes
David A. Huffman, 1952"""

from collections import defaultdict, Sequence
from ctypes import cdll
from copy import copy
from heapq import heappush, heappop, heapify

lib_bit_array = cdll.LoadLibrary('./lib_bit_array.so')


class bit_array(Sequence):
    """an array of bits, wrapped C"""
    def __init__(self, lenght):
        assert lenght >= 0
        self.lenght = lenght
        self.bit_array = lib_bit_array.calloc_bit_array(lenght)

    def __len__(self):
        return self.lenght


    def set_bit(self, position):
        assert 0 <= position < self.lenght, "outside array"
        lib_bit_array.set_bit(self.bit_array, position)


    def clear_bit(self, position):
        assert 0 <= position < self.lenght, "outside array"
        lib_bit_array.clear_bit(sel.bit_array, position)


    def __getitem__(self, key):
        assert(0 <= key < self.lenght), "outside array"
        return(lib_bit_array.get_bit(self.bit_array, key))


    def __str__(self):
        result_string = []
        for i in range(self.lenght):
            result_string.append(str(self.__getitem__(i)))
            if (i+1)%10 == 0:
                result_string.append("  ")
            if (i+1)%50 == 0:
                result_string.append("{0}\n".format(i))
        return "".join(result_string)

    def __repr__(self):
        return self.__str__()



def get_occurence_dict(text):
    """get the mapping {character : number of occurences in the text}"""
    char_to_occurrence = defaultdict(int)  # char c -> int occurrences
    for char in text:
        char_to_occurrence[char] = char_to_occurrence.get(char, 0) + 1
    return char_to_occurrence


def get_frequency_table(occurence_dict, text_length):
    """convert occurrences to frequency"""
    frequency_table = []  # (char c, frequency (sums to 1))
    for k, v in occurence_dict.items():
        frequency_table.append((v / text_length, [k]))
    return frequency_table


def generate_binary_code(frequency_table):
    """generates binary code for each char
    use the exact same encoding as in the article"""
    frequency_table = copy(frequency_table)
    heapify(frequency_table)
    code = defaultdict(list)

    while len(frequency_table) > 1:
        one  = heappop(frequency_table)
        zero = heappop(frequency_table)
        for char in one[1]:
            code[char].append(1)
        for char in zero[1]:
            code[char].append(0)
        heappush(frequency_table, (one[0] + zero[0], one[1] + zero[1]))
    return code


def get_message_stats(occurrence_dict, binary_code):
    """get the size of the message, in bits"""
    sym_to_len = defaultdict(int)
    message_length = 0
    for (sym, occurrences) in occurrence_dict.items():
        message_length += len(binary_code[sym]) * occurrences
    print("message length = {0}".format(message_length))
    return (message_length)





