var x = true;
var v1: String = x.string(); // v1 <- "true".
x.toggle();
var v2 = x.string(); // v2 <- "false".