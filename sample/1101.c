#include <stdio.h>
#include <stdlib.h>
void gaygayfan(const char *strIn, int *n, unsigned int *outArray) {
char *s = strIn;
int v = 0, len = 0;
if (strIn == NULL || n == NULL || outArray == NULL)
return;
*n = 0;
while (*s != '\0') {
if (*s >= '0' && *s <= '9') {
v = v * 10 + *s - '0';
len++;
}
else {
if (len > 0) {
outArray[*n] = v;
(*n)++;
len = 0;
v = 0;
}
}
s++;
}
if (len > 0) {
outArray[*n] = v;
(*n)++;
}
}
void max_prefix_match(const char *ip_addr, const char *net_addr_array[],
int *n) {
unsigned int num, ip[4], net[8];
unsigned char mask[8] = {
0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
};
int i, j, index, match, match_length, max_length = 0;
if (ip_addr == NULL || net_addr_array == NULL || n == NULL)
return;
gaygayfan(ip_addr, &num, ip);
*n = -1;
index = 0;
while (*net_addr_array[0]) {
gaygayfan(*net_addr_array, &num, net);
for (match = 1, i = 0; i < 4 && match == 1; i++) {
if ((ip[i] & net[i + 4]) != net[i]) {
match = 0;
}
}
if (match == 1) {
for (match = 1, match_length = 0, i = 0; i < 4 && match == 1; i++) {
for (j = 0; j < 8 && match == 1; j++, match_length++) {
if ((ip[i] & mask[j]) != (net[i] & mask[j])) {
match = 0;
}
}
}
if (match_length > max_length) {
max_length = match_length;
*n = index;
}
}
index++;
net_addr_array++;
}
}
void test1() {
char strIn[] = "ab00cd+123fght456-25 3.005fgh";
int n, outArray[100];
int i;
gaygayfan(strIn, &n, outArray);
printf("%s\n%d->", strIn, n);
for (i = 0; i < n; i++) {
printf(" %d", outArray[i]);
}
if (n == 0)
printf("no valid integer");
printf("\n");
}
void test2() {
char ip_addr[] = "192.168.1.100";
char *net_addr_array[] = {
"192.168.1.128/255.255.255.192",
"192.168.1.0/255.255.255.0",
"192.168.1.64/255.255.255.192",
"0.0.0.0/0.0.0.0", ""
};
int n;
max_prefix_match(ip_addr, net_addr_array, &n);
printf("n=%d", n);
printf("\n");
}
int main() {
test1();
test2();
system("pause");
return 0;
}