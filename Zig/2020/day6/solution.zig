const std = @import("std");
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;
const splitSeq = std.mem.splitSequence;

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const lines = try read_lines(&allocator, "input.txt");

    std.debug.print("Part 1: {!}\n", .{part1(&allocator, lines.items)});
    std.debug.print("Part 2: {!}\n", .{part2(&allocator, lines.items)});
}

fn part1(allocator: *const Allocator, lines: [][]const u8) !u32 {
    var count: u32 = 0;
    for (lines) |l| {
        var answers: std.AutoHashMap(u8, void) = std.AutoHashMap(u8, void).init(allocator.*);
        for (l) |question| {
            if (question == '\n') {
                continue;
            }

            if (!answers.contains(question)) {
                try answers.put(question, {});
            }
        }
        count += answers.count();
    }
    return count;
}

fn part2(allocator: *const Allocator, lines: [][]const u8) !u32 {
    var count: u32 = 0;
    for (lines) |l| {
        var answers: std.AutoHashMap(u8, u32) = std.AutoHashMap(u8, u32).init(allocator.*);
        var people: u32 = 1;
        for (l, 0..) |question, i| {
            if (question == '\n') {
                if (i == l.len - 1) //Last uncaught new line, discard it
                    break;
                people += 1;
                continue;
            }

            const v = try answers.getOrPut(question);
            if (v.found_existing) {
                v.value_ptr.* += 1;
            } else {
                v.value_ptr.* = 1;
            }
        }
        var iter = answers.iterator();

        while (iter.next()) |entry| {
            if (entry.value_ptr.* == people)
                count += 1;
        }
        answers.clearAndFree();
    }
    return count;
}

fn read_lines(allocator: *const Allocator, filename: []const u8) !ArrayList([]const u8) {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    var lines = ArrayList([]const u8).init(allocator.*);

    const file_size = (try file.stat()).size;
    const content = try file.reader().readAllAlloc(allocator.*, file_size);

    var iter = splitSeq(u8, content, "\n\n");
    while (iter.next()) |line| {
        try lines.append(line);
    }

    return lines;
}
