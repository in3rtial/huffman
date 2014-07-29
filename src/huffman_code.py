"""a method for the construction of minimum-redundancy codes. Huffman"""

from bit_array import BitArray
from huffman_trie import HuffmanTrie

from collections import defaultdict, Counter
from heapq import heappush, heappop, heapify


class HuffmanCode(object):
    """allows object encoding and decoding"""
    def __init__(self, message):
        self.code = self.generate_code(self.to_frequency(Counter(message)))
        self.sym_length = defaultdict(int)
        for (sym, bin_code) in self.code.items():
            self.sym_length[sym] = len(bin_code)

    def to_frequency(self, symbol_count):
        """get the frequency of each symbol in the message"""
        frequencies = []
        total = sum(symbol_count.values())
        for (symbol, count) in symbol_count.items():
            frequencies.append((count / total, [symbol]))
        return frequencies

    def generate_code(self, frequency_table):
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
        sym_to_string = defaultdict(str)
        for (symbol, binary) in code.items():
            bin_str = "".join([str(i) for i in reversed(binary)])
            sym_to_string[symbol] = bin_str
        return sym_to_string

    def encode(self, message):
        """huffman code the message into bits"""
        bit_size = 0
        for sym in message:
            bit_size += self.sym_length[sym]

        bit_array = BitArray(bit_size)

        position = 0
        for sym in message:
            for bit in self.code[sym]:
                if bit == '1':
                    bit_array.set_bit(position)
                position += 1

        return bit_array

    def decode(self, bit_array):
        """decode the message previously encoded with the code"""
        assert isinstance(bit_array, BitArray)
        decode_trie = HuffmanTrie()
        for (symbol, code) in self.code.items():
            decode_trie.add_code(symbol, code)

        return decode_trie.decode(bit_array.get_bits())


__all__ = ["HuffmanCode"]
