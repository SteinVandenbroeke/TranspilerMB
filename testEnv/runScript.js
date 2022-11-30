/*
The for loop is very useful in JavaScript
syntax:
for (initialiser; condition; increment) {
	code
}
*/
// print each number between 0 and 9
for (let i = 0; i < 10; i++) {
    console.log(i);
}

// for loops are useful when working with arrays
let array = [ "a", "b", "c" ];
// print each element in the array
for (let i = 0; i < array.length; i++) {
    console.log(array[i]);
}

/*
Often for loops start at 0 rather than 1
because they are so commonly used with arrays.
*/