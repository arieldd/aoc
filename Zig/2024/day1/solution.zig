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
    const lists = try read_locations(&allocator, args[1]);

    std.debug.print("Part 1: {!}\n", .{part1(lists)});
    std.debug.print("Part 2: {!}\n", .{part2(lists)});
}

fn part1(lists: Locations) !u64 {
    var sum: u64 = 0;
    var i: usize = 0;
    const n: usize = lists.l1.items.len;
    while (i < n) : (i += 1) {
        sum += @abs(lists.l1.items[i] - lists.l2.items[i]);
    }
    return sum;
}

fn part2(locations: Locations) !u64 {
    var freq: std.AutoHashMap(i64, u64) = std.AutoHashMap(i64, u64).init(locations.l1.allocator);
    for (locations.l1.items) |value| {
        try freq.put(value, 0);
    }

    for (locations.l2.items) |value| {
        if (freq.contains(value)) {
            freq.getPtr(value).?.* += 1;
        }
    }

    var score: u64 = 0;
    var pairs = freq.iterator();
    while (pairs.next()) |entry| {
        score += @as(u64, @intCast(entry.key_ptr.*)) * entry.value_ptr.*;
    }
    return score;
}

fn read_locations(allocator: *const Allocator, filename: []const u8) !Locations {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    var lists: Locations = .{ .l1 = ArrayList(i64).init(allocator.*), .l2 = ArrayList(i64).init(allocator.*) };

    const file_size = (try file.stat()).size;
    const content = try file.reader().readAllAlloc(allocator.*, file_size);

    var iter = splitSeq(u8, content, "\n");
    while (iter.next()) |line| {
        if (line.len == 0) break;

        var parts = splitSeq(u8, line, "   ");
        try lists.l1.append(try std.fmt.parseInt(i64, parts.first(), 10));
        try lists.l2.append(try std.fmt.parseInt(i64, parts.next().?, 10));
    }

    std.mem.sort(i64, lists.l1.items, {}, std.sort.asc(i64));
    std.mem.sort(i64, lists.l2.items, {}, std.sort.asc(i64));

    return lists;
}

const Locations = struct {
    l1: ArrayList(i64),
    l2: ArrayList(i64),
};
