"""array of bits, represented as int in C++"""


from collections import Sequence
from ctypes import cdll


bit_array = cdll.LoadLibrary('lib/lib_bit_array.so')


class BitArray(Sequence):
    """an array of bits, wrapped C"""
    def __init__(self, length):
        assert length >= 0
        self.length = length
        self.obj = bit_array._new(length)

    def __len__(self):
        return self.length

    def __getitem__(self, position):
        assert(0 <= position < self.length), "outside array"
        return(bit_array._get_bit(self.obj, position))

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

    def __repr__(self):
        return self.__str__()

    def __del__(self):
        bit_array._free(self.obj)

    def get_bits(self):
        """get the contiguous array of bits, in string form"""
        result_string = []
        for i in range(self.length):
            result_string.append(str(self.__getitem__(i)))
        return "".join(result_string)

    def set_bit(self, position):
        assert 0 <= position < self.length, "outside array"
        bit_array._set_bit(self.obj, position)

    def clear_bit(self, position):
        assert 0 <= position < self.length, "outside array"
        bit_array._clear_bit(self.obj, position)

__all__ = ["BitArray"]
