const std = @import("std");
const Allocator = std.mem.Allocator;
const split = std.mem.splitSequence;

const ListEntry = struct {
    rule: Rule,
    password: []const u8,

    pub fn print(self: *const ListEntry) void {
        std.debug.print("{d}-{d} {c}: {s}\n", .{ self.rule.min, self.rule.max, self.rule.char, self.password });
    }

    pub fn isValid1(self: *const ListEntry) bool {
        var occurrences: u32 = 0;
        for (self.password) |c| {
            if (c == self.rule.char) {
                occurrences += 1;
            }
        }
        return occurrences >= self.rule.min and occurrences <= self.rule.max;
    }

    pub fn isValid2(self: *const ListEntry) bool {
        var occurrences: u32 = 0;
        const places: [2]u16 = [2]u16{ self.rule.min - 1, self.rule.max - 1 };
        for (places) |p| {
            if (self.password[p] == self.rule.char)
                occurrences += 1;
        }
        return occurrences == 1;
    }
};

const Rule = struct {
    char: u8,
    min: u16,
    max: u16,
};

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const lines = try read_lines(&allocator, "input.txt");
    const entries = try parse_input(&allocator, lines);

    std.debug.print("Part 1: {}\n", .{solve(entries, 1)});
    std.debug.print("Part 2: {}\n", .{solve(entries, 2)});
}

fn solve(entries: []const ListEntry, part: u8) u32 {
    var total: u32 = 0;
    for (entries) |entry| {
        if ((part == 1 and entry.isValid1()) or (part == 2 and entry.isValid2())) {
            total += 1;
        }
    }
    return total;
}

fn parse_input(allocator: *const Allocator, lines: [][]const u8) ![]ListEntry {
    var result: []ListEntry = try allocator.alloc(ListEntry, lines.len);
    for (lines, 0..) |line, i| {
        var iter = split(u8, line, ": ");
        const rule_str = iter.next().?;
        const password = iter.next().?;

        var iter2 = std.mem.splitScalar(u8, rule_str, ' ');
        const min_max = iter2.next().?;
        const char = (iter2.next().?)[0];

        var iter3 = std.mem.splitScalar(u8, min_max, '-');
        const min = try std.fmt.parseInt(u16, iter3.next().?, 10);
        const max = try std.fmt.parseInt(u16, iter3.next().?, 10);

        result[i] = ListEntry{
            .password = password,
            .rule = Rule{
                .char = char,
                .min = min,
                .max = max,
            },
        };
    }
    return result;
}

fn read_lines(allocator: *const Allocator, filename: []const u8) ![][]const u8 {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    const file_size = (try file.stat()).size;
    const input = try file.readToEndAlloc(allocator.*, file_size);

    var line_count: usize = 0;
    for (input) |c| {
        if (c == '\n') {
            line_count += 1;
        }
    }

    var lines: [][]const u8 = try allocator.alloc([]u8, line_count);
    errdefer allocator.free(lines);

    var iter = split(u8, input, "\n");

    var i: usize = 0;
    while (iter.next()) |line| : (i += 1) {
        if (line.len == 0) break;
        lines[i] = line;
    }

    return lines;
}
