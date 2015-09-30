def fib(n):
    if n == 0: 
    	return 0
    if n == 1: 
    	return 1
    return fib(n-1) + fib(n-2)
n = 10
i = 1
while i <= n:
	print fib(i),
	print ',',
	i = i+1
print
