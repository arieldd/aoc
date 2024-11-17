const std = @import("std");
const Allocator = std.mem.Allocator;
const split = std.mem.splitSequence;

const Slope = struct {
    right: u8,
    down: u8,
};

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const lines = try read_lines(&allocator, "input.txt");

    std.debug.print("Part 1: {}\n", .{solve(lines, .{ .right = 3, .down = 1 })});
    std.debug.print("Part 2: {}\n", .{part2(lines)});
}

fn solve(lines: [][]const u8, slope: Slope) u32 {
    const n: u32 = @intCast(lines.len);
    const m: u32 = @intCast(lines[0].len);

    var row: u32 = 0;
    var column: u32 = 0;

    var trees: u32 = 0;

    while (row < n - slope.down) {
        column += slope.right;
        row += slope.down;
        if (column >= m) {
            column %= m;
        }

        if (lines[row][column] == '#') {
            trees += 1;
        }
    }
    return trees;
}

fn part2(lines: [][]const u8) u32 {
    var result: u32 = 1;

    const slopes = [_]Slope{
        .{ .right = 1, .down = 1 },
        .{ .right = 3, .down = 1 },
        .{ .right = 5, .down = 1 },
        .{ .right = 7, .down = 1 },
        .{ .right = 1, .down = 2 },
    };

    for (slopes) |slope| {
        result *= solve(lines, slope);
    }

    return result;
}

fn read_lines(allocator: *const Allocator, filename: []const u8) ![][]const u8 {
    const file = try std.fs.cwd().openFile(filename, .{ .mode = .read_only });
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

    var iter = split(u8, input, "\n");

    var i: usize = 0;
    while (iter.next()) |line| : (i += 1) {
        if (line.len == 0) break;
        lines[i] = line;
    }

    return lines;
}
