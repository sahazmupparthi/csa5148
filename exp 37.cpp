#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_LETTERS 26
#define TOP_PLAINTEXTS 10

// English letter frequency table (percentages)
const float english_letter_frequency[NUM_LETTERS] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153,
    0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056,
    2.758, 0.978, 2.360, 0.150, 1.974, 0.074
};

// Function to calculate letter frequencies in ciphertext
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

// Function to perform letter frequency attack
void letter_frequency_attack(const char *ciphertext, int top_plaintexts) {
    int freq[NUM_LETTERS] = {0};
    int ciphertext_len = strlen(ciphertext);
    
    // Calculate letter frequencies in ciphertext
    calculate_letter_frequencies(ciphertext, freq);
    
    // Prepare to sort frequencies
    int sorted_freq[NUM_LETTERS];
    for (int i = 0; i < NUM_LETTERS; i++) {
        sorted_freq[i] = freq[i];
    }
    
    // Sort frequencies in descending order
    qsort(sorted_freq, NUM_LETTERS, sizeof(int), compare_frequencies);
    
    // Determine the top possible plaintexts based on frequency analysis
    printf("Top %d possible plaintexts:\n", top_plaintexts);
    for (int k = 0; k < top_plaintexts; k++) {
        int key = sorted_freq[k] - english_letter_frequency[k] * ciphertext_len / 100 + 0.5;
        printf("Key guess %d: ", k + 1);
        for (int i = 0; ciphertext[i] != '\0'; i++) {
            char ch = ciphertext[i];
            if (isalpha(ch)) {
                char plaintext_ch = 'A' + (ch - 'A' - key + 26) % 26;
                if (islower(ciphertext[i])) {
                    plaintext_ch = tolower(plaintext_ch);
                }
                printf("%c", plaintext_ch);
            } else {
                printf("%c", ch); // Non-alphabetic characters remain unchanged
            }
        }
        printf("\n");
    }
}

int main() {
    const char *ciphertext = "Khoor, Jllihv Fhduhw!";
    int top_plaintexts = 10; // Specify the number of top plaintexts to display
    
    // Perform letter frequency attack
    letter_frequency_attack(ciphertext, top_plaintexts);
    
    return 0;
}
