#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define TOP_PLAINTEXTS 10

// English letter frequency table (percentages)
const float english_letter_frequency[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153,
    0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056,
    2.758, 0.978, 2.360, 0.150, 1.974, 0.074
};

// Function to convert a character to its corresponding index in the alphabet
int char_to_index(char ch) {
    if (isupper(ch)) {
        return ch - 'A';
    } else if (islower(ch)) {
        return ch - 'a';
    } else {
        return -1; // Not a valid alphabet character
    }
}

// Function to convert an index in the alphabet to a character
char index_to_char(int idx) {
    return 'A' + idx;
}

// Function to calculate the frequency of letters in the ciphertext
void calculate_letter_frequencies(const char *ciphertext, int *freq) {
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        char ch = toupper(ciphertext[i]);
        if (isalpha(ch)) {
            freq[ch - 'A']++;
        }
    }
}

// Function to compare frequencies for sorting (descending order)
int compare_frequencies(const void *a, const void *b) {
    return (*(int *)b - *(int *)a);
}

// Function to perform letter frequency attack on a monoalphabetic substitution cipher
void letter_frequency_attack(const char *ciphertext, int top_plaintexts) {
    int freq[ALPHABET_SIZE] = {0};
    int ciphertext_len = strlen(ciphertext);
    
    // Calculate letter frequencies in the ciphertext
    calculate_letter_frequencies(ciphertext, freq);
    
    // Prepare to sort frequencies
    int sorted_freq[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sorted_freq[i] = freq[i];
    }
    
    // Sort frequencies in descending order
    qsort(sorted_freq, ALPHABET_SIZE, sizeof(int), compare_frequencies);
    
    // Determine the top possible plaintexts based on frequency analysis
    printf("Top %d possible plaintexts:\n", top_plaintexts);
    for (int k = 0; k < top_plaintexts; k++) {
        // Calculate the shift based on frequency analysis
        int shift = sorted_freq[k] - english_letter_frequency[k] * ciphertext_len / 100 + 0.5;
        printf("Key guess %d: ", k + 1);
        for (int i = 0; ciphertext[i] != '\0'; i++) {
            char ch = ciphertext[i];
            if (isalpha(ch)) {
                char plaintext_ch = 'A' + (ch - 'A' - shift + ALPHABET_SIZE) % ALPHABET_SIZE;
                printf("%c", plaintext_ch);
            } else {
                printf("%c", ch); // Non-alphabetic characters remain unchanged
            }
        }
        printf("\n");
    }
}

int main() {
    const char *ciphertext = "Wkh vhfuhw vwrub rii erxqg phvvxulwb!";
    int top_plaintexts = 10; // Specify the number of top plaintexts to display
    
    // Perform letter frequency attack on the ciphertext
    letter_frequency_attack(ciphertext, top_plaintexts);
    
    return 0;
}
