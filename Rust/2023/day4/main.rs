use std::collections::HashMap;
use std::env;
use std::fs;

fn main() {
    let input = fs::read_to_string(env::args().skip(1).next().expect("Input file name"))
        .expect("Input read");

    let cards = read_cards(&input);

    println!("Part 1 {}", part1(&cards));
    println!("Part 2 {}", part2(&cards));
}

fn part1(cards: &[Card]) -> u32 {
    cards
        .iter()
        .map(|c| {
            let hits = c.owned.iter().fold(0, |mut count, n| {
                if c.winning.contains(n) {
                    count += 1
                }
                count
            });

            match hits {
                0 => 0,
                _ => 2_u32.pow(hits - 1),
            }
        })
        .sum()
}

fn part2(cards: &[Card]) -> u32 {
    let mut map: HashMap<u32, u32> = cards.iter().map(|c| (c.id, 1)).collect();

    for c in cards {
        let hits = c.owned.iter().fold(0, |mut count, n| {
            if c.winning.contains(n) {
                count += 1
            }
            count
        });

        for won_id in c.id + 1..=c.id + hits {
            *map.entry(won_id).or_default() += map[&c.id];
        }
    }

    map.values().sum()
}

fn read_cards(input: &str) -> Vec<Card> {
    input
        .lines()
        .map(|l| {
            let (front, numbers) = l.split_once(':').unwrap();
            let id = front.split_whitespace().last().unwrap().parse().unwrap();

            let (win, yours) = numbers.split_once('|').unwrap();
            let winning = win.split_whitespace().map(|n| n.parse().unwrap()).collect();
            let owned = yours
                .split_whitespace()
                .map(|n| n.parse().unwrap())
                .collect();

            Card { id, winning, owned }
        })
        .collect()
}

#[derive(Debug)]
struct Card {
    id: u32,
    winning: Vec<u32>,
    owned: Vec<u32>,
}
