const std = @import("std");
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;
const splitSeq = std.mem.splitSequence;

const Input = struct {
    timestamp: usize,
    timetable: []usize,
};

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const args = try std.process.argsAlloc(allocator);
    std.debug.assert(args.len > 1);
    const input = try read_input(&allocator, args[1]);

    std.debug.print("Part 1: {}\n", .{part1(&input)});
    part2(&input);
}

fn part1(input: *const Input) usize {
    var min_wait: usize = input.timestamp;
    var bus_id: usize = 0;
    for (input.timetable) |value| {
        if (value == 0) continue;
        const wait = value - @rem(input.timestamp, value);
        if (wait < min_wait) {
            min_wait = wait;
            bus_id = value;
        }
    }
    std.debug.print("{} {}\n", .{ min_wait, bus_id });
    return min_wait * bus_id;
}

fn part2(input: *const Input) void {
    // Print CRT equations and solve online :)
    std.debug.print("Part 2: Solve this CRT equations\n", .{});
    for (input.timetable, 0..) |value, i| {
        if (value == 0) continue;
        if (i == 0) {
            std.debug.print("{d} mod({})\n", .{ 0, value });
            continue;
        }

        std.debug.print("{d} mod({})\n", .{ value - @rem(i, value), value });
    }
}

fn read_input(allocator: *const Allocator, filename: []const u8) !Input {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    const file_size = (try file.stat()).size;
    const content = try file.reader().readAllAlloc(allocator.*, file_size);

    var iter = splitSeq(u8, content, "\n");
    const timestamp: usize = try std.fmt.parseInt(usize, iter.first(), 10);
    var buses = std.mem.splitScalar(u8, iter.next().?, ',');

    var timetable: ArrayList(usize) = ArrayList(usize).init(allocator.*);
    while (buses.next()) |value| {
        const time: usize = if (value[0] == 'x') 0 else try std.fmt.parseInt(usize, value, 10);
        try timetable.append(time);
    }

    return .{ .timestamp = timestamp, .timetable = timetable.items };
}
