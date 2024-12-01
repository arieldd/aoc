const fs = require("fs");

const lines = fs.readFileSync("input.txt", "utf-8").split("\n");
var locations = {
  l1: [],
  l2: [],
};
lines.map((entry) => {
  if (!entry) return;
  values = entry.split("   ").map((v) => Number(v));

  locations.l1.push(values[0]);
  locations.l2.push(values[1]);
});

function part1(locations) {
  locations.l1.sort();
  locations.l2.sort();

  var distance = 0;
  locations.l1.map((v1, i) => {
    const v2 = locations.l2[i];
    distance += Math.abs(v1 - v2);
  });
  return distance;
}

function part2(locations) {
  var score = 0;
  var freq = {};
  locations.l1.forEach((value) => {
    freq[value] = 0;
  });

  locations.l2.forEach((value) => {
    if (value in freq) freq[value]++;
  });

  for (key in freq) {
    score += key * freq[key];
  }

  return score;
}

console.log(part1(locations));
console.log(part2(locations));
