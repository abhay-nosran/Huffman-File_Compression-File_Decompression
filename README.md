# Huffman-File_Compression-File_Decompression

This project is a **File Compression and Decompression Tool** that implements the **Huffman Coding Technique** to efficiently compress and decompress files.

## Features

### File Compression:
- Accepts a `.txt` file as input.
- Outputs a compressed `.bin` file containing the encoded data.

### File Decompression:
- Accepts a `.bin` file as input.
- Outputs the original `.txt` file after decompression.

## How It Works

### Compression:
The tool uses **Huffman coding** to encode the input text file into a highly compact binary representation, significantly reducing file size.

### Decompression:
The tool decodes the compressed binary file back into the original text format.

## Use Cases:
- Reducing file sizes for storage or transmission.
- Efficiently encoding and decoding text data.

## Technology Stack:

- **Backend**: Built with **Node.js**.
- **Frontend**: Responsive user interface using **HTML**, **CSS**, and **Bootstrap**.
- **External Tool**: A **C++ executable** (`compressor.exe` and `decompressor.exe`) to perform the actual Huffman coding operations.

## How to Use:
1. **Compression**: Upload a `.txt` file to compress it into a `.bin` file.
2. **Decompression**: Upload a `.bin` file to decompress it back into a `.txt` file.

## Developed by:
**Abhay Nosran**

This tool demonstrates the practical application of data compression algorithms in real-world scenarios.
