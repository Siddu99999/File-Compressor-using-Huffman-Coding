# File-Compressor-using-Huffman-Coding
This project implements lossless compression of files using Huffman encoding in C++.The core idea is to calculate frequencies of input characters and assign variable-length codes to them, with shorter codes assigned to more frequent characters. This method effectively reduces the overall size of the data.

| File| Description | 
| -------- | -------- |
| FileCompressor.cpp | The main program file containing all functions and the main entry point |
| input.txt | The input file containing the text to be encoded |
| encoded.txt | A text file where encoded characters are written |
| encoded.bin | A binary file containing the encoded data |
| decoded.txt | A text file where the decoded text is written from the binary file |
| decoded_2.txt | A text file where the decoded text is written directly from the encoded string for verification | 

### Example Usage
To compile use `g++ -o huffman FileCompressor.cpp`.

To Run use `./Huffman`.

