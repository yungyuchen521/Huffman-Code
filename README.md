# Huffman-Code

## Compression
### 1. Read the given file and count the occurrence of each unique characters.

### 2. Build the Huffman Tree with C++ built in priority queue.

### 3. Export the header of to the compressed file.
The header is composed of:
1. The type of the original file
2. The length (in terms of bits) of the compressed content.
3. Each uniqe characters and their occurence in the original file.

### 4. Export the compressed content to the compressed file.
Translate the characters in the original file into Huffman Code.
Output the code eight by eight. ( each characters (0/1) of the code represents a bit, eight of them combined become a byte )

---

## Decompression
### 1. Read the header of the compressed file.

### 2. With the info of the header, rebuild the Huffman Tree.

### 3. With the tree, read the compressed content and translate it into the original content.
