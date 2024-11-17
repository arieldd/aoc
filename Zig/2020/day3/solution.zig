const std = @import("std");
const Allocator = std.mem.Allocator;
const split = std.mem.splitSequence;

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const lines = try read_lines(&allocator, "input.txt");

    std.debug.print("Part 1: {}\n", .{part1(lines)});
    std.debug.print("Part 2: {}\n", .{part2(lines)});
}

fn part1(_: [][]const u8) u32 {
    return 0;
}

fn part2(_: [][]const u8) u32 {
    return 0;
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
