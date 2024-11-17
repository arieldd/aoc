const std = @import("std");
const Allocator = std.mem.Allocator;
const split = std.mem.splitSequence;

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const lines = try read_lines(&allocator, "input.txt");

    std.debug.print("Part 1: {!}\n", .{part1(lines, 2020)});
    std.debug.print("Part 2: {!}\n", .{part2(lines, 2020)});
}

fn part1(lines: [][]const u8, target: u32) !u32 {
    for (lines, 0..) |line_1, i| {
        const v1 = try std.fmt.parseInt(u32, line_1, 10);
        for (lines[i + 1 ..]) |line_2| {
            const v2 = try std.fmt.parseInt(u32, line_2, 10);
            if (v1 + v2 == target)
                return v1 * v2;
        }
    }
    return 0;
}

fn part2(lines: [][]const u8, target: u32) !u32 {
    for (lines, 0..) |line_1, i| {
        const v1 = try std.fmt.parseInt(u32, line_1, 10);
        for (lines[i + 1 ..]) |line_2| {
            const v2 = try std.fmt.parseInt(u32, line_2, 10);
            for (lines[i + 2 ..]) |line_3| {
                const v3 = try std.fmt.parseInt(u32, line_3, 10);
                if (v1 + v2 + v3 == target)
                    return v1 * v2 * v3;
            }
        }
    }
    return 0;
}

fn read_lines(allocator: *const Allocator, filename: []const u8) ![][]const u8 {
    const file = try std.fs.cwd().openFile(filename, .{ .mode = .read_only });
    defer file.close();

    const file_size = (try file.stat()).size;
    const input = try file.readToEndAlloc(allocator.*, file_size);

    const initial_lines = 100;
    var lines: [][]const u8 = try allocator.alloc([]u8, initial_lines);

    var iter = split(u8, input, "\n");

    var i: usize = 0;
    while (iter.next()) |line| : (i += 1) {
        if (line.len == 0) break;
        if (i == lines.len) {
            lines = try allocator.realloc(lines, lines.len + initial_lines);
        }
        lines[i] = line;
    }

    if (i < lines.len) {
        lines = try allocator.realloc(lines, i);
    }

    return lines;
}
