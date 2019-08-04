#include <math.h>
#include <stdlib.h>

static const int hash_range = 1000000;
#define hash_size  100000
static int hash[hash_size] = { 0 };
#define arr_len(arr) (int)(sizeof(arr)/sizeof(*arr))

size_t gen_hash_key(size_t n) 
{
   size_t key = (n + 4096 - (1 << 12) + 9111 - (1 << 10)) % hash_size;
   return key;
}

int find_positive_min_not_in(int *A, int N) 
{
   for (int i = 0; i < N; i++) 
{
      hash[gen_hash_key(A[i])] = 1;
   }

   int i = 1;
   for (; i <= hash_size; i++) 
{
      if (!hash[gen_hash_key(i)]) 
{
         return i;
      }
   }
   return i;
}

int main() 
{

   return 0;
}