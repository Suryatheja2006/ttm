#include <iostream>
#include <vector>
#include <bitset>
#include <random>  // Include the random library
#include <algorithm>

#include <openssl/sha.h>  // To use SHA256 hash function

const int NUM_BITS = 64;  // Number of bits in SimHash

// Function to compute a 64-bit hash using SHA-256 and convert it to a binary string
std::bitset<NUM_BITS> computeHash(int num) {
    std::string input = std::to_string(num);
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input.c_str(), input.size(), hash);

    // Convert first 8 bytes of SHA-256 hash to 64-bit binary representation
    uint64_t hash64 = 0;
    for (int i = 0; i < 8; ++i) {
        hash64 = (hash64 << 8) | hash[i];
    }
    return std::bitset<NUM_BITS>(hash64);
}

// Function to compute SimHash for a vector
std::bitset<NUM_BITS> simHash(const std::vector<int>& vector) {
    int accumulator[NUM_BITS] = {0};  // Initialize accumulator

    // Process each number in the vector
    for (int num : vector) {
        std::bitset<NUM_BITS> hashBits = computeHash(num);

        // Update accumulator based on hash bits
        for (int i = 0; i < NUM_BITS; ++i) {
            if (hashBits[i]) {
                accumulator[i] += 1;
            } else {
                accumulator[i] -= 1;
            }
        }
    }

    // Create the final SimHash based on accumulator values
    std::bitset<NUM_BITS> finalHash;
    for (int i = 0; i < NUM_BITS; ++i) {
        finalHash[i] = accumulator[i] > 0;
    }
    return finalHash;
}

// const int NUM_BITS = 32;  // Number of bits for each "hash" (integer representation)

int randomNumber(int minValue, int maxValue){
    // Use a random device to seed the random number generator
    std::random_device rd;
    std::mt19937 generator(rd());  // Initialize the generator with the seed

    // Define the distribution range
    std::uniform_int_distribution<int> distribution(minValue, maxValue);

    return distribution(generator);
}

// Function to generate a random vector of given length
std::vector<int> generateRandomVector(int length, int minValue, int maxValue) {
    std::vector<int> randomVector(length);  // Create a vector of the specified length

    // Fill the vector with random values
    for (int i = 0; i < length; ++i) {
        randomVector[i] = randomNumber(minValue, maxValue);  // Generate a random number in the specified range
    }

    return randomVector;  // Return the filled vector
}

// // Function to convert an integer to its binary representation
// std::bitset<NUM_BITS> integerToBinary(int num) {
//     return std::bitset<NUM_BITS>(num);  // Convert integer to 64-bit binary directly
// }

// // Function to compute SimHash for a vector
// std::bitset<NUM_BITS> simHash(const std::vector<int>& vector) {
//     int accumulator[NUM_BITS] = {0};  // Initialize accumulator

//     // Process each integer in the vector
//     for (int num : vector) {
//         std::bitset<NUM_BITS> bits = integerToBinary(num);

//         // Update accumulator based on binary bits
//         for (int i = 0; i < NUM_BITS; ++i) {
//             if (bits[i]) {
//                 accumulator[i] += 1;
//             } else {
//                 accumulator[i] -= 1;
//             }
//         }
//     }

//     // Create the final SimHash based on accumulator values
//     std::bitset<NUM_BITS> finalHash;
//     for (int i = 0; i < NUM_BITS; ++i) {
//         finalHash[i] = accumulator[i] > 0;
//     }
//     return finalHash;
// }


int hammingDistance(const std::bitset<NUM_BITS>& hash1, const std::bitset<NUM_BITS>& hash2) {
    return (hash1 ^ hash2).count();  // XOR and count differing bits
}


// Returns length of LCS for s1[0..m-1], s2[0..n-1]
int lcs(std::vector<int> &s1, std::vector<int> &s2) {
    int m = s1.size();
    int n = s2.size();

    // Initializing a matrix of size (m+1)*(n+1)
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    // Building dp[m+1][n+1] in bottom-up fashion
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
        }
    }

    // dp[m][n] contains length of LCS for s1[0..m-1]
    // and s2[0..n-1]
    return dp[m][n];
}

// Example usage
int main() {
    // Example vectors
    int size=20;
    int cnt=0;
    while(cnt<50){
        std::vector<int> vector1 = generateRandomVector(size,0,600);   //{1, 2, 3 , 1, 2, 3, 4, 5, 6, 7, 5, 4, 3, 2, 6, 7, 8, 9, 10, 3};
        std::vector<int> vector2 = generateRandomVector(size,0,600);   //{2, 3 ,4, 1, 2, 3,2, 5,3, 6, 7, 3, 2, 6, 7, 8,5, 9, 9, 3};
        // std::vector<int> ignoreidx = generateRandomVector(0.8*size,0,size-1);
        // std::vector<int> addidx = generateRandomVector(0.8*size,0,size-1);
        // sort(ignoreidx.begin(),ignoreidx.end());
        // sort(addidx.begin(), addidx.end());

        // for(int i=0,j=0,pi=0,pa=0;i<size;i++,j++){
        //     if(i==addidx[pa]){
        //         vector2[i] = randomNumber(0,600);
        //         if(pa!=15) pa++;
        //     }
        //     else if(pi<16&&j==ignoreidx[pi]){
        //         pi++;
        //         i--;
        //     }
        //     else{
        //         vector2[i]= vector1[j];
        //     }
        // }

        int LCS = lcs(vector1,vector2);

        if(LCS<4){
            cnt++;
            // Compute SimHashes
            std::bitset<NUM_BITS> hash1 = simHash(vector1);
            std::bitset<NUM_BITS> hash2 = simHash(vector2);
            int distance = hammingDistance(hash1, hash2);

            std::cout<<LCS<<" "<<distance<<'\n';
        }
    }
}
