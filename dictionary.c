// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <ctype.h>

#include "dictionary.h"

// Number of words in the dictionary
int num = 0;

bool loaded = false;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1000;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char word2[LENGTH + 1]; // word2 is same as word but lowercase
    strcpy(word2, word);
    for (int i = 0; i < strlen(word2); i++)
    {
        word2[i] = tolower(word2[i]); // convert each char to lower
    }
    int index = hash(word2);
    for (node *temp = table[index]; temp != NULL; temp = temp->next)
    {
        if (strcmp(temp->word, word2) == 0)
        {
            return true; // return true if word is found in the dictionary
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int hash = 0;
    int n;
    for (int i = 0; word[i] != '\0'; i++)
    {
        if (isalpha(word[i]))
        {
            n = word[i] - 'a' + 1;
        }
        else
        {
            n = 27;
        }
        hash = ((hash << 3) + n) % N;
    }
    return hash % N; // return hash code
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    node *n;
    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF) // read from file into word
    {
        num++; // increment count of number of words in the dictionary
        //do
        //{
            n = malloc(sizeof(node));
            if (n == NULL)
            {
                //free(n);
                return false;
            }
        //}
        //while (n == NULL);
        strcpy(n->word, word);
        int index = hash(word);
        if (table[index] == NULL)
        {
            table[index] = n;
            n->next = NULL;
        }
        else
        {
            n->next = table[index]; // insert the new node into the linked list
            table[index] = n;
        }
    }
    fclose(file); // close the file
    loaded = true; // file has been loaded successfully
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (!loaded)
    {
        return 0;
    }
    return num; // return number of words
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    if (!loaded)
    {
        return false;
    }
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *temp = cursor;
        while (cursor != NULL)
        {
            cursor = temp->next;
            free(temp);
            temp = cursor;
        } // delete each node individually by going through each one through a cursor
    }
    return true;
}
