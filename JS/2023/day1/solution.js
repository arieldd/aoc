const fs = require("node:fs");

let lines = fs.readFileSync("input.txt", "utf8").split("\n");
let numbers = {
    one: 1,
    two: 2,
    three: 3,
    four: 4,
    five: 5,
    six: 6,
    seven: 7,
    eight: 8,
    nine: 9,
};

function solve(lines, part2) {
    return lines
        .map((l) => {
            let digits = [];
            for (const [i, c] of l.split("").entries()) {
                if (c >= "0" && c <= "9") digits.push(c - "0");
                else if (part2) {
                    for (const size of [3, 4, 5]) {
                        let chunk = l.substr(i, size);
                        if (chunk in numbers) digits.push(numbers[chunk]);
                    }
                }
            }

            return digits.at(0) * 10 + digits.at(-1);
        })
        .reduce((acc, v) => acc + v, 0);
}

console.log("Part 1: %d", solve(lines, false));
console.log("Part 2: %d", solve(lines, true));
