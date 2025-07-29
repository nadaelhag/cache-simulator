#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

//my code takes on any associativity level--> bonus 
//my code calculates the miss penalty-->bonus
int main() {
    //inputting parameters
    unsigned int address_bits, cache_size, block_size, assoc;
    char trace[50];
    printf("Enter the number of address bits: ");
    scanf("%u", &address_bits);
    
    printf("Enter the cache size in bytes: ");
    scanf("%u", &cache_size);
    
    printf("Enter the block size in bytes: ");
    scanf("%u", &block_size);
    
    printf("Enter the associativity level: ");
    scanf("%u", &assoc);
    
    printf("Enter the name of the file: ");
    scanf("%s", trace);
    
    //Cache Tag/Index/Offset Calculations
    unsigned int sets, offset, tag, index;
    sets = cache_size / (block_size * assoc);
    index = (unsigned int) log2(sets);
    offset = (unsigned int) log2(block_size);
    tag = address_bits - index - offset;
    
    // Initializing the cache
    unsigned int cache[sets][assoc];
    
    // Set elements to zero
    for (int i = 0; i < sets; i++) {
        for (int j = 0; j < assoc; j++) {
            cache[i][j] = 0;
        }
    }
    
    //Open trace file
    FILE *f = fopen(trace, "r");
    if (f == NULL) {
        printf("File did not open\n");
        return 1;
    }
    
    //assign variables
    unsigned int address;
    unsigned int tag_bits;
    unsigned int index_bits;
    unsigned int offset_bits;
    unsigned int hits = 0;
    unsigned int misses = 0;
    double miss_penalty = 0.0;
    clock_t end_time, start_time;
    
    //read addresses
    while (fscanf(f, "%x", &address) != EOF) {
    
        //extract bits
        tag_bits = (address >> (index + offset));
        index_bits = (address >> offset) & ((1 << index) - 1);
        offset_bits = address & ((1 << offset) - 1);
        
        //convert index from hexadecimal to decimal to match sets
        int index_dec = 0;
        int base = 1;
    	while (index_bits > 0) {
        int digit = index_bits % 16;
        index_dec += digit * base;
        index_bits /= 16;
        base *= 16;
    }
    
    //cache lookup
    int found = 0;
    for (int j = 0; j < assoc; j++) {
      if ( cache[index_dec][j] == tag_bits){
      	  found = 1;
          hits++;
          break;
      } }
      
      //replacement policy
      if(!found){
          misses++;
          //measure miss penalty
          start_time = clock();
          
          int line_index = index_dec % sets;
          if(assoc == 1){
          cache[line_index][0] = tag_bits;
          }
          else{
          int line = rand() % assoc; 
          cache[line_index][line] = tag_bits;
          }
          end_time = clock();
          }
         miss_penalty = miss_penalty + (double)(end_time - start_time) / CLOCKS_PER_SEC; 
    }
    
    //output
    float hitratio = (float) hits/ (hits+misses);
    float missratio = (float) misses/ (hits+misses);
    
    printf("The number of hits is %d\n", hits);
    printf("The number of misses is %d\n", misses);
    printf("The hit ratio is %.2f percent\n", hitratio*100);
    printf("The miss ratio is %.2f percent\n", missratio*100);
    printf("The miss penalty is %.2f percent\n", miss_penalty);
    
    return 0;
}
