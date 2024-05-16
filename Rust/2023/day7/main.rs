use std::cmp::Ordering;
use std::collections::HashMap;
use std::env;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide an input file.")
    }
    let input = fs::read_to_string(&args[1]).expect("Should be able to read input file");
    let lines = input.lines().collect::<Vec<_>>();

    let hands = read_hands(&lines);

    println!("Part 1: {}", solve(&hands, false));
    println!("Part 2: {}", solve(&hands, true));
}

fn solve(hands: &Vec<Hand>, joker: bool) -> u32 {
    let mut sorted = hands.to_vec();

    sorted.iter_mut().for_each(|c| {
        if joker {
            c.change_joker();
        }
        c.set_value();
    });
    sorted.sort_unstable();

    let mut winnings = 0;
    for (i, h) in sorted.iter().enumerate() {
        winnings += (i as u32 + 1) * h.bid;
    }
    winnings
}

fn read_hands(lines: &[&str]) -> Vec<Hand> {
    lines
        .iter()
        .map(|l| {
            let parts = l.split(' ').collect::<Vec<_>>();
            let cards = parts[0];

            let mut hand = Hand {
                cards: vec![],
                bid: parts[1].parse::<u32>().expect("Bid should be a number"),
                value: 0,
            };
            for c in cards.chars() {
                if c > '9' {
                    hand.cards.push(get_card_value(&c))
                } else {
                    hand.cards
                        .push(c.to_digit(10).expect("Card should be a number") as u8);
                }
            }
            hand
        })
        .collect()
}

fn get_card_value(c: &char) -> u8 {
    match c {
        'A' => 14,
        'K' => 13,
        'Q' => 12,
        'J' => 11,
        'T' => 10,
        _ => panic!("{} is not a valid card", c),
    }
}

#[derive(Debug, Eq, PartialEq, Ord, Clone)]
struct Hand {
    cards: Vec<u8>,
    bid: u32,
    value: u32,
}

impl PartialOrd for Hand {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        let val_cmp = self.value.cmp(&other.value);
        if val_cmp != Ordering::Equal {
            return Some(val_cmp);
        }

        for (i, c) in self.cards.iter().enumerate() {
            let ord = c.cmp(&other.cards[i]);
            if ord != Ordering::Equal {
                return Some(ord);
            }
        }
        Some(Ordering::Equal)
    }
}

impl Hand {
    fn change_joker(&mut self) {
        for card in self.cards.iter_mut() {
            if *card == get_card_value(&'J') {
                *card = 1;
            }
        }
    }

    fn set_value(&mut self) {
        self.value = self.calculate_value();
    }

    fn calculate_value(&self) -> u32 {
        let mut value = 0;

        let mut freq: HashMap<u8, u32> = HashMap::new();

        for c in self.cards.iter() {
            freq.entry(*c).and_modify(|count| *count += 1).or_insert(1);
        }

        // Deal with Joker cards, only if there is at least 1 other card than can increase its
        // strength
        if freq.contains_key(&1) && freq.len() > 1 {
            // Take joker out of the pool and use it to strengthen the max
            let joker_count = *freq.get(&1).expect("Joker should be present");
            freq.remove(&1);

            let max_entry = freq
                .iter()
                .max_by(|a, b| {
                    let count_cmp = a.1.cmp(&b.1);
                    if count_cmp != Ordering::Equal {
                        return count_cmp;
                    }
                    a.0.cmp(b.0)
                })
                .expect("Cards should have a max");

            freq.entry(*max_entry.0).and_modify(|v| *v += joker_count);
        }

        for (_, v) in freq {
            value += 5_u32.pow(v);
        }

        value
    }
}
