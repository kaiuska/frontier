#include <cstdlib>
#include <cstdio>


int main()
{
    int seed = 5;
    srand(seed);
    for(int i = 0; i < 10; i++){
        printf("%d ", rand());
    }
    printf("\n");

    seed = 5;
    srand(seed);
    for(int i = 0; i < 10; i++){
        printf("%d ", rand());
    }
    printf("\n");



    return 0;
}
