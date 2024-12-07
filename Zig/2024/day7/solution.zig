const std = @import("std");
const mem = std.mem;
const print = std.debug.print;
const Allocator = std.mem.Allocator;
const ArrayList = std.ArrayList;

const Calibration = struct {
    result: usize,
    operands: []usize = undefined,

    pub fn display(self: *const Calibration) void {
        print("{}: ", .{self.result});
        for (self.operands) |value| {
            print("{} ", .{value});
        }
        print("\n", .{});
    }
};

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const args = try std.process.argsAlloc(allocator);
    std.debug.assert(args.len > 1);
    const lines = try read_lines(&allocator, args[1]);

    const solutions: struct { usize, usize } = solve(lines);
    print("Part 1: {}\n", .{solutions[0]});
    print("Part 2: {}\n", .{solutions[1]});
}

fn solve(calibrations: []const Calibration) struct { usize, usize } {
    var part1: usize = 0;
    var part2: usize = 0;
    for (calibrations) |c| {
        if (valid_calibration(&c, c.operands[0], 1, false)) {
            part1 += c.result;
            part2 += c.result;
        } else if (valid_calibration(&c, c.operands[0], 1, true)) {
            part2 += c.result;
        }
    }
    return .{ part1, part2 };
}

fn valid_calibration(c: *const Calibration, total: usize, index: usize, use_concat: bool) bool {
    if (index >= c.operands.len) return total == c.result;
    if (total > c.result) return false;

    const value = c.operands[index];

    const result: bool = valid_calibration(c, total + value, index + 1, use_concat) //
    or valid_calibration(c, total * value, index + 1, use_concat);

    return if (!use_concat) result else result or valid_calibration(c, concatenate(total, value), index + 1, true);
}

fn concatenate(v1: usize, v2: usize) usize {
    var digits: usize = 0;
    var dividing: usize = v2;
    while (dividing > 0) {
        digits += 1;
        dividing /= 10;
    }
    return v1 * std.math.pow(usize, 10, digits) + v2;
}

fn read_lines(allocator: *const Allocator, filename: []const u8) ![]Calibration {
    const file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    var lines = ArrayList(Calibration).init(allocator.*);

    const file_size = (try file.stat()).size;
    const content = try file.reader().readAllAlloc(allocator.*, file_size);

    var iter = mem.splitScalar(u8, content, '\n');
    while (iter.next()) |line| {
        if (line.len == 0) break;

        var parts = mem.splitSequence(u8, line, ": ");
        var next: Calibration = .{ .result = try std.fmt.parseInt(usize, parts.first(), 10) };
        var operands: ArrayList(usize) = ArrayList(usize).init(allocator.*);

        var values = mem.splitScalar(u8, parts.next().?, ' ');
        while (values.next()) |value|
            try operands.append(try std.fmt.parseInt(usize, value, 10));

        next.operands = try operands.toOwnedSlice();
        try lines.append(next);
    }

    return try lines.toOwnedSlice();
}
