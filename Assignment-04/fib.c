/*
 * No changes are allowed in this file
 */

char arr[100000];
int fib(int n) {
  if(n<2) return n;
  else return fib(n-1)+fib(n-2);
}

int _start() {
  arr[0] = 'a';
  arr[5000] = 'z';
  arr[8440] = '2';
	int val = fib(10);
	return val;
}
