#include <stdio.h>
#include <stdlib.h>

extern int request(const char *method, const char *url, const char *data);
int main(void) {
 	int i = request("GET", "https://google.com", NULL);
	printf("%d", i);
}
