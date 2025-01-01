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
    const numbers = try read_numbers(&allocator, args[1]);

    std.debug.print("Part 1: {!}\n", .{solve(&allocator, numbers.items, 2020)});
    std.debug.print("Part 2: {!}\n", .{solve(&allocator, numbers.items, 30000000)});
}

fn solve(allocator: *const Allocator, numbers: []const usize, last_round: usize) !usize {
    var spoken: std.AutoHashMap(usize, usize) = std.AutoHashMap(usize, usize).init(allocator.*);
    defer spoken.clearAndFree();

    for (numbers, 1..) |number, i| {
        try spoken.put(number, i);
    }

    var next: usize = 0;
    var round: usize = numbers.len + 1;

    while (round < last_round) : (round += 1) {
        const previous = spoken.get(next);
        try spoken.put(next, round);

        next = if (previous != null) round - previous.? else 0;
    }
    return next;
}

fn read_numbers(allocator: *const Allocator, filename: []const u8) !ArrayList(usize) {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    var numbers = ArrayList(usize).init(allocator.*);

    const file_size = (try file.stat()).size;
    const content = try file.reader().readAllAlloc(allocator.*, file_size);

    var iter = std.mem.splitScalar(u8, content, '\n');
    var number_iter = std.mem.splitScalar(u8, iter.first(), ',');
    while (number_iter.next()) |number| {
        if (number.len == 0) break;
        try numbers.append(try std.fmt.parseInt(u8, number, 10));
    }

    return numbers;
}

test "Part 1" {
    const test_input = [3]usize{ 3, 2, 1 };
    const test_result = try solve(&std.testing.allocator, &test_input, 2020);
    try std.testing.expect(test_result == 438);
}

test "Part 2" {
    const test_input = [3]usize{ 1, 3, 2 };
    const test_result = try solve(&std.testing.allocator, &test_input, 30000000);
    try std.testing.expect(test_result == 2578);
}
