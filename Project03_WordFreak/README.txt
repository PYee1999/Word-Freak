CS 230 Project #3 WordFreak
Preston Yee

This program will take in single or multiple text files, and print the list of words (and their frequencies) in each of the files, formatted with spaces and a colon (:) in between, on an output text file. The program accepts the three types of input (one of them, two of them, or all three):

cat text.txt | ./wordfreak
./wordfreak text.txt
WORD_FREAK=text.txt ./wordfreak

The program will then open each file, and parse out every single word in the file, excluding punctuations, numbers or any characters that are non-alphabetical. For each and every word, the program will use the function "get_hashTable_pointer(char word[])" to take the first letter of every word, and determine which alphabetical BST tree the word should be in the bst array (hash table); for example, the word "apple" will be placed in the hash table 0 because the first element in the bst array is the bst for all words that start with the letter "a". 

Once the correct alphabetical tree has been found, the program will traverse that bst, and see if it needs to either add to the tree (if the word doesn't exist in the tree), or increment the count on the word (if the word already exists in the tree), using the "add_word_to_bst(char* word, struct BSTNode* head)" function.

Finally, after all of the words has been added to their respective binary search trees (bst), the program will close the files, and then print the list of all words and their respective counts in the output file (called output.txt). The functions "get_longest_word_length(struct BSTNode* node)" and "get_longest_word_count_length(struct BSTNode* node)" will find the length of the longest word and the length of the longest word count digit possible, respectively, which will be used to properly format the words with their counts.

Video Link: https://youtu.be/M31mOoMyRs4