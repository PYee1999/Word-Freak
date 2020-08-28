#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

// Debug variables for debugging function print_bst()
//#define DEBUG     // DEBUG #1: Use this to print out the step by step process of printing list onto output file
//#define DEBUG2    // DEBUG #2: Use this to print out list onto terminal to verify that the BST is correct

int fdout, w;          // Create File Descriptor to write words and their counts onto output.txt, and count w
char bufout[1024];     // Buffer for output

struct BSTNode  // BST Stucture (Contains left child node, right child node, word, and count)
{
    struct BSTNode* left;   // Get left child of BST node
    char* word;             // Get word
    int count;              // Get frequency of word (word count)
    struct BSTNode* right;  // Get right child of BST node
};

struct BSTNode* bstNode(struct BSTNode* left, char* word, int count, struct BSTNode* right) // Create BSTNode
{
    struct BSTNode* bstNode;                                    // Create BSTNode
    bstNode = (struct BSTNode*) malloc(sizeof(struct BSTNode)); // Allocate size
    
    bstNode->left = left;       // Set left child node
    bstNode->word = word;       // Set word
    bstNode->count = count;     // Set count (word frequency)
    bstNode->right = right;     // Set right child node
    
    return bstNode;
}

struct BSTNode* hashTable[26];  // List of 26 BSTNode pointers (Hashtable); for letters A-Z

struct BSTNode* get_left(struct BSTNode* node)  // Get Left BST of node
{
    return node->left;
}

void set_left(struct BSTNode* node, struct BSTNode* left)   // Set Left BST of node
{
    node->left = left;
}

struct BSTNode* get_right(struct BSTNode* node) // Get Right BST of node
{
    return node->right;
}

void set_right(struct BSTNode* node, struct BSTNode* right) // Set Right BST of node
{
    node->right = right;
}

char* get_word(struct BSTNode* node)    // Get word of node
{
    return node->word;
}

void set_word(struct BSTNode* node, char* word) // Set word of node
{
    node->word = word;
}

int compare_word_to_node(char* word, struct BSTNode* node)  // Compare word to current node
{
    return strcmp(word, get_word(node));
}

int get_count(struct BSTNode* node) // Get count of node (frequency of word)
{
    return node->count;
}

void set_count(struct BSTNode* node, int count) // Set count of node (frequency of word)
{
    node->count = count;
}

void increment_count(struct BSTNode* node)  // Increment count of node by 1
{
    node->count++;
}

void lower_string(char s[]) // Convert string s to lowercase
{
   int c = 0;
   
   while (s[c] != '\0') {
      if (s[c] >= 'A' && s[c] <= 'Z') {
         s[c] = s[c] + 32;
      }
      c++;
   }
}

char* str_concat(char *strg1, char *strg2)   // Concat two char pointers into one char pointer (concat 2 strings)
{
    char* out = malloc(sizeof(char) * (strlen(strg1) + strlen(strg2))); // Create string (char*) and set size
    
    out[0] = '\0';  // Set string as empty initially
    
    strcat(out, strg1); // Concat first string
    strcat(out, strg2); // COncat second string
    
    return out;
}

void print_bst(struct BSTNode* node, int longestWord, int longestCount) // Print word and their count in order
{
    if (node == NULL)   // If node does not exist, skip
    {
        return;
    }
    
    if (get_left(node) != NULL) // Go to left child first, if it exists
    {
#ifdef DEBUG
        printf("Going to left node\n");
#endif
        print_bst(get_left(node), longestWord, longestCount);
    }
    
    // Create string that will form the line to be written onto output file (Word : count)
    char* list = "";
    
    // Get length of word
    char* word = get_word(node);
    int wordLen = 0;
    for (wordLen = 0; word[wordLen] != '\0'; ++wordLen);
    
#ifdef DEBUG2
    printf("%s", get_word(node));
#endif
#ifdef DEBUG
    printf("Concat words: '%s' + '%s'\n", list, word);
#endif
    
    // Concatenate word
    list = str_concat(list, word);
    
#ifdef DEBUG
    printf("Concat word done\n");
    printf("Start adding spaces\n");
#endif

    // Format spaces between word and colon (:)
    if (wordLen < longestWord)
    {
        for (int i = wordLen; i <= longestWord; i++)
        {
#ifdef DEBUG2
            printf(" ");
#endif
            list = str_concat(list, " ");
        }
    }
    else
    {
#ifdef DEBUG2
        printf(" ");
#endif
        list = str_concat(list, " ");
    }
#ifdef DEBUG
    printf("Concated left spaces\n");
#endif
    
    // Concatenate colon (:)
    list = str_concat(list, ":");
    
#ifdef DEBUG2
    printf(":");
#endif
#ifdef DEBUG
    printf("Concated colon (:)\n");
#endif
    
    // Format spaces between colon (:) and word count
    int wordCount = get_count(node);
    int counter = 0;
    while (wordCount != 0) {
        wordCount /= 10;
        ++counter;
    }
    
    if (counter < longestCount)
    {
        for (int i = counter; i <= longestCount; i++)
        {
#ifdef DEBUG2
            printf(" ");
#endif
            list = str_concat(list, " ");
        }
    }
    else
    {
#ifdef DEBUG2
        printf(" ");
#endif
        list = str_concat(list, " ");
    }

#ifdef DEBUG
    printf("Concated right spaces\n");
    printf("Word Count: %i\n", wordCount);
#endif
#ifdef DEBUG2
    printf("%i\n", get_count(node));
#endif

    // Convert word count from int to string (char*)
    char* wordCountStr = malloc(sizeof(char) * 4);
    sprintf(wordCountStr, "%d", get_count(node));
    
#ifdef DEBUG
    printf("Concat words: '%s' + '%s'\n", list, wordCountStr);
#endif
    
    // Concatenate word count
    list = str_concat(list, wordCountStr);

#ifdef DEBUG
    printf("Concated Word Count\n");
#endif

    // Concatenate newline
    list = str_concat(list, "\n");

#ifdef DEBUG
    printf("Concated backslash n \n");
#endif

    // Assign buffer to string
    strcpy(bufout, list);
    
    w = write(fdout, bufout, strlen(list)); // Write line into output.txt
    if (w < 0) // If w is less than zero (File doesn't exist or problem writing), stop program
    {
       printf("Error: Problem with writing file\n");
       return;
    }
    
    if (get_right(node) != NULL) // Go to right child
    {
#ifdef DEBUG
        printf("Going right node\n");
#endif
        print_bst(get_right(node), longestWord, longestCount);
    }
}

int largestNumOfThree(int a, int b, int c) // Helper method to return largest number of out three numbers
{
    if (a >= b && a >= c)
    {
        return a;
    }
    else if (b >= a && b >= c)
    {
        return b;
    }
    else
    {
        return c;
    }
}

int get_longest_word_length(struct BSTNode* node) // Get longest word length from the bst
{
    if (node == NULL)   // If node does not exist, skip
    {
        return 0;
    }
    
    int left = 0;
    int middle = 0;
    int right = 0;
    
    if (get_left(node) != NULL) // Get longest word length from left child node
    {
        left = get_longest_word_length(get_left(node));
    }
    
    char* word = get_word(node);
    for (middle = 0; word[middle] != '\0'; ++middle);   // Get middle word count

    if (get_right(node) != NULL) // Get longest word length from right child node
    {
        right = get_longest_word_length(get_right(node));
    }
    
    return largestNumOfThree(left, middle, right);  // Return largest of the three words
}

int get_longest_word_count_length(struct BSTNode* node) // Get longest word count length (digit length) from the bst
{
    if (node == NULL)   // If node does not exist, skip
    {
        return 0;
    }
    
    int left = 0;
    int middle = 0;
    int right = 0;
    
    if (get_left(node) != NULL) // Get longest word count digit length from left child node
    {
        left = get_longest_word_count_length(get_left(node));
    }
    
    int wordCount = get_count(node);    // Get word count digit length of current node
    while (wordCount != 0) {
        wordCount /= 10;
        ++middle;
    }
    
    if (get_right(node) != NULL) // Get longest word count digit length from right child node
    {
        right = get_longest_word_count_length(get_right(node));
    }
    
    return largestNumOfThree(left, middle, right);  // Return largest of the three words
}

void add_word_to_bst(char* word, struct BSTNode* head)  // Add word to bst head
{
    if (word == NULL)   // If word is NULL (does not exist), stop
    {
        printf("Error: word does not exist - cannot be added to BST\n");
        return;
    }
    else                // Otherwise, convert word to lowercase
    {
        lower_string(word);
    }
    
    if (get_word(head) == "0")  // At dummy head, set all words at the left child node
    {
        if (get_left(head) == NULL) // If left node is NULL,
        {
            struct BSTNode* newWord = bstNode(NULL, word, 1, NULL); // Create new bst node that contains word
            set_left(head, newWord);    // Set head's left node to newWord
            return;
        }
        else                        // Otherwise, recurse to next node
        {
            add_word_to_bst(word, get_left(head));  // Recurse to head's left node
        }
    }
    
    int difference = compare_word_to_node(word, head);  // Get difference between word and head's word
    
    if (difference == 0)        // If the word and head's word are the same,
    {
        increment_count(head);  // Increment word count on head
        return;
    }
    else if (difference < 0)    // If the word is smaller than the head's word, go to left node
    {
        if (get_left(head) == NULL) // If left node is NULL,
        {
            struct BSTNode* newWord = bstNode(NULL, word, 1, NULL); // Create new bst node that contains word
            set_left(head, newWord);    // Set head's left node to newWord
            return;
        }
        else                        // Otherwise, recurse to next node
        {
            add_word_to_bst(word, get_left(head));  // Recurse to head's left node
        }
    }
    else                        // If the word is larger than the head's word, go to right node
    {
        if (get_right(head) == NULL) // If right node is NULL,
        {
            struct BSTNode* newWord = bstNode(NULL, word, 1, NULL); // Create new bst node that contains word
            set_right(head, newWord);    // Set head's right node to newWord
            return;
        }
        else                        // Otherwise, recurse to next node
        {
            add_word_to_bst(word, get_right(head)); // Recurse to head's right node
        }
    }
}

int letter_to_number(char word[])  // Convert first letter of word to number (A-Z to 0-25)
{
    if (word[0] == '\0')         // Check that the first letter exist; otherwise, return -1;
    {
        printf("Error: Word does not exist or is empty\n");
        return -1;
    }
    lower_string(word);          // Convert word to lowercase
    char firstLetter = word[0];  // Obtain first letter
    int num = firstLetter - 97;  // Convert letter to number
    return num;                  // Return number
}

struct BSTNode* get_hashTable_pointer(char word[])  // Get the hashTable pointer of certain index, given word
{
    int index = letter_to_number(word); // Get index of first letter of word
    
    if (index < 0 || index > 25)    // If index of out of range (0-25), return NULL;
    {
        printf("Error: Cannot get Hashtable Pointer - Index %i is out of range\n", index);
        return NULL;
    }
    
    return hashTable[index];        // Return pointer of respective hashTable index
}

// Source: https://www.techiedelight.com/implement-substr-function-c/
char* substr(char* src, int m, int n) // get substring of src between m and n (excluding n)
{
    // get length of the destination string
    int len = n - m;

    // allocate (len + 1) chars for destination (+1 for extra null character)
    char *dest = (char*)malloc(sizeof(char) * (len + 1));

    // extracts characters between m'th and n'th index from source string
    // and copy them into the destination string
    for (int i = m; i < n && (*(src + i) != '\0'); i++)
    {
        *dest = *(src + i);
        dest++;
    }

    // null-terminate the destination string
    *dest = '\0';

    // return the destination string
    return dest - len;
}

int main(int argc, char *argv[], char* envp[])    // Main Method
{
    for (int i = 0; i < 26; i++)    // Initially set all hash Table pointers to a dummy head (with "word" of 0)
    {
        hashTable[i] = bstNode(NULL, "0", 0, NULL);
    }
    
    int fdin, cnt;      // Integers for File reading/writing/scanning
    char buf[1024];     // Buffer for reading file(s)
    
    // Get any textfiles from cat input (Ex: cat aladdin.txt | ./wordfreak)
    while ((cnt = read(0, buf, 1023)) > 0)
    {
        if (cnt < 0) // If the file is less than zero (Doesn't exist or problem reading file), stop program
        {
            printf("Error: Problem with reading file\n");
            return -1;
        }
        
        // Parse buf into words, excluding spaces, numbers and punctuations (Anything non-alphabetical)
        int lastIndex = 0;              // Set last index
        int curIndex = 0;               // Set current index
        
        for (int i = 0; i < cnt; ++i)   // Traverse through whole cnt
        {
            curIndex = i; // Update current index to index i
            
            // If index i is in the alphabet, but lastIndex is not in the alphabet, and i is greater than lastIndex,
            if (isalpha(buf[i]) != 0 && isalpha(buf[lastIndex]) == 0 && (i > lastIndex))
            {
                lastIndex = i;  // Set lastIndex to i
                curIndex = i;   // Set current Index to i
                continue;       // Skip to end of loop and iterate
            }
            
            // If there are more than 1 non-alphabetical characters in succession, skip
            if (isalpha(buf[curIndex]) == 0 && isalpha(buf[lastIndex]) == 0)
            {
                continue;
            }
            
            // If char from index i is NOT in the alphabet, or i is the last index of buf,
            if (isalpha(buf[i]) == 0 || (i == cnt-1))
            {
                char* word;
                
                if (isalpha(buf[i]) == 0 && (i == cnt-1)) // If at the end of cnt and it is not an alphabet,
                {
                    word = substr(buf, lastIndex, curIndex);  // Parse from lastIndex to right before curIndex
                }
                else if (i != cnt-1) // If current index i is NOT at end of buf
                {
                    word = substr(buf, lastIndex, curIndex); // Parse word from last to current-1
                }
                else  // Parse word from last to current if current index i is last index of buf
                {
                    char buf2[1];
                    cnt = read(fdin, buf2, 1);  // Get next character after first buf
                    
                    if (isalpha(buf2[0]) != 0)  // If the next buf does contain an alphabet,
                    {
                        lseek(fdin, lastIndex+1, SEEK_SET); // Set buffer to start of cut-off word of buf
                        break;                              // Get out of loop
                    }
                    else    // Otherwise, the last index of first buf is last letter of the word
                    {
                        word = substr(buf, lastIndex, curIndex+1);  // Get last word
                    }
                }
                
                add_word_to_bst(word, get_hashTable_pointer(word)); // Add word to Hash Table (BST List)
                lastIndex = curIndex;                               // Set last index to current index
            }
        }
    }
    
    // Get any textfiles inputed in environment (envp)
    if (envp != NULL)
    {
        for (int i = 0; envp[i] != NULL; i++)
        {
            if (strstr(envp[i], ".txt") == NULL)    // If envp[i] does NOT contain textfiles (.txt), skip
            {
                continue;
            }
            
            char* envpTxtFileName = strstr(envp[i], "=");   // Extract/Parse out textfile name
            envpTxtFileName = substr(envpTxtFileName, 1, strlen(envpTxtFileName));
            fdin = open(envpTxtFileName, O_RDONLY); // Read file
            
            if (fdin < 0)   // Check that the file has been successfully opened
            {
                printf("Error: Cannot open file (envp)\n");
                return -1;
            }

            while ((cnt = read(fdin, buf, 1024)) > 0)   // Read all words from files
            {
                if (cnt < 0) // If the file is less than zero (Doesn't exist or problem reading file), stop program
                {
                    printf("Error: Problem with reading file\n");
                    return -1;
                }

                // Parse buf into words, excluding spaces, numbers and punctuations (Anything non-alphabetical)
                int lastIndex = 0;              // Set last index
                int curIndex = 0;               // Set current index

                for (int i = 0; i < cnt; ++i)   // Traverse through whole cnt
                {
                    curIndex = i; // Update current index to index i

                    // If index i is in the alphabet, but lastIndex is not in the alphabet, and i is greater than lastIndex,
                    if (isalpha(buf[i]) != 0 && isalpha(buf[lastIndex]) == 0 && (i > lastIndex))
                    {
                        lastIndex = i;  // Set lastIndex to i
                        curIndex = i;   // Set current Index to i
                        continue;       // Skip to end of loop and iterate
                    }

                    // If there are more than 1 non-alphabetical characters in succession, skip
                    if (isalpha(buf[curIndex]) == 0 && isalpha(buf[lastIndex]) == 0)
                    {
                        continue;
                    }

                    // If char from index i is NOT in the alphabet, or i is the last index of buf,
                    if (isalpha(buf[i]) == 0 || (i == cnt-1))
                    {
                        char* word;

                        if (isalpha(buf[i]) == 0 && (i == cnt-1)) // If at the end of cnt and it is not an alphabet,
                        {
                            word = substr(buf, lastIndex, curIndex);  // Parse from lastIndex to right before curIndex
                        }
                        else if (i != cnt-1) // If current index i is NOT at end of buf
                        {
                            word = substr(buf, lastIndex, curIndex); // Parse word from last to current-1
                        }
                        else  // Parse word from last to current if current index i is last index of buf
                        {
                            char buf2[1];
                            cnt = read(fdin, buf2, 1);  // Get next character after first buf

                            if (isalpha(buf2[0]) != 0)  // If the next buf does contain an alphabet,
                            {
                                lseek(fdin, lastIndex+1, SEEK_SET); // Set buffer to start of cut-off word of buf
                                break;                              // Get out of loop
                            }
                            else    // Otherwise, the last index of first buf is last letter of the word
                            {
                                word = substr(buf, lastIndex, curIndex+1);  // Get last word
                            }
                        }

                        add_word_to_bst(word, get_hashTable_pointer(word)); // Add word to Hash Table (BST List)
                        lastIndex = curIndex;                               // Set last index to current index
                    }
                }
            }

            if (close(fdin) < 0) // Check that fdin file closed successfully
            {
                printf("\nError: Closing fdin File Failed\n");
                return -1;
            }
        }
    }
    
    // Get any textfiles inputed in argument (argv)
    for (int i = 1; i < argc; i++)
    {
        fdin = open(argv[i], O_RDONLY); // Read file
        if (fdin < 0)                   // Check that the file has been successfully opened
        {
            printf("Error: Cannot open file (argv)\n");
            return -1;
        }
        
        while ((cnt = read(fdin, buf, 1024)) > 0)   // Read all words from files
        {
            if (cnt < 0) // If the file is less than zero (Doesn't exist or problem reading file), stop program
            {
                printf("Error: Problem with reading file\n");
                return -1;
            }
            
            // Parse buf into words, excluding spaces, numbers and punctuations (Anything non-alphabetical)
            int lastIndex = 0;              // Set last index
            int curIndex = 0;               // Set current index
            
            for (int i = 0; i < cnt; ++i)   // Traverse through whole cnt
            {
                curIndex = i; // Update current index to index i
                
                // If index i is in the alphabet, but lastIndex is not in the alphabet, and i is greater than lastIndex,
                if (isalpha(buf[i]) != 0 && isalpha(buf[lastIndex]) == 0 && (i > lastIndex))
                {
                    lastIndex = i;  // Set lastIndex to i
                    curIndex = i;   // Set current Index to i
                    continue;       // Skip to end of loop and iterate
                }
                
                // If there are more than 1 non-alphabetical characters in succession, skip
                if (isalpha(buf[curIndex]) == 0 && isalpha(buf[lastIndex]) == 0)
                {
                    continue;
                }
                
                // If char from index i is NOT in the alphabet, or i is the last index of buf,
                if (isalpha(buf[i]) == 0 || (i == cnt-1))
                {
                    char* word;
                    
                    if (isalpha(buf[i]) == 0 && (i == cnt-1)) // If at the end of cnt and it is not an alphabet,
                    {
                        word = substr(buf, lastIndex, curIndex);  // Parse from lastIndex to right before curIndex
                    }
                    else if (i != cnt-1) // If current index i is NOT at end of buf
                    {
                        word = substr(buf, lastIndex, curIndex); // Parse word from last to current-1
                    }
                    else  // Parse word from last to current if current index i is last index of buf
                    {
                        char buf2[1];
                        cnt = read(fdin, buf2, 1);  // Get next character after first buf
                        
                        if (isalpha(buf2[0]) != 0)  // If the next buf does contain an alphabet,
                        {
                            lseek(fdin, lastIndex+1, SEEK_SET); // Set buffer to start of cut-off word of buf
                            break;                              // Get out of loop
                        }
                        else    // Otherwise, the last index of first buf is last letter of the word
                        {
                            word = substr(buf, lastIndex, curIndex+1);  // Get last word
                        }
                    }
                    
                    add_word_to_bst(word, get_hashTable_pointer(word)); // Add word to Hash Table (BST List)
                    lastIndex = curIndex;                               // Set last index to current index
                }
            }
        }
        
        if (close(fdin) < 0) // Check that fdin file closed successfully
        {
            printf("\nError: Closing fdin File Failed\n");
            return -1;
        }
    }
    
    /*
    Print the list of words and their counts from all 26 bst nodes from the HashTable onto the file output.txt
        - Find the longest word length and longest word count digit for formatting
        - Open output file
        - Write list onto output file (Formatted based on longest word count digit length and word length)
        - Close output file
    */
    int longestWordLength = 0;      // Longest word length
    int longestCountLength = 0;     // Longest word count digit length
    for (int i = 0; i < 26; i++)    // Get longest word length and count digit length
    {
        int tempWordLen = get_longest_word_length(get_left(hashTable[i]));        // Longest word length
        int tempCountLen = get_longest_word_count_length(get_left(hashTable[i])); // Longest count length
        
        if (tempWordLen > longestWordLength)    // Update largest word length if necessary
        {
            longestWordLength = tempWordLen;
        }
        
        if (tempCountLen > longestCountLength)    // Update largest word count digit length if necessary
        {
            longestCountLength = tempCountLen;
        }
    }
 
    fdout = open("output.txt", O_WRONLY);  // Open output file to write onto
    if (fdout < 0)                         // Check that the file has been successfully opened
    {
        printf("Error: Cannot open output.txt file\n");
        return -1;
    }
    
    for (int i = 0; i < 26; i++)    // Print and write entire list of words and their frequency onto output file
    {
        print_bst(get_left(hashTable[i]), longestWordLength, longestCountLength);
    }
    
    if (close(fdout) < 0) // Check that fdout file closed successfully
    {
        printf("\nError: Closing fdout File Failed\n");
        return -1;
    }
    
    return 0;
}
