#! usr/bin/python3
"""a method for the construction of minimum-redundancy codes
David A. Huffman, 1952"""

from collections import defaultdict, Sequence, Counter
from ctypes import cdll, c_char_p
from heapq import heappush, heappop, heapify


utilities = cdll.LoadLibrary('./utilities.so')
utilities.Trie_decode.restype = c_char_p


class BitArray(Sequence):
    """an array of bits, wrapped C"""
    def __init__(self, length):
        assert length >= 0
        self.length = length
        self.obj = utilities.BitArray_new(length)

    def __len__(self):
        return self.length

    def set_bit(self, position):
        assert 0 <= position < self.length, "outside array"
        utilities.BitArray_set_bit(self.obj, position)

    def clear_bit(self, position):
        assert 0 <= position < self.length, "outside array"
        utilities.BitArray_clear_bit(self.obj, position)

    def __getitem__(self, position):
        assert(0 <= position < self.length), "outside array"
        return(utilities.BitArray_get_bit(self.obj, position))

    def __str__(self):
        """pretty print of the bit array"""
        result_string = []
        for i in range(self.length):
            result_string.append(str(self.__getitem__(i)))
            if (i+1) % 10 == 0:
                result_string.append("  ")
            if (i+1) % 50 == 0:
                result_string.append("{0}\n".format(i))
        return "".join(result_string)

    def to_string(self):
        """get the contiguous array of bits, in string form"""
        result_string = []
        for i in range(self.length):
            result_string.append(str(self.__getitem__(i)))
        return "".join(result_string)

    def __repr__(self):
        return self.__str__()

    def __del__(self):
        utilities.BitArray_free(self.obj)


class Trie(object):
    """trie structure to decode huffman encoded string"""
    def __init__(self):
        self.obj = utilities.Trie_new()

    def add_code(self, symbol, code):
        code, symbol = str.encode(code), ord(symbol)
        utilities.Trie_add_code(self.obj, code, symbol)

    def decode(self, message):
        return(utilities.Trie_decode(self.obj, str.encode(message)))


def get_frequency_table(message):
    """get the frequency of each symbol in the message"""
    symbol_count = Counter(message)
    frequency_table = []  # (sym, frequency (sums to 1))
    for (symbol, count) in symbol_count.items():
        frequency_table.append((count / len(message), [symbol]))
    return frequency_table


def generate_binary_code(frequency_table):
    """generates binary code for each symbol
    use the exact same encoding as in the article"""
    heapify(frequency_table)
    code = defaultdict(list)
    while len(frequency_table) > 1:
        one = heappop(frequency_table)
        zero = heappop(frequency_table)
        for sym in one[1]:
            code[sym].append(1)
        for sym in zero[1]:
            code[sym].append(0)
        heappush(frequency_table, (one[0] + zero[0], one[1] + zero[1]))
    code2 = defaultdict(str)
    for (symbol, binary) in code.items():
        bin_str = "".join([str(i) for i in reversed(binary)])
        code2[symbol] = bin_str
    return code2


def encode(message):
    """huffman code the message"""
    # step 1: get the symbol frequency
    symbol_count = Counter(message)
    frequency_table = []  # (sym, frequency (sums to 1))
    for (symbol, count) in symbol_count.items():
        frequency_table.append((count / len(message), [symbol]))

    # step 2: create the binary code
    heapify(frequency_table)
    sym_to_binary = defaultdict(list)
    while len(frequency_table) > 1:
        one = heappop(frequency_table)
        zero = heappop(frequency_table)
        for sym in one[1]:
            sym_to_binary[sym].append(1)
        for sym in zero[1]:
            sym_to_binary[sym].append(0)
        heappush(frequency_table, (one[0] + zero[0], one[1] + zero[1]))
    code = defaultdict(str)
    for (symbol, binary) in sym_to_binary.items():
        bin_str = "".join([str(i) for i in reversed(binary)])
        code[symbol] = bin_str

    # step 3: figure out the total number of bits that will be used
    code_length = defaultdict(int)
    for (sym, binary) in code.items():
        code_length[sym] = len(binary)
    encoded_length = 0
    for sym in message:
        encoded_length += code_length[sym]
    encoded_binary = BitArray(encoded_length)

    # step 4: encode the message
    position = 0
    for sym in message:
        for bit in code[sym]:
            if bit == '1':
                encoded_binary.set_bit(position)
            position += 1

    return(code, encoded_binary)


def decode(message, code):
    """ """
    assert isinstance(message, BitArray)
    message = message.to_string()
    T = Trie()
    for (symbol, binary) in code.items():
        T.add_code(symbol, binary)
    decoded_message = T.decode(message)
    return(decoded_message)
