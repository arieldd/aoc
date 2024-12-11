const std = @import("std");
const mem = std.mem;
const print = std.debug.print;
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const args = try std.process.argsAlloc(allocator);
    std.debug.assert(args.len > 1);
    const stones = try read_input(&allocator, args[1]);

    print("Part 1: {!}\n", .{solve_dp(&allocator, stones, 25)});
    print("Part 2: {!}\n", .{solve_dp(&allocator, stones, 75)});
}

fn solve(allocator: *const Allocator, stones: []const u64, blinks: u8) !u64 {
    var current = std.AutoHashMap(u64, u64).init(allocator.*);
    var next = std.AutoHashMap(u64, u64).init(allocator.*);
    for (stones) |s| {
        try current.put(s, 1);
    }

    var steps: u8 = blinks;
    while (steps > 0) : (steps -= 1) {
        var entries = current.iterator();
        while (entries.next()) |entry| {
            if (entry.key_ptr.* == 0) {
                const new = try next.getOrPutValue(1, 0);
                new.value_ptr.* += entry.value_ptr.*;
                continue;
            }
            const digits = try to_digits(allocator, entry.key_ptr.*);
            if (digits.len % 2 != 0) {
                const new = try next.getOrPutValue(to_int(digits) * 2024, 0);
                new.value_ptr.* += entry.value_ptr.*;
            } else {
                var new = try next.getOrPutValue(to_int(digits[0 .. digits.len / 2]), 0);
                new.value_ptr.* += entry.value_ptr.*;
                new = try next.getOrPutValue(to_int(digits[digits.len / 2 ..]), 0);
                new.value_ptr.* += entry.value_ptr.*;
            }
        }

        current = next.move();
    }

    var result: u64 = 0;
    var entries = current.iterator();
    while (entries.next()) |entry| {
        result += entry.value_ptr.*;
    }
    return result;
}

fn solve_dp(allocator: *const Allocator, stones: []const u64, blinks: u8) !u64 {
    var result: u64 = 0;
    var cache = std.AutoHashMap(struct { u64, u8 }, u64).init(allocator.*);
    for (stones) |s| {
        result += try dp(allocator, s, blinks, &cache);
    }

    return result;
}

fn dp(allocator: *const Allocator, stone: u64, blinks: u8, cache: *std.AutoHashMap(struct { u64, u8 }, u64)) !u64 {
    if (blinks == 0) return 1;

    if (cache.contains(.{ stone, blinks })) return cache.get(.{ stone, blinks }).?;

    var result: u64 = 0;
    if (stone == 0) {
        result = try dp(allocator, 1, blinks - 1, cache);
    } else {
        const digits = try to_digits(allocator, stone);
        if (digits.len % 2 != 0) {
            result = try dp(allocator, stone * 2024, blinks - 1, cache);
        } else {
            result += try dp(allocator, to_int(digits[0 .. digits.len / 2]), blinks - 1, cache);
            result += try dp(allocator, to_int(digits[digits.len / 2 ..]), blinks - 1, cache);
        }
    }
    try cache.put(.{ stone, blinks }, result);
    return result;
}

fn to_digits(allocator: *const Allocator, value: u64) ![]u8 {
    var result: []u8 = try allocator.alloc(u8, 50);
    var num: u64 = value;
    var i: usize = 0;
    while (num != 0) : (i += 1) {
        result[i] = @truncate(num % 10);
        num /= 10;
    }

    return result[0..i];
}

fn to_int(digits: []u8) u64 {
    const n: usize = digits.len;
    if (n == 0) return 0;
    var result: u64 = 0;
    var i: usize = n - 1;
    while (true) : (i -= 1) {
        result *= 10;
        result += digits[i];
        if (i == 0)
            break;
    }
    return result;
}

fn read_input(allocator: *const Allocator, filename: []const u8) ![]const u64 {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    var stones = ArrayList(u64).init(allocator.*);

    const file_size = (try file.stat()).size;
    const content = try file.reader().readAllAlloc(allocator.*, file_size);

    var iter = mem.splitScalar(u8, content, '\n');
    while (iter.next()) |line| {
        if (line.len == 0) break;
        var numbers = mem.splitScalar(u8, line, ' ');
        while (numbers.next()) |value|
            try stones.append(try std.fmt.parseInt(u64, value, 10));
    }

    return try stones.toOwnedSlice();
}
