"""custom trie for decoding an huffman encoded string """

from ctypes import c_char_p, cdll


huffman_trie = cdll.LoadLibrary('lib/lib_huffman_trie.so')
huffman_trie._decode.restype = c_char_p


class HuffmanTrie(object):
    """HuffmanTrie structure to decode huffman encoded string"""
    def __init__(self):
        self.obj = huffman_trie._new()

    def add_code(self, symbol, code):
        code, symbol = str.encode(code), ord(symbol)
        huffman_trie._add_code(self.obj, code, symbol)

    def decode(self, message):
        return(huffman_trie._decode(self.obj, str.encode(message)))

    def __del__(self):
        huffman_trie._delete(self.obj)
        print("collected")

__all__ = ["HuffmanTrie"]
