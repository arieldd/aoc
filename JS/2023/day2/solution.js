const fs = require("node:fs");

const lines = fs.readFileSync("input.txt", "utf-8").split("\n");

let indices = { red: 0, green: 1, blue: 2 };

let games = lines.map((l) => {
    const parts = l.split(":");
    let id = parts.at(0).split(" ").at(-1);
    let rounds = [];
    parts
        .at(-1)
        .split(";")
        .forEach((r) => {
            let set = Array(3).fill(0);

            let plays = r.split(",");
            for (const p of plays) {
                const [value, color] = p.trim().split(" ");
                let idx = indices[color];
                set[idx] = Number(value);
            }
            rounds.push(set);
        });
    return { id, rounds };
});

let limits = [12, 13, 14];

let part1 = games.reduce((acc, g) => {
    let value = Number(g.id);
    g.rounds.forEach((r) => {
        for (const idx of [0, 1, 2]) {
            if (r[idx] > limits[idx]) {
                value = 0;
                return;
            }
        }
    });
    return acc + value;
}, 0);

let part2 = games.reduce((acc, g) => {
    let max = Array(3).fill(0);

    g.rounds.forEach((r) => {
        for (const idx of [0, 1, 2]) {
            if (r[idx] > max[idx]) {
                max[idx] = r[idx];
            }
        }
    });

    return acc + max.reduce((acc, p) => acc * p, 1);
}, 0);

console.log("Part 1: %d", part1);
console.log("Part 2: %d", part2);
