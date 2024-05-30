pub mod grid {

    pub const DX4: [isize; 4] = [1, 0, -1, 0];
    pub const DY4: [isize; 4] = [0, 1, 0, -1];
    pub const DX8: [isize; 8] = [1, 1, 0, -1, -1, -1, 0, 1];
    pub const DY8: [isize; 8] = [0, 1, 1, 1, 0, -1, -1, -1];

    #[derive(Debug)]
    pub struct Grid<T> {
        contents: Vec<T>,
        pub width: usize,
        pub height: usize,
    }

    pub trait FromCharElem {
        fn new(c: char, row: usize, column: usize) -> Self;
    }

    impl<T: FromCharElem> Grid<T> {
        pub fn from_lines(lines: &[&str]) -> Self {
            Grid {
                contents: lines
                    .iter()
                    .enumerate()
                    .fold(vec![], |mut contents, (i, row)| {
                        row.char_indices().for_each(|(j, c)| {
                            contents.push(T::new(c, i, j));
                        });
                        contents
                    }),
                height: lines.len(),
                width: lines[0].len(),
            }
        }
    }

    impl<T> Grid<T> {
        pub fn new() -> Self {
            Grid {
                contents: vec![],
                width: 0,
                height: 0,
            }
        }

        pub fn at(&self, i: usize, j: usize) -> Option<&T> {
            let index = i * self.width + j;
            if index < self.contents.len() {
                Some(&self.contents[index])
            } else {
                None
            }
        }

        pub fn get(&self, i: isize, j: isize) -> Option<&T> {
            if i >= 0 && j >= 0 {
                self.at(i as usize, j as usize)
            } else {
                None
            }
        }

        pub fn iter(&self) -> GridIterator<T> {
            GridIterator {
                grid: self,
                index: 0,
            }
        }
    }

    impl<T> IntoIterator for Grid<T> {
        type Item = T;

        type IntoIter = GridIntoIterator<T>;

        fn into_iter(self) -> Self::IntoIter {
            GridIntoIterator { grid: self }
        }
    }

    pub struct GridIterator<'a, T> {
        grid: &'a Grid<T>,
        index: usize,
    }

    impl<'a, T> Iterator for GridIterator<'a, T> {
        type Item = &'a T;

        fn next(&mut self) -> Option<Self::Item> {
            if self.index < self.grid.contents.len() {
                let next = Some(&self.grid.contents[self.index]);
                self.index += 1;
                next
            } else {
                None
            }
        }
    }

    pub struct GridIntoIterator<T> {
        grid: Grid<T>,
    }

    impl<T> Iterator for GridIntoIterator<T> {
        type Item = T;

        fn next(&mut self) -> Option<Self::Item> {
            if self.grid.contents.len() == 0 {
                None
            } else {
                Some(self.grid.contents.remove(0))
            }
        }
    }

    #[derive(Debug, PartialEq, Eq, Hash)]
    pub struct Point(pub isize, pub isize);

    impl Point {
        pub fn new(i: usize, j: usize) -> Self {
            Point(i as isize, j as isize)
        }
    }

    #[cfg(test)]
    mod tests {}
}
