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
    const lines = try read_lines(&allocator, args[1]);
    const v1: u64 = try run_program_v1(&allocator, lines.items);
    const v2: u64 = try run_program_v2(&allocator, lines.items);

    std.debug.print("Part 1: {}\n", .{v1});
    std.debug.print("Part 2: {}\n", .{v2});
}

fn run_program_v1(allocator: *const Allocator, lines: [][]const u8) !u64 {
    var memory: std.AutoHashMap(u64, u64) = std.AutoHashMap(u64, u64).init(allocator.*);

    var or_mask: [36]u1 = .{0} ** 36;
    var and_mask: [36]u1 = .{0} ** 36;
    for (lines) |command| {
        var split = splitSeq(u8, command, " = ");

        const header = split.first();
        const value_str = split.next().?;

        if (value_str.len == 36) { //Mask value
            for (value_str, 0..) |bit, i| {
                or_mask[i] = if (bit == '0' or bit == 'X') 0 else 1;
                and_mask[i] = if (bit == '1' or bit == 'X') 1 else 0;
            }
        } else {
            const addr = try std.fmt.parseInt(u64, header[4 .. header.len - 1], 10);
            const value = try std.fmt.parseInt(u64, value_str, 10);
            const number = (value & to_decimal(&and_mask)) | to_decimal(&or_mask);
            try memory.put(addr, number);
        }
    }

    var sum: u64 = 0;
    var values = memory.valueIterator();
    while (values.next()) |number| {
        sum += number.*;
    }
    return sum;
}

fn run_program_v2(allocator: *const Allocator, lines: [][]const u8) !u64 {
    var memory: std.AutoHashMap(u64, u64) = std.AutoHashMap(u64, u64).init(allocator.*);

    var mask: [36]u8 = .{0} ** 36;
    for (lines) |command| {
        var split = splitSeq(u8, command, " = ");

        const header = split.first();
        const value_str = split.next().?;

        if (value_str.len == 36) { //Mask value
            for (value_str, 0..) |bit, i| {
                mask[i] = bit;
            }
        } else {
            const addr = try std.fmt.parseInt(u64, header[4 .. header.len - 1], 10);
            const value = try std.fmt.parseInt(u64, value_str, 10);

            const b_addr = try to_binary(allocator, addr, 36);
            var masked_addr: [36]u8 = .{0} ** 36;
            for (b_addr, 0..) |bit, i| {
                if (mask[i] == '0') {
                    masked_addr[i] = @as(u8, bit) + '0';
                } else {
                    masked_addr[i] = mask[i];
                }
            }

            var generated: std.AutoHashMap(u64, void) = std.AutoHashMap(u64, void).init(allocator.*);
            try generate_addr(&masked_addr, 0, &generated);

            var keys = generated.keyIterator();
            while (keys.next()) |new_addr| {
                try memory.put(new_addr.*, value);
            }
        }
    }

    var sum: u64 = 0;
    var values = memory.valueIterator();
    while (values.next()) |number| {
        sum += number.*;
    }
    return sum;
}

fn to_decimal(bits: []const u1) u64 {
    const n = bits.len;
    var number: u64 = 0;
    var i: usize = 0;
    while (i < n) : (i += 1) {
        if (bits[i] == 1)
            number += std.math.pow(u64, 2, n - 1 - i);
    }

    return number;
}

fn to_binary(allocator: *const Allocator, value: u64, n: usize) ![]u1 {
    var result: []u1 = try allocator.alloc(u1, n);

    var remaining = value;
    var i: usize = n - 1;
    while (i >= 0) : (i -= 1) {
        result[i] = @truncate(remaining % 2);
        remaining /= 2;

        if (i == 0) break;
    }

    return result;
}

fn generate_addr(chars: []u8, index: usize, generated: *std.AutoHashMap(u64, void)) !void {
    if (index >= chars.len) {
        var addr_binary = try generated.allocator.alloc(u1, chars.len);
        for (chars, 0..) |value, i| {
            addr_binary[i] = @truncate(value - '0');
        }
        const addr = to_decimal(addr_binary);
        if (!generated.contains(addr))
            try generated.put(addr, {});
        return;
    }

    var i: usize = index;
    while (i < chars.len - 1 and chars[i] != 'X') : (i += 1) {}

    if (chars[i] == 'X') {
        chars[i] = '1';
        try generate_addr(chars, i + 1, generated);
        chars[i] = '0';
        try generate_addr(chars, i + 1, generated);

        chars[i] = 'X';
    } else {
        try generate_addr(chars, i + 1, generated);
    }
}

fn read_lines(allocator: *const Allocator, filename: []const u8) !ArrayList([]const u8) {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    var lines = ArrayList([]const u8).init(allocator.*);

    const file_size = (try file.stat()).size;
    const content = try file.reader().readAllAlloc(allocator.*, file_size);

    var iter = splitSeq(u8, content, "\n");
    while (iter.next()) |line| {
        if (line.len == 0) break;
        try lines.append(line);
    }

    return lines;
}
