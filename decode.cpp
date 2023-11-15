#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
#include <queue>
#include <vector>
#include <sstream>
#include <conio.h>

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

vector<int> decode_file(struct HuffmanNode* root, string s)
{
    vector<int> ans;
    struct HuffmanNode* curr = root;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '0')
            curr = curr->left;
        else
            curr = curr->right;
 
        // reached leaf node
        if (curr->left == NULL and curr->right == NULL) {
            //cout << curr->data << endl;
            ans.push_back(curr->data);
            curr = root;
        }
    }
    // cout<<ans<<endl;
    return ans;
}

void vectortxt(const vector<int>& myVector, const string& fileName) {
    ofstream textFile(fileName);

    // Write each element of the vector to the text file
    for (const int& value : myVector) {
        textFile << value << ",";
    }
    textFile.close();
}

int main() {

    vector<int> frequencies = readFile("Resultado/5-RunLength.txt");

    map<int, int> charFrequency = countFrequencies(frequencies);

    // Build the Huffman tree
    HuffmanNode* root = buildHuffmanTree(charFrequency);

    // Generate Huffman codes
    map<int, string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    vector<int> decodedData;

    ifstream codedfile;
    codedfile.open("Resultado/8-Huffman.txt");
    stringstream buffer; 
    buffer << codedfile.rdbuf(); 
    string codedstring;
    codedstring = buffer.str();


    decodedData = decode_file(root, codedstring);

    vectortxt(decodedData, "Resultado/decodedHuff.txt");

    return 0;
}