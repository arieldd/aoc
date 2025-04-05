fn main() {
    let input = include_bytes!("input.txt").to_vec();
    unsafe {
        let data = String::from_utf8_unchecked(input);
        println!("Part 1: {}", part1(&data));
        println!("Part 2: {}", part2(&data));
    }
}

fn part1(data: &str) -> i32 {
    data.split_whitespace()
        .map(|mass_str| {
            let mass = mass_str.parse::<i32>().expect("Should be a valid number");
            let fuel = mass / 3 - 2;
            fuel
        })
        .sum()
}

fn part2(data: &str) -> i32 {
    data.split_whitespace()
        .map(|mass_str| {
            let mass = mass_str.parse::<i32>().expect("Should be a valid number");
            let mut fuel = mass;
            let mut total = 0;
            while fuel > 0 {
                fuel = fuel / 3 - 2;
                total += if fuel > 0 { fuel } else { 0 };
            }
            total
        })
        .sum()
}

#[test]
fn test_part2() {
    assert_eq!(part2("1969"), 966);
    assert_eq!(part2("100756"), 50346);
}
