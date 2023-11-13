#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
#include <queue>

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

HuffmanNode* buildHuffmanTree(const map<char, int>& charFrequency) {
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

void generateHuffmanCodes(HuffmanNode* root, const string& code, map<char, string>& huffmanCodes) {
    if (root == nullptr) {
        return;
    }

    if (root->data != '$') {
        huffmanCodes[root->data] = code;
    }

    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}

// Assuming you have the previously generated Huffman codes in a map<char, string> huffmanCodes

// Function to encode a text file using Huffman codes and write the result to a binary file
void encodeTextFile(const string& inputFileName, const string& outputFileName, const map<char, string>& huffmanCodes) {
    ifstream inputFile(inputFileName, ios::in);

    if (!inputFile) {
        cerr << "Error: Unable to open input file: " << inputFileName << endl;
        return;
    }

    ofstream outputFile(outputFileName, ios::out | ios::binary);

    if (!outputFile) {
        cerr << "Error: Unable to open output file: " << outputFileName << endl;
        return;
    }

    // Write the number of characters in the original text to the output file
    int numCharacters = 0;
    while (inputFile.get() != EOF) {
        numCharacters++;
    }

    inputFile.clear(); // Clear the end-of-file flag
    inputFile.seekg(0, ios::beg); // Move the file pointer back to the beginning

    outputFile.write(reinterpret_cast<const char*>(&numCharacters), sizeof(numCharacters));

    // Encode the text using Huffman codes and write the result to the output file
    char buffer = 0; // Buffer to store bits before writing to the file
    int bitCount = 0; // Count of bits in the buffer

    char ch;
    while (inputFile.get(ch)) {
        string code = huffmanCodes.at(ch);

        for (char bit : code) {
            if (bit == '1') {
                buffer |= (1 << (7 - bitCount));
            }

            bitCount++;

            if (bitCount == 8) {
                outputFile.put(buffer);
                buffer = 0;
                bitCount = 0;
            }
        }
    }

    // Write the last bits in the buffer
    if (bitCount > 0) {
        outputFile.put(buffer);
    }

    // Close the files
    inputFile.close();
    outputFile.close();

    cout << "File encoded successfully!" << endl;
}

int main() {

        // Example map<char, int>
    map<char, int> charFrequency = {
        {'m', 1},
        {'i', 4},
        {'s', 4},
        {'p', 2}
    };

    // Build the Huffman tree
    HuffmanNode* root = buildHuffmanTree(charFrequency);

    // Generate Huffman codes
    map<char, string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    // Specify input and output file names
    string inputFileName = "input.txt";
    string outputFileName = "output.bin";

    // Encode the text file using Huffman codes and write to a binary file
    encodeTextFile(inputFileName, outputFileName, huffmanCodes);

    return 0;
}