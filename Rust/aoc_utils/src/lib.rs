pub mod grid {
    use std::collections::HashMap;

    pub fn read_grid<T: NewFromChar>(lines: &[&str]) -> Grid<T> {
        Grid {
            contents: lines
                .iter()
                .enumerate()
                .fold(HashMap::new(), |mut contents, (i, row)| {
                    row.char_indices().for_each(|(j, c)| {
                        contents.insert(Point(i as isize, j as isize), T::new(&c));
                    });
                    contents
                }),
        }
    }

    #[derive(Debug)]
    pub struct Grid<T> {
        contents: HashMap<Point, T>,
    }

    #[derive(Debug, PartialEq, Eq, Hash)]
    pub struct Point(isize, isize);

    impl Point {
        pub fn new(i: usize, j: usize) -> Self {
            Point(i as isize, j as isize)
        }
    }

    pub trait NewFromChar {
        fn new(c: &char) -> Self;
    }

    #[cfg(test)]
    mod tests {}
}
