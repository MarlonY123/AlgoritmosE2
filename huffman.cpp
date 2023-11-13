#include <iostream>
#include <queue>
#include <map>

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

void printHuffmanCodes(HuffmanNode* root, string code = "") {
    if (root == nullptr) {
        return;
    }

    if (root->data != '$') {
        cout << root->data << ": " << code << endl;
    }

    printHuffmanCodes(root->left, code + "0");
    printHuffmanCodes(root->right, code + "1");
}

int main() {
    // Example map<char, int>
    map<char, int> charFrequency = {
        {'a', 5},
        {'b', 9},
        {'c', 12},
        {'d', 13},
        {'e', 16},
        {'f', 5},
        {'g', 15},
        {'h', 1},
        {'i', 4},
        {'j', 35},
        {'k', 11},
        {'l', 22}
    };

    // Build the Huffman tree
    HuffmanNode* root = buildHuffmanTree(charFrequency);

    // Print Huffman codes
    cout << "Huffman Codes:" << endl;
    printHuffmanCodes(root);

    return 0;
}
