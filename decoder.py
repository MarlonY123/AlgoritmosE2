import os
import subprocess

def saveFile(filename, toSave):
    f = open(filename, "w")
    f.write(toSave)
    f.close()

with open("output.bin", "rb") as file:
    data = file.read()

# Convert binary data to a string and replace \x00 with 0 and \x01 with 1
datastring = ''.join(['0' if b == 0x00 else '1' if b == 0x01 else f'\\x{b:02x}' for b in data])
filename = 'output.txt'

saveFile(filename, datastring)

def HuffmanCpp(nombre_programa_cpp, nombre_ejecutable):
    # Compilar el programa C++
    comando_compilacion = f"g++ {nombre_programa_cpp} -o {nombre_ejecutable}"
    proceso_compilacion = subprocess.run(comando_compilacion, shell=True, check=True)

    # Ejecutar el programa compilado
    comando_ejecucion = f"{nombre_ejecutable}.exe"  # Cambia a nombre_ejecutable.exe en Windows
    proceso_ejecucion = subprocess.run(comando_ejecucion, shell=True, check=True)

nombre_programa_cpp = "decode.cpp"
nombre_ejecutable = "a"
HuffmanCpp(nombre_programa_cpp, nombre_ejecutable)

