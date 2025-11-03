#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct SpeedInt {
    uint64_t *limbs;  
    size_t size;      
    size_t capacity;  
} SpeedInt;



SpeedInt* speed_new_zero() {
    SpeedInt* num = (struct SpeedInt*)malloc(sizeof(SpeedInt));
    if (num == NULL)   return NULL;  


    num->limbs = (uint64_t*)malloc(sizeof(uint64_t));
    if (num->limbs == NULL) {
        free(num);
        return NULL; 
    }
    
    num->limbs[0] = 0;      
    num->size = 1;          
    num->capacity = 1;      
    
    return num;
}

bool speed_is_zero(SpeedInt* zero){
int k;
for(k=0;k<zero->size;k++) if(zero->limbs[k]!=0)return false;
return true;

}

void speed_free(struct SpeedInt *num) {
    if (num) {
        free(num->limbs);
        free(num);
    }
}

struct SpeedInt *speed_copy(const struct SpeedInt *src) {
    struct SpeedInt *copy = malloc(sizeof(struct SpeedInt));
    copy->size = src->size;
    copy->capacity = src->size;
    copy->limbs = malloc(src->size * sizeof(uint64_t));
    memcpy(copy->limbs, src->limbs, src->size * sizeof(uint64_t));
    return copy;
}



struct SpeedInt *speed_add(const struct SpeedInt *a, const struct SpeedInt *b) {
    size_t max_size = (a->size > b->size) ? a->size : b->size;
    
    struct SpeedInt *result = malloc(sizeof(SpeedInt));

    result->limbs = calloc(max_size + 1, sizeof(uint64_t));
    result->size = max_size;
    result->capacity = max_size + 1;

    uint64_t carry = 0;


    size_t k;
    size_t min_size = (a->size < b->size) ? a->size : b->size;
    for (k = 0; k < min_size; k++) {
        uint64_t sum = a->limbs[k] + b->limbs[k] + carry;
        carry = (sum < a->limbs[k]) || (sum < b->limbs[k]); 
        result->limbs[k] = sum;
    }


    const struct SpeedInt *larger = (a->size > b->size) ? a : b;
    for (; k < max_size; k++) {
        uint64_t sum = larger->limbs[k] + carry;
        carry = (sum < larger->limbs[k]); 
        result->limbs[k] = sum;
    }


    if (carry) {
        result->limbs[max_size] = carry;
        result->size++;
    }

    return result;
}


SpeedInt *speed_mult(const SpeedInt *a, const SpeedInt *b) {
    SpeedInt *result = malloc(sizeof(SpeedInt));


    result->size = a->size + b->size;
    result->capacity = result->size;
    result->limbs = calloc(result->size, sizeof(uint64_t));


    for (size_t k = 0; k < a->size; k++) {
        uint64_t carry = 0;
        for (size_t l = 0; l < b->size; l++) {

            __uint128_t product = a->limbs[k] * b->limbs[l];
            __uint128_t sum = result->limbs[k + l] + product + carry;
            
            result->limbs[k + l] = sum & 0xFFFFFFFFFFFFFFFF; 
            carry = sum >> 64;                               
        }
        result->limbs[k + b->size] += carry;
    }


    while (result->size > 1 && result->limbs[result->size - 1] == 0) {
        result->size--;
    }

    return result;
}


void speed_shift_left(SpeedInt *x, unsigned int shift) {
    if (shift == 0 || x->size == 0) return;


    unsigned int limb_shift = shift >> 6;  
    unsigned int bit_shift = shift & 0x3F; 


    size_t new_size = x->size + limb_shift + (bit_shift != 0);
    if (new_size > x->capacity) {
        x->limbs = realloc(x->limbs, new_size * sizeof(uint64_t));
        memset(x->limbs + x->size, 0, (new_size - x->size) * sizeof(uint64_t));
        x->capacity = new_size;
    }


    if (limb_shift > 0) {
        memmove(x->limbs + limb_shift, x->limbs, x->size * sizeof(uint64_t));
        memset(x->limbs, 0, limb_shift * sizeof(uint64_t));
        x->size += limb_shift;
    }


    if (bit_shift > 0) {
        uint64_t carry = 0;
        for (size_t k = limb_shift; k < x->size; k++) {
            uint64_t new_carry = x->limbs[k] >> (64 - bit_shift);
            x->limbs[k] = (x->limbs[k] << bit_shift) | carry;
            carry = new_carry;
        }
        if (carry != 0) {
            x->limbs[x->size++] = carry;
        }
    }
}


void speed_shift_right(SpeedInt *x, unsigned int shift) {
    if (shift == 0 || x->size == 0) return;


    unsigned int limb_shift = shift >> 6;   
    unsigned int bit_shift = shift & 0x3F; 


    if (limb_shift >= x->size) {
        memset(x->limbs, 0, x->size * sizeof(uint64_t));
        x->size = (x->size > 0) ? 1 : 0; 
        return;
    }


    if (limb_shift > 0) {
        memmove(x->limbs, x->limbs + limb_shift, (x->size - limb_shift) * sizeof(uint64_t));
        memset(x->limbs + (x->size - limb_shift), 0, limb_shift * sizeof(uint64_t));
        x->size -= limb_shift;
    }


    if (bit_shift > 0) {
        uint64_t carry = 0;
        for (size_t k = x->size; k-- > 0; ) {
            uint64_t new_carry = x->limbs[k] << (64 - bit_shift);
            x->limbs[k] = (x->limbs[k] >> bit_shift) | carry;
            carry = new_carry;
        }
    }

    while (x->size > 1 && x->limbs[x->size - 1] == 0) {
        x->size--;
    }
}



