#include <bits/stdc++.h>
using namespace std;

struct hnode {
    char ch;
    int f;
    hnode *lc, *rc;
    hnode(char c, int freq, hnode* left = nullptr, hnode* right = nullptr)
        : ch(c), f(freq), lc(left), rc(right) {}
};

struct comparator {
    bool operator()(hnode* l, hnode* r) {
        return l->f > r->f;
    }
};

void encode(hnode* root, unordered_map<char, string>& huffmanCode, string str) {
    if (!root) return;

    if (!root->lc && !root->rc) {
        huffmanCode[root->ch] = str;
    }

    encode(root->lc, huffmanCode, str + "0");
    encode(root->rc, huffmanCode, str + "1");
}

void decode(hnode* root, const string& str, int& index, ofstream& decodedFile) {
    hnode* current = root;
    while (index < str.size()) {
        if (!current->lc && !current->rc) {
            decodedFile << current->ch;
            return;
        }
        current = (str[index++] == '0') ? current->lc : current->rc;
    }
}

void freeTree(hnode* root) {
    if (!root) return;
    freeTree(root->lc);
    freeTree(root->rc);
    delete root;
}

void createTree(const string& text) {
    unordered_map<char, int> freq;
    for (char ch : text) {
        freq[ch]++;
    }

    priority_queue<hnode*, vector<hnode*>, comparator> pq;
    for (const auto& p : freq) {
        pq.push(new hnode(p.first, p.second));
    }

    while (pq.size() != 1) {
        hnode* left = pq.top(); pq.pop();
        hnode* right = pq.top(); pq.pop();
        pq.push(new hnode('\0', left->f + right->f, left, right));
    }

    hnode* root = pq.top();
    unordered_map<char, string> huffmanCode;
    encode(root, huffmanCode, "");

    string encodedStr;
    for (char ch : text) {
        encodedStr += huffmanCode[ch];
    }

    ofstream efile("encoded.txt");
    ofstream encodedFile("encoded.bin", ios::binary);
    if (!encodedFile) {
        cerr << "Error opening encoded.bin for writing." << endl;
        freeTree(root);
        return;
    }

    bitset<8> bits;
    for (size_t i = 0; i < encodedStr.size(); i += 8) {
        string byte = encodedStr.substr(i, 8);
        bits = bitset<8>(byte);
        char character = static_cast<char>(bits.to_ulong());
        efile << character;
        encodedFile.put(static_cast<unsigned char>(bits.to_ulong()));
    }
    efile.close();
    encodedFile.close();

    ifstream encodedBinaryFile("encoded.bin", ios::binary);
    if (!encodedBinaryFile) {
        cerr << "Error opening encoded.bin for reading." << endl;
        freeTree(root);
        return;
    }

    string binaryStr;
    unsigned char c;
    while (encodedBinaryFile.read(reinterpret_cast<char*>(&c), 1)) {
        bits = c;
        binaryStr += bits.to_string();
    }
    encodedBinaryFile.close();

    ofstream decodedFile("decoded.txt");
    if (!decodedFile) {
        cerr << "Error opening decoded.txt for writing." << endl;
        freeTree(root);
        return;
    }

    int index = 0;
    while (index < binaryStr.size()) {
        decode(root, binaryStr, index, decodedFile);
    }
    decodedFile.close();

    ofstream decodedFile2("decoded_2.txt");
    index = 0;
    while (index < encodedStr.size()) {
        decode(root, encodedStr, index, decodedFile2);
    }
    decodedFile2.close();

    freeTree(root);
}

int main() {
    ifstream infile("input.txt");
    if (!infile) {
        cerr << "Error opening input text file" << endl;
        return 1;
    }

    stringstream buffer;
    buffer << infile.rdbuf();
    string text = buffer.str();
    infile.close();

    cout << "Started:\n";
    createTree(text);

    cout << "Actual size (in bits): " << text.size() * 8 << endl;

    ifstream encodedFile("encoded.bin", ios::binary);
    if (!encodedFile) {
        cerr << "Error opening encoded.bin for reading." << endl;
        return 1;
    }
    encodedFile.seekg(0, ios::end);
    long long encodedSize = encodedFile.tellg();
    encodedFile.close();

    cout << "Encoded size (in bits): " << encodedSize * 8 << endl;

    return 0;
}
