const std = @import("std");
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;
const splitSeq = std.mem.splitSequence;

const PREAMBLE = 25;

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const args = try std.process.argsAlloc(allocator);
    std.debug.assert(args.len > 1);
    const numbers = try read_lines(&allocator, args[1]);

    const invalid_number = try part1(&allocator, numbers.items);
    std.debug.print("Part 1: {}\n", .{invalid_number});
    std.debug.print("Part 2: {}\n", .{try part2(&allocator, numbers.items, invalid_number)});
}

fn part1(allocator: *const Allocator, numbers: []i64) !i64 {
    var i: usize = PREAMBLE + 1;
    while (i < numbers.len) : (i += 1) {
        if (!try is_sum_of_two(allocator, numbers[i - PREAMBLE - 1 .. i], numbers[i]))
            return numbers[i];
    }

    return 0;
}

fn is_sum_of_two(allocator: *const Allocator, numbers: []const i64, target: i64) !bool {
    var map: std.AutoHashMap(i64, usize) = std.AutoHashMap(i64, usize).init(allocator.*);
    for (numbers, 0..) |value, i| {
        const complement: i64 = target - value;
        if (complement < 0) continue;

        if (map.contains(complement)) return true;
        try map.put(value, i);
    }
    return false;
}

fn part2(allocator: *const Allocator, numbers: []i64, target: i64) !i64 {
    var prefix_sum: []i64 = try allocator.alignedAlloc(i64, null, numbers.len + 1);
    for (numbers, 0..) |value, i| {
        prefix_sum[i + 1] = prefix_sum[i] + value;
    }

    var left: usize = 0;
    var right: usize = 2;

    while (right < prefix_sum.len) {
        const sum = prefix_sum[right] - prefix_sum[left];
        if (sum == target) return calculate_weakness(numbers[left..right]);
        if (sum > target) {
            left += 1;
        } else {
            right += 1;
        }
    }
    return 0;
}

fn calculate_weakness(slice: []const i64) i64 {
    var min: i64 = slice[0];
    var max: i64 = slice[0];
    var i: usize = 1;
    while (i < slice.len) : (i += 1) {
        if (slice[i] > max) max = slice[i];
        if (slice[i] < min) min = slice[i];
    }
    return min + max;
}

fn read_lines(allocator: *const Allocator, filename: []const u8) !ArrayList(i64) {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    var lines = ArrayList(i64).init(allocator.*);

    const file_size = (try file.stat()).size;
    const content = try file.reader().readAllAlloc(allocator.*, file_size);

    var iter = splitSeq(u8, content, "\n");
    while (iter.next()) |line| {
        if (line.len == 0) break;
        try lines.append(try std.fmt.parseInt(i64, line, 10));
    }

    return lines;
}
