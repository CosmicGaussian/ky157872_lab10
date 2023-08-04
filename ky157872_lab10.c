#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie node structure
struct TrieNode
{
    struct TrieNode* children[26]; // Array to store children nodes for each character (a-z)
    int count; // Counter to keep track of the number of occurrences of a word
};

// Trie structure
struct Trie
{
    struct TrieNode* root; // Pointer to the root node of the trie
};

// Helper function to create a new TrieNode
struct TrieNode* createTrieNode()
{
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    if (newNode)
    {
        newNode->count = 0;
        for (int i = 0; i < 26; i++)
        {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

// Inserts the word into the trie structure
void insert(struct Trie *pTrie, char *word)
{
    if (pTrie == NULL || word == NULL)
    {
        return;
    }

    struct TrieNode* current = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a'; // Get the index for the current character (a: 0, b: 1, ..., z: 25)
        if (current->children[index] == NULL)
        {
            // If the node for the character does not exist, create a new node
            current->children[index] = createTrieNode();
        }
        current = current->children[index]; // Move to the next node for the next character
    }

    current->count++; // Increment the count to indicate another occurrence of the word
}

// Helper function to find the TrieNode corresponding to a word
struct TrieNode* findTrieNode(struct TrieNode* root, char* word)
{
    struct TrieNode* current = root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a'; // Get the index for the current character (a: 0, b: 1, ..., z: 25)
        if (current->children[index] == NULL)
        {
            // If the node for the character does not exist, the word is not in the trie
            return NULL;
        }
        current = current->children[index]; // Move to the next node for the next character
    }
    return current; // Return the node corresponding to the complete word
}

// Computes the number of occurrences of the word in the trie
int numberOfOccurrences(struct Trie *pTrie, char *word)
{
    if (pTrie == NULL || word == NULL)
    {
        return 0;
    }

    struct TrieNode* node = findTrieNode(pTrie->root, word);
    if (node)
    {
        return node->count; // If the word exists in the trie, return its count
    }

    return 0; // If the word is not found, return 0
}

// Helper function to deallocate the trie structure recursively
void deallocateTrieHelper(struct TrieNode* node)
{
    if (node == NULL)
    {
        return;
    }

    for (int i = 0; i < 26; i++)
    {
        if (node->children[i] != NULL)
        {
            // Recursively deallocate all the child nodes of the current node
            deallocateTrieHelper(node->children[i]);
        }
    }

    free(node); // Free the memory for the current node
}

// Deallocate the trie structure and free the memory
struct Trie* deallocateTrie(struct Trie *pTrie)
{
    if (pTrie == NULL)
    {
        return NULL;
    }

    deallocateTrieHelper(pTrie->root); // Start the recursive deallocation from the root
    free(pTrie); // Free the memory for the Trie structure itself
    return NULL; // Return NULL to indicate successful deallocation
}

// Initializes a new trie structure
struct Trie* createTrie()
{
    struct Trie* newTrie = (struct Trie*)malloc(sizeof(struct Trie));
    if (newTrie)
    {
        // Create the root node for the trie
        newTrie->root = createTrieNode();
    }
    return newTrie;
}

// This function will return the number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char* filename, char** pInWords)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening the file: %s\n", filename);
        exit(1);
    }

    int numWords = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file))
    {
        // Removing the trailing newline character
        buffer[strcspn(buffer, "\n")] = '\0';
        pInWords[numWords] = strdup(buffer); // Allocate memory and copy the word into the array
        numWords++;
    }

    fclose(file);
    return numWords; // Return the number of words read from the dictionary
}

int main(void)
{
    char *inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    // Create a new trie and insert all words from the dictionary
    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Parse line by line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // Deallocate the trie to free the memory
    pTrie = deallocateTrie(pTrie);
    if (pTrie == NULL)
    {
        printf("Trie deallocated successfully.\n");
    }
    else
    {
        printf("There is an error in this program\n");
    }
    return 0;
}