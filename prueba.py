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

file=openFile("huffman.txt")
fb=file.encode('utf-8')
with open("test.bin","wb") as f:
    f.write(fb)