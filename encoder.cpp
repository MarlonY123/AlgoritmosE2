#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
#include <queue>
#include <vector>
#include <sstream>

using namespace std;

struct HuffmanNode {
    char data;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char c, int freq) : data(c), frequency(freq), left(nullptr), right(nullptr) {}
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

        HuffmanNode* newNode = new HuffmanNode('$', left->frequency + right->frequency);
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

    if (root->data != '$') {
        huffmanCodes[root->data] = code;
    }

    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}


void encodeTextFile(const string& inputFileName, const string& outputFileName, const map<int, string>& huffmanCodes) {
    ifstream inputFile(inputFileName, ios::in);

    ofstream outputFile(outputFileName, ios::out | ios::binary);

    // Encode the text using Huffman codes and write the result to the output file
    int value;
    while (inputFile >> value) {
        string code = huffmanCodes.at(value);

        for (char bit : code) {
            if (bit == '1') {
                outputFile.put(1);
            } else {
                outputFile.put(0);
            }
        }
    }

    // Close the files
    inputFile.close();
    outputFile.close();

    cout << "File encoded successfully!" << endl;
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

int main() {

    vector<int> frequencies = readFile("runLengthEncoding.txt");
        for (int i: frequencies) cout << i << ' ';

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

    // Specify input and output file names
    string inputFileName = "runLengthEncoding.txt";
    string outputFileName = "output.bin";

    cout << "Huffman Codes:" << endl;
    for (const auto& pair : huffmanCodes) {
        cout << pair.first << ": " << pair.second << endl;
    }

    encodeTextFile(inputFileName, outputFileName, huffmanCodes);

    return 0;
}