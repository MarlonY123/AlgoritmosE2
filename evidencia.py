import time
import subprocess
import bitarray
import numpy as np
from memory_profiler import memory_usage
import pylab as pl
from bitarray import _bitarray_reconstructor
#SA-IS
def getBuckets(T):
    count = {}
    buckets = {}
    for c in T:
        count[c] = count.get(c, 0) + 1
    start = 0
    for c in sorted(count.keys()):
        buckets[c] = (start, start + count[c])
        start += count[c]
    return buckets
def sais(T):
    t = ["_"] * len(T)
    
    t[len(T) - 1] = "S"
    for i in range(len(T) - 1, 0, -1):
        if T[i-1] == T[i]:
            t[i - 1] = t[i]
        else:
            t[i - 1] = "S" if T[i-1] < T[i] else "L"
    
    buckets = getBuckets(T)

    count = {}
    SA = [-1] * len(T)
    LMS = {}
    end = None
    for i in range(len(T) - 1, 0, -1):
        if t[i] == "S" and t[i - 1] == "L":
            revoffset = count[T[i]] = count.get(T[i], 0) + 1
            SA[buckets[T[i]][1] - revoffset] = i
            if end is not None:
                LMS[i] = end
            end = i
    
    LMS[len(T) - 1] = len(T) - 1
    count = {}
    for i in range(len(T)):
        if SA[i] >= 0:
            if t[SA[i] - 1] == "L":
                symbol = T[SA[i] - 1]
                offset = count.get(symbol, 0)
                SA[buckets[symbol][0] + offset] = SA[i] - 1
                count[symbol] = offset + 1

    count = {}
    for i in range(len(T) - 1, 0, -1):
        if SA[i] > 0:
            if t[SA[i] - 1] == "S":
                symbol = T[SA[i] - 1]
                revoffset = count[symbol] = count.get(symbol, 0) + 1
                SA[buckets[symbol][1] - revoffset] = SA[i] - 1

    namesp = [-1] * len(T)
    name = 0
    prev = None
    for i in range(len(SA)):
        if t[SA[i]] == "S" and t[SA[i] - 1] == "L":
            if prev is not None and T[SA[prev]:LMS[SA[prev]]] != T[SA[i]:LMS[SA[i]]]:
                name += 1
            prev = i
            namesp[SA[i]] = name

    names = []
    SApIdx = []
    for i in range(len(T)):
        if namesp[i] != -1:
            names.append(namesp[i])
            SApIdx.append(i)

    if name < len(names) - 1:
        names = sais(names)

    names = list(reversed(names))

    SA = [-1] * len(T)
    count = {}
    for i in range(len(names)):
        pos = SApIdx[names[i]]
        revoffset = count[T[pos]] = count.get(T[pos], 0) + 1
        SA[buckets[T[pos]][1] - revoffset] = pos

    count = {}
    for i in range(len(T)):
        if SA[i] >= 0:
            if t[SA[i] - 1] == "L":
                symbol = T[SA[i] - 1]
                offset = count.get(symbol, 0)
                SA[buckets[symbol][0] + offset] = SA[i] - 1
                count[symbol] = offset + 1

    count = {}
    for i in range(len(T) - 1, 0, -1):
        if SA[i] > 0:
            if t[SA[i] - 1] == "S":
                symbol = T[SA[i] - 1]
                revoffset = count[symbol] = count.get(symbol, 0) + 1
                SA[buckets[symbol][1] - revoffset] = SA[i] - 1

    return SA
#Files
def openFile(filename):
    f=open(filename,encoding="utf8")
    text=f.read()
    f.close()
    return text

def saveFile(filename, toSave):
    f = open(filename, "w")
    f.write(toSave)
    f.close()


#BURROWS WHEELER CODING AND DECODING
def bwtFunction(s, sa, bwt, secciones, occ):
    for i in range(len(s)):
        #print(i)
        #print(secciones[s[sa[i]]])
        #print(s[sa[i]])
        if i == 0 or s[sa[i-1]] != s[sa[i]]:
            secciones[s[sa[i]]] = i
        bwt[i] = s[sa[i]-1]
        
        for j in secciones.keys():
            if bwt[i] == j:
                occ[j].append(occ[j][i] + 1)
            else:
                occ[j].append(occ[j][i])
        

def bwtInverseFunction(bwt, secciones, occurs):
    texto = ""
    n = 0
    letra = bwt[n]
    texto += letra
    while letra != '\0':
        n = secciones[letra] + occurs[letra][n]
        letra = bwt[n]
        texto = letra + texto
    return texto
#MOVE TO FRONT CODING AND DECODING
def match(char,alph):
    for j in range (len(alph)):
        if alph[j]==char:
            return j

def moveToFront(index,alph):
    word=alph[index]
    alph2=alph[:]
    alph[1:index+1]=alph2[:index]
    alph[0]=word

def moveToFrontCoding(alphabet,T):
    alph2=alphabet[:]
    mtf=[]
    for i in range (len(T)):
        j=match(T[i],alph2)
        mtf.append(j)
        moveToFront(j,alph2)
    return mtf
            

def moveToFrontDecoding(alph,mtf):
    mtfd=[]
    for i in range(len(mtf)):
        mtfd.append(alph[mtf[i]])
        moveToFront(mtf[i],alph)
    return mtfd

#RUN LENGTH ENCODING AND DECODING
def runLengthEncoding(moveToFront):
    encoded = []
    zeroCounter = 0
    for i in moveToFront:
        if i == 0:
            zeroCounter += 1
            if zeroCounter == 255:
                encoded.append(0)
                encoded.append(255)
                zeroCounter = 0
        else:
            if zeroCounter > 0:
                encoded.append(0)
                encoded.append(zeroCounter)
                zeroCounter = 0
            encoded.append(i)
    if zeroCounter > 0:
        encoded.append(0)
        encoded.append(zeroCounter)
    return encoded

def runLengthDecoding(runLength):
    decode = []
    for i in range(len(runLength)):
        if i == 0:
            decode.append(runLength[i])
        elif runLength[i] == 0:
            decode.extend([0]*runLength[i+1])
        elif runLength[i-1] != 0:
            decode.append(runLength[i])
    return decode

#COMPILE AND EXECUTE C++ HUFFMAN SCRIPT
def HuffmanCpp(nombre_programa_cpp):
    # Compilar el programa C++
    comando_compilacion = f"g++ {nombre_programa_cpp}"
    proceso_compilacion = subprocess.run(comando_compilacion, shell=True, check=True)

    # Ejecutar el programa compilado
    proceso_ejecucion = subprocess.run("./a.out")

def insert_string_to_file(filename, input_string):
    try:
        with open(filename, 'a') as file:
            file.write(input_string + '\n')  # Appends the string to the file with a newline character
        print(f'String inserted into {filename} successfully.')
    except Exception as e:
        print(f'Error: {e}')



if __name__ == '__main__':
    
    #Declaramos el libro a utilizar
    s = openFile("Libros/Dracula.txt")
    s+='\0'
    #Se crea el suffix array de la palabra
    T = []
    for c in s:
        T.append(ord(c))
    alph=set(s)
    alph=sorted(alph)
    alph = list(alph)
    sa = sais(T)
    #Se inicia el proceso BURROWS WHEELER
    bwt = ['-'] * len(s)
    #Definicion del diccionario para las secciones del first
    secciones = dict.fromkeys(alph,0)
    #Definicion del diccionario para las ocurrencias del last
    occ = {}
    for key in alph:
        occ[key] = [0]
    #Se aplica el BURROWS WHEELER
    startbw=time.time()
    bwtFunction(s, sa, bwt, secciones, occ)

    endbw=time.time()
    print("BURROWS WHEELER")
    print("Tiempo de Ejecución:",(endbw-startbw),"s")
    #memory profiling
    memoryUsage=memory_usage((bwtFunction,(s, sa, bwt, secciones, occ, )),interval=0.01)
    pl.plot(np.arange(len(memoryUsage)) *0.1, memoryUsage, label='SA-IS Algorithm')
    pl.title('Consumo de Memoria (BWT)')
    pl.xlabel('Tiempo (s)')
    pl.ylabel('Uso de memoria (kB)')
    pl.show()
    
    # Se aplica MOVE TO FRONT
    # mtf coded list
    
    startmtf=time.time()
    mtf=moveToFrontCoding(alph,bwt)
    endmtf=time.time()
    print("MOVE TO FRONT")
    print("Tiempo de Ejecución:",(endmtf-startmtf),"s")
    #memory profiling
    memoryUsage=memory_usage((moveToFrontCoding,(alph,bwt, )),interval=0.01)
    pl.plot(np.arange(len(memoryUsage)) *0.1, memoryUsage, label='SA-IS Algorithm')
    pl.title('Consumo de Memoria (MTF)')
    pl.xlabel('Tiempo (s)')
    pl.ylabel('Uso de memoria (kB)')
    pl.show()
    
    # RUN-LENGTH 
    startrl=time.time()
    rLE = runLengthEncoding(mtf)
    endrl=time.time()
    print("RUN-LENGTH")
    print("Tiempo de Ejecución:",(endrl-startrl),"s")
    memoryUsage=memory_usage((runLengthEncoding,(mtf, )),interval=0.01)
    pl.plot(np.arange(len(memoryUsage)) *0.1, memoryUsage, label='SA-IS Algorithm')
    pl.title('Consumo de Memoria (RLT)')
    pl.xlabel('Tiempo (s)')
    pl.ylabel('Uso de memoria (kB)')
    pl.show()
    
    #Ejecucion de Huffman
    nombre_programa_cpp = "encoder.cpp"

    HuffmanCpp(nombre_programa_cpp)

    saveFile("Resultado/1-SuffixArray.txt",''.join(map(str,sa)))
    saveFile("Resultado/2-abecedario.txt",''.join(map(str,alph)))
    saveFile("Resultado/3-BurrowsWheelerTransform.txt",''.join(map(str,bwt)))
    saveFile("Resultado/4-MoveToFront.txt",''.join(map(str,mtf)))
    saveFile("Resultado/5-RunLength.txt",', '.join(map(str,rLE)))
    saveFile("Resultado/9-TextCompressed.txt",''.join(map(str,rLE)))
    #INICIO DE DECODIFICACION
    HFMD=[]
    with open("Resultado/7-Huffman.bin", "rb") as file:
        data = file.read()
    datastring = ''.join(['0' if b == 0x00 else '1' if b == 0x01 else f'\\x{b:02x}' for b in data])
    filename = 'Resultado/8-Huffman.txt'
    insert_string_to_file(filename, datastring)
    HuffmanCpp("decode.cpp")
    rLE=runLengthDecoding(rLE)
    rLE=moveToFrontDecoding(alph,rLE)
    rLE=bwtInverseFunction(rLE,secciones,occ)
    saveFile("Resultado/10-TextDecompressed.txt",''.join(map(str,rLE)))
    
