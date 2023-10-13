/*
 * No changes are allowed in this file
 */
// #include <stdio.h>
int fib(int n) {
  if(n<2) return n;
  else return fib(n-1)+fib(n-2);
}

int _start() {
  // printf("Hello me");
  int number = 109;
  int prime = 1;
  for(int i = 2;i<number;i++){
    if (number % i ==0){
      prime = 0;
    }
  }
  return prime==1 ? number : -1;
	// int val = fib(40);
	// return val;
}
