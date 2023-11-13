import time

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
#BURROWS WHEELER CODING AND DECODING
def bwtFunction(s, sa, bwt, secciones, occ):
    for i in range(len(s)):
        if i == 0 or s[sa[i-1]] != s[sa[i]]:
            secciones[s[sa[i]]] = i
        bwt[i] = s[sa[i]-1]
        for j in keys:
            if bwt[i] == j:
                occ[j].append(occ[j][i] + 1)
            else:
                occ[j].append(occ[j][i])

def bwtInverseFunction(bwt, secciones, occurs):
    texto = ""
    n = 0
    letra = bwt[n]
    texto += letra
    while letra != '$':
        n = secciones[letra] + occurs[letra][n]
        letra = bwt[n]
        texto = letra + texto
    return texto
#MOVE TO FRONT CODING AND DECODING
def moveToFront(index,alph):
    word=alph[index]
    alph2=alph
    alph[1:index+1]=alph2[:index]
    alph[0]=word
def moveToFrontCoding(alphabet,T):
    alph2=alphabet[:]
    for i in range (len(T)):
        for j in range(len(alph2)):
            if T[i] == alph2[j]:
                mtf.append(j)
                moveToFront(j,alph2)
                break

def moveToFrontDecoding(alph,mtf):
    for i in range(len(mtf)):
        mtfd.append(alph[mtf[i]])
        moveToFront(mtf[i],alph)

    
                
    

if __name__ == '__main__':

    #Declaramos el alfabeto a utilizar
    alph = openFile("abecedario.txt")
    alph=list(alph)
    #Declaramos la palabra a utilizar
    s= "banana$"
    #Se crea el suffix array de la palabra
    T = [ord(c) for c in s]
    sa = sais(T)
    #Se inicia el proceso BURROWS WHEELER
    bwt = ['-'] * len(s)
    abc = set('banana$')
    keys = sorted(abc)
    #Definicion del diccionario para las secciones del first
    secciones = dict.fromkeys(keys,0)
    #Definicion del diccionario para las ocurrencias del last
    occ = {}
    for key in keys:
        occ[key] = [0]
    #Se aplica el BURROWS WHEELER
    startbw=time.time()
    bwtFunction(s, sa, bwt, secciones, occ)
    endbw=time.time()
    print("Ejecución de Burrows Wheeler:",(endbw-startbw),"s")
    #Se aplica MOVE TO FRONT
    #mtf coded list
    mtf=[]
    startmtf=time.time()
    moveToFrontCoding(alph,bwt)
    endmtf=time.time()
    print("Ejecución de Move To Front:",(endmtf-startmtf),"s")
