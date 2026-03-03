#include <stdio.h>

int main() {
    float a, b, c;
    printf("Digite os tres lados: ");
    scanf("%f %f %f", &a, &b, &c);

    if (a == b && b == c)
        printf("Equilatero\n");
    else if (a == b || b == c || a == c)
        printf("Isosceles\n");
    else
        printf("Escaleno\n");

    return 0;
}
