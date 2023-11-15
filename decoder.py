import os
import subprocess

def saveFile(filename, toSave):
    f = open(filename, "w")
    f.write(toSave)
    f.close()

with open("Resultado/7-Huffman.bin", "rb") as file:
    data = file.read()

# Convert binary data to a string and replace \x00 with 0 and \x01 with 1
datastring = ''.join(['0' if b == 0x00 else '1' if b == 0x01 else f'\\x{b:02x}' for b in data])
filename = '8-Huffman.txt'

insert_string_to_file(filename, datastring)

