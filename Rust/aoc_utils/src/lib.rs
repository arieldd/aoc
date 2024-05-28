pub mod grid {
    use std::collections::HashMap;

    #[derive(Debug)]
    pub struct Grid<T> {
        contents: HashMap<Point, T>,
        width: usize,
        height: usize,
    }

    impl<T: From<char>> Grid<T> {
        pub fn from_lines(lines: &[&str]) -> Self {
            Grid {
                contents: lines.iter().enumerate().fold(
                    HashMap::new(),
                    |mut contents, (i, row)| {
                        row.char_indices().for_each(|(j, c)| {
                            contents.insert(Point::new(i, j), T::from(c));
                        });
                        contents
                    },
                ),
                height: lines.len(),
                width: lines[0].len(),
            }
        }
    }

    impl<'a, T> Iterator for Grid<T> {
        type Item = T;

        fn next(&mut self) -> Option<Self::Item> {
            todo!()
        }
    }

    #[derive(Debug, PartialEq, Eq, Hash)]
    pub struct Point(isize, isize);

    impl Point {
        pub fn new(i: usize, j: usize) -> Self {
            Point(i as isize, j as isize)
        }
    }

    #[cfg(test)]
    mod tests {}
}
