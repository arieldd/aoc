const std = @import("std");
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;
const splitSeq = std.mem.splitSequence;

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const args = try std.process.argsAlloc(allocator);
    std.debug.assert(args.len > 1);
    const numbers = try read_lines(&allocator, args[1]);

    std.debug.print("Part 1: {}\n", .{part1(&numbers)});
    std.debug.print("Part 2: {}\n", .{part2(numbers.items)});
}

fn part1(numbers: *const ArrayList(i64)) u64 {
    const sorted = numbers.clone() catch return 0;
    std.mem.sort(i64, sorted.items, {}, comptime std.sort.asc(i64));
    var ones: u64 = 0;
    var threes: u64 = 1;

    if (sorted.items[0] == 1) ones += 1;
    if (sorted.items[0] == 3) ones += 3;

    var i: usize = 1;
    while (i < sorted.items.len) : (i += 1) {
        const jolt = sorted.items[i];
        const prev_jolt = sorted.items[i - 1];
        if (jolt - prev_jolt == 1) ones += 1;
        if (jolt - prev_jolt == 3) threes += 1;
    }

    return ones * threes;
}

fn part2(_: []i64) u32 {
    return 0;
}

fn read_lines(allocator: *const Allocator, filename: []const u8) !ArrayList(i64) {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    var numbers = ArrayList(i64).init(allocator.*);

    const file_size = (try file.stat()).size;
    const content = try file.reader().readAllAlloc(allocator.*, file_size);

    var iter = splitSeq(u8, content, "\n");
    while (iter.next()) |line| {
        if (line.len == 0) break;
        try numbers.append(try std.fmt.parseInt(i64, line, 10));
    }

    return numbers;
}
