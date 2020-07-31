
import Console;

func fib(n: Integer) -> Integer {
	if (n <= 2) return 1;
	return fib(n - 1) + fib(n - 2);
}

for (var i = 0; i < 10; i += 1) {
	Console::writeLine(fib(i));
}
