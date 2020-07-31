
import Console;

for (var i = 0; i < 10; i += 1) {
	if (i == 6) continue;
	if (i == 8) break;
	Console::writeLine(i);
}

var y = 0;

loop {
	if (y == 4) break;
	y += 1;
}

do {
	Console::writeLine("Hello");
	y -= 1;
} while (y > 0);

repeat {
	Console::writeLine("World");
	y += 1;
} until (y == 4);

while (y > 0) {
	Console::writeLine("Again");
	y -= 1;
}

until (y == 4) {
	Console::writeLine("Again");
	y += 1;
}
