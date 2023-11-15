#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
#include <queue>
#include <vector>
#include <sstream>

using namespace std;

struct HuffmanNode {
    int data;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(int c, int freq) : data(c), frequency(freq), left(nullptr), right(nullptr) {}
};

struct CompareNodes {
    bool operator()(HuffmanNode* left, HuffmanNode* right) {
        return left->frequency > right->frequency;
    }
};

HuffmanNode* buildHuffmanTree(const map<int, int>& charFrequency) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> minHeap;

    // Create a leaf node for each character and add it to the min heap
    for (const auto& pair : charFrequency) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    // Build the Huffman tree
    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top();
        minHeap.pop();
        HuffmanNode* right = minHeap.top();
        minHeap.pop();

        HuffmanNode* newNode = new HuffmanNode('ā', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;

        minHeap.push(newNode);
    }

    return minHeap.top();
}

void generateHuffmanCodes(HuffmanNode* root, const string& code, map<int, string>& huffmanCodes) {
    if (root == nullptr) {
        return;
    }

    if (root->data != 'ā') {
        huffmanCodes[root->data] = code;
    }

    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}


map<int, int> countFrequencies(const vector<int>& encodedVector) {
    map<int, int> frequencyMap;
    
    for (size_t i = 0; i < encodedVector.size(); ++i) {
        if (encodedVector[i] == 0) {
            // The number following 0 represents the count of consecutive 0s
            if (i + 1 < encodedVector.size()) {
                int count = encodedVector[i + 1];
                frequencyMap[0] += count;
                i++; // Skip the count value
            } else {
                // Handle the case where the last element is 0 without a count
                frequencyMap[0]++;
            }
        } else {
            frequencyMap[encodedVector[i]]++;
        }
    }

    return frequencyMap;
}

vector<int> readFile(const string& fileName) {
    vector<int> values;
    ifstream inputFile(fileName);
    string line;
    while (getline(inputFile, line, ',')) {
        int value;
        istringstream iss(line);
        
        if (!(iss >> value)) {
            cerr << "Error: Invalid value in file" << endl;
            return values;
        }
        values.push_back(value);
    }
    return values;
}

void encodeToBinaryFile(const map<int, string>& huffmanCodes, const vector<int>& inputVector, const string& outputFile) {
    ofstream binaryFile(outputFile, ios::binary);

    for (size_t i = 0; i < inputVector.size(); ++i) {
        int value = inputVector[i];
        const string& code = huffmanCodes.at(value);
        //cout << value << " = " << code << endl;
        // If the value is 0, move to the next position and encode zeros
        if (value == 0) {
            // Move to the next position
            ++i;
            // Encode the specified number of zeros
            for (int j = 0; j < inputVector[i]; ++j) {
                for (char bit : code) {
                binaryFile.put(bit == '0' ? 0 : 1);
                //cout << "# of 0" << " = " << j << endl;
                }
            }
        } else {
            // Encode the Huffman code for the current value
            for (char bit : code) {
                binaryFile.put(bit == '0' ? 0 : 1);
            }
        }
    }

    binaryFile.close();
}

void mapToTxt(const std::map<int, std::string>& myMap, const std::string& fileName) {
    std::ofstream textFile(fileName);
    for (const auto& entry : myMap) {
        textFile << entry.first << ": " << entry.second << std::endl;
    }
    textFile.close();
}

int main() {

    vector<int> frequencies = readFile("Resultado/5-RunLength.txt");
        //for (int i: frequencies) cout << i << ' ';

    map<int, int> charFrequency = countFrequencies(frequencies);
    
    cout << "Frequencies:" << endl;
    for (const auto& pair : charFrequency) {
        cout << pair.first << ": " << pair.second << endl;
    }


    // Build the Huffman tree
    HuffmanNode* root = buildHuffmanTree(charFrequency);

    // Generate Huffman codes
    map<int, string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    mapToTxt(huffmanCodes, "huffman.txt");

    cout << "Huffman Codes:" << endl;
    for (const auto& pair : huffmanCodes) {
        cout << pair.first << ": " << pair.second << endl;
    }

    cout << "Bytes of int vector: " << sizeof(vector<int>) + (sizeof(int) * frequencies.size()) << endl;

    encodeToBinaryFile(huffmanCodes, frequencies, "output.bin");
    return 0;
}