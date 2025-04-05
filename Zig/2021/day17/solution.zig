const std = @import("std");
const mem = std.mem;
const print = std.debug.print;

const input = @embedFile("input.txt");

const Region = struct {
    range_x: [2]i32 = undefined,
    range_y: [2]i32 = undefined,
};

pub fn main() !void {
    _ = try parse_region(input);
}

fn parse_region(data: []const u8) !Region {
    var tokens = mem.tokenizeAny(u8, data, ".,= :\n");
    var result: Region = .{};

    var token_count: u4 = 0;
    while (tokens.next()) |token| : (token_count += 1) {
        switch (token_count) {
            3 => result.range_x[0] = try std.fmt.parseInt(i32, token, 10),
            4 => result.range_x[1] = try std.fmt.parseInt(i32, token, 10),
            6 => result.range_y[0] = try std.fmt.parseInt(i32, token, 10),
            7 => result.range_y[1] = try std.fmt.parseInt(i32, token, 10),
            else => continue,
        }
    }

    return result;
}

test "Test parse" {
    const data = "target area: x=20..30, y=-10..-5";
    const region = try parse_region(data);
    try std.testing.expectEqual(20, region.range_x[0]);
    try std.testing.expectEqual(30, region.range_x[1]);
    try std.testing.expectEqual(-10, region.range_y[0]);
    try std.testing.expectEqual(-5, region.range_y[1]);
}

test "Test case 1" {}
